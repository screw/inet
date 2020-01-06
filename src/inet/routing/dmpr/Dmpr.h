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

#ifndef __INET_DMPR_H_
#define __INET_DMPR_H_

#include "inet/common/INETDefs.h"

//#include "inet/routing/dmpr/DmprInterfaceData.h"

#include "inet/networklayer/contract/IInterfaceTable.h"
#include "inet/networklayer/contract/IRoutingTable.h"
#include "inet/networklayer/ipv4/IIpv4RoutingTable.h"
#include "inet/networklayer/contract/INetfilter.h"
//#include "inet/common/lifecycle/ILifecycle.h"
#include "inet/common/ModuleAccess.h"

#include "inet/routing/dmpr/DmprForwardingTable.h"
#include "inet/routing/ospfv2/router/Ospfv2RoutingTableEntry.h"




class DmprInterfaceData;

namespace inet {

/**
 * TODO - Generated class
 */
class Dmpr : public cSimpleModule, public NetfilterBase::HookBase
{
  private:
    IIpv4RoutingTable *routingTable = nullptr;
    IInterfaceTable *ift = nullptr;
    INetfilter *networkProtocol = nullptr;
//    DmprForwardingTable *forwardingTable = nullptr;

    double alpha;
    double interval;
    double logPar;
    double min_threshold;

    bool randomNextHopEnabled = true;
    bool lastNextHopIndexEnabled = false;

  protected:
    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage *msg) override;
    virtual int numInitStages() const override { return NUM_INIT_STAGES; }

    /* Netfilter hooks */
    virtual Result datagramPreRoutingHook(Packet *datagram) override;
    virtual Result datagramForwardHook(Packet *datagram) override;
    virtual Result datagramPostRoutingHook(Packet *datagram) override;
    virtual Result datagramLocalInHook(Packet *datagram) override;
    virtual Result datagramLocalOutHook(Packet *datagram) override;
    void updateCongestionLevel(int ece, DmprInterfaceData* dmprData, Ipv4Address srcIp, int interfaceId );
    simsignal_t registerSignal(std::stringstream title, std::stringstream name, std::stringstream interfaceName, Ipv4Address destination);
    void updateFwdCongLevel(int ece, DmprInterfaceData* dmprData, const Ipv4Address& destAddr, int interfaceId,
        Ipv4Route* route);
    void updateIntervalCong(ospfv2::O2NextHop* nextHop, DmprInterfaceData* dmprData);
    void updateNextHop(ospfv2::Ospfv2RoutingTableEntry* route);

  public:
    void emitSignal(simsignal_t signal, double value);
    void registerNextHop(int interfaceId, ospfv2::O2NextHop* nextHop, const ospfv2::Ospfv2RoutingTableEntry* route);

//    // Lifecycle
//    virtual bool handleOperationStage(LifecycleOperation *operation, int stage, IDoneCallback *doneCallback) override;
    double getInterval() const;
    void setInterval(double interval);
    double getAlpha() const;
    void setAlpha(double alpha);
    IIpv4RoutingTable* getRoutingTable() const;
};

} //namespace

#endif
