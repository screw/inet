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

#include "inet/routing/dmpr/DmprForwardingTable.h"

namespace inet {

//Define_Module(DmprForwardingTable);

//void DmprForwardingTable::initialize()
//{
//    // TODO - Generated method body
//}
//
//void DmprForwardingTable::handleMessage(cMessage *msg)
//{
//    // TODO - Generated method body
//}

bool DmprForwardingTable::isInCache(Socket socket)
{

//  auto it = forwardingCache.find(socket);
//  if(it!= forwardingCache.end())
//  {
//    return true;
//  }

  return false;
}

//DmprForwardingTable::NextHopInterface DmprForwardingTable::getNextHopForSocket(Socket socket)
//{
//  auto it = forwardingCache.find(socket);
//  if(it != forwardingCache.end())
//  {
//    return it->second;
//  }
//  else
//  {
//    NextHopInterface nextHop;
//    nextHop.nextHop = Ipv4Address::ALLONES_ADDRESS;
//    return nextHop;
//  }
//}

void DmprForwardingTable::insertEntry(Ipv4Address address, ospfv2::Ospfv2RoutingTableEntry* entry)
{
//  forwardingCache.insert( std::pair<Socket, NextHopInterface>(socket, nextHopInterface));

//  auto pos = upper_bound(routes.begin(), routes.end(), entry, RouteLessThan(*this));

  auto pos = upper_bound(routes.begin(), routes.end(), entry, OspfRouteLessThan(*this));
  routes.insert(pos, entry);

}

ospfv2::Ospfv2RoutingTableEntry *DmprForwardingTable::findBestMatchingRoute(const Ipv4Address& dest) const
{
//    Enter_Method("findBestMatchingRoute(%u.%u.%u.%u)", dest.getDByte(0), dest.getDByte(1), dest.getDByte(2), dest.getDByte(3));    // note: str().c_str() too slow here

//    auto it = routingCache.find(dest);
//    if (it != routingCache.end()) {
//        if (it->second == nullptr || it->second->isValid())
//            return it->second;
//    }

    // find best match (one with longest prefix)
    // default route has zero prefix length, so (if exists) it'll be selected as last resort
  ospfv2::Ospfv2RoutingTableEntry *bestRoute = nullptr;
    for (auto e : routes) {
        if (e->isValid()) {
            if (Ipv4Address::maskedAddrAreEqual(dest, e->getDestination(), e->getNetmask())) {    // match
                bestRoute = const_cast<ospfv2::Ospfv2RoutingTableEntry *>(e);
                break;
            }
        }
    }

//    routingCache[dest] = bestRoute;
    return bestRoute;
}

bool DmprForwardingTable::ospfRouteLessThan(const ospfv2::Ospfv2RoutingTableEntry *a, const ospfv2::Ospfv2RoutingTableEntry *b) const
{
    // longer prefixes are better, because they are more specific
    if (a->getNetmask() != b->getNetmask())
        return a->getNetmask() > b->getNetmask();

    if (a->getDestination() != b->getDestination())
        return a->getDestination() < b->getDestination();

    // for the same destination/netmask:

//    // smaller administration distance is better (if useAdminDist)
//    if (useAdminDist && (a->getAdminDist() != b->getAdminDist()))
//        return a->getAdminDist() < b->getAdminDist();

    // smaller metric is better
    return a->getMetric() < b->getMetric();
}

} //namespace


