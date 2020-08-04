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

#ifndef INET_NETWORKLAYER_IPV4_IPSEC_SPDENTRY_H_
#define INET_NETWORKLAYER_IPV4_IPSEC_SPDENTRY_H_


#include "inet/common/INETDefs.h"
#include "inet/networklayer/ipv4/ipsec/IPsecRule.h"
#include "inet/networklayer/ipv4/ipsec/SADEntry.h"

namespace inet {

class INET_API SPDEntry {


private:

    IPsecRule rule;
public:
    std::vector<SADEntry*> sadEntryList; //TODO make private - create a wrapper


public:
    SPDEntry();
//    SPDEntry(IPsecSelector selector, unsigned int spi, unsigned int direction, unsigned int action, unsigned int processing);
    virtual ~SPDEntry();

    unsigned int getAction() const;

    void setAction(unsigned int action);

    unsigned int getDirection() const;

    void setDirection(unsigned int direction);

    unsigned int getProcessing() const;

    void setProcessing(unsigned int processing);

    const IPsecSelector& getSelector() const;

    void setSelector(const IPsecSelector &selector);
//    unsigned int getSpi() const;
//    void setSpi(unsigned int spi);
    const IPsecRule& getRule() const;
    IPsecRule& getRuleForUpdate();
    void setRule(const IPsecRule &rule);
    const std::vector<SADEntry*>& getSadEntryList() const;
    void setSadEntryList(const std::vector<SADEntry*> &sadEntryList);
    void addSADEntry(SADEntry* sadEntry);
};

} /* namespace inet */

#endif /* INET_NETWORKLAYER_IPV4_IPSEC_SPDENTRY_H_ */
