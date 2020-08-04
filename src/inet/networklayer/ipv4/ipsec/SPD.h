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
#include "inet/networklayer/ipv4/ipsec/SPDEntry.h"


namespace inet {

/**
 * TODO - Security Policy Database
 */
class INET_API SPD : public cSimpleModule
{
private:
    std::vector<SPDEntry*> db;


public:

    SPDEntry* findEntry(unsigned int direction, IPsecSelector *selector);
    void addEntry(SPDEntry *entry);

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

} //namespace

#endif
