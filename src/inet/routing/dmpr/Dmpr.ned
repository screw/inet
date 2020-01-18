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

package inet.routing.dmpr;

//
// TODO auto-generated module
//
simple Dmpr
{
    parameters:
        string interfaceTableModule = default("^.interfaceTable");
        string routingTableModule = default("^.ipv4.routingTable");
        string networkProtocolModule = default("^.ipv4.ip");
//        string dmprForwardingTableModule = default("^.dmprForwardingTable");
        
        double alpha = default(0.03);
        double interval = default(0.02);
        double logPar = default(1.2);
        bool randomNextHopEnabled = default(true); //in case there are multiple nextHops with equal 'congestion ratio' - random one is chosen instead of the first one
        bool lastNextHopIndexEnabled = default(false); //when enabled - starts iterating over nextHops with index+1 of the previous chosen nextHop
        
        @signal[congLevel*](type=double);
        @signal[downstreamCongLevel*](type=double);
        @signal[fwdCongLevel*](type=double);
        @signal[fwdPacketCount*](type=double);
        
        //@statistics -> created dynamically 
        // vectors for congLevel and inUseCongLevel  
        
}
