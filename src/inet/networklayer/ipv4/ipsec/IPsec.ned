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

package inet.networklayer.ipv4.ipsec;

// <b> IPsec</b>
// This module represents the main part of the IPsec implementation.
// The model implements limited IPsec functionality.
// It supports Authentication Header (AH) and Encapsulating Security Payload (ESP) in transport mode for IPv4 unicast traffic (UDP/TCP/ICMP). 
// It is configured via an XML (spdConfig).
// The configuration specify the Security Policy Database (SPD) entries and the Security Association Database (SAD) entries that are used to filter the traffic and apply appropriate processing algorithms.
// It also supports modelling delay for protecting a packet.
// The main IPsec functionality is implemented as a dedicated module (iPsec) inside the network layer, and it is activated via hasIPsec variable in IPv4NetworkLayer.ned.
// The module registers a hook into IPv4 module which enables it to intercept all packets and apply IPsec rules.
//  The egress traffic is processed in datagramPostRoutingHook. If matching entry is found in the SPD (direction OUT and matching Selector) then depending on the Action the packet is:
//  PROTECT - the associated SA rules are applied. If the processing delay is bigger than 0s, the function return INetfilter::IHook::QUEUE and schedule self-message to reinject the datagram later.
//  BYPASS - packet is untouched
//  DROP - packet is discarded
//  The ingress traffic is processed in datagramLocalInHook. 
// First, the transportProtocol type is checked. If it is AH or ESP, then the SAD is searched for matching SPI. 
// If found, packet is decapsulated. If no matching entry is found, then the packet is discarded.
// Unprotected traffic (with transport type other than AH/ESP) is searched in SPD in the same way 
// as egress traffic. If matching rule with "BYPASS" action is found, the packet is accepted, discarded otherwise.

// This module also intialises SPD and SAD modules with rules found in the spdConfig.

// <b> Processing delay</b>
// There are separate delays for IN/OUT and AH/ESP traffic; four in total.
// The default is 0s or no delay.
// There are two queues IN/OUT. Only one packet can be processed at a time in either direction.
// If a second packet arrives during the IPsec processing (identified by lastProtected[In/Out]) the packet is QUEUED (using the hook interface) for additional time and reinjected later into the IPv4 layer.


// <b>Message overhead</b>
// Each protocol (AH/ESP) adds its own header which can be seen when running with QT interface. The header includes essential information for processing (Security Policy Index (SPI), Sequence Number, Next Header Type). The AH adds fixed header overhead of 24B. The ESP uses equation modelling confidentiality with AES (16B blocks) to add the correct padding.
//
// <b>Filter rules</b>
// The filter rules specify Direction, Protocol Type, Local/Remote Address and additionally Local/Remote Port or ICMP code (in case Protocol Type is ICMP). 
// The rules support wildcard (0.0.0.0 for IP address and 0 for Port and ICMP Code).
//
// <b>Configuration file</b>
// The configuration files specify the Security Policy Database (SPD) entries and the Security Association Database (SAD) entries that are used to filter the traffic and apply appropriate processing algorithms.
// The structure is documented as XSD XML schema.
// Each host or device has a separate section in the config and the spdConfig parameter of each node needs to point to the appropriate sub-section of the XML file.
// For example 
// **.client1.**spdConfig = xmldoc("ipsecConfig.xml", "ipsecConfig/Devices/Device[@id='client1']/")
// Each device section contains number of <SPDEntry> attributes.
// SPDEntry contains a Selector (Local/Remote Address/Port, ICMPCode), Direction (IN,OUT), Action (PROTECT, BYPASS, DROP).
// If the action is PROTECT, then there needs to be <SAEntry> with <Processing> (AH,ESP) and SPI(int).
// Since there is currently no IKE the configuration for pair of devices needs to specify matching SPI used by the receiver to find the matching SAD entry.
// The originator have SPDEntry OUT with SPI 10, then the receiver needs to have SPDEntry IN with SPI 10.


simple IPsec
{
    parameters:
		@display("i=block/control;is=s");
        
        string networkProtocolModule = default("^.ip");
		string spdModule = default("^.spd");
		string sadModule = default("^.sad");
		xml spdConfig;
          
        double ahProtectOutDelay @unit(s) = default(0.0s);
        double ahProtectInDelay @unit(s) = default(0.0s);
        
        double espProtectOutDelay @unit(s) = default(0.0s);
        double espProtectInDelay @unit(s) = default(0.0s);
        
        @signal[inProtectedAccept](type=long);
        @signal[inProtectedDrop](type=long);
        @signal[inUnprotectedBypass](type=long);
        @signal[inUnprotectedDrop](type=long);
        @signal[outBypass](type=long);
        @signal[outProtect](type=long);
        @signal[outDrop](type=long);
        @signal[inProcessDelay](type=double);
        @signal[outProcessDelay](type=double);
            
        @statistic[inProtectedAccept](title="Incoming Protected Packet Accepted"; record=count,vector);
        @statistic[inProtectedDrop](title="Incoming Protected Packet Dropped"; record=count,vector);
        @statistic[inUnprotectedBypass](title="Incoming Unprotected Packet Bypassed"; record=count,vector);
        @statistic[inUnprotectedDrop](title="Incoming Unprotected Packet Dropped"; record=count,vector);
        @statistic[outBypass](title="Outgoing Packet Bypassed"; record=count,vector);
        @statistic[outProtect](title="Outgoing Packet Protected"; record=count,vector);
        @statistic[outDrop](title="Outgoing Packet Dropped"; record=count,vector);
        @statistic[inProcessDelay](title="Incoming Processing Delay"; record=vector,stats);
        @statistic[outProcessDelay](title="Outgoing Processing Delay"; record=vector,stats);

}
