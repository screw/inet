//
// Copyright (C) 2019 Marcel Marek
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

#include "inet/common/misc/TcpReorder.h"

#include "inet/common/packet/printer/PacketPrinter.h"
#include "inet/networklayer/ipv4/Ipv4Header_m.h"
#include "inet/linklayer/ethernet/EtherFrame_m.h"
#include "inet/networklayer/common/L3Tools.h"
#include "inet/linklayer/ethernet/EtherEncap.h"
#include "inet/transportlayer/tcp_common/TcpHeader_m.h"
#include "inet/transportlayer/tcp/Tcp.h"

namespace inet {

Define_Module(TcpReorder);
TcpReorder::~TcpReorder(){


  for (auto it=tcpConnMap.begin(); it!=tcpConnMap.end(); it=tcpConnMap.begin()){
    ConnInfo &connInfo = it->second;
    while(!connInfo.seqnoPacketmap.empty()){
      delete connInfo.seqnoPacketmap.begin()->second.second;
      connInfo.seqnoPacketmap.erase(connInfo.seqnoPacketmap.begin());
    }
    tcpConnMap.erase(it);
  }


}
void TcpReorder::initialize()
{
    startTime = par("startTime");
    long _batchSize = par("batchSize");
    if ((_batchSize < 0) || (((long)(unsigned int)_batchSize) != _batchSize))
        throw cRuntimeError("Invalid 'batchSize=%ld' parameter at '%s' module", _batchSize, getFullPath().c_str());
    batchSize = (unsigned int)_batchSize;
    maxInterval = par("maxInterval");

    numPackets = numBits = 0;
    intvlStartTime = intvlLastPkTime = 0;
    intvlNumPackets = intvlNumBits = 0;

    WATCH(numPackets);
    WATCH(numBits);
    WATCH(intvlStartTime);
    WATCH(intvlNumPackets);
    WATCH(intvlNumBits);

    bitpersecVector.setName("thruput (bit/sec)");
    pkpersecVector.setName("packet/sec");
}

void TcpReorder::handleMessage(cMessage *msg)
{
  Packet *packet = check_and_cast<Packet *>(msg);
//  PacketPrinter printer;
//    printer.printPacket(std::cout, packet);

    const Protocol *payloadProtocol = nullptr;
    auto headerOffset = packet->getFrontOffset();

//    auto ethHeader = packet->peekAtFront<EthernetMacHeader>();
//    if (isEth2Header(*ethHeader))
//    {
//      payloadProtocol = ProtocolGroup::ethertype.getProtocol(ethHeader->getTypeOrLength());
//      if (*payloadProtocol == Protocol::ipv4)
//      {
  //      auto headerOffset = packet->getFrontOffset();

//        packet->setFrontOffset(headerOffset + ethHeader->getChunkLength());
        auto ipv4Header = packet->peekAtFront<Ipv4Header>();
        const Protocol *protocolPtr = ipv4Header->getProtocol();
        Ipv4Address srcAddr = ipv4Header->getSrcAddress();

        Ipv4Address destAddr = ipv4Header->getDestAddress();
        if(*protocolPtr == Protocol::tcp)
        {
          packet->setFrontOffset(headerOffset + ipv4Header->getChunkLength());
          auto tcpHeader = packet->peekAtFront<tcp::TcpHeader>();
          tcp::Tcp::SockPair key;
              key.localAddr = destAddr;
              key.remoteAddr = srcAddr;
              key.localPort = tcpHeader->getDestPort();
              key.remotePort = tcpHeader->getSrcPort();
              int64_t payloadLen = (ipv4Header->getTotalLengthField() - ipv4Header->getHeaderLength() - tcpHeader->getHeaderLength()).get();
              int64_t seqNo = tcpHeader->getSequenceNo();

              packet->setFrontOffset(headerOffset);



              auto i = tcpConnMap.find(key);
              if (i != tcpConnMap.end()){
                ConnInfo &connInfo = i->second;
//                std::cout<<"connInfo.nextSeqNo: " << connInfo.nextSeqNo;
                if(seqNo == connInfo.nextSeqNo){
                  //in-order
                  connInfo.nextSeqNo = seqNo + payloadLen;
                  send(msg, "out");

                  while(!connInfo.seqnoPacketmap.empty()){
//                    std::cout<<"First seqNum: " << connInfo.seqnoPacketmap.begin()->first << std::endl;
                    if(connInfo.seqnoPacketmap.begin()->first == connInfo.nextSeqNo){
                      connInfo.nextSeqNo = connInfo.seqnoPacketmap.begin()->first + connInfo.seqnoPacketmap.begin()->second.first;

//                      std::cout<<"Sending out seqNum: " << connInfo.seqnoPacketmap.begin()->first << std::endl;
                      send(connInfo.seqnoPacketmap.begin()->second.second, "out");
                      connInfo.seqnoPacketmap.erase(connInfo.seqnoPacketmap.begin());
                    }else{
                      break;
                    }
                  }

                  return;
                }else{
                  if(seqNo < connInfo.nextSeqNo){
//                    std::cout<<"Adding packet with seqNo smaller than expected: " << seqNo << std::endl;
                    delete packet;
                    return;
                  }
                  if(connInfo.seqnoPacketmap.find(seqNo) != connInfo.seqnoPacketmap.end()){
//                    std::cout<<"This SeqNo already exists: " << seqNo << std::endl;
                    delete packet;
                    return;
                  }
                  connInfo.seqnoPacketmap[seqNo] = std::make_pair(payloadLen, packet);
//                  std::cout<<"Adding packet with seqNo: " << seqNo << std::endl;
                  return;

                }


              }else{
                //create new
                ConnInfo connInfo;
                connInfo.nextSeqNo = tcpHeader->getSequenceNo() + 1;
//                tcpConnMap.i
                tcpConnMap[key] = connInfo;
              }

        }

//      }

//    updateStats(simTime(), PK(msg)->getBitLength());
    send(msg, "out");
}

void TcpReorder::updateStats(simtime_t now, unsigned long bits)
{
    numPackets++;
    numBits += bits;

    // packet should be counted to new interval
    if (intvlNumPackets >= batchSize || now - intvlStartTime >= maxInterval)
        beginNewInterval(now);

    intvlNumPackets++;
    intvlNumBits += bits;
    intvlLastPkTime = now;
}

void TcpReorder::beginNewInterval(simtime_t now)
{
    simtime_t duration = now - intvlStartTime;

    // record measurements
    double bitpersec = intvlNumBits / duration.dbl();
    double pkpersec = intvlNumPackets / duration.dbl();

    bitpersecVector.recordWithTimestamp(intvlStartTime, bitpersec);
    pkpersecVector.recordWithTimestamp(intvlStartTime, pkpersec);

    // restart counters
    intvlStartTime = now;    // FIXME this should be *beginning* of tx of this packet, not end!
    intvlNumPackets = intvlNumBits = 0;
}

void TcpReorder::finish()
{
    simtime_t duration = simTime() - startTime;

    recordScalar("duration", duration);
    recordScalar("total packets", numPackets);
    recordScalar("total bits", numBits);

    recordScalar("avg throughput (bit/s)", numBits / duration.dbl());
    recordScalar("avg packets/s", numPackets / duration.dbl());
}

} // namespace inet

