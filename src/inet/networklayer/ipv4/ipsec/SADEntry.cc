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

#include "inet/networklayer/ipv4/ipsec/SADEntry.h"

namespace inet {

SADEntry::SADEntry() {
    // TODO Auto-generated constructor stub

}

const IPsecRule& SADEntry::getRule() const {
    return rule;
}

IPsecRule* SADEntry::getRuleForUpdate()  {
    return &rule;
}

void SADEntry::setRule(const IPsecRule &rule) {
    this->rule = rule;
}

unsigned int SADEntry::getSpi() const {
    return spi;
}

void SADEntry::setSpi(unsigned int spi) {
    this->spi = spi;
}

SADEntry::~SADEntry() {
    // TODO Auto-generated destructor stub
}

unsigned int SADEntry::getProcessing() const {
    return rule.getProcessing();
}

void SADEntry::setProcessing(unsigned int processing) {

    rule.setProcessing(processing);
}

unsigned int SADEntry::getAction() const {

    return rule.getAction();
}

void SADEntry::setAction(unsigned int action) {

    rule.setAction(action);
}

unsigned int SADEntry::getDirection() const {
    return rule.getDirection();
}

unsigned int SADEntry::getSeqNum() const {
    return seqNum;
}

unsigned int SADEntry::getAndIncSeqNum() {

    this->seqNum +=  1;
    return seqNum - 1;
}

void SADEntry::setSeqNum(unsigned int seqNum) {
    this->seqNum = seqNum;
}

void SADEntry::setDirection(unsigned int direction) {

    rule.setDirection(direction);
}

} /* namespace inet */
