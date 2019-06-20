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

#include "inet/routing/dmpr/Dmpr.h"

#include "inet/applications/udpapp/UdpEcnAppHeader_m.h"

#include "inet/transportlayer/tcp/Tcp.h"
#include "inet/transportlayer/udp/Udp.h"
#include "inet/common/packet/printer/PacketPrinter.h"
#include "inet/linklayer/common/InterfaceTag_m.h"

#include "inet/networklayer/common/NextHopAddressTag_m.h"
#include "inet/networklayer/common/L3Tools.h"
#include "inet/networklayer/ipv4/Ipv4Header_m.h"
#include "inet/transportlayer/common/TransportPseudoHeader_m.h"
#include "inet/transportlayer/contract/TransportHeaderBase_m.h"


#include "inet/routing/dmpr/DmprInterfaceData.h"

//#include "inet/common/ProtocolTag_m.h"
//
#include "inet/common/packet/Packet.h"

#include "inet/transportlayer/common/L4Tools.h"
#include "inet/transportlayer/tcp_common/TcpHeader_m.h"

#include "inet/transportlayer/common/TransportPseudoHeader_m.h"
#include "inet/transportlayer/contract/tcp/TcpCommand_m.h"
#include "inet/transportlayer/tcp/TcpConnection.h"
#include "inet/transportlayer/tcp/TcpSendQueue.h"
#include "inet/transportlayer/tcp_common/TcpHeader.h"
#include "inet/transportlayer/tcp/TcpReceiveQueue.h"
#include "inet/networklayer/ipv4/Ipv4InterfaceData.h"

#include "inet/routing/ospfv2/router/OspfRoutingTableEntry.h"
#include "inet/networklayer/common/EcnTag_m.h"

#include "inet/routing/dmpr/DmprInterfaceData.h"



namespace inet {

Define_Module(Dmpr);

simsignal_t Dmpr::registerSignal(std::stringstream title, std::stringstream name, std::stringstream interfaceName, Ipv4Address destination)
{
    Enter_Method_Silent();

  std::stringstream signalName;
  signalName << name.str();
  signalName << "." <<  destination.str() << "_" << interfaceName.str();
  simsignal_t signal = cComponent::registerSignal(signalName.str().c_str());

  cResultRecorder *vectorRecorder = cResultRecorderType::get("vector")->create();

  opp_string_map *attrs = new opp_string_map;
  (*attrs)["title"] = title.str() + " " + destination.str() + " " + interfaceName.str();
//  (*attrs)["title"] += ie->getFullName();

  vectorRecorder->init(this, signalName.str().c_str() , "vector", nullptr, attrs);
  subscribe(signal,vectorRecorder);

  return signal;

}

void Dmpr::initialize(int stage)
{

  if (stage == INITSTAGE_LOCAL)
  {
    routingTable = getModuleFromPar<IIpv4RoutingTable>(par("routingTableModule"), this);
    interfaceTable = getModuleFromPar<IInterfaceTable>(par("interfaceTableModule"), this);
    networkProtocol = getModuleFromPar<INetfilter>(par("networkProtocolModule"), this);
//    forwardingTable = getModuleFromPar<DmprForwardingTable>(par("dmprForwardingTableModule"), this);

    alpha = par("alpha").doubleValue();
    interval = par("interval").doubleValue();
  }
  else if (stage == INITSTAGE_LINK_LAYER)
  {

    int numInter = interfaceTable->getNumInterfaces();
    InterfaceEntry* ie;
    for (int i = 0; i < numInter; i++ )
    {

      ie = interfaceTable->getInterface(i);

//      std::stringstream sigCongLevel;
//      sigCongLevel << "congLevel." << ie->getFullName();
////      std::cout << sigCongLevel.str() << std::endl;
//      std::stringstream sigInUseCongLevel;
//      sigInUseCongLevel<< "inUseCongLevel." << ie->getFullName();
//
//
//      simsignal_t signalCongLevel = cComponent::registerSignal(sigCongLevel.str().c_str());
//      simsignal_t signalInUseCongLevel = cComponent::registerSignal(sigInUseCongLevel.str().c_str());
//
//      cResultRecorder *vectorRecorder = cResultRecorderType::get("vector")->create();
//      opp_string_map *attrs = new opp_string_map;
//      (*attrs)["title"] = "DMPR  Load ";
//      (*attrs)["title"] += ie->getFullName();
//      //      (*attrs)["unit"] = "packets";
//      vectorRecorder->init(this, sigCongLevel.str().c_str() , "vector", nullptr, attrs);
//      subscribe(signalCongLevel,vectorRecorder);
//
//      simsignal_t signal = registerSignal("congLevel", ie->getFullName(), destIp);
//
//
//      cResultRecorder *vectorRecorderInUse = cResultRecorderType::get("vector")->create();
//      opp_string_map *attrsInUse = new opp_string_map;
//      (*attrsInUse)["title"] = "DMPR  InUse Load ";
//      (*attrsInUse)["title"] += ie->getFullName();
//      //      (*attrs)["unit"] = "packets";
//      vectorRecorderInUse->init(this, sigInUseCongLevel.str().c_str(), "vector", nullptr, attrsInUse);
//      subscribe(signalInUseCongLevel, vectorRecorderInUse);
//
//      registerSignal(signalInUseCongLevel, vectorRecorderInUse);




      DmprInterfaceData *d = ie->addProtocolData<DmprInterfaceData>();
//      d->setSignalCongLevel(signalCongLevel);
//      d->setSignalInUseCongLevel(signalInUseCongLevel);
//      ie->setDmprInterfaceData(d);





    }
  }
  else if (stage == INITSTAGE_ROUTING_PROTOCOLS)
  {
    networkProtocol->registerHook(0, this);
  }
}

double Dmpr::getInterval() const
{
  return interval;
}

double Dmpr::getAlpha() const
{
  return alpha;
}

void Dmpr::setAlpha(double alpha)
{
  this->alpha = alpha;
}

void Dmpr::setInterval(double interval)
{
  this->interval = interval;
}

void Dmpr::emitSignal(simsignal_t signal, double value)
{
    Enter_Method_Silent();
  emit(signal, value);

//  std::cout << "DMPR: " << getFullPath() << " signal: " << signal << " value: " << value << std::endl;
}

void Dmpr::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
}

