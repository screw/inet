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

#include "inet/applications/udpapp/UdpEcnSender.h"
#include "inet/applications/udpapp/UdpEcnAppHeader_m.h"
#include "inet/common/TimeTag_m.h"
#include "inet/networklayer/common/EcnTag_m.h"
#include "inet/networklayer/common/RouteRecordTag_m.h"



namespace inet {

Define_Module(UdpEcnSender);

void UdpEcnSender::initialize(int stage)
{
    UdpBasicBurst::initialize(stage);
    if(stage == INITSTAGE_LOCAL){
    packetName = par("packetName");

//    probeTime = par("probeTime").isSet() ? par("probeTime").intValue() : -1;
    }
}

void UdpEcnSender::handleMessage(cMessage *msg)
{
    UdpBasicBurst::handleMessage(msg);
}

void UdpEcnSender::sendPacket()
{
  std::ostringstream str;
  str << packetName << "-" << numSent;
  Packet *packet = new Packet(str.str().c_str());
  const auto& payload = makeShared<UdpEcnAppHeader>();
  payload->setChunkLength(B(par("messageLength")));
  payload->setSequenceNo(numSent);
  payload->setType(UDP_ECN_DATA);
  auto creationTimeTag = payload->addTag<CreationTimeTag>();
  creationTimeTag->setCreationTime(simTime());

  packet->addTagIfAbsent<EcnReq>()->setExplicitCongestionNotification(IP_ECN_ECT_0);
  auto routeRecordTag  = packet->addTagIfAbsent<Ipv4RouteRecordReq>();
  routeRecordTag->setType(IPOPTION_RECORD_ROUTE);


  packet->insertAtBack(payload);
  L3Address destAddr = chooseDestAddr();
  emit(packetSentSignal, packet);
  socket.sendTo(packet, destAddr, destPort);
  numSent++;
}

} //namespace
