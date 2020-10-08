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
#include "inet/networklayer/ipv4/ipsec/SecurityAssociationDatabase.h"
#include "inet/networklayer/ipv4/ipsec/SecurityPolicyDatabase.h"



namespace inet {
namespace ipsec {
/**
 * Implements IPsec for IPv4. Supports AH and ESP in transport mode.
 */
class INET_API IPsec : public cSimpleModule, INetfilter::IHook
{
  private:
    IPv4 *ipLayer;
    SecurityPolicyDatabase *spdModule;
    SecurityAssociationDatabase *sadModule;

    double ahProtectOutDelay;
    double ahProtectInDelay;

    double espProtectOutDelay;
    double espProtectInDelay;

    simtime_t lastProtectedIn = 0;
    simtime_t lastProtectedOut = 0;

    int inAccept = 0;
    int inDrop = 0;
    int inBypass = 0;

    int outProtect = 0;
    int outDrop = 0;
    int outBypass = 0;

  protected:
    static simsignal_t inProtectedAcceptSignal;
    static simsignal_t inProtectedDropSignal;
    static simsignal_t inUnprotectedBypassSignal;
    static simsignal_t inUnprotectedDropSignal;

    static simsignal_t outBypassSignal;
    static simsignal_t outProtectSignal;
    static simsignal_t outDropSignal;

    static simsignal_t inProcessDelaySignal;
    static simsignal_t outProcessDelaySignal;


  private:
    void initSecurityDBs(omnetpp::cXMLElement *spdConfig);

    void initSelectorFromIngressPacket(IPv4Datagram *ipv4datagram, IPsecSelector *ingressPacketSelector);
    void initSelectorFromEgressPacket(IPv4Datagram *ipv4datagram, IPsecSelector *egressPacketSelector);

    cPacket* espProtect(cPacket *transport, SecurityAssociationDatabase::Entry *sadEntry, int transportType);
    cPacket* ahProtect(cPacket *transport, SecurityAssociationDatabase::Entry *sadEntry, int transportType);

    INetfilter::IHook::Result protectDatagram(IPv4Datagram* ipv4datagram, IPsecSelector* egressPacketSelector, SecurityPolicyDatabase::Entry *spdEntry);

  public:
    IPsec();
    virtual ~IPsec();

  protected:
    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage *msg) override;
    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
    void refreshDisplay() const override;

    /* Netfilter hooks */
    IHook::Result datagramPreRoutingHook(INetworkDatagram *datagram, const InterfaceEntry *inIE, const InterfaceEntry *& outIE, L3Address& nextHopAddr) override;
    IHook::Result datagramForwardHook(INetworkDatagram *datagram, const InterfaceEntry *inIE, const InterfaceEntry *& outIE, L3Address& nextHopAddr) override;
    IHook::Result datagramPostRoutingHook(INetworkDatagram *datagram, const InterfaceEntry *inIE, const InterfaceEntry *& outIE, L3Address& nextHopAddr) override;
    IHook::Result datagramLocalInHook(INetworkDatagram *datagram, const InterfaceEntry *inIE) override;
    IHook::Result datagramLocalOutHook(INetworkDatagram *datagram, const InterfaceEntry *& outIE, L3Address& nextHopAddr) override;


};

} //ipsec namespace
} //namespace

#endif // ifndef __INET_IPSEC_H_
