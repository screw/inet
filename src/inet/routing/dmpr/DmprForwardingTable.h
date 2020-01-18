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

#ifndef __INET_DMPRFORWARDINGTABLE_H_
#define __INET_DMPRFORWARDINGTABLE_H_

#include <map>

#include "inet/common/INETDefs.h"

#include "inet/common/Protocol.h"
#include "inet/networklayer/contract/ipv4/Ipv4Address.h"
#include "inet/networklayer/common/IpProtocolId_m.h"
//#include "inet/networklayer/ipv4/Ipv4Route.h"
#include "inet/routing/ospfv2/router/Ospfv2RoutingTableEntry.h"


namespace inet {

/**
 * TODO - Generated class
 */
class DmprForwardingTable : public cObject
{
  public:
      typedef struct Socket{
        Ipv4Address srcIp;
        Ipv4Address dstIp;
        int srcPort;
        int dstPort;
        IpProtocolId protocol;

        bool operator<(const Socket& socket2) const
        {
          if (srcIp < socket2.srcIp)
          {
            return true;
          }
          else if (srcIp == socket2.srcIp)
          {
            if (dstIp < socket2.dstIp)
            {
              return true;
            }
            else if (dstIp == socket2.dstIp)
            {
              if (srcPort < socket2.srcPort)
              {
                return true;
              }
              else if (srcPort == socket2.srcPort)
              {
                if (dstPort < socket2.dstPort)
                {
                  return true;
                }
                else if (dstPort == socket2.dstPort)
                {
                  if (dstPort < socket2.dstPort)
                  {
                    return true;
                  }
                  else if (dstPort == socket2.dstPort)
                  {
                    if (protocol < socket2.protocol)
                    {
                      return true;
                    }
                  }
                }
              }
            }
          }
          return false;
        }
        std::string str() const
        {
        std::stringstream out;
        out << srcIp << ":" << srcPort << " > " << dstIp << ":" << dstPort << "\n";
        return out.str();
        }

      } Socket;


      typedef struct NextHopInterface{
          Ipv4Address nextHop;
          int interfaceId;
      }NextHopInterface;

      typedef std::map<Socket, NextHopInterface> ForwardingCache;


      class OspfRouteLessThan
      {
          const DmprForwardingTable &c;
        public:
          OspfRouteLessThan(const DmprForwardingTable& c) : c(c) {}
          bool operator () (const ospfv2::Ospfv2RoutingTableEntry *a, const ospfv2::Ospfv2RoutingTableEntry *b) { return c.ospfRouteLessThan(a, b); }
      };
      bool ospfRouteLessThan(const ospfv2::Ospfv2RoutingTableEntry *a, const ospfv2::Ospfv2RoutingTableEntry *b) const;



  protected:


    typedef std::vector<ospfv2::Ospfv2RoutingTableEntry *> RouteVector;
    RouteVector routes;
    typedef std::map<Ipv4Address, ospfv2::Ospfv2RoutingTableEntry *> RoutingCache;


    mutable RoutingCache routingCache;



//    ForwardingCache forwardingCache;



//  protected:
//    virtual void initialize();
//    virtual void handleMessage(cMessage *msg);


  public:
    bool isInCache(Socket socket);
    NextHopInterface getNextHopForSocket(Socket socket);
    void insertEntry(Ipv4Address address, ospfv2::Ospfv2RoutingTableEntry* entry);

    ospfv2::Ospfv2RoutingTableEntry *findBestMatchingRoute(const Ipv4Address& dest) const;
};

} //namespace

#endif
