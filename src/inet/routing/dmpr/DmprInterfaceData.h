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

#ifndef INET_ROUTING_DMPR_DMPRINTERFACEDATA_H_
#define INET_ROUTING_DMPR_DMPRINTERFACEDATA_H_

#include "inet/common/INETDefs.h"
#include "inet/networklayer/common/InterfaceEntry.h"

#include "inet/routing/dmpr/Dmpr.h"

namespace inet {

class Dmpr;

class INET_API DmprInterfaceData : public InterfaceProtocolData
{
  private:

//    double congLevel;
//    double fwdCongLevel; //forwarded Congestion level
//    double inUseCongLevel;
//    int packetCount;

//    simtime_t lastChange;

//    simsignal_t signalCongLevel;
//    simsignal_t signalInUseCongLevel;

  public:
    Dmpr* dmpr;
    DmprForwardingTable* table;
//    IIpv4RoutingTable *table = nullptr;


  public:

    DmprInterfaceData();
    DmprInterfaceData(Dmpr* dmpr);

    virtual ~DmprInterfaceData();
    virtual std::string str() const override;
    virtual std::string detailedInfo() const;

//    double getCongestionLevel() const;
//    void setCongestionLevel(double congestionLevel);
//    int getPacketCount() const;
//    void setPacketCount(int packetCount);
//    void incPacketCount();
//    double getInUseCongLevel() const;
//    void setInUseCongLevel(double inUseCongLevel);
//    const simtime_t& getLastChange() const;
//    void setLastChange(const simtime_t& lastChange);
//    simsignal_t getSignalCongLevel() const;
//    simsignal_t getSignalInUseCongLevel() const;
//    void setSignalCongLevel(simsignal_t signalCongLevel);
//    void setSignalInUseCongLevel(simsignal_t signalInUseCongLevel);
//    double getFwdCongLevel() const;
//    void setFwdCongLevel(double fwdCongLevel);

};

} /* namespace inet */

#endif /* INET_ROUTING_DMPR_DMPRINTERFACEDATA_H_ */
