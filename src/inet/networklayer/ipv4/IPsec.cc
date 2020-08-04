//
// Copyright (C) 2020 Marcel Marek
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include <algorithm>

#include "inet/networklayer/ipv4/IPsec.h"
#include "inet/common/ModuleAccess.h"
#include "inet/networklayer/ipv4/IPv4Datagram_m.h"
#include "inet/networklayer/ipv4/ipsec/IPAH_m.h"
#include "inet/networklayer/ipv4/ipsec/IPESP_m.h"
#include "inet/transportlayer/tcp_common/TCPSegment.h"
#include "inet/transportlayer/udp/UDPPacket.h"





namespace inet {

Define_Module(IPsec);

IPsec::IPsec()
{

}

IPsec::~IPsec()
{
//    if (ipLayer)
//           ipLayer->unregisterHook(0, this);

}

void IPsec::initSecurityDBs(cXMLElement *spdConfig)
{
    cXMLElementList spdEntries = spdConfig->getChildrenByTagName("SPDEntry");
    for (auto spdEntriesIt = spdEntries.begin(); spdEntriesIt != spdEntries.end(); spdEntriesIt++) {
        //process one SPDEntry
        SPDEntry *spdEntry = new SPDEntry();
        //get selector(s)
        cXMLElementList selectorList = (*spdEntriesIt)->getChildrenByTagName("Selector");
        for (auto selectorListIt = selectorList.begin(); selectorListIt != selectorList.end(); selectorListIt++) {
            IPsecSelector selector;
            cXMLElementList localAddressList = (*selectorListIt)->getChildrenByTagName("LocalAddress");
            for (auto localAddressListIt = localAddressList.begin(); localAddressListIt != localAddressList.end(); localAddressListIt++) {

                selector.setLocalAddress(IPv4Address((*localAddressListIt)->getNodeValue()));
            }
            cXMLElementList remoteAddressList = (*selectorListIt)->getChildrenByTagName("RemoteAddress");
            for (auto remoteAddressListIt = remoteAddressList.begin(); remoteAddressListIt != remoteAddressList.end(); remoteAddressListIt++) {

                selector.setRemoteAddress(IPv4Address((*remoteAddressListIt)->getNodeValue()));
            }
            cXMLElement *protocolTag = (*selectorListIt)->getFirstChildWithTag("Protocol");
            if (protocolTag != nullptr) {
                selector.setNextProtocol(atoi(protocolTag->getNodeValue()));
                //if TCP/UDP check PORT
                if (selector.getNextProtocol() == IP_PROT_TCP || selector.getNextProtocol() == IP_PROT_UDP) {
                    cXMLElementList localPortList = (*selectorListIt)->getChildrenByTagName("LocalPort");
                    for (auto localPortListIt = localPortList.begin(); localPortListIt != localPortList.end(); localPortListIt++) {

                        selector.setLocalPort(atoi((*localPortListIt)->getNodeValue()));
                    }
                    cXMLElementList remotePortList = (*selectorListIt)->getChildrenByTagName("RemotePort");
                    for (auto remotePortListIt = remotePortList.begin(); remotePortListIt != remotePortList.end(); remotePortListIt++) {

                        selector.setRemotePort(atoi((*remotePortListIt)->getNodeValue()));
                    }
                }
                else if (selector.getNextProtocol() == IP_PROT_ICMP) {
                    cXMLElement *icmpCodeTag = (*selectorListIt)->getFirstChildWithTag("ICMPCode");
                    if (icmpCodeTag != nullptr) {
                        selector.setIcmpCode(atoi(icmpCodeTag->getNodeValue()));
                    }
                }
            }
            spdEntry->setSelector(selector);
            cXMLElement *directionTag = (*spdEntriesIt)->getFirstChildWithTag("Direction");
            if (directionTag != nullptr) {
                if (!strcmp(directionTag->getNodeValue(), "IN")) {
                    spdEntry->setDirection(IPsecRule::Direction::dIN);
                }
                else if (!strcmp(directionTag->getNodeValue(), "OUT")) {
                    spdEntry->setDirection(IPsecRule::Direction::dOUT);
                }
                else {
                    throw cRuntimeError("SPDEntry: Rule has unknown direction only IN or OUT are supported");
                }
            }
            //Action
            cXMLElement *actionTag = (*spdEntriesIt)->getFirstChildWithTag("Action");
            if (actionTag != nullptr) {
                if (!strcmp(actionTag->getNodeValue(), "DISCARD")) {
                    spdEntry->setAction(IPsecRule::Action::aDISCARD);
                }
                else if (!strcmp(actionTag->getNodeValue(), "BYPASS")) {
                    spdEntry->setAction(IPsecRule::Action::aBYPASS);
                }
                else if (!strcmp(actionTag->getNodeValue(), "PROTECT")) {
                    spdEntry->setAction(IPsecRule::Action::aPROTECT);
                    //load SA details
                    cXMLElementList saEntryList = (*spdEntriesIt)->getChildrenByTagName("SAEntry");
                    for (auto saEntryListIt = saEntryList.begin(); saEntryListIt != saEntryList.end(); saEntryListIt++) {
                        cXMLElement *processingTag = (*saEntryListIt)->getFirstChildWithTag("Processing");
                        if (processingTag != nullptr) {
                            if (!strcmp(processingTag->getNodeValue(), "AH")) {
                                spdEntry->setProcessing(IPsecRule::ProcessingAlgs::pAH);
                            }
                            else if (!strcmp(processingTag->getNodeValue(), "ESP")) {
                                spdEntry->setProcessing(IPsecRule::ProcessingAlgs::pESP);
                            }
//                            else if(!strcmp(processingTag->getNodeValue(), "AH_ESP"))
//                            {
//                                spdEntry->setProcessing(IPsecRule::ProcessingAlgs::AH_ESP);
//                            }
                            else
                            {
                                throw cRuntimeError("IpsecEntry: Unknown processing alg (AH/ESP)");
                            }
                        }
                        else {
                            throw cRuntimeError("SAEntry: Missing Processing tag");
                        }
                        //LOAD SPI
                        cXMLElement *spiTag = (*saEntryListIt)->getFirstChildWithTag("SPI");
                        unsigned int spi = atoi(spiTag->getNodeValue());
                        if (spdEntry->getProcessing() == IPsecRule::ProcessingAlgs::pESP || spdEntry->getProcessing() == IPsecRule::ProcessingAlgs::pAH_ESP) {
                            SADEntry *sadEntry = new SADEntry();
                            sadEntry->setRule(spdEntry->getRule());
                            sadEntry->getRuleForUpdate()->setProcessing(IPsecRule::ProcessingAlgs::pESP);
                            sadEntry->setSpi(spi);
                            sadModule->addEntry(sadEntry);
                            spdEntry->addSADEntry(sadEntry);
                        }
                        else if (spdEntry->getProcessing() == IPsecRule::ProcessingAlgs::pAH || spdEntry->getProcessing() == IPsecRule::ProcessingAlgs::pAH_ESP) {
                            SADEntry *sadEntry = new SADEntry();
                            sadEntry->setRule(spdEntry->getRule());
                            sadEntry->getRuleForUpdate()->setProcessing(IPsecRule::ProcessingAlgs::pAH);
                            sadEntry->setSpi(spi);
                            sadModule->addEntry(sadEntry);
                            spdEntry->addSADEntry(sadEntry);
                        }
                    }
                }
                else {
                    throw cRuntimeError("SPDEntry: Rule has unknown action only DISCARD, BYPASS, PROTECT are supported");
                }
            }
            else {
                throw cRuntimeError("SPDEntry: Missing ACTION tag");
            }
        }
        spdModule->addEntry(spdEntry);
    }
}

void IPsec::initialize(int stage)
{
    if(stage == INITSTAGE_NETWORK_LAYER)
    {


        ahProtectOutDelay = par("ahProtectOutDelay");
        ahProtectInDelay = par("ahProtectInDelay");

        espProtectOutDelay = par("espProtectOutDelay");
        espProtectInDelay = par("espProtectInDelay");



        ipLayer = getModuleFromPar<IPv4>(par("networkProtocolModule"), this);

        //register IPsec hook
        ipLayer->registerHook(0, this);

        spdModule = getModuleFromPar<SPD>(par("spdModule"), this);
        sadModule = getModuleFromPar<SAD>(par("sadModule"), this);

        cXMLElement *spdConfig = par("spdConfig").xmlValue();

        initSecurityDBs(spdConfig);
    }

}


void IPsec::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        INetworkDatagram *context = (INetworkDatagram *)msg->getContextPointer();
        delete msg;
        ipLayer->reinjectQueuedDatagram(context);
    }
}


