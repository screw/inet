//
// Copyright (C) 2006 Andras Babos and Andras Varga
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

#ifndef __INET_OSPFROUTINGTABLEENTRY_H
#define __INET_OSPFROUTINGTABLEENTRY_H

#include <memory.h>

#include "inet/common/INETDefs.h"

#include "inet/networklayer/contract/IInterfaceTable.h"
#include "inet/networklayer/ipv4/IIpv4RoutingTable.h"
#include "inet/routing/ospfv2/router/Lsa.h"
#include "inet/routing/ospfv2/router/OspfCommon.h"
#include "inet/routing/ospfv2/OspfPacket_m.h"

namespace inet {

namespace ospf {

class INET_API RoutingTableEntry : public Ipv4Route
{
  public:
    enum RoutingPathType {
        INTRAAREA = 0,
        INTERAREA = 1,
        TYPE1_EXTERNAL = 2,
        TYPE2_EXTERNAL = 3
    };

    typedef unsigned char RoutingDestinationType;

    // destinationType bitfield values
    static const unsigned char NETWORK_DESTINATION = 0;
    static const unsigned char AREA_BORDER_ROUTER_DESTINATION = 1;
    static const unsigned char AS_BOUNDARY_ROUTER_DESTINATION = 2;

  private:
    IInterfaceTable *ift = nullptr;
    RoutingDestinationType destinationType = 0;
    OspfOptions optionalCapabilities;
    AreaId area;
    RoutingPathType pathType = static_cast<RoutingPathType>(-1);
    Metric cost = 0;
    Metric type2Cost = 0;
    const OspfLsa *linkStateOrigin = nullptr;
    std::vector<NextHop> nextHops;
    int lastNextHopIndex = 0;
    bool hasDmpr;
    // Ipv4Route::interfacePtr comes from nextHops[0].ifIndex
    // Ipv4Route::gateway is nextHops[0].hopAddress

  public:
    RoutingTableEntry(IInterfaceTable *ift);
    RoutingTableEntry(const RoutingTableEntry& entry);
    virtual ~RoutingTableEntry() {}

    bool operator==(const RoutingTableEntry& entry) const;
    bool operator!=(const RoutingTableEntry& entry) const { return !((*this) == entry); }

    void setDestinationType(RoutingDestinationType type) { destinationType = type; }
    RoutingDestinationType getDestinationType() const { return destinationType; }
    void setOptionalCapabilities(OspfOptions options) { optionalCapabilities = options; }
    OspfOptions getOptionalCapabilities() const { return optionalCapabilities; }
    void setArea(AreaId source) { area = source; }
    AreaId getArea() const { return area; }
    void setPathType(RoutingPathType type);
    RoutingPathType getPathType() const { return pathType; }
    void setCost(Metric pathCost);
    Metric getCost() const { return cost; }
    void setType2Cost(Metric pathCost);
    Metric getType2Cost() const { return type2Cost; }
    void setLinkStateOrigin(const OspfLsa *lsa) { linkStateOrigin = lsa; }
    const OspfLsa *getLinkStateOrigin() const { return linkStateOrigin; }
    void addNextHop(NextHop hop);
    void clearNextHops() { nextHops.clear(); }
    void removeNextHop(unsigned int index) {nextHops.erase(nextHops.begin() + index); }
    unsigned int getNextHopCount() const { return nextHops.size(); }
    NextHop getNextHop(unsigned int index) const { return nextHops[index]; }
    void setNextHop(unsigned int index, NextHop nextHop) { nextHops[index] = nextHop; }

    /** Next hop address */
       Ipv4Address getGateway() const;

       /** Next hop interface */
       InterfaceEntry *getInterface() const;
    bool isHasDmpr() const;
    void setHasDmpr(bool hasDmpr);

    int getLastNextHopIndex() const
    {
      return lastNextHopIndex;
    }

    void setLastNextHopIndex(int lastNextHopIndex = 0)
    {
      this->lastNextHopIndex = lastNextHopIndex;
    }
};

std::ostream& operator<<(std::ostream& out, const RoutingTableEntry& entry);

} // namespace ospf

} // namespace inet

#endif // ifndef __INET_OSPFROUTINGTABLEENTRY_H

