//
// author: Thomas Reschka
//

package inet.examples.dmpr.meh;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.ethernet.EtherSwitch;
import inet.node.inet.Router;
import inet.node.dmpr.DmprRouter;
//import inet.nodes.inet.Router;
import inet.node.inet.StandardHost;
//import inet.nodes.inet.StandardHost;
import ned.DatarateChannel;

//import inet.linklayer.queue.REDDropper;

import inet.common.queue.IOutputQueue;
//import inet.linklayer.queue.DropTailQueue;
import inet.common.queue.FifoQueue;
import inet.common.queue.RedDropper;
//import inet.linklayer.queue.REDDropperNUMDual;
//import inet.linklayer.queue.REDDropperNUMDual2;

//module REDQueueNUM like IOutputQueue
//{
//    gates:
//        input in;
//        output out;
//
//    submodules:
//        red: REDDropperNUM {
//            numGates = 1;
//            wq = default(0.002);
//            minths = default("10");
//            maxths = default("40");
//            maxps = default("0.10");
//            pkrates = default("8333.333");
//        }
//        fifo: FIFOQueue;
//    connections:
//        in --> red.in[0];
//        red.out[0] --> fifo.in++;
//        fifo.out --> out;
//}

module REDQueueNUMDual like IOutputQueue
{
    gates:
        input in;
        output out;

    submodules:
        red: RedDropper {
            numGates = 1;
            wq = default(0.002);
            minths = default("10");
            maxths = default("40");
            maxps = default("0.10");
            pkrates = default("8333.333");
        }
        fifo: FifoQueue;
    connections:
        in --> red.in[0];
        red.out[0] --> fifo.in++;
        fifo.out --> out;
}

