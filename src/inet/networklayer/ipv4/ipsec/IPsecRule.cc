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

#include "inet/networklayer/ipv4/ipsec/IPsecRule.h"

namespace inet {
namespace ipsec {

std::ostream& operator<<(std::ostream& os, const IPsecRule& e)
{
    os << e.str();
    return os;
};

IPsecRule::IPsecRule() {


}

IPsecRule::~IPsecRule() {

}


unsigned int IPsecRule::getAction() const {
    return action;
}

void IPsecRule::setAction(unsigned int action) {
    this->action = action;
}

unsigned int IPsecRule::getDirection() const {
    return direction;
}

void IPsecRule::setDirection(unsigned int direction) {
    this->direction = direction;
}

unsigned int IPsecRule::getProcessing() const {
    return processing;
}

void IPsecRule::setProcessing(unsigned int processing) {
    this->processing = processing;
}

const IPsecSelector& IPsecRule::getSelector() const {
    return selector;
}

void IPsecRule::setSelector(const IPsecSelector &selector) {
    this->selector = selector;
}

std::string IPsecRule::str() const
{
    std::stringstream out;

    out << "Rule: " << selector;


    out << " Direction: ";
    switch(direction)
    {
        case Direction::dIN:
            out << "IN";
            break;
        case Direction::dOUT:
            out << "OUT";
            break;
        case Direction::dIN_OUT:
            out << "IN_OUT";
            break;
    }

    out << " Action: ";
    switch(action)
    {
        case Action::aBYPASS:
            out << "BYPASS";
            break;
        case Action::aDISCARD:
            out << "DISCARD";
            break;
        case Action::aPROTECT:
            out << "PROTECT";

            out << " Alg: ";
            switch(processing)
            {
                case ProcessingAlgs::pAH:
                    out << "AH";
                    break;
                case ProcessingAlgs::pESP:
                    out << "ESP";
                    break;
                case ProcessingAlgs::pAH_ESP:
                    out << "AH_ESP";
                    break;
            }

    }



    return out.str();
}

} //ipsec namespace
} /* namespace inet */
