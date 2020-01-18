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
  inUseCongLevel = 0;
  packetCount = 0;

  lastChange = 0;

}

double DmprInterfaceData::getCongestionLevel() const
{
  return congestionLevel;
}


void DmprInterfaceData::setCongestionLevel(double congestionLevel)
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

int DmprInterfaceData::getPacketCount() const
{
  return packetCount;
}

void DmprInterfaceData::setPacketCount(int packetCount)
{
  this->packetCount = packetCount;
}

double DmprInterfaceData::getInUseCongLevel() const
{
  return inUseCongLevel;
}

const simtime_t& DmprInterfaceData::getLastChange() const
{
  return lastChange;
}

void DmprInterfaceData::setLastChange(const simtime_t& lastChange)
{
  this->lastChange = lastChange;
}

void DmprInterfaceData::setInUseCongLevel(double inUseCongLevel)
{
  this->inUseCongLevel = inUseCongLevel;
}

void DmprInterfaceData::incPacketCount()
{
  packetCount++;
}

} /* namespace inet */


