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

Define_Module(DmprForwardingTable);

void DmprForwardingTable::initialize()
{
    // TODO - Generated method body
}

void DmprForwardingTable::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
}

bool DmprForwardingTable::isInCache(Socket socket)
{

  auto it = forwardingCache.find(socket);
  if(it!= forwardingCache.end())
  {
    return true;
  }

  return false;
}

DmprForwardingTable::NextHopInterface DmprForwardingTable::getNextHopForSocket(Socket socket)
{
  auto it = forwardingCache.find(socket);
  if(it != forwardingCache.end())
  {
    return it->second;
  }
  else
  {
    NextHopInterface nextHop;
    nextHop.nextHop = Ipv4Address::ALLONES_ADDRESS;
    return nextHop;
  }
}

void DmprForwardingTable::insertEntry(Socket socket, NextHopInterface nextHopInterface)
{
  forwardingCache.insert( std::pair<Socket, NextHopInterface>(socket, nextHopInterface));

}

} //namespace


