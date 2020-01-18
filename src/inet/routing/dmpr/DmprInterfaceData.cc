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

#include "inet/routing/dmpr/DmprInterfaceData.h"


namespace inet {

DmprInterfaceData::DmprInterfaceData()
{
  congestionLevel = 0;

}

int DmprInterfaceData::getCongestionLevel() const
{
  return congestionLevel;
}


void DmprInterfaceData::setCongestionLevel(int congestionLevel)
{
  this->congestionLevel = congestionLevel;
}

DmprInterfaceData::~DmprInterfaceData()
{
  // TODO Auto-generated destructor stub
}

std::string DmprInterfaceData::str() const
{
  std::stringstream out;
  out << "CongestLevel: " << getCongestionLevel() << "\n";
  return out.str();
}

std::string DmprInterfaceData::detailedInfo() const
{
  return str();
}

void DmprInterfaceData::incCongestionLevel()
{
  congestionLevel++;
}

} /* namespace inet */
