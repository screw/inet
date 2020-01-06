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

#ifndef __INET_LSA_H
#define __INET_LSA_H

#include <math.h>
#include <vector>

#include "inet/common/INETDefs.h"
#include "inet/routing/ospfv2/Ospfv2Packet_m.h"
#include "inet/routing/ospfv2/router/Ospfv2Common.h"
#include "inet/networklayer/ipv4/Ipv4Route.h"

//class Ipv4Route;

namespace inet {

namespace ospfv2 {

class O2NextHop : public Ipv4Route::Ipv4RouteNextHop
{
  public:
    O2NextHop(int ifIndex, Ipv4Address hopAddress, RouterId advertisingRouter):
    Ipv4RouteNextHop(hopAddress),
    ifIndex(ifIndex),
    advertisingRouter(advertisingRouter)
    {
//      IRoute::NextHop(hopAddress);



      cost = -1;

      congLevel = 0.000001; //0.001; //0.000001
      fwdCongLevel = 0; //forwarded Congestion level
      downstreamCongLevel = 0.000001; //0.001;  //0.000001
  //    int packetCount = 0;

      fwdPacketCount = 0; //total number of ECN capable packet forwarded within current interval
      fwdPacketSum = 0; //sum of ECE marks for forwarded packets within current interval (effectively the number of marked packet)
      ackPacketCount = 0; //total number of ECN capable Ack packets within current interval
      ackPacketSum = 0; //sum of ECE marks for ECN capable Acks packets within current interval (effectively the number of marked packet)

      lastChange = 0;

      signalCongLevel = 0;
      signalfwdCongLevel = 0;
      signalDownstreamCongLevel = 0;
      signalFwdPacketCount = 0;
      signalMaxRatio = 0;

    };
    O2NextHop(){
      //      IRoute::NextHop(hopAddress);
            ifIndex = -1;
      //      Ipv4Address hopAddress = Ipv4Address::UNSPECIFIED_ADDRESS;
//            advertisingRouter  = Ipv4Address::UNSPECIFIED_ADDRESS;
            cost = -1;

            congLevel = 0.000001; //0.001; //0.000001
            fwdCongLevel = 0; //forwarded Congestion level
            downstreamCongLevel = 0.000001; //0.001;  //0.000001
        //    int packetCount = 0;

            fwdPacketCount = 0; //total number of ECN capable packet forwarded within current interval
            fwdPacketSum = 0; //sum of ECE marks for forwarded packets within current interval (effectively the number of marked packet)
            ackPacketCount = 0; //total number of ECN capable Ack packets within current interval
            ackPacketSum = 0; //sum of ECE marks for ECN capable Acks packets within current interval (effectively the number of marked packet)

            lastChange = 0;

            signalCongLevel = 0;
            signalfwdCongLevel = 0;
            signalDownstreamCongLevel = 0;
            signalFwdPacketCount = 0;
            signalMaxRatio = 0;

    };
    O2NextHop(const O2NextHop &other): Ipv4Route::Ipv4RouteNextHop(other){
      copy(other);
    };

    void copy(const O2NextHop &other){
      ifIndex = other.ifIndex;
//      hopAddress = other.hopAddress;
       advertisingRouter = other.advertisingRouter;
       cost = other.cost;

       congLevel = other.congLevel;
       fwdCongLevel = other.fwdCongLevel;
       downstreamCongLevel = other.downstreamCongLevel;
       fwdPacketCount = other.fwdPacketCount;
       ackPacketCount = other.ackPacketCount;
       ackPacketSum = other.ackPacketSum;

       lastChange = other.lastChange;

       signalCongLevel = other.signalCongLevel;
       signalfwdCongLevel = other.signalfwdCongLevel;
       signalDownstreamCongLevel = other.signalDownstreamCongLevel;
       signalFwdPacketCount = other.signalFwdPacketCount;
       signalMaxRatio = other.signalMaxRatio;
    }

    int ifIndex;
//    Ipv4Address hopAddress = Ipv4Address::UNSPECIFIED_ADDRESS;
    RouterId advertisingRouter;
    Metric cost = -1;

    double congLevel = 0.000001; //0.001; //0.000001
    double fwdCongLevel = 0; //forwarded Congestion level
    double downstreamCongLevel = 0.000001; //0.001;  //0.000001
//    int packetCount = 0;

