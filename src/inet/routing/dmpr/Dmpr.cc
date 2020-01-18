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


#include "inet/transportlayer/tcp/Tcp.h"
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

#include "inet/routing/dmpr/DmprInterfaceData.h"



namespace inet {

Define_Module(Dmpr);

void Dmpr::initialize(int stage)
{

  if (stage == INITSTAGE_LOCAL)
  {
    routingTable = getModuleFromPar<IIpv4RoutingTable>(par("routingTableModule"), this);
    interfaceTable = getModuleFromPar<IInterfaceTable>(par("interfaceTableModule"), this);
    networkProtocol = getModuleFromPar<INetfilter>(par("networkProtocolModule"), this);
    forwardingTable = getModuleFromPar<DmprForwardingTable>(par("dmprForwardingTableModule"), this);

    alpha = par("alpha").doubleValue();
  }
  else if (stage == INITSTAGE_LINK_LAYER)
  {

    int numInter = interfaceTable->getNumInterfaces();
    InterfaceEntry* ie;
    for (int i = 0; i < numInter; i++ )
    {

      ie = interfaceTable->getInterface(i);


//       = new DmprInterfaceData(this);

       DmprInterfaceData *d = ie->addProtocolData<DmprInterfaceData>();

    }
  }
  else if (stage == INITSTAGE_ROUTING_PROTOCOLS)
  {
    networkProtocol->registerHook(0, this);
  }
}

void Dmpr::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
}

INetfilter::IHook::Result Dmpr::datagramPreRoutingHook(Packet* datagram)
{
  PacketPrinter printer;

//  std::cout << "DMPR: PreRouting: Packet: ";
//  printer.printPacket(std::cout, datagram);
//  EV_DEBUG << "DMPR Prerouting detailedInf: " << datagram->;

  auto ipv4Header = datagram->peekAtFront<Ipv4Header>();

  const Protocol *protocolPtr = ipv4Header->getProtocol();
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

      double p = dmprData->getCongestionLevel();
//      double alpha = 0.1
      p = (1 - alpha) * p + ece * alpha;
      dmprData->setCongestionLevel(p);

      for(int i = ipv4Header->getOptionArraySize(); i > 0; i--)
      {

        if(ipv4Header->getOption(i-1).getType() == IPOPTION_STRICT_SOURCE_ROUTING)
        {
//          ipv4Header->q
          auto ipv4HeaderForUpdate = removeNetworkProtocolHeader<Ipv4Header>(datagram);
          TlvOptionBase& option = ipv4HeaderForUpdate->getOptionForUpdate(i-1);
          Ipv4OptionRecordRoute& strictRoute = dynamic_cast<Ipv4OptionRecordRoute&>(option);
          short position = strictRoute.getNextAddressIdx();
          if (strictRoute.getRecordAddressArraySize() > 0)
          {

            const Ipv4Address& nextHop = strictRoute.getRecordAddress(position);
            strictRoute.eraseRecordAddress(position);
            strictRoute.setNextAddressIdx(position - 1);

            datagram->addTagIfAbsent<NextHopAddressReq>()->setNextHopAddress(nextHop);

          }
          insertNetworkProtocolHeader(datagram, Protocol::ipv4, ipv4HeaderForUpdate);
        }
      }
    }
  }



  return ACCEPT;
}

INetfilter::IHook::Result Dmpr::datagramForwardHook(Packet* datagram)
{
  PacketPrinter printer;
//  if(datagram->findTag<NextHopAddressReq() != nullptr)
//  {
//
//  }



  const auto& ipv4Header = datagram->peekAtFront<Ipv4Header>();

  Ipv4Address destAddr = ipv4Header->getDestAddress();

  if(!destAddr.isUnicast())
  {
    //Only supports unicast
    std::cout << "DMPR: ForwardHook: Skipping: ";
    printer.printPacket(std::cout, datagram);
    return ACCEPT;
  }
//  std::cout << "DMPR: ForwardHook: Processing: ";
//  printer.printPacket(std::cout, datagram);

//  DmprSocket socket;
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
  Ipv4Route *route = routingTable->findBestMatchingRoute(destAddr);

  if (route)
  {
    InterfaceEntry* ie = route->getInterface();

    Ipv4Address nextHopAddr = route->getGateway();
//    DmprForwardingTable::NextHopInterface nextHop;
    nextHop.interfaceId = ie->getInterfaceId();
    nextHop.nextHop = nextHopAddr;

    forwardingTable->insertEntry(socket, nextHop);
    std::cout << "DMPR: Adding socket: " << socket.str() << std::endl;

    DmprInterfaceData* dmprData = ie->getProtocolData<DmprInterfaceData>();
    dmprData->incCongestionLevel();

    datagram->addTagIfAbsent<InterfaceReq>()->setInterfaceId(nextHop.interfaceId);
    datagram->addTagIfAbsent<NextHopAddressReq>()->setNextHopAddress(nextHop.nextHop);
    std::cout << "DMPR: new nextHop: " << nextHop.nextHop.str() << " with congest: " << dmprData->getCongestionLevel()
        << "\n";
  }





  return ACCEPT;
}

INetfilter::IHook::Result Dmpr::datagramPostRoutingHook(Packet* datagram)
{
//
//  auto ipv4Header = removeNetworkProtocolHeader<Ipv4Header>(datagram);
////  auto ipv4Header = datagram->peekAtFront<Ipv4Header>();
//
//const Protocol *protocolPtr = ipv4Header->getProtocol();
//if(*protocolPtr == Protocol::tcp)
//{
//  auto headerOffset = datagram->getFrontOffset();
//  datagram->setFrontOffset(headerOffset + ipv4Header->getChunkLength());
//  const auto& transportHeader = peekTransportProtocolHeader(datagram, *protocolPtr);
//
//  // must be a TcpHeader
//  auto tcpHeader = datagram->peekAtFront<tcp::TcpHeader>();
//
////    auto tcpHeader = datagram->peekAtFront<TcpHeader>();
//  datagram->setFrontOffset(headerOffset);
//
//
//    int interfaceId = datagram->getTag<InterfaceInd>()->getInterfaceId();
//    DmprInterfaceData *dmprData =  interfaceTable->getInterfaceById(interfaceId)->dmprData();
//
//
//
//    for(int i = ipv4Header->getOptionArraySize(); i > 0; i--)
//    {
//
//      if(ipv4Header->getOption(i-1).getType() == IPOPTION_STRICT_SOURCE_ROUTING)
//      {
////          ipv4Header->q
//        TlvOptionBase& option = ipv4Header->getOptionForUpdate(i-1);
//        Ipv4OptionRecordRoute& strictRoute = dynamic_cast<Ipv4OptionRecordRoute&>(option);
//        short position = strictRoute.getNextAddressIdx();
//        if (strictRoute.getRecordAddressArraySize() > 0)
//        {
//          const Ipv4Address& nextHop = strictRoute.getRecordAddress(position);
//          strictRoute.eraseRecordAddress(position);
//          strictRoute.setNextAddressIdx(position - 1);
//
//          datagram->addTagIfAbsent<NextHopAddressReq>()->setNextHopAddress(nextHop);
//        }
//      }
//    }
//  }
//}
//
//
//insertNetworkProtocolHeader(datagram, Protocol::ipv4, ipv4Header);
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