void Dmpr::updateIntervalCong(ospf::NextHop& nextHop, DmprInterfaceData* dmprData)
{

    // if the packetCount for previous period is 0 then don't change the current value -> use the current value as smootEce in the exponential smooth equation
    double smoothEce = nextHop.ackPacketCount == 0 ? nextHop.congLevel : (double) (nextHop.ackPacketSum) / (double) (nextHop.ackPacketCount);
    nextHop.ackPacketCount = 0;
    nextHop.ackPacketSum = 0;
    nextHop.congLevel = (1 - alpha) * nextHop.congLevel + smoothEce * getAlpha();
    emitSignal(nextHop.signalCongLevel, nextHop.congLevel);

    smoothEce = nextHop.fwdPacketCount == 0 ? nextHop.fwdCongLevel : (double) (nextHop.fwdPacketSum) / (double) (nextHop.fwdPacketCount);

    nextHop.fwdCongLevel = (1 - alpha) * nextHop.fwdCongLevel + smoothEce * getAlpha();
    emitSignal(nextHop.signalfwdCongLevel, nextHop.fwdCongLevel);

    emitSignal(nextHop.signalFwdPacketCount, nextHop.fwdPacketCount);

    nextHop.fwdPacketCount = 0;
    nextHop.fwdPacketSum = 0;

    nextHop.inUseCongLevel = (nextHop.congLevel - nextHop.fwdCongLevel) < 0 ? 0 : nextHop.congLevel - nextHop.fwdCongLevel; //dmprData->setInUseCongLevel(dmprData->getCongestionLevel());
    emitSignal(nextHop.signalInUseCongLevel, nextHop.inUseCongLevel);

    nextHop.lastChange = simTime();
    nextHop.packetCount = 0;

}

