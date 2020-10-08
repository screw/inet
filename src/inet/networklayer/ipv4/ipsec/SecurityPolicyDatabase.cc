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

#include "inet/networklayer/ipv4/ipsec/SecurityPolicyDatabase.h"

namespace inet {
namespace ipsec {

Define_Module(SecurityPolicyDatabase);

std::ostream& operator<<(std::ostream& os, const SecurityPolicyDatabase::Entry& e)
{
    os << e.str();
    return os;
};

void SecurityPolicyDatabase::initialize()
{
    WATCH_PTRVECTOR(entries);

}

SecurityPolicyDatabase::Entry* SecurityPolicyDatabase::findEntry(unsigned int direction, IPsecSelector *selector)
{
    std::vector<Entry*>::iterator it = entries.begin();

    for (; it != entries.end(); it++) {
        if(((*it)->getDirection() == direction || (*it)->getDirection() == IPsecRule::Direction::dIN_OUT ) && (*it)->getSelector().isMatchingSelector(selector) ) {
            return (*it);
        }
    }

    return nullptr;
}

void SecurityPolicyDatabase::addEntry(SecurityPolicyDatabase::Entry *entry)
{
    entries.push_back(entry);
}

void SecurityPolicyDatabase::refreshDisplay() const
{
    char buf[80];
    sprintf(buf, "entries: %ld", entries.size() );
    getDisplayString().setTagArg("t", 0, buf);
}


SecurityPolicyDatabase::Entry::Entry() {


}


SecurityPolicyDatabase::Entry::~Entry() {

}

unsigned int SecurityPolicyDatabase::Entry::getAction() const {
    return rule.getAction();
}

void SecurityPolicyDatabase::Entry::setAction(unsigned int action) {
    rule.setAction(action);
}

unsigned int SecurityPolicyDatabase::Entry::getDirection() const {
    return rule.getDirection();
}

void SecurityPolicyDatabase::Entry::setDirection(unsigned int direction) {
    rule.setDirection(direction);
}

unsigned int SecurityPolicyDatabase::Entry::getProcessing() const {
    return rule.getProcessing();
}

void SecurityPolicyDatabase::Entry::setProcessing(unsigned int processing) {
    rule.setProcessing(processing);
}

const IPsecSelector& SecurityPolicyDatabase::Entry::getSelector() const {
    return rule.getSelector();
}

const IPsecRule& SecurityPolicyDatabase::Entry::getRule() const {
    return rule;
}

IPsecRule& SecurityPolicyDatabase::Entry::getRuleForUpdate(){
    return rule;
}

void SecurityPolicyDatabase::Entry::setRule(const IPsecRule &rule) {
    this->rule = rule;
}

const std::vector<SecurityAssociationDatabase::Entry*>& SecurityPolicyDatabase::Entry::getEntries() const {
    return entries;
}

void SecurityPolicyDatabase::Entry::setEntries(const std::vector<SecurityAssociationDatabase::Entry*> &sadEntryList) {
    this->entries = sadEntryList;
}

void SecurityPolicyDatabase::Entry::setSelector(const IPsecSelector &selector) {
    rule.setSelector(selector);
}

void SecurityPolicyDatabase::Entry::addSADEntry(SecurityAssociationDatabase::Entry *sadEntry) {

    entries.push_back(sadEntry);
}

std::string SecurityPolicyDatabase::Entry::str() const
{
    std::stringstream out;

    out << rule;

    return out.str();
}


} //ipsec namespace
} //namespace
