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

namespace inet {

class INET_API DmprInterfaceData : public InterfaceProtocolData
{
  private:
    double congestionLevel;
    double inUseCongLevel;
    int packetCount;

    simtime_t lastChange;

  public:

    DmprInterfaceData();
    virtual ~DmprInterfaceData();
    virtual std::string str() const override;
    virtual std::string detailedInfo() const override;
    double getCongestionLevel() const;
    void setCongestionLevel(double congestionLevel);
    void incCongestionLevel();
    int getPacketCount() const;
    void setPacketCount(int packetCount);
    void incPacketCount();
    double getInUseCongLevel() const;
    void setInUseCongLevel(double inUseCongLevel);
    const simtime_t& getLastChange() const;
    void setLastChange(const simtime_t& lastChange);

};

} /* namespace inet */

#endif /* INET_ROUTING_DMPR_DMPRINTERFACEDATA_H_ */
