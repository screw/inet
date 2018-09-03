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

import inet.common.INETDefs;
import inet.common.TagBase;
import inet.networklayer.ipv4.Ipv4Header;

namespace inet;

class Ipv4OptionTagBase extends TagBase
{
    Ipv4OptionType type;
    Ipv4Option option;
}


class Ipv4RouteRecordTagBase extends Ipv4OptionTagBase
{
	type = IPOPTION_RECORD_ROUTE;
}

// request the 
class Ipv4RouteRecordReq extends Ipv4RouteRecordTagBase
{
    
    
}


class Ipv4RouteRecordInd extends Ipv4RouteRecordTagBase
{
}


class Ipv4StrictSourceRoutingTagBase extends Ipv4OptionTagBase
{
 	type = IPOPTION_STRICT_SOURCE_ROUTING;  
} 


class Ipv4StrictSourceRoutingReq extends Ipv4StrictSourceRoutingTagBase
{
 	
} 



class Ipv4StrictSourceRoutingInd extends Ipv4StrictSourceRoutingTagBase
{
 	
} 
