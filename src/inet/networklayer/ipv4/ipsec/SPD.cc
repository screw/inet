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

#include "SPD.h"

namespace inet {

Define_Module(SPD);

void SPD::initialize()
{
    // TODO - Generated method body
}

SPDEntry* SPD::findEntry(unsigned int direction, IPsecSelector *selector)
{
    std::vector<SPDEntry*>::iterator it = db.begin();

    for (; it != db.end(); it++) {
        if(((*it)->getDirection() == direction || (*it)->getDirection() == IPsecRule::Direction::dIN_OUT ) && (*it)->getSelector().isMatchingSelector(selector) ) {
            return (*it);
        }
    }

    return nullptr;
}

void SPD::addEntry(SPDEntry *entry)
{
    db.push_back(entry);
}

void SPD::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
}

} //namespace
