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

#ifndef __INET_IPSEC_H_
#define __INET_IPSEC_H_

#include "inet/common/INETDefs.h"

#include "inet/networklayer/contract/INetfilter.h"


#include "inet/common/ModuleAccess.h"
#include "inet/networklayer/ipv4/IPv4.h"
#include "inet/networklayer/ipv4/ipsec/SAD.h"
#include "inet/networklayer/ipv4/ipsec/SPD.h"



namespace inet {
/**
 * TODO - Generated class
 */
class INET_API IPsec : public cSimpleModule, INetfilter::IHook
{
  private:
//    IIpv4RoutingTable *routingTable = nullptr;
//    IInterfaceTable *ift = nullptr;
//    INetfilter *networkProtocol = nullptr;
    IPv4 *ipLayer;
    SPD *spdModule;
    SAD *sadModule;

    double ahProtectOutDelay;
    double ahProtectInDelay;

    double espProtectOutDelay;
    double espProtectInDelay;

    simtime_t lastProtectedIn = 0;
    simtime_t lastProtectedOut = 0;




    void initSelectorFromIngressPacket(IPv4Datagram *ipv4datagram,
            IPsecSelector *ingressPacketSelector);
    void initSelectorFromEgressPacket(IPv4Datagram *ipv4datagram,
                IPsecSelector *egressPacketSelector);
    cPacket* espProtect(cPacket *transport, SADEntry *sadEntry, int transportType);
    cPacket* ahProtect(cPacket *transport, SADEntry *sadEntry, int transportType);
    void initSecurityDBs(omnetpp::cXMLElement *spdConfig);
    INetfilter::IHook::Result protectDatagram(IPv4Datagram* ipv4datagram, IPsecSelector* egressPacketSelector, SPDEntry *spdEntry);

  public:
    IPsec();
    virtual ~IPsec();

  protected:
    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage *msg) override;
    virtual int numInitStages() const override { return NUM_INIT_STAGES; }

    /* Netfilter hooks */
    IHook::Result datagramPreRoutingHook(INetworkDatagram *datagram, const InterfaceEntry *inIE, const InterfaceEntry *& outIE, L3Address& nextHopAddr) override;
        IHook::Result datagramForwardHook(INetworkDatagram *datagram, const InterfaceEntry *inIE, const InterfaceEntry *& outIE, L3Address& nextHopAddr) override;
        IHook::Result datagramPostRoutingHook(INetworkDatagram *datagram, const InterfaceEntry *inIE, const InterfaceEntry *& outIE, L3Address& nextHopAddr) override;
        IHook::Result datagramLocalInHook(INetworkDatagram *datagram, const InterfaceEntry *inIE) override;
        IHook::Result datagramLocalOutHook(INetworkDatagram *datagram, const InterfaceEntry *& outIE, L3Address& nextHopAddr) override;


};

} //namespace

#endif // ifndef __INET_IPSEC_H_