    int fwdPacketCount = 0; //total number of ECN capable packet forwarded within current interval
    int fwdPacketSum = 0; //sum of ECE marks for forwarded packets within current interval (effectively the number of marked packet)
    int ackPacketCount = 0; //total number of ECN capable Ack packets within current interval
    int ackPacketSum = 0; //sum of ECE marks for ECN capable Acks packets within current interval (effectively the number of marked packet)

    simtime_t lastChange = 0;

    simsignal_t signalCongLevel = 0;
    simsignal_t signalfwdCongLevel = 0;
    simsignal_t signalDownstreamCongLevel = 0;
    simsignal_t signalFwdPacketCount = 0;
    simsignal_t signalMaxRatio = 0;

};

class INET_API RoutingInfo
{
  private:
    std::vector<O2NextHop> nextHops;
    unsigned long distance;
    Ospfv2Lsa *parent;

  public:
    RoutingInfo() : distance(0), parent(nullptr) {}
    RoutingInfo(const RoutingInfo& routingInfo) : nextHops(routingInfo.nextHops), distance(routingInfo.distance), parent(routingInfo.parent) {}
    virtual ~RoutingInfo() {}

    void addNextHop(O2NextHop nextHop) { nextHops.push_back(nextHop); }
    void clearNextHops() { nextHops.clear(); }
    unsigned int getNextHopCount() const { return nextHops.size(); }
    O2NextHop getNextHop(unsigned int index) const { return nextHops[index]; }
    void setDistance(unsigned long d) { distance = d; }
    unsigned long getDistance() const { return distance; }
    void setParent(Ospfv2Lsa *p) { parent = p; }
    Ospfv2Lsa *getParent() const { return parent; }
};

class INET_API LsaTrackingInfo
{
  public:
    enum InstallSource {
        ORIGINATED = 0,
        FLOODED = 1
    };

  private:
    InstallSource source;
    unsigned long installTime;

  public:
    LsaTrackingInfo() : source(FLOODED), installTime(0) {}
    LsaTrackingInfo(const LsaTrackingInfo& info) : source(info.source), installTime(info.installTime) {}

