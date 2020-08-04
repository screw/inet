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

#include "SAD.h"

namespace inet {

Define_Module(SAD);

void SAD::initialize()
{
    // TODO - Generated method body
}

void SAD::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
}

void SAD::addEntry(SADEntry *sadEntry) {

    db.push_back(sadEntry);
}

SADEntry* SAD::findEntry(IPsecRule *rule, unsigned int spi)
{
    std::vector<SADEntry*>::iterator it = db.begin();
    for (; it != db.end(); it++) {
        if(((*it)->getDirection() == rule->getDirection() || (*it)->getDirection() == IPsecRule::Direction::dIN_OUT)
                && (*it)->getProcessing() == rule->getProcessing()
                && (*it)->getSpi() == spi){

            return (*it);
        }

    }

    return nullptr;
}

} //namespace