void Dmpr::registerNextHop(int interfaceId, ospf::NextHop& nextHop, const ospf::RoutingTableEntry* route)
{
  InterfaceEntry* ie = interfaceTable->getInterfaceById(interfaceId);
  nextHop.signalCongLevel = registerSignal(std::stringstream("DMPR Load"), std::stringstream("congLevel"),
      std::stringstream(ie->getFullName()), route->getDestination());
  nextHop.signalfwdCongLevel = registerSignal(std::stringstream("DMPR Fwd Load"), std::stringstream("fwdCongLevel"),
      std::stringstream(ie->getFullName()), route->getDestination());
  nextHop.signalInUseCongLevel = registerSignal(std::stringstream("DMPR InUseLoad"),
      std::stringstream("inUseCongLevel"), std::stringstream(ie->getFullName()), route->getDestination());
  nextHop.signalFwdPacketCount = registerSignal(std::stringstream("DMPR Fwd Packet Count"),
      std::stringstream("fwdPacketCount"), std::stringstream(ie->getFullName()), route->getDestination());
  nextHop.lastChange = simTime();
}

void Dmpr::updateCongestionLevel(int ece, DmprInterfaceData* dmprData, Ipv4Address srcIp, int interfaceId)
{

  ospf::RoutingTableEntry* route = dmprData->table->findBestMatchingRoute(srcIp);
  if(!route)
  {
    //This should never happen as the entry should have been created when the data was forwarded
    route = (ospf::RoutingTableEntry*)routingTable->findBestMatchingRoute(srcIp);
    dmprData->table->insertEntry(srcIp, (ospf::RoutingTableEntry*) route);

    int count = route->getNextHopCount();
    for (int i = 0; i< count; i++)
    {
      ospf::NextHop nextHop = route->getNextHop(i);
      if(nextHop.ifIndex == interfaceId)
      {
        registerNextHop(interfaceId, nextHop, route);
        route->setNextHop(i, nextHop);
      }
    }
  }

  ospf::RoutingTableEntry *ospfEntry = dynamic_cast<ospf::RoutingTableEntry*>(route);

  for(int i = 0; i < ospfEntry->getNextHopCount(); i++)
  {
    ospf::NextHop nextHop = ospfEntry->getNextHop(i);
    if(nextHop.ifIndex == interfaceId) // && (nextHop.hopAddress == srcIp || nextHop.hopAddress == Ipv4Address::UNSPECIFIED_ADDRESS)
    {
      if (nextHop.lastChange + getInterval() < simTime())
      {
        updateIntervalCong(nextHop, dmprData);
      }
      nextHop.ackPacketCount++;
      nextHop.ackPacketSum += ece;

      ospfEntry->setNextHop(i, nextHop);

    }

  }

}

INetfilter::IHook::Result Dmpr::datagramPreRoutingHook(Packet* datagram)
{
  PacketPrinter printer;

//  std::cout << "DMPR: PreRouting: Packet: ";
//  printer.printPacket(std::cout, datagram);
//  EV_DEBUG << "DMPR Prerouting detailedInf: " << datagram->;

  auto ipv4Header = datagram->peekAtFront<Ipv4Header>();

  const Protocol *protocolPtr = ipv4Header->getProtocol();
  Ipv4Address srcIp = ipv4Header->getSrcAddress();
  if(*protocolPtr == Protocol::tcp)
  {
    auto headerOffset = datagram->getFrontOffset();
    datagram->setFrontOffset(headerOffset + ipv4Header->getChunkLength());
    const auto& transportHeader = peekTransportProtocolHeader(datagram, *protocolPtr);

    // must be a TcpHeader
    auto tcpHeader = datagram->peekAtFront<tcp::TcpHeader>();

//    auto tcpHeader = datagram->peekAtFront<TcpHeader>();
    datagram->setFrontOffset(headerOffset);

    int64_t payload = (ipv4Header->getTotalLengthField() - ipv4Header->getHeaderLength() - tcpHeader->getHeaderLength()).get();

    if (tcpHeader->getAckBit() && !(payload > 0))
    {
      //ACKnowledgement
      int interfaceId = datagram->getTag<InterfaceInd>()->getInterfaceId();
      DmprInterfaceData *dmprData =  interfaceTable->getInterfaceById(interfaceId)->getProtocolData<DmprInterfaceData>();

      int ece = tcpHeader->getEceBit();

      updateCongestionLevel(ece, dmprData, srcIp, interfaceId);
    }
  }
  else if (*protocolPtr == Protocol::udp)
  {

    auto originalOffset = datagram->getFrontOffset();
    auto newOffset = originalOffset + ipv4Header->getChunkLength();
    datagram->setFrontOffset(newOffset);
    const auto& transportHeader = peekTransportProtocolHeader(datagram, *protocolPtr);


    newOffset += transportHeader->getChunkLength();
    datagram->setFrontOffset(newOffset);
    // must be a UdpHeader
//    auto udpHeader = datagram->peekAtFront<UdpHeader>();

    //    auto udpHeader = datagram->peekAtFront<TcpHeader>();
    // hasAtFront() doesn't work the way I would expect. Returns true if the header matches, but
    // raises an error if not.
    if(datagram->hasAtFront<UdpEcnAppHeader>())
    {
      auto udpEcnAppHeader = datagram->peekAtFront<UdpEcnAppHeader>();
      datagram->setFrontOffset(originalOffset);

      //    int64_t payload = (ipv4Header->getTotalLengthField() - ipv4Header->getHeaderLength() - udpHeader->getHeaderLength()).get();

      if (udpEcnAppHeader->getType() == UDP_ECN_ACK)
      {
        //ACKnowledgement
        int interfaceId = datagram->getTag<InterfaceInd>()->getInterfaceId();
        DmprInterfaceData *dmprData =  interfaceTable->getInterfaceById(interfaceId)->getProtocolData<DmprInterfaceData>();

        int ece = udpEcnAppHeader->getEceBit();

        updateCongestionLevel(ece, dmprData, srcIp, interfaceId);

//        emit(dmprData->getSignalCongLevel(), p);

      }
    }

    datagram->setFrontOffset(originalOffset);

  }



  return ACCEPT;
}
/*
 * Updating statistics for the 'data-path'. It includes packets forwarded within current interval and the sum of ECN marks
 */