INetfilter::IHook::Result IPsec::datagramPreRoutingHook(INetworkDatagram *datagram, const InterfaceEntry *inIE, const InterfaceEntry *& outIE, L3Address& nextHopAddr)
{
// First hook at the receiver (before fragment reassembly)

    return INetfilter::IHook::ACCEPT;
}

INetfilter::IHook::Result IPsec::datagramForwardHook(INetworkDatagram *datagram, const InterfaceEntry *inIE, const InterfaceEntry *& outIE, L3Address& nextHopAddr)
{


    return INetfilter::IHook::ACCEPT;
}

/**
 * Initialize IPsecSelector from outgoing packet header
 * EXCEPT for local IP address
 */

void IPsec::initSelectorFromEgressPacket(IPv4Datagram *ipv4datagram,
        IPsecSelector *packetSelector) {

//    packetSelector->setLocalAddress(ipv4datagram->getSourceAddress().toIPv4());
    packetSelector->setRemoteAddress(ipv4datagram->getDestinationAddress().toIPv4());

    packetSelector->setNextProtocol(ipv4datagram->getTransportProtocol());

    if (ipv4datagram->getTransportProtocol() == IP_PROT_TCP) {
        tcp::TCPSegment *tcpSegment = (tcp::TCPSegment*) (ipv4datagram->getEncapsulatedPacket());
        packetSelector->setLocalPort(tcpSegment->getSourcePort());
        packetSelector->setRemotePort(tcpSegment->getDestinationPort());
    }
    else if (ipv4datagram->getTransportProtocol() == IP_PROT_UDP) {
        UDPPacket *udpPacket =( UDPPacket*) (ipv4datagram->getEncapsulatedPacket());
        packetSelector->setLocalPort(udpPacket->getSourcePort());
        packetSelector->setRemotePort(udpPacket->getDestinationPort());
    }
    else if (ipv4datagram->getTransportProtocol() == IP_PROT_ICMP) {
        ICMPMessage *icmpMessage = (ICMPMessage*) (ipv4datagram->getEncapsulatedPacket());
        packetSelector->setIcmpCode(icmpMessage->getCode());

    }
}


