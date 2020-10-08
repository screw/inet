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

#include "inet/networklayer/ipv4/ipsec/IPsecSelector.h"
#include "inet/networklayer/common/IPProtocolId_m.h"

namespace inet {
namespace ipsec {

std::ostream& operator<<(std::ostream& os, const IPsecSelector& e)
{
    os << e.str();
    return os;
};

IPsecSelector::IPsecSelector() {

    localAddress = IPv4Address::UNSPECIFIED_ADDRESS;
    remoteAddress = IPv4Address::UNSPECIFIED_ADDRESS;
    nextProtocol = 0;
    localPort = 0;
    remotePort = 0;
    icmpCode = 0;

}

IPsecSelector::IPsecSelector(IPv4Address localAddress,
        IPv4Address remoteAddress, unsigned int nextProtocol,
        unsigned int localPort, unsigned int remotePort,
        unsigned int icmpCode)
{
    this->localAddress = localAddress;
    this->remoteAddress = remoteAddress;
    this->nextProtocol = nextProtocol;
    this->localPort = localPort;
    this->remotePort = remotePort;
    this->icmpCode = icmpCode;

}

unsigned int IPsecSelector::getIcmpCode() const {
    return icmpCode;
}

void IPsecSelector::setIcmpCode(unsigned int icmpCode) {
    this->icmpCode = icmpCode;
}

const IPv4Address& IPsecSelector::getLocalAddress() const {
    return localAddress;
}

void IPsecSelector::setLocalAddress(const IPv4Address &localAddress) {
    this->localAddress = localAddress;
}

unsigned int IPsecSelector::getLocalPort() const {
    return localPort;
}

void IPsecSelector::setLocalPort(unsigned int localPort) {
    this->localPort = localPort;
}

unsigned int IPsecSelector::getNextProtocol() const {
    return nextProtocol;
}

void IPsecSelector::setNextProtocol(unsigned int nextProtocol) {
    this->nextProtocol = nextProtocol;
}

const IPv4Address& IPsecSelector::getRemoteAddress() const {
    return remoteAddress;
}

void IPsecSelector::setRemoteAddress(const IPv4Address &remoteAddress) {
    this->remoteAddress = remoteAddress;
}

unsigned int IPsecSelector::getRemotePort() const {
    return remotePort;
}

void IPsecSelector::setRemotePort(unsigned int remotePort) {
    this->remotePort = remotePort;
}

IPsecSelector::~IPsecSelector() {

}

bool IPsecSelector::isMatchingSelector(IPsecSelector *other) const {


    if((localAddress == other->getLocalAddress() || localAddress == IPv4Address(IPv4Address::UNSPECIFIED)) &&
            (remoteAddress == other->getRemoteAddress() || remoteAddress == IPv4Address(IPv4Address::UNSPECIFIED)) &&
            (nextProtocol == other->getNextProtocol() || nextProtocol == 0 ))
    {
        if(other->nextProtocol == IP_PROT_UDP || other->nextProtocol == IP_PROT_TCP)
        {
            return ((localPort == other->getLocalPort() || localPort == 0 )  && (remotePort == other->getRemotePort() || remotePort == 0));

        }
        else if(other->nextProtocol == IP_PROT_ICMP)
        {
            return (icmpCode == other->getIcmpCode() || icmpCode == 0);
        }else{
            throw cRuntimeError("Unsupported nextProtocol");
        }
    }

    return false;
}

std::string IPsecSelector::str() const
{
    std::stringstream out;

    out << "Prot: " << nextProtocol;
    switch(nextProtocol)
    {
        case IP_PROT_TCP:
        case IP_PROT_UDP:
            out << " Local: " << localAddress.str()<<":" << localPort << ";";
            out << " Remote: " << remoteAddress.str() << ":" << remotePort << ";";
            break;

        case IP_PROT_ICMP:
        case IP_PROT_ESP:
        case IP_PROT_AH:
            out << " Local: " << localAddress.str() <<  ";";
            out << " Remote: " << remoteAddress.str() <<";";
    }
    return out.str();
}

} //ipsec namespace
} /* namespace inet */
