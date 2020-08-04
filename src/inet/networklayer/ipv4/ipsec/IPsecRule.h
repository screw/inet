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
//  along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef INET_NETWORKLAYER_IPV4_IPSEC_IPSECRULE_H_
#define INET_NETWORKLAYER_IPV4_IPSEC_IPSECRULE_H_

#include "inet/common/INETDefs.h"
#include "inet/networklayer/ipv4/ipsec/IPsecSelector.h"

namespace inet {

class INET_API IPsecRule {
public:
    enum Direction {
        dIN = 1,
        dOUT = 2,
        dIN_OUT = 3
    };

    enum Action {
        aDISCARD = 0,
        aBYPASS = 1,
        aPROTECT = 2
    };

    enum ProcessingAlgs {
        pNONE = 0,
        pAH = 1,
        pESP = 2,
        pAH_ESP = 3
    };

private:
    unsigned int direction = 0;  //rules applies to IN, OUT, or both INOUT traffic
    unsigned int action = Action::aDISCARD;
//    unsigned int spi; // TOOD move to SAD?
    unsigned int processing = 0; //aply AH, ESP, or both pAH_ESP
    IPsecSelector selector;

public:
    IPsecRule();
    virtual ~IPsecRule();
    unsigned int getAction() const;
    void setAction(unsigned int action = Action::aDISCARD);
    unsigned int getDirection() const;
    void setDirection(unsigned int direction = 0);
    unsigned int getProcessing() const;
    void setProcessing(unsigned int processing = 0);
    const IPsecSelector& getSelector() const;
    void setSelector(const IPsecSelector &selector);
};

} /* namespace inet */

#endif /* INET_NETWORKLAYER_IPV4_IPSEC_IPSECRULE_H_ */
