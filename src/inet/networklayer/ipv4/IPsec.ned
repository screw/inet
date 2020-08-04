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

package inet.networklayer.ipv4;

//
// TODO auto-generated module
//
simple IPsec
{
    parameters:
//        string interfaceTableModule = default("^.interfaceTable");
//        string routingTableModule = default("^.ipv4.routingTable");
        string networkProtocolModule = default("^.ip");
		string spdModule = default("^.spd");
		string sadModule = default("^.sad");
		xml spdConfig;// = default(xml("<spdConfig></spdConfig>"));
    
        
        double ahProtectOutDelay @unit(s) = default(0.0s);
        double ahProtectInDelay @unit(s) = default(0.0s);
        
        double espProtectOutDelay @unit(s) = default(0.0s);
        double espProtectInDelay @unit(s) = default(0.0s);


        @display("i=block/control;is=s");
}