void Dmpr::updateFwdCongLevel(int ecn, DmprInterfaceData* dmprData, const Ipv4Address& destAddr, int interfaceId, Ipv4Route* route)
{
  // get the best matching route from DMPR table
  //TODO what if route exists in DMPR table, but there is better route in the global Routing Table
  ospf::RoutingTableEntry* dmprRoute = dmprData->table->findBestMatchingRoute(destAddr);
  if (!dmprRoute)
  {
    //if it doesn't exist yet;
    dmprRoute = new ospf::RoutingTableEntry(*(const_cast<ospf::RoutingTableEntry*>((ospf::RoutingTableEntry*) (route))));
    dmprData->table->insertEntry(destAddr, dmprRoute);
    int count = dmprRoute->getNextHopCount();
    for (int i = 0; i < count; i++)
    {
      ospf::NextHop nextHop = dmprRoute->getNextHop(i);
      InterfaceEntry* ie = interfaceTable->getInterfaceById(nextHop.ifIndex);
      ie->dmprData()->table->insertEntry(destAddr, dmprRoute);
      nextHop.signalCongLevel = registerSignal(std::stringstream("DMPR Load"), std::stringstream("congLevel"), std::stringstream(ie->getFullName()), route->getDestination());
      nextHop.signalfwdCongLevel = registerSignal(std::stringstream("DMPR Fwd Load"), std::stringstream("fwdCongLevel"), std::stringstream(ie->getFullName()), route->getDestination());
      nextHop.signalInUseCongLevel = registerSignal(std::stringstream("DMPR InUseLoad"), std::stringstream("inUseCongLevel"), std::stringstream(ie->getFullName()), route->getDestination());
      nextHop.signalFwdPacketCount = registerSignal(std::stringstream("DMPR Fwd Packet Count"), std::stringstream("fwdPacketCount"), std::stringstream(ie->getFullName()), route->getDestination());
      nextHop.lastChange = simTime();
      dmprRoute->setNextHop(i, nextHop);
    }
  }

  ospf::RoutingTableEntry* ospfEntry = dynamic_cast<ospf::RoutingTableEntry*>(dmprRoute);
  for (int i = 0; i < ospfEntry->getNextHopCount(); i++)
  {
    ospf::NextHop nextHop = ospfEntry->getNextHop(i);
    if (nextHop.ifIndex == interfaceId) // && (nextHop.hopAddress == srcIp || nextHop.hopAddress == Ipv4Address::UNSPECIFIED_ADDRESS)
    {
      if(nextHop.lastChange + getInterval() < simTime())
      {
        updateIntervalCong(nextHop, dmprData);
      }


      nextHop.fwdPacketCount++;
      nextHop.fwdPacketSum =+ ecn;

      nextHop.packetCount++;
      ospfEntry->setNextHop(i, nextHop);

    }
  }
}

