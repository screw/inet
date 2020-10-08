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

#ifndef __INET_SPD_H_
#define __INET_SPD_H_

#include "inet/common/INETDefs.h"
#include "inet/networklayer/ipv4/ipsec/SecurityAssociationDatabase.h"


namespace inet {
namespace ipsec {
/**
 * Security Policy Database
 */
class INET_API SecurityPolicyDatabase : public cSimpleModule
{

public:

    class INET_API Entry
    {

    private:

        IPsecRule rule;
    public:
        std::vector<SecurityAssociationDatabase::Entry*> entries;

    public:
        Entry();
        virtual ~Entry();
        unsigned int getAction() const;
        void setAction(unsigned int action);
        unsigned int getDirection() const;
        void setDirection(unsigned int direction);
        unsigned int getProcessing() const;
        void setProcessing(unsigned int processing);
        const IPsecSelector& getSelector() const;
        void setSelector(const IPsecSelector &selector);
        const IPsecRule& getRule() const;
        IPsecRule& getRuleForUpdate();
        void setRule(const IPsecRule &rule);
        const std::vector<SecurityAssociationDatabase::Entry*>& getEntries() const;
        void setEntries(const std::vector<SecurityAssociationDatabase::Entry*> &entries);
        void addSADEntry(SecurityAssociationDatabase::Entry *sadEntry);
        std::string str() const;
    };


private:
    std::vector<Entry*> entries;

protected:
    virtual void initialize() override;
    void refreshDisplay() const override;

public:
    Entry* findEntry(unsigned int direction, IPsecSelector *selector);
    void addEntry(Entry *entry);


};

} //ipsec namespace
} //namespace

#endif
