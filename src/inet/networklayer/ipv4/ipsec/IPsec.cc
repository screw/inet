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

#include "inet/networklayer/ipv4/ipsec/IPsec.h"
#include "inet/common/ModuleAccess.h"
#include "inet/networklayer/ipv4/IPv4Datagram_m.h"
#include "inet/networklayer/ipv4/ipsec/IPAH_m.h"
#include "inet/networklayer/ipv4/ipsec/IPESP_m.h"
#include "inet/transportlayer/tcp_common/TCPSegment.h"
#include "inet/transportlayer/udp/UDPPacket.h"



namespace inet {
namespace ipsec {

simsignal_t IPsec::inProtectedAcceptSignal = registerSignal("inProtectedAccept");
simsignal_t IPsec::inProtectedDropSignal = registerSignal("inProtectedDrop");
simsignal_t IPsec::inUnprotectedBypassSignal = registerSignal("inUnprotectedBypass");
simsignal_t IPsec::inUnprotectedDropSignal = registerSignal("inUnprotectedDrop");

simsignal_t IPsec::outBypassSignal = registerSignal("outBypass");
simsignal_t IPsec::outProtectSignal = registerSignal("outProtect");
simsignal_t IPsec::outDropSignal = registerSignal("outDrop");

simsignal_t IPsec::inProcessDelaySignal = registerSignal("inProcessDelay");
simsignal_t IPsec::outProcessDelaySignal = registerSignal("outProcessDelay");

Define_Module(IPsec);

IPsec::IPsec()
{

}

IPsec::~IPsec()
{

}

void IPsec::initSecurityDBs(cXMLElement *spdConfig)
{
    cXMLElementList spdEntries = spdConfig->getChildrenByTagName("SPDEntry");
    for (auto spdEntriesIt = spdEntries.begin(); spdEntriesIt != spdEntries.end(); spdEntriesIt++) {
        //process one SPDEntry
        SecurityPolicyDatabase::Entry *spdEntry = new SecurityPolicyDatabase::Entry();
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
                            SecurityAssociationDatabase::Entry *sadEntry = new SecurityAssociationDatabase::Entry();
                            sadEntry->setRule(spdEntry->getRule());
                            sadEntry->getRuleForUpdate()->setProcessing(IPsecRule::ProcessingAlgs::pESP);
                            sadEntry->setSpi(spi);
                            sadModule->addEntry(sadEntry);
                            spdEntry->addSADEntry(sadEntry);
                        }
                        else if (spdEntry->getProcessing() == IPsecRule::ProcessingAlgs::pAH || spdEntry->getProcessing() == IPsecRule::ProcessingAlgs::pAH_ESP) {
                            SecurityAssociationDatabase::Entry *sadEntry = new SecurityAssociationDatabase::Entry();
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

        spdModule = getModuleFromPar<SecurityPolicyDatabase>(par("spdModule"), this);
        sadModule = getModuleFromPar<SecurityAssociationDatabase>(par("sadModule"), this);

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
    SecurityPolicyDatabase::Entry *spdEntry = spdModule->findEntry(IPsecRule::Direction::dOUT, &egressPacketSelector);
    if(spdEntry != nullptr){
        if (spdEntry->getAction() == IPsecRule::Action::aPROTECT) {
            emit(outProtectSignal, 1L);
            outProtect++;
            return protectDatagram(ipv4datagram, &egressPacketSelector, spdEntry);
        }
        else if (spdEntry->getAction() == IPsecRule::Action::aBYPASS) {

            EV_INFO << "IPsec OUT BYPASS rule, packet: " << egressPacketSelector.str() << std::endl;
            emit(outBypassSignal, 1L);
            outBypass++;
            return INetfilter::IHook::ACCEPT;
        }
        else if (spdEntry->getAction() == IPsecRule::Action::aDISCARD) {
            EV_INFO << "IPsec OUT DROP rule, packet: " << egressPacketSelector.str() << std::endl;
            emit(outDropSignal, 1L);
            outDrop++;
            return INetfilter::IHook::DROP;
        }

    }
    EV_INFO << "IPsec OUT BYPASS, no matching rule, packet: " << egressPacketSelector.str() << std::endl;
    emit(outDropSignal, 1L);
    outDrop++;
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

cPacket* IPsec::espProtect(cPacket *transport, SecurityAssociationDatabase::Entry* sadEntry, int transportType)
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

cPacket* IPsec::ahProtect(cPacket* transport, SecurityAssociationDatabase::Entry* sadEntry, int transportType)
{

    IPAH* ahPacket = new IPAH();
    ahPacket->setByteLength(IP_AH_HEADER_BYTES);
    ahPacket->setSequenceNumber(sadEntry->getAndIncSeqNum());

    ahPacket->setSpi(sadEntry->getSpi());
    ahPacket->encapsulate(transport);
    ahPacket->setNextHeader(transportType);

    return ahPacket;

}

INetfilter::IHook::Result IPsec::protectDatagram(IPv4Datagram  *ipv4datagram, IPsecSelector *egressPacketSelector, SecurityPolicyDatabase::Entry  *spdEntry )
{
    Enter_Method_Silent();
    cPacket* transport = ipv4datagram->decapsulate();
    double delay = 0;

    for (auto saIt = spdEntry->entries.begin(); saIt != spdEntry->entries.end(); saIt++) {
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
        lastProtectedOut = std::max(simTime(), lastProtectedOut) + delay;
        scheduleAt(lastProtectedOut, selfmsg);

        simtime_t actualDelay = lastProtectedOut - simTime();
        EV_INFO << "IPsec OUT PROTECT (delaying by: " << actualDelay << "s), packet: " << egressPacketSelector->str() << std::endl;
        emit(outProcessDelaySignal, actualDelay.dbl());

        return INetfilter::IHook::QUEUE;
    }
    else{
        EV_INFO << "IPsec OUT PROTECT packet: " << egressPacketSelector->str() << std::endl;
        emit(outProcessDelaySignal, 0.0);
        return INetfilter::IHook::ACCEPT;
    }
}

INetfilter::IHook::Result IPsec::datagramLocalInHook(INetworkDatagram *datagram, const InterfaceEntry *inIE)
{
    Enter_Method_Silent();

    IPv4Datagram* ipv4datagram = (IPv4Datagram*)datagram;
    int transportProtocol  = ipv4datagram->getTransportProtocol();

    IPsecSelector ingressPacketSelector;
    initSelectorFromIngressPacket(ipv4datagram, &ingressPacketSelector);

    IPsecRule rule;
    rule.setDirection(IPsecRule::Direction::dIN);

    SecurityAssociationDatabase::Entry *sadEntry;

    double delay = 0.0;
    if (transportProtocol == IP_PROT_AH) {
        IPAH* ah = (IPAH*)ipv4datagram->decapsulate();
        //FIND SA Entry in SAD

        rule.setProcessing(IPsecRule::ProcessingAlgs::pAH);
        sadEntry = sadModule->findEntry(&rule, ah->getSpi());

        if(sadEntry != nullptr){

            //found SA
            ipv4datagram->encapsulate(ah->decapsulate());
            ipv4datagram->setTransportProtocol(ah->getNextHeader());

            delay = ahProtectInDelay;

            if (ah->getNextHeader() != IP_PROT_ESP) {

                delete ah;

                emit(inProtectedAcceptSignal, 1L);
                inAccept++;

                if (delay > 0 || lastProtectedIn > simTime()) {
                    cMessage *selfmsg = new cMessage("IPsecProtectInDelay");
                    selfmsg->setContextPointer(datagram);
                    lastProtectedIn = std::max(simTime(), lastProtectedIn) + delay;
                    scheduleAt(lastProtectedIn, selfmsg);
                    simtime_t actualDelay = lastProtectedIn - simTime();

                    EV_INFO << "IPsec IN ACCEPT AH (delaying by: " << actualDelay << "s), packet: " << ingressPacketSelector.str() << std::endl;
                    emit(inProcessDelaySignal, actualDelay.dbl());
                    return INetfilter::IHook::QUEUE;
                }
                else {
                    EV_INFO << "IPsec IN ACCEPT AH, packet: " << ingressPacketSelector.str() << std::endl;
                    emit(inProcessDelaySignal, 0.0);
                    return INetfilter::IHook::ACCEPT;
                }
            }

        }
        delete ah;

        if (sadEntry == nullptr) {
            EV_INFO << "IPsec IN DROP AH, no matching rule, packet: " << ingressPacketSelector.str() << std::endl;
            emit(inProtectedDropSignal, 1L);
            inDrop++;
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
            emit(inProtectedAcceptSignal, 1L);
            inAccept++;

            ipv4datagram->encapsulate(esp->decapsulate());
            ipv4datagram->setTransportProtocol(esp->getNextHeader());

            delay += espProtectInDelay;

            delete esp;
            if (delay > 0 || lastProtectedIn > simTime()) {
                cMessage *selfmsg = new cMessage("IPsecProtectInDelay");
                selfmsg->setContextPointer(datagram);
                lastProtectedOut = std::max(simTime(), lastProtectedOut) + delay;
                scheduleAt(lastProtectedOut, selfmsg);
                simtime_t actualDelay = lastProtectedOut - simTime();

                EV_INFO << "IPsec IN ACCEPT ESP (delaying by: " << actualDelay << "s), packet: " << ingressPacketSelector.str() << std::endl;
                emit(inProcessDelaySignal, actualDelay.dbl());
                return INetfilter::IHook::QUEUE;
            }
            else {
                EV_INFO << "IPsec IN ACCEPT ESP, packet: " << ingressPacketSelector.str() << std::endl;
                emit(inProcessDelaySignal, 0.0);
                return INetfilter::IHook::ACCEPT;
            }
        }

        delete esp;

        if(sadEntry == nullptr)
        {
            EV_INFO << "IPsec IN DROP ESP, no matching rule, packet: " << ingressPacketSelector.str() << std::endl;
            emit(inProtectedDropSignal, 1L);
            inDrop++;
            return INetfilter::IHook::DROP;
        }

    }

    SecurityPolicyDatabase::Entry *spdEntry = spdModule->findEntry(IPsecRule::Direction::dIN, &ingressPacketSelector);
    if (spdEntry != nullptr) {

        if (spdEntry->getAction() == IPsecRule::Action::aBYPASS) {
            EV_INFO << "IPsec BYPASS rule, packet: " << ingressPacketSelector.str() << std::endl;
            emit(inUnprotectedBypassSignal, 1L);
            inBypass++;
            return INetfilter::IHook::ACCEPT;
        }
        else {
            EV_INFO << "IPsec DROP rule, packet: " << ingressPacketSelector.str() << std::endl;
            emit(inUnprotectedDropSignal, 1L);
            inDrop++;
            return INetfilter::IHook::DROP;
        }

    }


    EV_INFO << "IPsec IN DROP, no matching rule, packet: " << ingressPacketSelector.str() << std::endl;
    emit(inUnprotectedDropSignal, 1L);
    inDrop++;
    return INetfilter::IHook::DROP;
}

INetfilter::IHook::Result IPsec::datagramLocalOutHook(INetworkDatagram *datagram, const InterfaceEntry *& outIE, L3Address& nextHopAddr)
{
    return INetfilter::IHook::ACCEPT;
}

void IPsec::refreshDisplay() const
{
    char buf[80];
    sprintf(buf, "IN: ACCEPT: %d DROP: %d BYPASS: %d\n OUT: PROTECT: %d DROP: %d BYPASS: %d", inAccept, inDrop, inBypass, outProtect, outDrop, outBypass );
    getDisplayString().setTagArg("t", 0, buf);
}

} //ipsec namespace
} //namespace
