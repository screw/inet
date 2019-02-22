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



class DmprInterfaceData;

namespace inet {

/**
 * TODO - Generated class
 */
class Dmpr : public cSimpleModule, public NetfilterBase::HookBase
{
  private:
    IIpv4RoutingTable *routingTable = nullptr;
    IInterfaceTable *interfaceTable = nullptr;
    INetfilter *networkProtocol = nullptr;
    DmprForwardingTable *forwardingTable = nullptr;

    double alpha;
    double interval;

  protected:
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
    virtual int numInitStages() const override { return NUM_INIT_STAGES; }

    /* Netfilter hooks */
    virtual Result datagramPreRoutingHook(Packet *datagram) override;
    virtual Result datagramForwardHook(Packet *datagram) override;
    virtual Result datagramPostRoutingHook(Packet *datagram) override;
    virtual Result datagramLocalInHook(Packet *datagram) override;
    virtual Result datagramLocalOutHook(Packet *datagram) override;
    void updateCongestionLevel(int ece, DmprInterfaceData* dmprData, Ipv4Address srcIp, int interfaceId );
    simsignal_t registerSignal(std::stringstream title, std::stringstream name, std::stringstream interfaceName, Ipv4Address destination);

  public:
    void emitSignal(simsignal_t signal, double value);

//    // Lifecycle
//    virtual bool handleOperationStage(LifecycleOperation *operation, int stage, IDoneCallback *doneCallback) override;
    double getInterval() const;
    void setInterval(double interval);
};

} //namespace

#endif