//module REDQueueNUMDual2 like IOutputQueue
//{
//    gates:
//        input in;
//        output out;
//
//    submodules:
//        red: REDDropperNUMDual2 {
//            numGates = 1;
//            wq = default(0.0002);
//            minths = default("10");
//            maxths = default("40");
//            maxps = default("0.10");
//            pkrates = default("8333.333");
//        }
//        fifo: FIFOQueue;
//    connections:
//        in --> red.in[0];
//        red.out[0] --> fifo.in++;
//        fifo.out --> out;
//}
//
//
//network Dumbbell
//{
//    parameters:
//        @display("bgb=740,580");
//    //        double ldelay @unit(s) = default(0.1s);
//    //        double ldatarate @unit(Mbps) = default(10Mbps);
//    types:
//        channel NormalP extends DatarateChannel
//        {
//            parameters:
//                //                datarate = ldatarate;
//                per = 0;
//                ber = 0;
//        }
//
//
//    submodules:
//        routerCore: DmprRouter {
//            parameters:
//                @display("p=315,141;i=abstract/router");
//        }
//        routerCore2: DmprRouter {
//            parameters:
//                @display("p=437,141;i=abstract/router");
//        }
//        client11: StandardHost {
//            parameters:
//                @display("p=50,100");
//        }
//        client12: StandardHost {
//            parameters:
//                @display("p=50,176");
//        }
//        client13: StandardHost {
//            parameters:
//                @display("p=32,239");
//        }
//        client14: StandardHost {
//            parameters:
//                @display("p=38,307");
//        }
//        client15: StandardHost {
//            parameters:
//                @display("p=44,386");
//        }
//        client16: StandardHost {
//            parameters:
//                @display("p=38,464");
//        }
//        client17: StandardHost {
//            parameters:
//                @display("p=83,518");
//        }
//        client18: StandardHost {
//            parameters:
//                @display("p=146,525");
//        }
//        client19: StandardHost {
//            parameters:
//                @display("p=204,525");
//        }
//        client110: StandardHost {
//            parameters:
//                @display("p=266,518");
//        }
//        client21: StandardHost {
//            parameters:
//                @display("p=584,100");
//        }
//        client22: StandardHost {
//            parameters:
//                @display("p=692,128");
//        }
//        client23: StandardHost {
//            parameters:
//                @display("p=681,200");
//        }
//        client24: StandardHost {
//            parameters:
//                @display("p=692,268");
//        }
//        client25: StandardHost {
//            parameters:
//                @display("p=692,337");
//        }
//        client26: StandardHost {
//            parameters:
//                @display("p=692,418");
//        }
//        client27: StandardHost {
//            parameters:
//                @display("p=692,503");
//        }
//        client28: StandardHost {
//            parameters:
//                @display("p=623,541");
//        }
//        client29: StandardHost {
//            parameters:
//                @display("p=558,541");
//        }
//        client210: StandardHost {
//            parameters:
//                @display("p=497,541");
//        }
//        configurator: Ipv4NetworkConfigurator{
////        	parameters:
////                @display("p=100,100;is=s");
////                config = xml("<config>"
////                        + "<interface among='routerCore routerCore1' address='192.168.1.x' netmask='255.255.255.x' />"
////                        + "<interface among='routerCore1 routerCore2' address='192.168.2.x' netmask='255.255.255.x' />"
////                        + "<interface among='routerCore2 routerCore3' address='192.168.3.x' netmask='255.255.255.x' />"
////                        + "<interface among='routerCore3 routerCore4' address='192.168.4.x' netmask='255.255.255.x' />"
////                        + "<interface among='routerCore4 routerCore5' address='192.168.5.x' netmask='255.255.255.x' />"
////                        + "<interface among='H2 R2' address='192.168.2.x' netmask='255.255.255.x' />"
////                        + "<interface among='R*' address='10.0.0.x' netmask='255.255.255.x' />"
////                        + "<multicast-group hosts='R*' address='224.0.0.5 224.0.0.6' />"
////                        + "<route hosts='H1' destination='*' gateway='R1'/>"
////                        + "<route hosts='H2' destination='*' gateway='R2'/>"
////                        + "<route hosts='R*' destination='224.0.0.0' netmask='240.0.0.0' interface='eth0'/>"
////                        + "<route hosts='R*' destination='224.0.0.0' netmask='240.0.0.0' interface='eth1'/>"
////                        + "<route hosts='R1 R2' destination='224.0.0.0' netmask='240.0.0.0' interface='eth2'/>"
////                        + "</config>");
//        }
//    connections:
//        routerCore.ethg++ <--> e1: NormalP <--> client11.ethg++;
//        routerCore.ethg++ <--> e2: NormalP <--> client12.ethg++;
//        routerCore.ethg++ <--> e3: NormalP <--> client13.ethg++;
//        routerCore.ethg++ <--> e4: NormalP <--> client14.ethg++;
//        routerCore.ethg++ <--> e5: NormalP <--> client15.ethg++;
//        routerCore.ethg++ <--> e6: NormalP <--> client16.ethg++;
//        routerCore.ethg++ <--> e7: NormalP <--> client17.ethg++;
//        routerCore.ethg++ <--> e8: NormalP <--> client18.ethg++;
//        routerCore.ethg++ <--> e9: NormalP <--> client19.ethg++;
//        routerCore.ethg++ <--> e10: NormalP <--> client110.ethg++;
//
//        routerCore2.ethg++ <--> e11: NormalP <--> client21.ethg++;
//        routerCore2.ethg++ <--> e12: NormalP <--> client22.ethg++;
//        routerCore2.ethg++ <--> e13: NormalP <--> client23.ethg++;
//        routerCore2.ethg++ <--> e14: NormalP <--> client24.ethg++;
//        routerCore2.ethg++ <--> e15: NormalP <--> client25.ethg++;
//        routerCore2.ethg++ <--> e16: NormalP <--> client26.ethg++;
//        routerCore2.ethg++ <--> e17: NormalP <--> client27.ethg++;
//        routerCore2.ethg++ <--> e18: NormalP <--> client28.ethg++;
//        routerCore2.ethg++ <--> e19: NormalP <--> client29.ethg++;
//        routerCore2.ethg++ <--> e20: NormalP <--> client210.ethg++;
//
//        routerCore.ethg++ <--> e21: NormalP <--> routerCore2.ethg++;
//}

