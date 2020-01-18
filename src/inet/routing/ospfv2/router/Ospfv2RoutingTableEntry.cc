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

#include "inet/routing/ospfv2/router/Ospfv2RoutingTableEntry.h"
#include "inet/routing/dmpr/DmprInterfaceData.h"

namespace inet {

namespace ospfv2 {

Ospfv2RoutingTableEntry::Ospfv2RoutingTableEntry(IInterfaceTable *_ift) :
    ift(_ift),
    destinationType(Ospfv2RoutingTableEntry::NETWORK_DESTINATION),
    area(BACKBONE_AREAID),
    pathType(Ospfv2RoutingTableEntry::INTRAAREA)
{
    setNetmask(Ipv4Address::ALLONES_ADDRESS);
    setSourceType(IRoute::OSPF);
    setAdminDist(Ipv4Route::dOSPF);
}

Ospfv2RoutingTableEntry::Ospfv2RoutingTableEntry(const Ospfv2RoutingTableEntry& entry) :
    ift(entry.ift),
    destinationType(entry.destinationType),
    optionalCapabilities(entry.optionalCapabilities),
    area(entry.area),
    pathType(entry.pathType),
    cost(entry.cost),
    type2Cost(entry.type2Cost),
    linkStateOrigin(entry.linkStateOrigin),
    nextHops(entry.nextHops),
    hasDmpr(entry.hasDmpr)
{
    setDestination(entry.getDestination());
    setNetmask(entry.getNetmask());
    setInterface(entry.getInterface());
    setGateway(entry.getGateway());
    setSourceType(entry.getSourceType());
    setMetric(entry.getMetric());
    setAdminDist(entry.getAdminDist());
}

void Ospfv2RoutingTableEntry::addNextHop(NextHop hop)
{
    if (nextHops.size() == 0) {
        InterfaceEntry *routingInterface = ift->getInterfaceById(hop.ifIndex);

        setInterface(routingInterface);
        // TODO: this used to be commented out, but it seems we need it
        // otherwise gateways will never be filled in and gateway is needed for broadcast networks
        setGateway(hop.hopAddress);
    }else{
      for(int i= 0; i < nextHops.size(); i++){
        if(hop.ifIndex == nextHops.at(i).ifIndex && hop.hopAddress == nextHops.at(i).hopAddress){
          return;
        }
      }
    }
    nextHops.push_back(hop);
}

bool Ospfv2RoutingTableEntry::operator==(const Ospfv2RoutingTableEntry& entry) const
{
    unsigned int hopCount = nextHops.size();
    unsigned int i = 0;

    if (hopCount != entry.nextHops.size()) {
        return false;
    }
    for (i = 0; i < hopCount; i++) {
        if ((nextHops[i] != entry.nextHops[i])) {
            return false;
        }
    }

    return (destinationType == entry.destinationType) &&
           (getDestination() == entry.getDestination()) &&
           (getNetmask() == entry.getNetmask()) &&
           (optionalCapabilities == entry.optionalCapabilities) &&
           (area == entry.area) &&
           (pathType == entry.pathType) &&
           (cost == entry.cost) &&
           (type2Cost == entry.type2Cost) &&
           (linkStateOrigin == entry.linkStateOrigin);
}

std::ostream& operator<<(std::ostream& out, const Ospfv2RoutingTableEntry& entry)
{
    if (entry.getDestination().isUnspecified())
        out << "0.0.0.0";
    else
        out << entry.getDestination();
    out << "/";
    if (entry.getNetmask().isUnspecified())
        out << "0";
    else
        out << entry.getNetmask().getNetmaskLength();
    out << " nextHops: ";
    for (unsigned int i = 0; i < entry.getNextHopCount(); i++) {
        Ipv4Address gateway = entry.getNextHop(i).hopAddress;
        if (gateway.isUnspecified())
            out << "*  ";
        else
            out << gateway << "  ";
    }
    out << "cost: " << entry.getCost() << " ";
    if(entry.getPathType() == Ospfv2RoutingTableEntry::TYPE2_EXTERNAL)
        out << "type2Cost: " << entry.getType2Cost() << " ";
    out << "if: " << entry.getInterfaceName() << " ";
    out << "destType: " << Ospfv2RoutingTableEntry::getDestinationTypeString(entry.getDestinationType());
    out << " area: " << entry.getArea().str(false) << " ";
    out << "pathType: " << Ospfv2RoutingTableEntry::getPathTypeString(entry.getPathType()) << " ";
    out << "Origin: [" << entry.getLinkStateOrigin()->getHeader() << "] ";

    return out;
}

std::string Ospfv2RoutingTableEntry::str() const
{
    std::stringstream out;
    out << getSourceTypeAbbreviation();
    out << " ";
    if (getDestination().isUnspecified())
        out << "0.0.0.0";
    else
        out << getDestination();
    out << "/";
    if (getNetmask().isUnspecified())
        out << "0";
    else
        out << getNetmask().getNetmaskLength();
    out << " gw:";
    if (getGateway().isUnspecified())
        out << "*  ";
    else
        out << getGateway() << "  ";
    if(getRoutingTable() && getRoutingTable()->isAdminDistEnabled())
        out << "AD:" << getAdminDist() << "  ";
    out << "metric:" << getMetric() << "  ";
    out << "if:";
    if (!getInterface())
        out << "*";
    else
        out << getInterfaceName();

    out << " destType:" << Ospfv2RoutingTableEntry::getDestinationTypeString(destinationType)
    << " pathType:" << Ospfv2RoutingTableEntry::getPathTypeString(pathType)
    << " area:" << area.str(false);

    return out.str();
}

const std::string Ospfv2RoutingTableEntry::getDestinationTypeString(RoutingDestinationType destType)
{
    std::stringstream out;

    if (destType == Ospfv2RoutingTableEntry::NETWORK_DESTINATION) {
        out << "Network";
    }
    else {
        if ((destType & Ospfv2RoutingTableEntry::AREA_BORDER_ROUTER_DESTINATION) != 0) {
            out << "AreaBorderRouter";
        }
        if ((destType & Ospfv2RoutingTableEntry::AS_BOUNDARY_ROUTER_DESTINATION) != 0) {
            if ((destType & Ospfv2RoutingTableEntry::AREA_BORDER_ROUTER_DESTINATION) != 0) {
                out << "+";
            }
            out << "ASBoundaryRouter";
        }
    }

    return out.str();
}

const std::string Ospfv2RoutingTableEntry::getPathTypeString(RoutingPathType pathType)
{
    switch (pathType) {
        case Ospfv2RoutingTableEntry::INTRAAREA:
            return "IntraArea";

        case Ospfv2RoutingTableEntry::INTERAREA:
            return "InterArea";

        case Ospfv2RoutingTableEntry::TYPE1_EXTERNAL:
            return "Type1External";

        case Ospfv2RoutingTableEntry::TYPE2_EXTERNAL:
            return "Type2External";
    }

    return "Unknown";
}

/** Next hop interface */
InterfaceEntry *Ospfv2RoutingTableEntry::getInterface() const
{
  return Ipv4Route::getInterface();
//  initstate()
  if(!ift || !hasDmpr || !getDestination().isUnicast() || nextHops.size() == 1){
    return Ipv4Route::getInterface();
  }
//  double congestLevel = INT_MAX;
  Ipv4Address nextHopAddr = Ipv4Address::UNSPECIFIED_ADDRESS;
  NextHop resNextHop, tmpNextHop;
  resNextHop.hopAddress = Ipv4Address::UNSPECIFIED_ADDRESS;
  std::vector<NextHop> resNextHops;

  int count = nextHops.size();

//  double loadIndicator[count];
  double availableLoad[count];
  double maxRatio[count];
  double actualRatio[count];
  double ratioDiff[count];
  int packetCount[count];
  double availableLoadSum = 0;
  int packetSum = 0;

//  double interval = 0.020;

  DmprInterfaceData *dmprData;
  for (int i = 0; i < count; i++)
  {
    tmpNextHop = nextHops.at(i);
    InterfaceEntry* ie = ift->getInterfaceById(tmpNextHop.ifIndex);
    if (ie)
    {
      dmprData = ie->getProtocolData<DmprInterfaceData>();
      Ospfv2RoutingTableEntry* entry =(Ospfv2RoutingTableEntry*)dmprData->dmpr->getRoutingTable()->findBestMatchingRoute(this->getDestination());

      if(!entry)
      {
        return Ipv4Route::getInterface();
      }
      //TODO: this() and 'entry' have to match (how exactly?; num of hops? same dest?, ???)
      // if not delete 'entry' from the forwarding table and replace it with this

      NextHop nextHop = entry->getNextHop(i);
      if(nextHop.signalDownstreamCongLevel == 0)
      {
        dmprData->dmpr->registerNextHop(nextHop.ifIndex, nextHop, this);

      }

      if(nextHop.lastChange + dmprData->dmpr->getInterval() < simTime())
      {
        //if there has been 0 ACKs in the previous period, use the current congLevel value
        double smoothEcn = nextHop.ackPacketCount == 0 ? nextHop.congLevel : (double) nextHop.ackPacketSum / (double) nextHop.ackPacketCount;
        nextHop.congLevel = (1 - dmprData->dmpr->getAlpha()) * nextHop.congLevel + smoothEcn * dmprData->dmpr->getAlpha();
        dmprData->dmpr->emitSignal(nextHop.signalCongLevel, nextHop.congLevel);

        double smoothEce = nextHop.fwdPacketCount == 0 ? nextHop.fwdCongLevel : (double) nextHop.fwdPacketSum / (double) nextHop.fwdPacketCount;
        nextHop.fwdCongLevel = (1 - dmprData->dmpr->getAlpha()) * nextHop.fwdCongLevel + smoothEce * dmprData->dmpr->getAlpha();
        dmprData->dmpr->emitSignal(nextHop.signalfwdCongLevel, nextHop.fwdCongLevel);

        dmprData->dmpr->emitSignal(nextHop.signalFwdPacketCount, nextHop.fwdPacketCount);

        nextHop.downstreamCongLevel = (nextHop.congLevel - nextHop.fwdCongLevel) < 0 ? 0 : nextHop.congLevel - nextHop.fwdCongLevel; //dmprData->setInUseCongLevel(dmprData->getCongestionLevel());
        dmprData->dmpr->emitSignal(nextHop.signalDownstreamCongLevel, nextHop.downstreamCongLevel);

        nextHop.lastChange = simTime(); //dmprData->setLastChange(simTime());
//        nextHop.packetCount = 0; //dmprData->setPacketCount(0);

        nextHop.fwdPacketCount = 0;
        nextHop.fwdPacketSum = 0;
        nextHop.ackPacketCount = 0;
        nextHop.ackPacketSum = 0;
        entry->setNextHop(i, nextHop);

      }

//      packetCount[i] = nextHop.packetCount; //dmprData->getPacketCount();
      packetCount[i] = nextHop.fwdPacketCount;
      //          availableLoad[i] = 1 - dmprData->getCongestionLevel();
      availableLoad[i] = 1 - nextHop.downstreamCongLevel;// dmprData->getInUseCongLevel();

      availableLoadSum += availableLoad[i];
      packetSum += packetCount[i];
    } else
    {
      packetCount[i] = 0;
      availableLoad[i] = 0;
    }

  }

  for (int i = 0; i < count; i++)
  {
    maxRatio[i] = availableLoadSum == 0 ? 0 : availableLoad[i] / availableLoadSum; // This can be pre-computed;

    actualRatio[i] =  (packetSum == 0) ? 0 : (double) packetCount[i] / (double)packetSum;


    //if the current portion of packets sent over this hop exceeds the maxRatio, then skip this interface in the decision process
    if (actualRatio[i] > maxRatio[i])
    {
      maxRatio[i] = 0;
    }

    ratioDiff[i] = maxRatio[i] - actualRatio[i];
  }

  double ratio = -1;

  /*
   * Chooses the one with the highest available ratio (except the ones that already exceeded maxRatio)
   */
  int lastIndex = lastNextHopIndex;
  EV << "lastNextHopIndex = " << lastIndex <<"\n";
  int index = 0;
  for (int i = 0; i < count; i++)
  {

    index = (i + lastIndex + 1) % count;
    tmpNextHop = nextHops.at(index);
    //    std::cout << "DMPR: nextHop: "<< tmpNextHop.hopAddress << " availableLoad: "<< availableLoad[i] << " loadSum: " << availableLoadSum << " ratioDiff: " << ratioDiff[i] << " maxRatio: " << maxRatio[i] << " actualRatio: "<< actualRatio[i] << " packets: " << packetCount[i] << " packetSum: " << packetSum << std::endl;

    if (ratioDiff[index] > ratio)
    {

      resNextHops.clear();
      resNextHops.push_back(tmpNextHop);
      ratio = ratioDiff[index];

      const_cast<ospfv2::Ospfv2RoutingTableEntry*> ( this )->lastNextHopIndex = index;

    }else if(ratioDiff[index] == ratio)
    {
      resNextHops.push_back(tmpNextHop);

    }
  }

  bool randomNextHopEnabled = true;


  if(randomNextHopEnabled && resNextHops.size() > 1)
  {
    cRNG* rand = dmprData->dmpr->getRNG(0);


    int i = rand->intRand(resNextHops.size());
//    std::cout<<resNextHops.size() << " int i: "<< i << std::endl;

    resNextHop = resNextHops.at(i);
  }else{
    resNextHop = resNextHops.at(0);
  }
//  /*
//   * Chooses the one with the highest available ratio (except the ones that already exceeded maxRatio)
//   */
//  for (int i = 0; i < count; i++)
//  {
//    tmpNextHop = nextHops.at(i);
//
//      if (maxRatio[i] > ratio)
//      {
//        nextHop = tmpNextHop;
//        ratio = maxRatio[i];
//    }
//  }

  if(resNextHop.hopAddress != Ipv4Address::UNSPECIFIED_ADDRESS){
    //TODO FIX Dirty hack with const_cast
    const_cast<Ospfv2RoutingTableEntry*> ( this )->setInterface(ift->getInterfaceById(resNextHop.ifIndex));
    const_cast<Ospfv2RoutingTableEntry*> ( this )->setGateway(resNextHop.hopAddress);
  }


  return Ipv4Route::getInterface();
}

bool Ospfv2RoutingTableEntry::isHasDmpr() const
{
  return hasDmpr;
}

void Ospfv2RoutingTableEntry::setHasDmpr(bool hasDmpr)
{
  this->hasDmpr = hasDmpr;
}

bool Ospfv2RoutingTableEntry::isDmprInit() const
{
  return dmprInit;
}

void Ospfv2RoutingTableEntry::setDmprInit(bool dmprInit)
{
  this->dmprInit = dmprInit;
}

} // namespace ospfv2

} // namespace inet