INetfilter::IHook::Result IPsec::datagramPostRoutingHook(INetworkDatagram *datagram, const InterfaceEntry *inIE, const InterfaceEntry *& outIE, L3Address& nextHopAddr)
{

    //packet will be fragmented (if necessary) later

    IPv4Datagram* ipv4datagram = (IPv4Datagram*)datagram;

    IPsecSelector egressPacketSelector;

    initSelectorFromEgressPacket(ipv4datagram, &egressPacketSelector);
    egressPacketSelector.setLocalAddress(outIE->getIPv4Address());

    //search Security Policy Database
    SPDEntry *spdEntry = spdModule->findEntry(IPsecRule::Direction::dOUT, &egressPacketSelector);
    if(spdEntry != nullptr){
        if (spdEntry->getAction() == IPsecRule::Action::aPROTECT) {
            return protectDatagram(ipv4datagram, &egressPacketSelector, spdEntry);
        }
        else if (spdEntry->getAction() == IPsecRule::Action::aBYPASS) {

            EV_INFO << "IPsec OUT BYPASS rule, packet: " << egressPacketSelector.str() << std::endl;
            return INetfilter::IHook::ACCEPT;
        }
        else if (spdEntry->getAction() == IPsecRule::Action::aDISCARD) {
            EV_INFO << "IPsec OUT DROP rule, packet: " << egressPacketSelector.str() << std::endl;
            return INetfilter::IHook::DROP;
        }

    }
    EV_INFO << "IPsec OUT BYPASS, no matching rule, packet: " << egressPacketSelector.str() << std::endl;
    return INetfilter::IHook::DROP;

}


/**
 * Initialize IPsecSelector from incoming packet header
 */
void IPsec::initSelectorFromIngressPacket(IPv4Datagram *ipv4datagram,
        IPsecSelector *packetSelector) {

    packetSelector->setLocalAddress(ipv4datagram->getDestinationAddress().toIPv4());
    packetSelector->setRemoteAddress(ipv4datagram->getSourceAddress().toIPv4());

    packetSelector->setNextProtocol(ipv4datagram->getTransportProtocol());

    if (ipv4datagram->getTransportProtocol() == IP_PROT_TCP) {
        tcp::TCPSegment *tcpSegment = (tcp::TCPSegment*) (ipv4datagram->getEncapsulatedPacket());
        packetSelector->setLocalPort(tcpSegment->getDestinationPort());
        packetSelector->setRemotePort(tcpSegment->getSourcePort());
    }
    else if (ipv4datagram->getTransportProtocol() == IP_PROT_UDP) {
        UDPPacket *udpPacket =( UDPPacket*) (ipv4datagram->getEncapsulatedPacket());
        packetSelector->setLocalPort(udpPacket->getDestinationPort());
        packetSelector->setRemotePort(udpPacket->getSourcePort());
    }
    else if (ipv4datagram->getTransportProtocol() == IP_PROT_ICMP) {
        ICMPMessage *icmpMessage = (ICMPMessage*) (ipv4datagram->getEncapsulatedPacket());
        packetSelector->setIcmpCode(icmpMessage->getCode());

    }
}

