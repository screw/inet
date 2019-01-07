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

#ifndef __INET_UDPECNSENDER_H_
#define __INET_UDPECNSENDER_H_


#include "inet/applications/udpapp/UdpBasicApp.h"

using namespace omnetpp;

namespace inet {

/**
 * This class in cooperation with UdpEcnReceiver implements
 * Explicit Congestion Notification feedback in the application layer.
 * It supplements the ECE-bit missing in UDP header compared to TCP for
 * notifying the sender about the congestion.
 */
class UdpEcnSender : public UdpBasicApp
{
  protected:
    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage *msg) override;

    virtual void sendPacket() override;
};

} //namespace

#endif