    void setSource(InstallSource installSource) { source = installSource; }
    InstallSource getSource() const { return source; }
    void incrementInstallTime() { installTime++; }
    void resetInstallTime() { installTime = 0; }
    unsigned long getInstallTime() const { return installTime; }
};

class INET_API RouterLsa : public Ospfv2RouterLsa,
    public RoutingInfo,
    public LsaTrackingInfo
{
  public:
    RouterLsa() : Ospfv2RouterLsa(), RoutingInfo(), LsaTrackingInfo() {}
    RouterLsa(const Ospfv2RouterLsa& lsa) : Ospfv2RouterLsa(lsa), RoutingInfo(), LsaTrackingInfo() {}
    RouterLsa(const RouterLsa& lsa) : Ospfv2RouterLsa(lsa), RoutingInfo(lsa), LsaTrackingInfo(lsa) {}
    virtual ~RouterLsa() {}

    bool validateLSChecksum() const { return true; }    // not implemented

    bool update(const Ospfv2RouterLsa *lsa);
    bool differsFrom(const Ospfv2RouterLsa *routerLSA) const;
};

class INET_API NetworkLsa : public Ospfv2NetworkLsa,
    public RoutingInfo,
    public LsaTrackingInfo
{
  public:
    NetworkLsa() : Ospfv2NetworkLsa(), RoutingInfo(), LsaTrackingInfo() {}
    NetworkLsa(const Ospfv2NetworkLsa& lsa) : Ospfv2NetworkLsa(lsa), RoutingInfo(), LsaTrackingInfo() {}
    NetworkLsa(const NetworkLsa& lsa) : Ospfv2NetworkLsa(lsa), RoutingInfo(lsa), LsaTrackingInfo(lsa) {}
    virtual ~NetworkLsa() {}

    bool validateLSChecksum() const { return true; }    // not implemented

    bool update(const Ospfv2NetworkLsa *lsa);
    bool differsFrom(const Ospfv2NetworkLsa *networkLSA) const;
};

class INET_API SummaryLsa : public Ospfv2SummaryLsa,
    public RoutingInfo,
    public LsaTrackingInfo
{
  protected:
    bool purgeable;

  public:
    SummaryLsa() : Ospfv2SummaryLsa(), RoutingInfo(), LsaTrackingInfo(), purgeable(false) {}
    SummaryLsa(const Ospfv2SummaryLsa& lsa) : Ospfv2SummaryLsa(lsa), RoutingInfo(), LsaTrackingInfo(), purgeable(false) {}
    SummaryLsa(const SummaryLsa& lsa) : Ospfv2SummaryLsa(lsa), RoutingInfo(lsa), LsaTrackingInfo(lsa), purgeable(lsa.purgeable) {}
    virtual ~SummaryLsa() {}

    bool getPurgeable() const { return purgeable; }
    void setPurgeable(bool purge = true) { purgeable = purge; }

    bool validateLSChecksum() const { return true; }    // not implemented

    bool update(const Ospfv2SummaryLsa *lsa);
    bool differsFrom(const Ospfv2SummaryLsa *summaryLSA) const;
};

class INET_API AsExternalLsa : public Ospfv2AsExternalLsa,
    public RoutingInfo,
    public LsaTrackingInfo
{
  protected:
    bool purgeable;

  public:
    AsExternalLsa() : Ospfv2AsExternalLsa(), RoutingInfo(), LsaTrackingInfo(), purgeable(false) {}
    AsExternalLsa(const Ospfv2AsExternalLsa& lsa) : Ospfv2AsExternalLsa(lsa), RoutingInfo(), LsaTrackingInfo(), purgeable(false) {}
    AsExternalLsa(const AsExternalLsa& lsa) : Ospfv2AsExternalLsa(lsa), RoutingInfo(lsa), LsaTrackingInfo(lsa), purgeable(lsa.purgeable) {}
    virtual ~AsExternalLsa() {}

    bool getPurgeable() const { return purgeable; }
    void setPurgeable(bool purge = true) { purgeable = purge; }

    bool validateLSChecksum() const { return true; }    // not implemented

    bool update(const Ospfv2AsExternalLsa *lsa);
    bool differsFrom(const Ospfv2AsExternalLsa *asExternalLSA) const;
};

/**
 * Returns true if leftLSA is older than rightLSA.
 */
bool operator<(const Ospfv2LsaHeader& leftLSA, const Ospfv2LsaHeader& rightLSA);

/**
 * Returns true if leftLSA is the same age as rightLSA.
 */
bool operator==(const Ospfv2LsaHeader& leftLSA, const Ospfv2LsaHeader& rightLSA);

bool operator==(const Ospfv2Options& leftOptions, const Ospfv2Options& rightOptions);

inline bool operator!=(const Ospfv2Options& leftOptions, const Ospfv2Options& rightOptions)
{
    return !(leftOptions == rightOptions);
}

inline bool operator==(const O2NextHop& leftHop, const O2NextHop& rightHop)
{
    return (leftHop.ifIndex == rightHop.ifIndex) &&
           (leftHop.hopAddress == rightHop.hopAddress) &&
           (leftHop.advertisingRouter == rightHop.advertisingRouter);
}

inline bool operator!=(const O2NextHop& leftHop, const O2NextHop& rightHop)
{
    return !(leftHop == rightHop);
}

B calculateLSASize(const Ospfv2Lsa *lsa);
B calculateLsaSize(const Ospfv2RouterLsa& lsa);
B calculateLsaSize(const Ospfv2NetworkLsa& lsa);
B calculateLsaSize(const Ospfv2SummaryLsa& lsa);
B calculateLsaSize(const Ospfv2AsExternalLsa& lsa);

std::ostream& operator<<(std::ostream& ostr, const Ospfv2LsaRequest& lsaReq);
std::ostream& operator<<(std::ostream& ostr, const Ospfv2LsaHeader& lsa);
std::ostream& operator<<(std::ostream& ostr, const Ospfv2Lsa& lsa);
std::ostream& operator<<(std::ostream& ostr, const Ospfv2NetworkLsa& lsa);
std::ostream& operator<<(std::ostream& ostr, const Ospfv2TosData& tos);
std::ostream& operator<<(std::ostream& ostr, const Ospfv2Link& link);
std::ostream& operator<<(std::ostream& ostr, const Ospfv2RouterLsa& lsa);
std::ostream& operator<<(std::ostream& ostr, const Ospfv2SummaryLsa& lsa);
std::ostream& operator<<(std::ostream& ostr, const Ospfv2ExternalTosInfo& tos);
std::ostream& operator<<(std::ostream& ostr, const Ospfv2AsExternalLsaContents& contents);
std::ostream& operator<<(std::ostream& ostr, const Ospfv2AsExternalLsa& lsa);

} // namespace ospfv2

} // namespace inet

#endif    // __LSA_HPP__

