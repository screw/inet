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

#include "inet/networklayer/ipv4/ipsec/SecurityAssociationDatabase.h"

namespace inet {
namespace ipsec {

Define_Module(SecurityAssociationDatabase);

std::ostream& operator<<(std::ostream& os, const SecurityAssociationDatabase::Entry& e)
{
    os << e.str();
    return os;
};

void SecurityAssociationDatabase::initialize()
{
    WATCH_PTRVECTOR(entries);

}

void SecurityAssociationDatabase::addEntry(Entry *sadEntry) {

    entries.push_back(sadEntry);
}

SecurityAssociationDatabase::Entry* SecurityAssociationDatabase::findEntry(IPsecRule *rule, unsigned int spi)
{
    std::vector<Entry*>::iterator it = entries.begin();
    for (; it != entries.end(); it++) {
        if(((*it)->getDirection() == rule->getDirection() || (*it)->getDirection() == IPsecRule::Direction::dIN_OUT)
                && (*it)->getProcessing() == rule->getProcessing()
                && (*it)->getSpi() == spi){

            return (*it);
        }

    }

    return nullptr;
}

void SecurityAssociationDatabase::refreshDisplay() const
{
    char buf[80];
    sprintf(buf, "entries: %ld", entries.size() );
    getDisplayString().setTagArg("t", 0, buf);
}

SecurityAssociationDatabase::Entry::Entry() {


}

const IPsecRule& SecurityAssociationDatabase::Entry::getRule() const {
    return rule;
}

IPsecRule* SecurityAssociationDatabase::Entry::getRuleForUpdate()  {
    return &rule;
}

void SecurityAssociationDatabase::Entry::setRule(const IPsecRule &rule) {
    this->rule = rule;
}

unsigned int SecurityAssociationDatabase::Entry::getSpi() const {
    return spi;
}

void SecurityAssociationDatabase::Entry::setSpi(unsigned int spi) {
    this->spi = spi;
}

SecurityAssociationDatabase::Entry::~Entry() {

}


unsigned int SecurityAssociationDatabase::Entry::getProcessing() const {
    return rule.getProcessing();
}

void SecurityAssociationDatabase::Entry::setProcessing(unsigned int processing) {

    rule.setProcessing(processing);
}

unsigned int SecurityAssociationDatabase::Entry::getAction() const {

    return rule.getAction();
}

void SecurityAssociationDatabase::Entry::setAction(unsigned int action) {

    rule.setAction(action);
}

unsigned int SecurityAssociationDatabase::Entry::getDirection() const {
    return rule.getDirection();
}

unsigned int SecurityAssociationDatabase::Entry::getSeqNum() const {
    return seqNum;
}

unsigned int SecurityAssociationDatabase::Entry::getAndIncSeqNum() {

    this->seqNum +=  1;
    return seqNum - 1;
}

void SecurityAssociationDatabase::Entry::setSeqNum(unsigned int seqNum) {
    this->seqNum = seqNum;
}

void SecurityAssociationDatabase::Entry::setDirection(unsigned int direction) {

    rule.setDirection(direction);
}

std::string SecurityAssociationDatabase::Entry::str() const
{
    std::stringstream out;

    out << "SPI: " << spi;
    out << " " << rule;
    out << " SeqNum: " << seqNum;

    return out.str();
}

} //ipsec namespace
} //namespace