INetfilter::IHook::Result Dmpr::datagramForwardHook(Packet* datagram)
{


  PacketPrinter printer;

  const auto& ipv4Header = datagram->peekAtFront<Ipv4Header>();

  Ipv4Address destAddr = ipv4Header->getDestAddress();

  if(!destAddr.isUnicast())
  {
    //Only supports unicast
//    std::cout << "DMPR: ForwardHook: Skipping: ";
    printer.printPacket(std::cout, datagram);
    return ACCEPT;
  }

  // if this packet is using Source Routing then override the DMPR mechanism
  for(int i = ipv4Header->getOptionArraySize(); i > 0; i--)
  {

    if(ipv4Header->getOption(i-1).getType() == IPOPTION_STRICT_SOURCE_ROUTING)
    {
      return ACCEPT;
    }
  }


  //This packet is not using Source Routing option so it is NOT ACK

  //This flow is not yet in our forwarding table -> create new entry
  Ipv4Route *route = routingTable->findBestMatchingRoute(destAddr);

  if (route)
  {
    InterfaceEntry* ie = route->getInterface();

    Ipv4Address nextHopAddr = route->getGateway();

    int interfaceId = ie->getInterfaceId();

    DmprInterfaceData* dmprData = ie->getProtocolData<DmprInterfaceData>();
//    dmprData->incPacketCount();

    datagram->addTagIfAbsent<InterfaceReq>()->setInterfaceId(interfaceId);
    datagram->addTagIfAbsent<NextHopAddressReq>()->setNextHopAddress(nextHopAddr);

    //update the congestionLevel for forwarded packets

    int ect = ipv4Header->getExplicitCongestionNotification();

    if (((ect & IP_ECN_ECT_0) == IP_ECN_ECT_0) || ((ect & IP_ECN_ECT_1) == IP_ECN_ECT_1))
    {
      int ecn = (ect == IP_ECN_CE) ? 1 : 0;

      updateFwdCongLevel(ecn, dmprData, destAddr, interfaceId, route);
    }
  }





  return ACCEPT;




  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!





  DmprForwardingTable::Socket socket;
  socket.dstIp = destAddr;
  socket.srcIp = ipv4Header->getSrcAddress();
  socket.protocol = ipv4Header->getProtocolId();

  const Protocol *protocolPtr = ipv4Header->getProtocol();
  if (!protocolPtr || !isTransportProtocol(*protocolPtr))
  {
    // only intended for transport level traffic (NB isTransportProtocol() currently supports only TCP and UDP)
    return ACCEPT;
  }
  auto headerOffset = datagram->getFrontOffset();
  datagram->setFrontOffset(headerOffset + ipv4Header->getChunkLength());
  const auto& transportHeader = peekTransportProtocolHeader(datagram, *protocolPtr);
  datagram->setFrontOffset(headerOffset);

  socket.srcPort = transportHeader->getSourcePort();
  socket.dstPort = transportHeader->getDestinationPort();


  DmprForwardingTable::NextHopInterface nextHop = forwardingTable->getNextHopForSocket(socket);

  if(nextHop.nextHop != Ipv4Address::ALLONES_ADDRESS)
  {
    //the 5-tuple (socket) identifier already exists in our forwarding table
    //set nextHop and return ACCEPT

    datagram->addTagIfAbsent<InterfaceReq>()->setInterfaceId(nextHop.interfaceId);
    datagram->addTagIfAbsent<NextHopAddressReq>()->setNextHopAddress(nextHop.nextHop);
    std::cout << "DMPR: Existing nextHop: " << nextHop.nextHop.str() << std::endl;


    return ACCEPT;
  }

  //This flow is not yet in our forwarding table -> create new entry
  route = routingTable->findBestMatchingRoute(destAddr);

  if (route)
  {
    InterfaceEntry* ie = route->getInterface();

    Ipv4Address nextHopAddr = route->getGateway();
//    DmprForwardingTable::NextHopInterface nextHop;
    nextHop.interfaceId = ie->getInterfaceId();
    nextHop.nextHop = nextHopAddr;

//    forwardingTable->insertEntry(socket, nextHop);
    std::cout << "DMPR: Adding socket: " << socket.str() << std::endl;

    DmprInterfaceData* dmprData = ie->getProtocolData<DmprInterfaceData>();
//    dmprData->incCongestionLevel();

    datagram->addTagIfAbsent<InterfaceReq>()->setInterfaceId(nextHop.interfaceId);
    datagram->addTagIfAbsent<NextHopAddressReq>()->setNextHopAddress(nextHop.nextHop);
//    std::cout << "DMPR: new nextHop: " << nextHop.nextHop.str() << " with congest: " << dmprData->getCongestionLevel()
//        << "\n";
  }





  return ACCEPT;
}