cPacket* IPsec::espProtect(cPacket *transport, SADEntry* sadEntry, int transportType)
{

    IPESP* espPacket = new IPESP();
    unsigned int blockSize = 16;
    unsigned int pad = (blockSize - (transport->getByteLength() + 2) % blockSize) % blockSize;
    unsigned int len = pad + 2 + IP_ESP_HEADER_BYTES;
    espPacket->setByteLength(len);
    espPacket->setSpi(sadEntry->getSpi());
    espPacket->encapsulate(transport);
    espPacket->setNextHeader(transportType);


    return espPacket;

}

cPacket* IPsec::ahProtect(cPacket* transport, SADEntry* sadEntry, int transportType)
{

    IPAH* ahPacket = new IPAH();
    ahPacket->setByteLength(IP_AH_HEADER_BYTES);
    ahPacket->setSequenceNumber(sadEntry->getAndIncSeqNum());

    ahPacket->setSpi(sadEntry->getSpi());
    ahPacket->encapsulate(transport);
    ahPacket->setNextHeader(transportType);

    return ahPacket;

}

INetfilter::IHook::Result IPsec::protectDatagram(IPv4Datagram  *ipv4datagram, IPsecSelector *egressPacketSelector, SPDEntry  *spdEntry )
{
    Enter_Method_Silent();
    cPacket* transport = ipv4datagram->decapsulate();
    double delay = 0;

    for (auto saIt = spdEntry->sadEntryList.begin(); saIt != spdEntry->sadEntryList.end(); saIt++) {
        int transportType = ipv4datagram->getTransportProtocol();

        if ((*saIt)->getProcessing() == IPsecRule::ProcessingAlgs::pESP) {
            EV_INFO << "IPsec OUT ESP PROTECT packet: " << egressPacketSelector->str() << std::endl;

            ipv4datagram->setTransportProtocol(IP_PROT_ESP);
            transport = espProtect(transport, ((*saIt)), transportType);

            delay += espProtectOutDelay;
        }

        transportType = ipv4datagram->getTransportProtocol();

        if ((*saIt)->getProcessing() == IPsecRule::ProcessingAlgs::pAH) {
            EV_INFO << "IPsec OUT AH PROTECT packet: " << egressPacketSelector->str() << std::endl;

            ipv4datagram->setTransportProtocol(IP_PROT_AH);
            transport = ahProtect(transport, (*saIt), transportType);

            delay += ahProtectOutDelay;
        }
    }

    ipv4datagram->encapsulate(transport);

    if (delay > 0 || lastProtectedOut > simTime()) {
        cMessage *selfmsg = new cMessage("IPsecProtectOutDelay");
        selfmsg->setContextPointer((INetworkDatagram*)ipv4datagram);
        delay = std::max(delay, delay + lastProtectedOut.dbl());
        scheduleAt(simTime() + delay, selfmsg);
        lastProtectedOut += delay;

        EV_INFO << "IPsec OUT PROTECT (delaying by: " << delay << "s), packet: " << egressPacketSelector->str() << std::endl;
        return INetfilter::IHook::QUEUE;
    }
    else{
        EV_INFO << "IPsec OUT PROTECT packet: " << egressPacketSelector->str() << std::endl;
        return INetfilter::IHook::ACCEPT;
    }
}

