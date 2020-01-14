//
//  Author: Marcel Marek (marcelma@ifi.uio.no)
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

package inet.node.dmpr;


import inet.node.inet.Router;
import inet.routing.dmpr.Dmpr;
import inet.routing.dmpr.DmprForwardingTable;

module DmprRouter extends Router
{
    parameters:
        bool hasDmpr = default(true);
        hasOspf = true;
        
        *.hasDmpr = hasDmpr;

    submodules:
        dmpr: Dmpr if hasDmpr {
            parameters:
                @display("p=1080.45,225");

        }
        dmprForwardingTable: DmprForwardingTable if hasDmpr {
            parameters:
                @display("p=1168.6499,225.22499");
        }

}