INetfilter::IHook::Result Dmpr::datagramPostRoutingHook(Packet* datagram)
{

  auto ipv4Header = datagram->peekAtFront<Ipv4Header>();

      for (int i = ipv4Header->getOptionArraySize(); i > 0; i--)
      {

        if(ipv4Header->getOption(i-1).getType() == IPOPTION_STRICT_SOURCE_ROUTING)
        {

          auto ipv4HeaderForUpdate = removeNetworkProtocolHeader<Ipv4Header>(datagram);
          TlvOptionBase& option = ipv4HeaderForUpdate->getOptionForUpdate(i-1);
          Ipv4OptionRecordRoute& strictRoute = dynamic_cast<Ipv4OptionRecordRoute&>(option);
          short position = strictRoute.getNextAddressIdx();
          if (strictRoute.getRecordAddressArraySize() > 0)
          {

            const Ipv4Address& nextHop = strictRoute.getRecordAddress(position);
            const Ipv4Route *re = routingTable->findBestMatchingRoute(nextHop);
            const InterfaceEntry *destIE;
            if (re) {
                destIE = re->getInterface();
                datagram->addTagIfAbsent<InterfaceReq>()->setInterfaceId(destIE->getInterfaceId());
            }
            strictRoute.eraseRecordAddress(position);
            strictRoute.setNextAddressIdx(position - 1);

            datagram->addTagIfAbsent<NextHopAddressReq>()->setNextHopAddress(nextHop);

          }
          insertNetworkProtocolHeader(datagram, Protocol::ipv4, ipv4HeaderForUpdate);

        }
        else if (ipv4Header->getOption(i-1).getType() == IPOPTION_RECORD_ROUTE)
        {

        NextHopAddressReq* nextHopTag = datagram->findTag<NextHopAddressReq>();

        //if this is the last hop, don't add it
        if (nextHopTag != nullptr && !nextHopTag->getNextHopAddress().toIpv4().isUnspecified()
            && nextHopTag->getNextHopAddress().toIpv4() != ipv4Header->getDestAddress())
        {
          const InterfaceEntry *destIE = interfaceTable->getInterfaceById(
              datagram->getTag<InterfaceReq>()->getInterfaceId());

          auto ipv4HeaderForUpdate = removeNetworkProtocolHeader<Ipv4Header>(datagram);

//          destIE->ipv4Data()->getIPAddress();
          TlvOptionBase& option = ipv4HeaderForUpdate->getOptionForUpdate(i - 1);
          Ipv4OptionRecordRoute& recordRoute = dynamic_cast<Ipv4OptionRecordRoute&>(option);

          recordRoute.insertRecordAddress(destIE->getProtocolData<Ipv4InterfaceData>()->getIPAddress());
          recordRoute.setNextAddressIdx(recordRoute.getRecordAddressArraySize() - 1);

          insertNetworkProtocolHeader(datagram, Protocol::ipv4, ipv4HeaderForUpdate);

          }
        }
      }



  return ACCEPT;
}

INetfilter::IHook::Result Dmpr::datagramLocalInHook(Packet* datagram)
{
  return ACCEPT;
}

INetfilter::IHook::Result Dmpr::datagramLocalOutHook(Packet* datagram)
{
  return ACCEPT;
}

//bool Dmpr::handleOperationStage(LifecycleOperation* operation, int stage, IDoneCallback* doneCallback)
//{
//  Enter_Method_Silent();
//}

} //namespace
