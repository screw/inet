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

#ifndef __INET_SAD_H_
#define __INET_SAD_H_

#include "inet/common/INETDefs.h"

#include "inet/networklayer/ipv4/ipsec/IPsecRule.h"



namespace inet {
namespace ipsec{
/**
 * Security Association Database
 */
class INET_API SecurityAssociationDatabase : public cSimpleModule
{


public:
    class INET_API Entry {

    private:
        IPsecRule rule;
        unsigned int spi;
        unsigned int seqNum = 0;
    public:
        Entry();
        virtual ~Entry();
        const IPsecRule& getRule() const;
        IPsecRule* getRuleForUpdate();
        void setRule(const IPsecRule &rule);
        unsigned int getSpi() const;
        void setSpi(unsigned int spi);

        unsigned int getProcessing() const;

        void setProcessing(unsigned int processing);
        unsigned int getAction() const;

        void setAction(unsigned int action);
        unsigned int getDirection() const;

        void setDirection(unsigned int direction);
        unsigned int getSeqNum() const;
        unsigned int getAndIncSeqNum();
        void setSeqNum(unsigned int seqNum = 0);

        std::string str() const;

    };

private:
    std::vector<Entry*> entries;

protected:
    virtual void initialize() override;
    void refreshDisplay() const override;


public:
    void addEntry(Entry* sadEntry);
    Entry* findEntry(IPsecRule *rule, unsigned int spi);
};

} //ipsec namespace
} //namespace

#endif