INetfilter::IHook::Result IPsec::datagramLocalInHook(INetworkDatagram *datagram, const InterfaceEntry *inIE)
{
//oh hey the datagram is for us

    Enter_Method_Silent();

    IPv4Datagram* ipv4datagram = (IPv4Datagram*)datagram;
    int transportProtocol  = ipv4datagram->getTransportProtocol();

    IPsecSelector ingressPacketSelector;
    initSelectorFromIngressPacket(ipv4datagram, &ingressPacketSelector);

    IPsecRule rule;
    rule.setDirection(IPsecRule::Direction::dIN);

    SADEntry *sadEntry;

    double delay = 0.0;
    if (transportProtocol == IP_PROT_AH) {
        IPAH* ah = (IPAH*)ipv4datagram->decapsulate();
        //FIND SA Entry in SAD

        rule.setProcessing(IPsecRule::ProcessingAlgs::pAH);
        sadEntry = sadModule->findEntry(&rule, ah->getSpi());

        if(sadEntry != nullptr){

            //found SA
            //TODO ICV verification, Replay Protection
            ipv4datagram->encapsulate(ah->decapsulate());
            ipv4datagram->setTransportProtocol(ah->getNextHeader());

            delay = ahProtectInDelay;

            if (ah->getNextHeader() != IP_PROT_ESP) {

                delete ah;

                if (delay > 0 || lastProtectedIn > simTime()) {
                    cMessage *selfmsg = new cMessage("IPsecProtectInDelay");
                    selfmsg->setContextPointer(datagram);
                    delay = std::max(delay, delay + lastProtectedIn.dbl());
                    scheduleAt(simTime() + delay, selfmsg);
                    lastProtectedIn += delay;

                    EV_INFO << "IPsec IN ACCEPT AH (delaying by: " << delay << "s), packet: " << ingressPacketSelector.str() << std::endl;
                    return INetfilter::IHook::QUEUE;
                }
                else {
                    EV_INFO << "IPsec IN ACCEPT AH, packet: " << ingressPacketSelector.str() << std::endl;
                    return INetfilter::IHook::ACCEPT;
                }
            }


        }
        delete ah;

        if (sadEntry == nullptr) {
            EV_INFO << "IPsec IN DROP AH, no matching rule, packet: " << ingressPacketSelector.str() << std::endl;
            return INetfilter::IHook::DROP;
        }

    }

    transportProtocol  = ipv4datagram->getTransportProtocol();


    if(transportProtocol == IP_PROT_ESP)
    {
        IPESP* esp = (IPESP*)ipv4datagram->decapsulate();

        rule.setProcessing(IPsecRule::ProcessingAlgs::pESP);
        sadEntry = sadModule->findEntry(&rule, esp->getSpi());

        if(sadEntry != nullptr){
            //found SA
            //TODO ICV verification

            ipv4datagram->encapsulate(esp->decapsulate());
            ipv4datagram->setTransportProtocol(esp->getNextHeader());

            delay += espProtectInDelay;

            delete esp;
            if (delay > 0 || lastProtectedIn > simTime()) {
                cMessage *selfmsg = new cMessage("IPsecProtectInDelay");
                selfmsg->setContextPointer(datagram);
                delay = std::max(delay, delay + lastProtectedIn.dbl());
                scheduleAt(simTime() + delay, selfmsg);
                lastProtectedIn += delay;

                EV_INFO << "IPsec IN ACCEPT ESP (delaying by: " << delay << "s), packet: " << ingressPacketSelector.str() << std::endl;
                return INetfilter::IHook::QUEUE;
            }
            else {
                EV_INFO << "IPsec IN ACCEPT ESP, packet: " << ingressPacketSelector.str() << std::endl;
                return INetfilter::IHook::ACCEPT;
            }


        }

        delete esp;

        if(sadEntry == nullptr)
        {
            //TODO EV_INFO << "Received ESP protected packet without matching SPI in SAD";
            EV_INFO << "IPsec IN DROP ESP, no matching rule, packet: " << ingressPacketSelector.str() << std::endl;
            return INetfilter::IHook::DROP;
        }

    }

    SPDEntry *spdEntry = spdModule->findEntry(IPsecRule::Direction::dIN, &ingressPacketSelector);
    if (spdEntry != nullptr) {

        if (spdEntry->getAction() == IPsecRule::Action::aBYPASS) {
            EV_INFO << "IPsec BYPASS rule, packet: " << ingressPacketSelector.str() << std::endl;
            return INetfilter::IHook::ACCEPT;
        }
        else {
            EV_INFO << "IPsec DROP rule, packet: " << ingressPacketSelector.str() << std::endl;
            return INetfilter::IHook::DROP;
        }

    }


    EV_INFO << "IPsec IN DROP, no matching rule, packet: " << ingressPacketSelector.str() << std::endl;
    return INetfilter::IHook::DROP;
}

INetfilter::IHook::Result IPsec::datagramLocalOutHook(INetworkDatagram *datagram, const InterfaceEntry *& outIE, L3Address& nextHopAddr)
{
// first for packet from Upper layer  (UDP/TCP) maybe ICMP as well?

    //packet does not have srcAddress yet
    return INetfilter::IHook::ACCEPT;

}


} //namespace