network ParkingLot
{
    parameters:
        @display("bgb=1491.5961,1313.776");
    //        double ldelay @unit(s) = default(0.1s);
    //            double ldatarate @unit(Mbps) = default(10Mbps);
    types:
        channel NormalP extends DatarateChannel
        {
            parameters:
                datarate = 1Gbps;
                delay = 1ms;
                per = 0;
                ber = 0;
        }


    submodules:
        routerCore: DmprRouter {
            parameters:
                @display("p=144,128;i=abstract/router");
        }
        routerCore2: DmprRouter {
            parameters:
                @display("p=463,128;i=abstract/router");
        }
        client11: StandardHost {
            parameters:
                @display("p=121.336006,479.06802");
        }
        client12: StandardHost {
            parameters:
                @display("p=188.28,535.552");
        }
        client23: StandardHost {
            parameters:
                @display("p=499.988,479.06802");
        }
        configurator: Ipv4NetworkConfigurator;
        routerCore1: DmprRouter {
            parameters:
                @display("p=307,128;i=abstract/router");
        }
        client22: StandardHost {
            parameters:
                @display("p=340.996,479.06802");
        }
        client13: StandardHost {
            parameters:
                @display("p=257.316,535.552");
        }
        routerCore3: DmprRouter {
            parameters:
                @display("p=644,128;i=abstract/router");
        }
        client24: StandardHost {
            parameters:
                @display("p=677.80804,479.06802");
        }
        client14: StandardHost {
            parameters:
                @display("p=412.12402,535.552");
        }
        client21: StandardHost {
            parameters:
                @display("p=1165.244,479.06802");
        }
        routerCore4: DmprRouter {
            parameters:
                @display("p=828,128;i=abstract/router");
        }
        client15: StandardHost {
            parameters:
                @display("p=598.312,535.552");
        }
        client25: StandardHost {
            parameters:
                @display("p=893.284,479.06802");
        }
        routerCore5: DmprRouter {
            parameters:
                @display("p=999,119;i=abstract/router");
        }
        client16: StandardHost {
            parameters:
                @display("p=794.96,535.552");
        }
        client26: StandardHost {
            parameters:
                @display("p=1004.16003,479.06802");
        }
        client111: StandardHost {
            parameters:
                @display("p=121.336006,535.552");
        }
        client112: StandardHost {
            parameters:
                @display("p=121.336006,583.668");
        }
        client113: StandardHost {
            parameters:
                @display("p=121.336006,638.06");
        }
        client114: StandardHost {
            parameters:
                @display("p=121.336006,692.452");
        }
        client115: StandardHost {
            parameters:
                @display("p=121.336006,742.66003");
        }
        client116: StandardHost {
            parameters:
                @display("p=121.336006,792.86804");
        }
        client117: StandardHost {
            parameters:
                @display("p=121.336006,845.168");
        }
        client118: StandardHost {
            parameters:
                @display("p=121.336006,897.468");
        }
        client119: StandardHost {
            parameters:
                @display("p=121.336006,951.86005");
        }
        client1110: StandardHost {
            parameters:
                @display("p=121.336006,1010.43604");
        }
        client1111: StandardHost {
            parameters:
                @display("p=121.336006,1056.4601");
        }
        client1112: StandardHost {
            parameters:
                @display("p=121.336006,1104.576");
        }
        client1113: StandardHost {
            parameters:
                @display("p=121.336006,1152.692");
        }
        client1114: StandardHost {
            parameters:
                @display("p=121.336006,1204.9921");
        }
        client1115: StandardHost {
            parameters:
                @display("p=69.036,479.06802");
        }
        client1116: StandardHost {
            parameters:
                @display("p=69.036,535.552");
        }
        client1117: StandardHost {
            parameters:
                @display("p=69.036,583.668");
        }
        client1118: StandardHost {
            parameters:
                @display("p=69.036,638.06");
        }
        client1119: StandardHost {
            parameters:
                @display("p=69.036,692.452");
        }
        client1120: StandardHost {
            parameters:
                @display("p=69.036,742.66003");
        }
        client1121: StandardHost {
            parameters:
                @display("p=69.036,792.86804");
        }
        client1122: StandardHost {
            parameters:
                @display("p=69.036,845.168");
        }
        client1123: StandardHost {
            parameters:
                @display("p=69.036,897.468");
        }
        client1124: StandardHost {
            parameters:
                @display("p=69.036,951.86005");
        }
        client1125: StandardHost {
            parameters:
                @display("p=69.036,1004.16003");
        }
        client1126: StandardHost {
            parameters:
                @display("p=69.036,1056.4601");
        }
        client1127: StandardHost {
            parameters:
                @display("p=69.036,1104.576");
        }
        client1128: StandardHost {
            parameters:
                @display("p=69.036,1152.692");
        }
        client1129: StandardHost {
            parameters:
                @display("p=69.036,1204.9921");
        }
        client121: StandardHost {
            parameters:
                @display("p=188.28,583.668");
        }
        client122: StandardHost {
            parameters:
                @display("p=188.28,638.06");
        }
        client123: StandardHost {
            parameters:
                @display("p=188.28,692.452");
        }
        client124: StandardHost {
            parameters:
                @display("p=188.28,742.66003");
        }
        client125: StandardHost {
            parameters:
                @display("p=188.28,792.86804");
        }
        client126: StandardHost {
            parameters:
                @display("p=188.28,845.168");
        }
        client127: StandardHost {
            parameters:
                @display("p=188.28,897.468");
        }
        client131: StandardHost {
            parameters:
                @display("p=257.316,583.668");
        }
        client132: StandardHost {
            parameters:
                @display("p=257.316,638.06");
        }
        client133: StandardHost {
            parameters:
                @display("p=257.316,692.452");
        }
        client134: StandardHost {
            parameters:
                @display("p=257.316,742.66003");
        }
        client135: StandardHost {
            parameters:
                @display("p=257.316,792.86804");
        }
        client136: StandardHost {
            parameters:
                @display("p=257.316,845.168");
        }
        client137: StandardHost {
            parameters:
                @display("p=257.316,897.468");
        }
        client221: StandardHost {
            parameters:
                @display("p=340.996,535.552");
        }
        client222: StandardHost {
            parameters:
                @display("p=340.996,583.668");
        }
        client223: StandardHost {
            parameters:
                @display("p=340.996,638.06");
        }
        client224: StandardHost {
            parameters:
                @display("p=340.996,692.452");
        }
        client225: StandardHost {
            parameters:
                @display("p=340.996,742.66003");
        }
        client226: StandardHost {
            parameters:
                @display("p=340.996,792.86804");
        }
        client227: StandardHost {
            parameters:
                @display("p=340.996,845.168");
        }
        client141: StandardHost {
            parameters:
                @display("p=412.12402,583.668");
        }
        client142: StandardHost {
            parameters:
                @display("p=412.12402,638.06");
        }
        client143: StandardHost {
            parameters:
                @display("p=412.12402,692.452");
        }
        client144: StandardHost {
            parameters:
                @display("p=412.12402,742.66003");
        }
        client145: StandardHost {
            parameters:
                @display("p=412.12402,792.86804");
        }
        client146: StandardHost {
            parameters:
                @display("p=412.12402,845.168");
        }
        client147: StandardHost {
            parameters:
                @display("p=412.12402,897.468");
        }
        client231: StandardHost {
            parameters:
                @display("p=499.988,535.552");
        }
        client232: StandardHost {
            parameters:
                @display("p=499.988,583.668");
        }
        client233: StandardHost {
            parameters:
                @display("p=499.988,638.06");
        }
        client234: StandardHost {
            parameters:
                @display("p=499.988,692.452");
        }
        client235: StandardHost {
            parameters:
                @display("p=499.988,742.66003");
        }
        client236: StandardHost {
            parameters:
                @display("p=499.988,792.86804");
        }
        client237: StandardHost {
            parameters:
                @display("p=499.988,845.168");
        }
        client151: StandardHost {
            parameters:
                @display("p=598.312,583.668");
        }
        client152: StandardHost {
            parameters:
                @display("p=598.312,638.06");
        }
        client153: StandardHost {
            parameters:
                @display("p=598.312,692.452");
        }
        client154: StandardHost {
            parameters:
                @display("p=598.312,742.66003");
        }
        client155: StandardHost {
            parameters:
                @display("p=598.312,792.86804");
        }
        client156: StandardHost {
            parameters:
                @display("p=598.312,845.168");
        }
        client157: StandardHost {
            parameters:
                @display("p=598.312,897.468");
        }
        client241: StandardHost {
            parameters:
                @display("p=677.80804,535.552");
        }
        client242: StandardHost {
            parameters:
                @display("p=677.80804,583.668");
        }
        client243: StandardHost {
            parameters:
                @display("p=677.80804,638.06");
        }
        client244: StandardHost {
            parameters:
                @display("p=677.80804,692.452");
        }
        client245: StandardHost {
            parameters:
                @display("p=677.80804,742.66003");
        }
        client246: StandardHost {
            parameters:
                @display("p=677.80804,792.86804");
        }
        client247: StandardHost {
            parameters:
                @display("p=677.80804,845.168");
        }
        client161: StandardHost {
            parameters:
                @display("p=794.96,583.668");
        }
        client162: StandardHost {
            parameters:
                @display("p=794.96,638.06");
        }
        client163: StandardHost {
            parameters:
                @display("p=794.96,692.452");
        }
        client164: StandardHost {
            parameters:
                @display("p=794.96,742.66003");
        }
        client165: StandardHost {
            parameters:
                @display("p=794.96,792.86804");
        }
        client166: StandardHost {
            parameters:
                @display("p=794.96,845.168");
        }
        client167: StandardHost {
            parameters:
                @display("p=794.96,897.468");
        }
        client251: StandardHost {
            parameters:
                @display("p=893.284,535.552");
        }
        client252: StandardHost {
            parameters:
                @display("p=893.284,583.668");
        }
        client253: StandardHost {
            parameters:
                @display("p=893.284,638.06");
        }
        client254: StandardHost {
            parameters:
                @display("p=893.284,692.452");
        }
        client255: StandardHost {
            parameters:
                @display("p=893.284,742.66003");
        }
        client256: StandardHost {
            parameters:
                @display("p=893.284,792.86804");
        }
        client257: StandardHost {
            parameters:
                @display("p=893.284,845.168");
        }
        client261: StandardHost {
            parameters:
                @display("p=1004.16003,535.552");
        }
        client262: StandardHost {
            parameters:
                @display("p=1004.16003,583.668");
        }
        client263: StandardHost {
            parameters:
                @display("p=1004.16003,638.06");
        }
        client264: StandardHost {
            parameters:
                @display("p=1004.16003,692.452");
        }
        client265: StandardHost {
            parameters:
                @display("p=1004.16003,742.66003");
        }
        client266: StandardHost {
            parameters:
                @display("p=1004.16003,792.86804");
        }
        client267: StandardHost {
            parameters:
                @display("p=1004.16003,845.168");
        }
        client211: StandardHost {
            parameters:
                @display("p=1158.968,535.552");
        }
        client212: StandardHost {
            parameters:
                @display("p=1158.968,583.668");
        }
        client213: StandardHost {
            parameters:
                @display("p=1158.968,638.06");
        }
        client214: StandardHost {
            parameters:
                @display("p=1158.968,692.452");
        }
        client215: StandardHost {
            parameters:
                @display("p=1158.968,742.66003");
        }
        client216: StandardHost {
            parameters:
                @display("p=1158.968,792.86804");
        }
        client217: StandardHost {
            parameters:
                @display("p=1158.968,845.168");
        }
        client218: StandardHost {
            parameters:
                @display("p=1158.968,897.468");
        }
        client219: StandardHost {
            parameters:
                @display("p=1158.968,951.86005");
        }
        client2110: StandardHost {
            parameters:
                @display("p=1158.968,1010.43604");
        }
        client2111: StandardHost {
            parameters:
                @display("p=1158.968,1056.4601");
        }
        client2112: StandardHost {
            parameters:
                @display("p=1158.968,1104.576");
        }
        client2113: StandardHost {
            parameters:
                @display("p=1158.968,1152.692");
        }
        client2114: StandardHost {
            parameters:
                @display("p=1158.968,1204.9921");
        }
        client2115: StandardHost {
            parameters:
                @display("p=1104.576,479.06802");
        }
        client2116: StandardHost {
            parameters:
                @display("p=1104.576,535.552");
        }
        client2117: StandardHost {
            parameters:
                @display("p=1104.576,583.668");
        }
        client2118: StandardHost {
            parameters:
                @display("p=1104.576,638.06");
        }
        client2119: StandardHost {
            parameters:
                @display("p=1104.576,692.452");
        }
        client2120: StandardHost {
            parameters:
                @display("p=1104.576,742.66003");
        }
        client2121: StandardHost {
            parameters:
                @display("p=1104.576,792.86804");
        }
        client2122: StandardHost {
            parameters:
                @display("p=1104.576,845.168");
        }
        client2123: StandardHost {
            parameters:
                @display("p=1104.576,897.468");
        }
        client2124: StandardHost {
            parameters:
                @display("p=1104.576,951.86005");
        }
        client2125: StandardHost {
            parameters:
                @display("p=1104.576,1004.16003");
        }
        client2126: StandardHost {
            parameters:
                @display("p=1104.576,1056.4601");
        }
        client2127: StandardHost {
            parameters:
                @display("p=1104.576,1104.576");
        }
        client2128: StandardHost {
            parameters:
                @display("p=1104.576,1152.692");
        }
        client2129: StandardHost {
            parameters:
                @display("p=1104.576,1204.9921");
        }
        etherSwitch: EtherSwitch {
            @display("p=121.336006,259.40802");
        }
        etherSwitch1: EtherSwitch {
            @display("p=305.432,259.40802");
        }
        etherSwitch2: EtherSwitch {
            @display("p=485.34402,242.67201");
        }
        etherSwitch3: EtherSwitch {
            @display("p=665.25604,242.67201");
        }
        etherSwitch4: EtherSwitch {
            @display("p=849.35205,242.67201");
        }
        etherSwitch5: EtherSwitch {
            @display("p=1020.89606,242.67201");
        }
    connections:

        routerCore.ethg++ <--> NormalP <--> routerCore1.ethg++;
        routerCore1.ethg++ <--> NormalP <--> routerCore2.ethg++;
        routerCore2.ethg++ <--> NormalP <--> routerCore3.ethg++;
        routerCore3.ethg++ <--> NormalP <--> routerCore4.ethg++;
        routerCore4.ethg++ <--> NormalP <--> routerCore5.ethg++;

        etherSwitch.ethg++ <--> NormalP <--> routerCore.ethg++;
        etherSwitch1.ethg++ <--> NormalP <--> routerCore1.ethg++;
        etherSwitch2.ethg++ <--> NormalP <--> routerCore2.ethg++;
        etherSwitch3.ethg++ <--> NormalP <--> routerCore3.ethg++;
        etherSwitch4.ethg++ <--> NormalP <--> routerCore4.ethg++;
        etherSwitch5.ethg++ <--> NormalP <--> routerCore5.ethg++;

        etherSwitch.ethg++ <--> NormalP <--> client11.ethg++;
        etherSwitch.ethg++ <--> NormalP <--> client12.ethg++;
        etherSwitch2.ethg++ <--> NormalP <--> client23.ethg++;



        client22.ethg++ <--> NormalP <--> etherSwitch1.ethg++;



        client13.ethg++ <--> NormalP <--> etherSwitch1.ethg++;
        
        client14.ethg++ <--> NormalP <--> etherSwitch2.ethg++;
        etherSwitch3.ethg++ <--> NormalP <--> client24.ethg++;



        client15.ethg++ <--> NormalP <--> etherSwitch3.ethg++;
        etherSwitch4.ethg++ <--> NormalP <--> client25.ethg++;



        client16.ethg++ <--> NormalP <--> etherSwitch4.ethg++;
        client26.ethg++ <--> NormalP <--> etherSwitch5.ethg++;
        client21.ethg++ <--> NormalP <--> etherSwitch5.ethg++;
        client1115.ethg++ <--> NormalP <--> etherSwitch.ethg++;
        client1116.ethg++ <--> NormalP <--> etherSwitch.ethg++;
        client1117.ethg++ <--> NormalP <--> etherSwitch.ethg++;
        client1118.ethg++ <--> NormalP <--> etherSwitch.ethg++;
        client111.ethg++ <--> NormalP <--> etherSwitch.ethg++;
        client112.ethg++ <--> NormalP <--> etherSwitch.ethg++;
        client113.ethg++ <--> NormalP <--> etherSwitch.ethg++;
        client1119.ethg++ <--> NormalP <--> etherSwitch.ethg++;
        client114.ethg++ <--> NormalP <--> etherSwitch.ethg++;
        client1120.ethg++ <--> NormalP <--> etherSwitch.ethg++;
        client115.ethg++ <--> NormalP <--> etherSwitch.ethg++;
        client1121.ethg++ <--> NormalP <--> etherSwitch.ethg++;
        client116.ethg++ <--> NormalP <--> etherSwitch.ethg++;
        client1122.ethg++ <--> NormalP <--> etherSwitch.ethg++;
        client117.ethg++ <--> NormalP <--> etherSwitch.ethg++;
        client1123.ethg++ <--> NormalP <--> etherSwitch.ethg++;
        client118.ethg++ <--> NormalP <--> etherSwitch.ethg++;
        client1124.ethg++ <--> NormalP <--> etherSwitch.ethg++;
        client119.ethg++ <--> NormalP <--> etherSwitch.ethg++;
        client1125.ethg++ <--> NormalP <--> etherSwitch.ethg++;
        client1110.ethg++ <--> NormalP <--> etherSwitch.ethg++;
        client1126.ethg++ <--> NormalP <--> etherSwitch.ethg++;
        client1111.ethg++ <--> NormalP <--> etherSwitch.ethg++;
        client1127.ethg++ <--> NormalP <--> etherSwitch.ethg++;
        client1112.ethg++ <--> NormalP <--> etherSwitch.ethg++;
        client1128.ethg++ <--> NormalP <--> etherSwitch.ethg++;
        client1113.ethg++ <--> NormalP <--> etherSwitch.ethg++;
        client1129.ethg++ <--> NormalP <--> etherSwitch.ethg++;
        client1114.ethg++ <--> NormalP <--> etherSwitch.ethg++;
        client121.ethg++ <--> NormalP <--> etherSwitch.ethg++;
        client122.ethg++ <--> NormalP <--> etherSwitch.ethg++;
        client123.ethg++ <--> NormalP <--> etherSwitch.ethg++;
        client124.ethg++ <--> NormalP <--> etherSwitch.ethg++;
        client125.ethg++ <--> NormalP <--> etherSwitch.ethg++;
        client126.ethg++ <--> NormalP <--> etherSwitch.ethg++;
        client127.ethg++ <--> NormalP <--> etherSwitch.ethg++;
        client131.ethg++ <--> NormalP <--> etherSwitch1.ethg++;
        client132.ethg++ <--> NormalP <--> etherSwitch1.ethg++;
        client133.ethg++ <--> NormalP <--> etherSwitch1.ethg++;
        client134.ethg++ <--> NormalP <--> etherSwitch1.ethg++;
        client135.ethg++ <--> NormalP <--> etherSwitch1.ethg++;
        client136.ethg++ <--> NormalP <--> etherSwitch1.ethg++;
        client137.ethg++ <--> NormalP <--> etherSwitch1.ethg++;
        client221.ethg++ <--> NormalP <--> etherSwitch1.ethg++;
        client222.ethg++ <--> NormalP <--> etherSwitch1.ethg++;
        client223.ethg++ <--> NormalP <--> etherSwitch1.ethg++;
        client224.ethg++ <--> NormalP <--> etherSwitch1.ethg++;
        client225.ethg++ <--> NormalP <--> etherSwitch1.ethg++;
        client226.ethg++ <--> NormalP <--> etherSwitch1.ethg++;
        client227.ethg++ <--> NormalP <--> etherSwitch1.ethg++;
        client141.ethg++ <--> NormalP <--> etherSwitch2.ethg++;
        client142.ethg++ <--> NormalP <--> etherSwitch2.ethg++;
        client143.ethg++ <--> NormalP <--> etherSwitch2.ethg++;
        client144.ethg++ <--> NormalP <--> etherSwitch2.ethg++;
        client145.ethg++ <--> NormalP <--> etherSwitch2.ethg++;
        client146.ethg++ <--> NormalP <--> etherSwitch2.ethg++;
        client147.ethg++ <--> NormalP <--> etherSwitch2.ethg++;
        client231.ethg++ <--> NormalP <--> etherSwitch2.ethg++;
        client232.ethg++ <--> NormalP <--> etherSwitch2.ethg++;
        client233.ethg++ <--> NormalP <--> etherSwitch2.ethg++;
        client234.ethg++ <--> NormalP <--> etherSwitch2.ethg++;
        client235.ethg++ <--> NormalP <--> etherSwitch2.ethg++;
        client236.ethg++ <--> NormalP <--> etherSwitch2.ethg++;
        client237.ethg++ <--> NormalP <--> etherSwitch2.ethg++;
        client151.ethg++ <--> NormalP <--> etherSwitch3.ethg++;
        client152.ethg++ <--> NormalP <--> etherSwitch3.ethg++;
        client153.ethg++ <--> NormalP <--> etherSwitch3.ethg++;
        client154.ethg++ <--> NormalP <--> etherSwitch3.ethg++;
        client155.ethg++ <--> NormalP <--> etherSwitch3.ethg++;
        client156.ethg++ <--> NormalP <--> etherSwitch3.ethg++;
        client157.ethg++ <--> NormalP <--> etherSwitch3.ethg++;
        client241.ethg++ <--> NormalP <--> etherSwitch3.ethg++;
        client242.ethg++ <--> NormalP <--> etherSwitch3.ethg++;
        client243.ethg++ <--> NormalP <--> etherSwitch3.ethg++;
        client244.ethg++ <--> NormalP <--> etherSwitch3.ethg++;
        client245.ethg++ <--> NormalP <--> etherSwitch3.ethg++;
        client246.ethg++ <--> NormalP <--> etherSwitch3.ethg++;
        client247.ethg++ <--> NormalP <--> etherSwitch3.ethg++;
        client161.ethg++ <--> NormalP <--> etherSwitch4.ethg++;
        client162.ethg++ <--> NormalP <--> etherSwitch4.ethg++;
        client163.ethg++ <--> NormalP <--> etherSwitch4.ethg++;
        client164.ethg++ <--> NormalP <--> etherSwitch4.ethg++;
        client165.ethg++ <--> NormalP <--> etherSwitch4.ethg++;
        client166.ethg++ <--> NormalP <--> etherSwitch4.ethg++;
        client167.ethg++ <--> NormalP <--> etherSwitch4.ethg++;
        client251.ethg++ <--> NormalP <--> etherSwitch4.ethg++;
        client252.ethg++ <--> NormalP <--> etherSwitch4.ethg++;
        client253.ethg++ <--> NormalP <--> etherSwitch4.ethg++;
        client254.ethg++ <--> NormalP <--> etherSwitch4.ethg++;
        client255.ethg++ <--> NormalP <--> etherSwitch4.ethg++;
        client256.ethg++ <--> NormalP <--> etherSwitch4.ethg++;
        client257.ethg++ <--> NormalP <--> etherSwitch4.ethg++;
        client261.ethg++ <--> NormalP <--> etherSwitch5.ethg++;
        client262.ethg++ <--> NormalP <--> etherSwitch5.ethg++;
        client263.ethg++ <--> NormalP <--> etherSwitch5.ethg++;
        client264.ethg++ <--> NormalP <--> etherSwitch5.ethg++;
        client265.ethg++ <--> NormalP <--> etherSwitch5.ethg++;
        client266.ethg++ <--> NormalP <--> etherSwitch5.ethg++;
        client267.ethg++ <--> NormalP <--> etherSwitch5.ethg++;
        client2115.ethg++ <--> NormalP <--> etherSwitch5.ethg++;
        client2116.ethg++ <--> NormalP <--> etherSwitch5.ethg++;
        client211.ethg++ <--> NormalP <--> etherSwitch5.ethg++;
        client2117.ethg++ <--> NormalP <--> etherSwitch5.ethg++;
        client212.ethg++ <--> NormalP <--> etherSwitch5.ethg++;
        client2118.ethg++ <--> NormalP <--> etherSwitch5.ethg++;
        client213.ethg++ <--> NormalP <--> etherSwitch5.ethg++;
        client2119.ethg++ <--> NormalP <--> etherSwitch5.ethg++;
        client214.ethg++ <--> NormalP <--> etherSwitch5.ethg++;
        client2120.ethg++ <--> NormalP <--> etherSwitch5.ethg++;
        client215.ethg++ <--> NormalP <--> etherSwitch5.ethg++;
        client2121.ethg++ <--> NormalP <--> etherSwitch5.ethg++;
        client216.ethg++ <--> NormalP <--> etherSwitch5.ethg++;
        client2122.ethg++ <--> NormalP <--> etherSwitch5.ethg++;
        client217.ethg++ <--> NormalP <--> etherSwitch5.ethg++;
        client2123.ethg++ <--> NormalP <--> etherSwitch5.ethg++;
        client218.ethg++ <--> NormalP <--> etherSwitch5.ethg++;
        client2124.ethg++ <--> NormalP <--> etherSwitch5.ethg++;
        client219.ethg++ <--> NormalP <--> etherSwitch5.ethg++;
        client2125.ethg++ <--> NormalP <--> etherSwitch5.ethg++;
        client2110.ethg++ <--> NormalP <--> etherSwitch5.ethg++;
        client2111.ethg++ <--> NormalP <--> etherSwitch5.ethg++;
        client2126.ethg++ <--> NormalP <--> etherSwitch5.ethg++;
        client2112.ethg++ <--> NormalP <--> etherSwitch5.ethg++;
        client2127.ethg++ <--> NormalP <--> etherSwitch5.ethg++;
        client2113.ethg++ <--> NormalP <--> etherSwitch5.ethg++;
        client2128.ethg++ <--> NormalP <--> etherSwitch5.ethg++;
        client2114.ethg++ <--> NormalP <--> etherSwitch5.ethg++;
        client2129.ethg++ <--> NormalP <--> etherSwitch5.ethg++;
}
