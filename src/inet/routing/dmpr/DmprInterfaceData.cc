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
#include "inet/networklayer/ipv4/Ipv4RoutingTable.h"


namespace inet {

DmprInterfaceData::DmprInterfaceData()
{

}
DmprInterfaceData::DmprInterfaceData(Dmpr* dmpr):
dmpr(dmpr)
{
//  dmpr = dmpr;
  //Note that the "zero" emit won't be recorded



//  congLevel = 0;
//  setCongestionLevel(0);
//  inUseCongLevel = 0;
//  setInUseCongLevel(0);
//  packetCount = 0;


//  lastChange = 0;

//  cModuleType *moduleType = cModuleType::get("inet.networklayer.ipv4.Ipv4RoutingTable");
//  table = (Ipv4RoutingTable*)moduleType->createScheduleInit("table", dmpr);
//  table = new Ipv4RoutingTable();
  table = new DmprForwardingTable();




}

//double DmprInterfaceData::getCongestionLevel() const
//{
//  return congLevel;
//}
//
//
//void DmprInterfaceData::setCongestionLevel(double congLevel)
//{
//  this->congLevel = congLevel;
//  dmpr->emitSignal(signalCongLevel, congLevel);
//}

DmprInterfaceData::~DmprInterfaceData()
{
  // TODO Auto-generated destructor stub
}

std::string DmprInterfaceData::str() const
{
  std::stringstream out;
//  out << "CongestLevel: " << getCongestionLevel() << "\n";
  return out.str();
}

std::string DmprInterfaceData::detailedInfo() const
{
  return str();
}


//int DmprInterfaceData::getPacketCount() const
//{
//  return packetCount;
//}

//void DmprInterfaceData::setPacketCount(int packetCount)
//{
//  this->packetCount = packetCount;
//}

//double DmprInterfaceData::getInUseCongLevel() const
//{
//  return inUseCongLevel;
//}
//
//const simtime_t& DmprInterfaceData::getLastChange() const
//{
//  return lastChange;
//}
//
//void DmprInterfaceData::setLastChange(const simtime_t& lastChange)
//{
//  this->lastChange = lastChange;
//}
//
//void DmprInterfaceData::setInUseCongLevel(double inUseCongLevel)
//{
//  this->inUseCongLevel = inUseCongLevel;
//  dmpr->emitSignal(signalInUseCongLevel, inUseCongLevel);
//}

//void DmprInterfaceData::incPacketCount()
//{
//  packetCount++;
//}

//simsignal_t DmprInterfaceData::getSignalCongLevel() const
//{
//  return signalCongLevel;
//}
//
//simsignal_t DmprInterfaceData::getSignalInUseCongLevel() const
//{
//  return signalInUseCongLevel;
//}
//
//void DmprInterfaceData::setSignalCongLevel(simsignal_t signalCongLevel)
//{
//  this->signalCongLevel = signalCongLevel;
//}
//
//void DmprInterfaceData::setSignalInUseCongLevel(simsignal_t signalInUseCongLevel)
//{
//  this->signalInUseCongLevel = signalInUseCongLevel;
//}

//double DmprInterfaceData::getFwdCongLevel() const
//{
//  return fwdCongLevel;
//}
//
//void DmprInterfaceData::setFwdCongLevel(double fwdCongLevel)
//{
//  this->fwdCongLevel = fwdCongLevel;
//}

} /* namespace inet */


