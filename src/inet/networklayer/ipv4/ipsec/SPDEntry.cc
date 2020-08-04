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

#include "inet/networklayer/ipv4/ipsec/SPDEntry.h"

namespace inet {

SPDEntry::SPDEntry() {


}

//SPDEntry::SPDEntry(IPsecSelector selector, unsigned  int spi, unsigned int direction,
//        unsigned int action, unsigned int processing) {
//
//    this->selector = selector;
//    this->spi = spi;
//    this->direction = direction;
//    this->action = action;
//    this->processing = processing;
//}


SPDEntry::~SPDEntry() {
    // TODO Auto-generated destructor stub
}

unsigned int SPDEntry::getAction() const {
    return rule.getAction();
}

void SPDEntry::setAction(unsigned int action) {
    rule.setAction(action);
}

unsigned int SPDEntry::getDirection() const {
    return rule.getDirection();
}

void SPDEntry::setDirection(unsigned int direction) {
    rule.setDirection(direction);
}

unsigned int SPDEntry::getProcessing() const {
    return rule.getProcessing();
}

void SPDEntry::setProcessing(unsigned int processing) {
    rule.setProcessing(processing);
}

const IPsecSelector& SPDEntry::getSelector() const {
    return rule.getSelector();
}

const IPsecRule& SPDEntry::getRule() const {
    return rule;
}

IPsecRule& SPDEntry::getRuleForUpdate(){
    return rule;
}

void SPDEntry::setRule(const IPsecRule &rule) {
    this->rule = rule;
}

const std::vector<SADEntry*>& SPDEntry::getSadEntryList() const {
    return sadEntryList;
}

void SPDEntry::setSadEntryList(const std::vector<SADEntry*> &sadEntryList) {
    this->sadEntryList = sadEntryList;
}

void SPDEntry::setSelector(const IPsecSelector &selector) {
    rule.setSelector(selector);
}

//unsigned int SPDEntry::getSpi() const {
//    return spi;
//}
//
//void SPDEntry::setSpi(unsigned int spi) {
//    this->spi = spi;
//}

void SPDEntry::addSADEntry(SADEntry *sadEntry) {

    sadEntryList.push_back(sadEntry);
}

} /* namespace inet */


