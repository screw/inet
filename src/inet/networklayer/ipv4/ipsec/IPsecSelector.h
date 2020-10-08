//
// Copyright (C) 2020 Marcel Marek
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

#ifndef INET_NETWORKLAYER_IPV4_IPSEC_IPSECSELECTOR_H_
#define INET_NETWORKLAYER_IPV4_IPSEC_IPSECSELECTOR_H_

#include "inet/common/INETDefs.h"
#include "inet/networklayer/contract/ipv4/IPv4Address.h"

namespace inet {
namespace ipsec {

class INET_API IPsecSelector {
private:
    IPv4Address localAddress = IPv4Address(IPv4Address::UNSPECIFIED);
    IPv4Address remoteAddress = IPv4Address(IPv4Address::UNSPECIFIED);
    unsigned int nextProtocol = 0; //next layer protocol
    unsigned int localPort = 0;
    unsigned int remotePort = 0;
    unsigned int icmpCode = 0; //if next layer is ICMP

public:
    IPsecSelector();
    IPsecSelector(IPv4Address localAddress, IPv4Address remoteAddress, unsigned int nextProtocol, unsigned int localPort = 0, unsigned int remotePort = 0, unsigned int icmpCode = 0 );
    virtual ~IPsecSelector();
    unsigned int getIcmpCode() const;
    void setIcmpCode(unsigned int icmpCode);
    const IPv4Address& getLocalAddress() const;
    void setLocalAddress(const IPv4Address &localAddress);
    unsigned int getLocalPort() const;
    void setLocalPort(unsigned int localPort);
    unsigned int getNextProtocol() const;
    void setNextProtocol(unsigned int nextProtocol);
    const IPv4Address& getRemoteAddress() const;
    void setRemoteAddress(const IPv4Address &remoteAddress);
    unsigned int getRemotePort() const;
    void setRemotePort(unsigned int remotePort);

    bool isMatchingSelector(IPsecSelector* selector) const;

    std::string str() const;
};

std::ostream& operator<<(std::ostream& os, const IPsecSelector& e);

} //ipsec namespace
} /* namespace inet */

#endif /* INET_NETWORKLAYER_IPV4_IPSEC_IPSECSELECTOR_H_ */
