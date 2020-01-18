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

#include "inet/applications/udpapp/UdpEcnReceiver.h"
#include "inet/applications/udpapp/UdpEcnAppHeader_m.h"
#include "inet/common/ModuleAccess.h"
#include "inet/networklayer/common/EcnTag_m.h"
#include "inet/networklayer/common/L3AddressTag_m.h"
#include "inet/networklayer/common/RouteRecordTag_m.h"
#include "inet/transportlayer/common/L4PortTag_m.h"
#include "inet/transportlayer/contract/udp/UdpControlInfo_m.h"

#include "inet/common/ProtocolTools.h"

namespace inet {

Define_Module(UdpEcnReceiver);

void UdpEcnReceiver::initialize(int step)
{
  UdpEchoApp::initialize(step);
}

//void UdpEcnReceiver::handleMessage(cMessage *msg)
//{
//    // TODO - Generated method body
//}

void UdpEcnReceiver::socketDataArrived(UdpSocket *socket, Packet *pk)
{
//  UdpEchoApp::socketDataArrived(socket, pk);

  const auto& appHeader = removeProtocolHeader<UdpEcnAppHeader>(pk);

  // determine its source address/port
  L3Address remoteAddress = pk->getTag<L3AddressInd>()->getSrcAddress();
  int srcPort = pk->getTag<L4PortInd>()->getSrcPort();

  EcnInd *ecnInd = pk->removeTagIfPresent<EcnInd>();

  Ipv4RouteRecordInd* routeRecordInd = pk->removeTag<Ipv4RouteRecordInd>();



  pk->clearTags();
  pk->trim();

  appHeader->setChunkLength(UDP_ECN_APP_HEADER_LENGTH);
  appHeader->setType(UDP_ECN_ACK);

  if(ecnInd != nullptr && ecnInd->getExplicitCongestionNotification() == IP_ECN_CE){
    appHeader->setEceBit(true);
    delete ecnInd;
  }

  //acks are marked as if they don't support ECN
  pk->addTagIfAbsent<EcnReq>()->setExplicitCongestionNotification(IP_ECN_NOT_ECT);

  Ipv4StrictSourceRoutingReq* ssrTag = pk->addTag<Ipv4StrictSourceRoutingReq>();
  ssrTag->setType(IPOPTION_STRICT_SOURCE_ROUTING);
  Ipv4OptionRecordRoute strictRouting = (routeRecordInd->getOptionForUpdate());
  strictRouting.setType(IPOPTION_STRICT_SOURCE_ROUTING);
//  ssrTag->setOption(*(const_cast<Ipv4OptionRecordRoute*>(strictRouting)));


  ssrTag->setOption(strictRouting);

  pk->insertAtBack(appHeader);
  std::ostringstream str;
  str << "Ack-" << appHeader->getSequenceNo();
  pk->setName(str.str().c_str());

  // statistics
  numEchoed++;
  emit(packetSentSignal, pk);
  // send back
  socket->sendTo(pk, remoteAddress, srcPort);
}

} //namespace
