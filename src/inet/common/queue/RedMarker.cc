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

#include "inet/common/queue/RedMarker.h"
#include "inet/common/INETUtils.h"

//#include "inet/networklayer/diffserv/DscpMarker.h"

#include "inet/common/ProtocolTag_m.h"


#include "inet/networklayer/ipv4/Ipv4Header_m.h"


//#ifdef WITH_IPv6
//#include "inet/networklayer/ipv6/Ipv6Header.h"
//#endif // ifdef WITH_IPv6

//#include "inet/networklayer/diffserv/Dscp_m.h"
//
//#include "inet/networklayer/diffserv/DiffservUtil.h"

namespace inet {

Define_Module(RedMarker);


RedMarker::RedMarker()
{


}

RedMarker::~RedMarker()
{
  delete[] marks;

}

void RedMarker::initialize()
{
  RedDropper::initialize();


  marks = new double[numGates];

  cStringTokenizer marksTokens(par("marks"));
  for (int i = 0; i < numGates; ++i) {
      marks[i] = marksTokens.hasMoreTokens() ? utils::atod(marksTokens.nextToken()) :
          (i > 0 ? marks[i - 1] : 0);


      if (marks[i] < 0.0)
          throw cRuntimeError("marks parameter must not be negative");

  }
}

void RedMarker::handleMessage(cMessage *msg)
{
  cPacket *packet = check_and_cast<cPacket *>(msg);

  if (shouldDrop(packet)){
    dropPacket(packet);
    return;
  }
//  else if (shouldMark(packet))
//  {
//    markPacket(check_and_cast<Packet *>(packet));
//  }

   sendOut(packet);
}

bool RedMarker::shouldDrop(cPacket *packet)
{
    const int i = packet->getArrivalGate()->getIndex();
    ASSERT(i >= 0 && i < numGates);
    const double minth = minths[i];
    const double maxth = maxths[i];
    const double maxp = maxps[i];
    const double pkrate = pkrates[i];
    const int queueLength = getLength();

    if (queueLength > 0)
    {
        // TD: This following calculation is only useful when the queue is not empty!
        avg = (1 - wq) * avg + wq * queueLength;
    }
    else
    {
        // TD: Added behaviour for empty queue.
        const double m = SIMTIME_DBL(simTime() - q_time) * pkrate;
        avg = pow(1 - wq, m) * avg;
    }

//    Random dropping is disabled; returns true only for hard limit

    if (minth <= avg && avg < maxth)
    {
        count[i]++;
        const double pb = maxp * (avg - minth) / (maxth - minth);
        const double pa = pb / (1 - count[i] * pb); // TD: Adapted to work as in [Floyd93].
        if (dblrand() < pa)
        {
            EV << "Random early packet drop (avg queue len=" << avg << ", pa=" << pb << ")\n";
            count[i] = 0;
            markPacket(check_and_cast<Packet *>(packet));
            return false;
        }
    }
    else if (avg >= maxth) {
        EV << "Avg queue len " << avg << " >= maxth, dropping packet.\n";
        count[i] = 0;
        markPacket(check_and_cast<Packet *>(packet));
        return false;
    }
    else if (queueLength >= maxth) {    // maxth is also the "hard" limit
        EV << "Queue len " << queueLength << " >= maxth, dropping packet.\n";
        count[i] = 0;
        return true;
    }
    else
    {
        count[i] = -1;
    }

    return false;
}

bool RedMarker::shouldMark(cPacket *packet)
{
  const int i = packet->getArrivalGate()->getIndex();
   ASSERT(i >= 0 && i < numGates);
   const double minth = minths[i];
   const double maxth = maxths[i];
   const double maxp = maxps[i];
   const double pkrate = pkrates[i];
   const int queueLength = getLength();

   if (queueLength > 0)
   {
       // TD: This following calculation is only useful when the queue is not empty!
       avg = (1 - wq) * avg + wq * queueLength;
   }
   else
   {
       // TD: Added behaviour for empty queue.
       const double m = SIMTIME_DBL(simTime() - q_time) * pkrate;
       avg = pow(1 - wq, m) * avg;
   }

   if (minth <= avg && avg < maxth)
   {
       count[i]++;
       const double pb = maxp * (avg - minth) / (maxth - minth);
       const double pa = pb / (1 - count[i] * pb); // TD: Adapted to work as in [Floyd93].
       if (dblrand() < pa)
       {
           EV << "Random early packet drop (avg queue len=" << avg << ", pa=" << pb << ")\n";
           count[i] = 0;
           return true;
       }
   }
   else if (avg >= maxth) {
       EV << "Avg queue len " << avg << " >= maxth, dropping packet.\n";
       count[i] = 0;
       return true;
   }
   else if (queueLength >= maxth) {    // maxth is also the "hard" limit
       EV << "Queue len " << queueLength << " >= maxth, dropping packet.\n";
       count[i] = 0;
       return true;
   }
   else
   {
       count[i] = -1;
   }

   return false;
}


bool RedMarker::markPacket(Packet *packet)
{
  EV_DETAIL << "Marking packet with ECN \n";

  auto protocol = packet->getTag<PacketProtocolTag>()->getProtocol();

  //TODO processing link-layer headers when exists


  if (protocol == &Protocol::ipv4) {
      packet->trimFront();
      const auto& ipv4Header = packet->removeAtFront<Ipv4Header>();

      ipv4Header->setExplicitCongestionNotification(1);
      packet->insertAtFront(ipv4Header);
      return true;
  }

//#ifdef WITH_IPv6
//  if (protocol == &Protocol::ipv6) {
//      packet->trimFront();
//      const auto& ipv6Header = packet->removeAtFront<Ipv6Header>();
//      ipv6Header->setDiffServCodePoint(dscp);
//      packet->insertAtFront(ipv6Header);
//      return true;
//  }
//#endif // ifdef WITH_IPv6

  return false;


}





} //namespace
