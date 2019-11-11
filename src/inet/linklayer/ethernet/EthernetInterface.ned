//
// Copyright (C) 2004 Andras Varga
//               2010 Zoltan Bojthe
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

package inet.linklayer.ethernet;

import inet.common.IHook;
import inet.queueing.contract.IPacketQueue;
import inet.linklayer.contract.IEtherMac;
import inet.linklayer.contract.IEthernetInterface;
import inet.linklayer.contract.ITrafficConditioner;
import inet.linklayer.ieee8021q.IIeee8021qEncap;
import inet.networklayer.common.InterfaceEntry;

//
// Ethernet network interface. Complements ~EtherMac and ~EtherEncap
// with an output queue for QoS and RED support.
//
module EthernetInterface extends InterfaceEntry like IEthernetInterface
{
    parameters:
        @class(InterfaceEntry);
        @lifecycleSupport;
        string interfaceTableModule;
        string address = default("auto");   // MAC address as hex string (12 hex digits), or
                                            // "auto". "auto" values will be replaced by
                                            // a generated MAC address in init stage 0.
        bool csmacdSupport = default(false);  // by default CSMA/CD is turned off, so only point-to-point duplex links are supported.
        string fcsMode @enum("declared","computed") = default("declared");
        int numOutputHooks = default(0);
        int numInputHooks = default(0);
        @display("i=block/ifcard;bgl=2");
        *.interfaceTableModule = default(absPath(interfaceTableModule));
        *.fcsMode = fcsMode;
    gates:
        input upperLayerIn;
        output upperLayerOut;
        inout phys @labels(EtherFrame);
    submodules:
        outputHook[numOutputHooks]: <default("Nop")> like IHook if numOutputHooks>0 {
            @display("p=100,100;is=s");
        }
        inputHook[numInputHooks]: <default("Nop")> like IHook if numInputHooks>0 {
            @display("p=400,100;is=s");
        }
        ingressTC: <default("")> like ITrafficConditioner if typename != "" {
            @display("p=400,100");
        }
        egressTC: <default("")> like ITrafficConditioner if typename != "" {
            @display("p=100,100");
        }
        mac: <default(csmacdSupport ? "EtherMac" : "EtherMacFullDuplex")> like IEtherMac {
            parameters:
                queue.packetCapacity = 10000;
                @display("p=250,400");
        }
        encap: <default("EtherEncap")> like IEtherEncap {
            parameters:
                @display("p=250,200");
        }
        qEncap: <default("")> like IIeee8021qEncap if typename != "" {
            parameters:
                @display("p=150,300");
        }
    connections:
        mac.upperLayerOut --> qEncap.lowerLayerIn if exists(qEncap);
        mac.upperLayerOut --> encap.lowerLayerIn if !exists(qEncap);

        mac.upperLayerIn <-- qEncap.lowerLayerOut if exists(qEncap);
        mac.upperLayerIn <-- encap.lowerLayerOut if !exists(qEncap);

        encap.lowerLayerOut --> qEncap.upperLayerIn if exists(qEncap);
        encap.lowerLayerIn <-- qEncap.upperLayerOut if exists(qEncap);

        mac.phys <--> { @display("m=s"); } <--> phys;

//        // no ingressTC
//        encap.upperLayerOut --> { @display("m=n"); } --> upperLayerOut if !exists(ingressTC);
//
//        // there is ingressTC
//        if exists(ingressTC) {
//            encap.upperLayerOut --> ingressTC.in;
//            ingressTC.out --> { @display("m=n"); } --> upperLayerOut;
//        }

        // no input hooks, no ingressTC
        encap.upperLayerOut --> { @display("m=n"); } --> upperLayerOut if numInputHooks == 0 && !exists(ingressTC);

        // no input hooks, there is ingressTC
        if numInputHooks == 0 && exists(ingressTC) {
            encap.upperLayerOut --> ingressTC.in;
            ingressTC.out --> { @display("m=n"); } --> upperLayerOut;
        }
        
        // there are input hooks
        if numInputHooks > 0 {
            inputHook[numInputHooks-1].out --> { @display("m=n"); } --> upperLayerOut;
        }
        for i=0..numInputHooks-2 {
            inputHook[i].out --> inputHook[i+1].in;
        }

        // there are input hooks, no ingressTC
        if numInputHooks > 0 && !exists(ingressTC) {
            encap.upperLayerOut --> inputHook[0].in;
        }

        // there are input hooks, there is ingressTC
        if numInputHooks > 0 && exists(ingressTC) {
            encap.upperLayerOut --> ingressTC.in;
            ingressTC.out --> inputHook[0].in;
        }

//        // no egressTC
//        upperLayerIn --> { @display("m=n"); } --> encap.upperLayerIn if !exists(egressTC);
//
//        // there is egressTC
//        if exists(egressTC) {
//            upperLayerIn --> { @display("m=n"); } --> egressTC.in;
//            egressTC.out --> encap.upperLayerIn;
//        }


          // chain output hooks
        for i=0..numOutputHooks-2 {
            outputHook[i].out --> outputHook[i+1].in;
        }

        // no output hooks, no egressTC
        upperLayerIn --> { @display("m=n"); } --> encap.upperLayerIn if numOutputHooks == 0 && !exists(egressTC);

        // there are output hooks
        if numOutputHooks > 0 {
            upperLayerIn --> { @display("m=n"); } --> outputHook[0].in;
        }

        // no output hooks, there is egressTC
        if numOutputHooks == 0 && exists(egressTC) {
            upperLayerIn --> { @display("m=n"); } --> egressTC.in;
        }

        // there is egressTC
        if exists(egressTC) {
            egressTC.out --> encap.upperLayerIn;
        }

        // there are output hooks, no egressTC
        if numOutputHooks > 0 && exists(egressTC) {
            outputHook[numOutputHooks-1].out --> encap.upperLayerIn;
        }

        // there are output hooks, there is egressTC
        if numOutputHooks > 0 && exists(egressTC) {
            outputHook[numOutputHooks-1].out --> egressTC.in;
        }
}

