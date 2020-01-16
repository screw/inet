//
// author: Thomas Reschka
//

package inet.examples.dmpr.fulltest;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.ethernet.EtherSwitch;
import inet.node.inet.Router;
import inet.node.dmpr.DmprRouter;
//import inet.nodes.inet.Router;
import inet.node.inet.StandardHost;
//import inet.nodes.inet.StandardHost;
import ned.DatarateChannel;

//import inet.linklayer.queue.REDDropper;


//import inet.common.queue.IOutputQueue;
////import inet.linklayer.queue.DropTailQueue;
//import inet.common.queue.FifoQueue;
////import inet.common.queue.RedDropper;
//import inet.common.queue.RedMarker;
////import inet.linklayer.queue.REDDropperNUMDual;
////import inet.linklayer.queue.REDDropperNUMDual2;


import inet.visualizer.integrated.IntegratedCanvasVisualizer;



//module RedMarkerQueue like IOutputQueue
//{
//    gates:
//        input in;
//        output out;
//
//    submodules:
//        red: RedMarker {
//            numGates = 1;
//            wq = 0.0032;
//            minths = "5";
//            maxths = "40";
//            maxps = "0.10";
//            pkrates = "8333.333";
//            marks = "1";
//        }
//        fifo: FifoQueue;
//    connections:
//        in --> red.in[0];
//        red.out[0] --> fifo.in++;
//        fifo.out --> out;
//}
network Fulltest
{
    parameters:
        @display("bgb=4166.738,3051.564;bgl=8");
    //        double ldelay @unit(s) = default(0.1s);
    //            double ldatarate @unit(Mbps) = default(10Mbps);
    types:
        channel NormalP extends DatarateChannel
        {
            parameters:
                datarate = 10Mbps;
                delay = 1ms;
                per = 0;
                ber = 0;
        }
        channel GEth extends DatarateChannel
        {
            parameters:
                datarate = 1Gbps;
                delay = 1ms;
                per = 0;
                ber = 0;
        }


    submodules:
        visualizer: IntegratedCanvasVisualizer {
            @display("p=356.4,118.799995");
        }

        routerCore1: DmprRouter {
            parameters:
                @display("p=1040.518,1371.804;i=abstract/router");
        }
        routerCore2: DmprRouter {
            parameters:
                @display("p=1068.5139,1903.7279;i=abstract/router");
        }
        routerCore3: DmprRouter {
            parameters:
                @display("p=1871.0659,2090.368;i=abstract/router");
        }
        routerCore4: DmprRouter {
            parameters:
                @display("p=2766.938,1871.0659;i=abstract/router");
        }
        routerCore5: DmprRouter {
            parameters:
                @display("p=2696.948,1222.492;i=abstract/router");
        }
        routerCore6: DmprRouter {
            parameters:
                @display("p=1861.7339,1035.8519;i=abstract/router");
        }

        configurator: Ipv4NetworkConfigurator {
            parameters:
                @display("p=100,100;is=s");
                config = xml("<config>"
                        + "<interface among='routerCore1 routerCore2' address='10.1.2.x' netmask='255.255.255.x' />"
                        + "<interface among='routerCore2 routerCore3' address='10.2.3.x' netmask='255.255.255.x' />"
                        + "<interface among='routerCore3 routerCore4' address='10.3.4.x' netmask='255.255.255.x' />"
                        + "<interface among='routerCore4 routerCore5' address='10.4.5.x' netmask='255.255.255.x' />"
                        + "<interface among='routerCore5 routerCore6' address='10.5.6.x' netmask='255.255.255.x' />"
                        + "<interface among='routerCore1 routerCore6' address='10.1.6.x' netmask='255.255.255.x' />"
                        
                        + "<interface among='routerCore1 routerCore3' address='10.1.3.x' netmask='255.255.255.x' />"
                        + "<interface among='routerCore2 routerCore4' address='10.2.4.x' netmask='255.255.255.x' />"
                        + "<interface among='routerCore3 routerCore5' address='10.3.5.x' netmask='255.255.255.x' />"
                        + "<interface among='routerCore4 routerCore6' address='10.4.6.x' netmask='255.255.255.x' />"
                        + "<interface among='routerCore5 routerCore1' address='10.1.5.x' netmask='255.255.255.x' />"
                        + "<interface among='routerCore6 routerCore2' address='10.2.6.x' netmask='255.255.255.x' />"
                        
                        + "<interface among='routerCore1 client1*' address='192.168.1.x' netmask='255.255.255.x' />"
                        + "<interface among='routerCore2 client2*' address='192.168.2.x' netmask='255.255.255.x' />"
                        + "<interface among='routerCore3 client3*' address='192.168.3.x' netmask='255.255.255.x' />"
                        + "<interface among='routerCore4 client4*' address='192.168.4.x' netmask='255.255.255.x' />"
                        + "<interface among='routerCore5 client5*' address='192.168.5.x' netmask='255.255.255.x' />"
                        + "<interface among='routerCore6 client6*' address='192.168.6.x' netmask='255.255.255.x' />"
                        
                        + "<interface host='client1*' address='192.168.1.x' netmask='255.255.255.x' />"
                        + "<interface host='client2*' address='192.168.2.x' netmask='255.255.255.x' />"
                        + "<interface host='client3*' address='192.168.3.x' netmask='255.255.255.x' />"
                        + "<interface host='client4*' address='192.168.4.x' netmask='255.255.255.x' />"
                        + "<interface host='client5*' address='192.168.5.x' netmask='255.255.255.x' />"
                        + "<interface host='client6*' address='192.168.6.x' netmask='255.255.255.x' />"
                        
                        + "<multicast-group hosts='r*' address='224.0.0.5 224.0.0.6' />"
                        + "<route hosts='client1*' destination='*' gateway='routerCore1'/>"
                        + "<route hosts='client2*' destination='*' gateway='routerCore2'/>"
                        + "<route hosts='client3*' destination='*' gateway='routerCore3'/>"
                        + "<route hosts='client4*' destination='*' gateway='routerCore4'/>"
                        + "<route hosts='client5*' destination='*' gateway='routerCore5'/>"
                        + "<route hosts='client6*' destination='*' gateway='routerCore6'/>"
                        + "</config>");

        }


        etherSwitch1: EtherSwitch {
            @display("p=676.56995,1236.49");
        }
        etherSwitch2: EtherSwitch {
            @display("p=676.56995,2281.6738");
        }
        etherSwitch3: EtherSwitch {
            @display("p=1833.7379,2407.656");
        }
        etherSwitch4: EtherSwitch {
            @display("p=3186.878,2193.02");
        }
        etherSwitch5: EtherSwitch {
            @display("p=3107.556,1082.512");
        }
        etherSwitch6: EtherSwitch {
            @display("p=1833.7379,713.89795");
        }


        client101: StandardHost {
            parameters:
                @display("p=517.92596,835.214");
        }
        client199: StandardHost {
            parameters:
                @display("p=186.64,1245.8219");
        }


        client201: StandardHost {
            parameters:
                @display("p=144.646,2160.358");
        }
        client299: StandardHost {
            parameters:
                @display("p=681.23596,2715.6118");
        }

        client301: StandardHost {
            parameters:
                @display("p=2277.0078,2734.276");
        }
        client399: StandardHost {
            parameters:
                @display("p=1493.12,2762.272");
        }
        client401: StandardHost {
            parameters:
                @display("p=2874.2559,2510.3079");
        }
        client499: StandardHost {
            parameters:
                @display("p=3835.452,2076.3699");
        }
        client501: StandardHost {
            parameters:
                @display("p=3606.8179,1362.4719");
        }

        client599: StandardHost {
            parameters:
                @display("p=3406.18,718.56396");
        }

        client601: StandardHost {
            parameters:
                @display("p=2165.024,513.26");
        }

        client699: StandardHost {
            parameters:
                @display("p=1329.8099,545.922");
        }
    connections:


        routerCore1.ethg++ <--> NormalP <--> routerCore2.ethg++;
        routerCore2.ethg++ <--> NormalP <--> routerCore3.ethg++;
        routerCore3.ethg++ <--> NormalP <--> routerCore4.ethg++;
        routerCore4.ethg++ <--> NormalP <--> routerCore5.ethg++;
        routerCore5.ethg++ <--> NormalP <--> routerCore6.ethg++;
        routerCore6.ethg++ <--> NormalP <--> routerCore1.ethg++;

        routerCore1.ethg++ <--> NormalP <--> routerCore3.ethg++;
        routerCore2.ethg++ <--> NormalP <--> routerCore4.ethg++;
        routerCore3.ethg++ <--> NormalP <--> routerCore5.ethg++;
        routerCore4.ethg++ <--> NormalP <--> routerCore6.ethg++;
        routerCore5.ethg++ <--> NormalP <--> routerCore1.ethg++;
        routerCore6.ethg++ <--> NormalP <--> routerCore2.ethg++;


        etherSwitch1.ethg++ <--> GEth <--> routerCore1.ethg++;
        etherSwitch2.ethg++ <--> GEth <--> routerCore2.ethg++;
        etherSwitch3.ethg++ <--> GEth <--> routerCore3.ethg++;
        etherSwitch4.ethg++ <--> GEth <--> routerCore4.ethg++;
        etherSwitch5.ethg++ <--> GEth <--> routerCore5.ethg++;
        etherSwitch6.ethg++ <--> GEth <--> routerCore6.ethg++;




        //      Switch1
        etherSwitch1.ethg++ <--> GEth <--> client101.ethg++;

        etherSwitch1.ethg++ <--> GEth <--> client199.ethg++;

        //      Switch2        
        etherSwitch2.ethg++ <--> GEth <--> client201.ethg++;

        etherSwitch2.ethg++ <--> GEth <--> client299.ethg++;

        //      Switch3       
        etherSwitch3.ethg++ <--> GEth <--> client301.ethg++;

        etherSwitch3.ethg++ <--> GEth <--> client399.ethg++;

        //      Switch4        
        etherSwitch4.ethg++ <--> GEth <--> client401.ethg++;

        etherSwitch4.ethg++ <--> GEth <--> client499.ethg++;

        //      Switch5        
        etherSwitch5.ethg++ <--> GEth <--> client501.ethg++;

        etherSwitch5.ethg++ <--> GEth <--> client599.ethg++;

        //      Switch6    
        etherSwitch6.ethg++ <--> GEth <--> client601.ethg++;

        etherSwitch6.ethg++ <--> GEth <--> client699.ethg++;




}



network ParkingLot
{
    parameters:
        @display("bgb=2591.9998,2329.2;bgl=8");
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
        visualizer: IntegratedCanvasVisualizer {
            @display("p=356.4,118.799995");
        }

        routerCore1: DmprRouter {
            parameters:
                @display("p=259.19998,525.6;i=abstract/router");
        }
        routerCore2: DmprRouter {
            parameters:
                @display("p=421.19998,525.6;i=abstract/router");
        }
        routerCore3: DmprRouter {
            parameters:
                @display("p=579.6,525.6;i=abstract/router");
        }
        routerCore4: DmprRouter {
            parameters:
                @display("p=1159.2,532.8;i=abstract/router");
        }
        routerCore5: DmprRouter {
            parameters:
                @display("p=1637.9999,547.19995;i=abstract/router");
        }
        routerCore6: DmprRouter {
            parameters:
                @display("p=2379.5999,529.19995;i=abstract/router");
        }

        configurator: Ipv4NetworkConfigurator {
            parameters:
                @display("p=100,100;is=s");
                config = xml("<config>"
                        + "<interface among='routerCore1 routerCore2' address='10.1.2.x' netmask='255.255.255.x' />"
                        + "<interface among='routerCore2 routerCore3' address='10.2.3.x' netmask='255.255.255.x' />"
                        + "<interface among='routerCore3 routerCore4' address='10.3.4.x' netmask='255.255.255.x' />"
                        + "<interface among='routerCore4 routerCore5' address='10.4.5.x' netmask='255.255.255.x' />"
                        + "<interface among='routerCore5 routerCore6' address='10.5.6.x' netmask='255.255.255.x' />"
                        + "<interface among='routerCore1 client1*' address='192.168.1.x' netmask='255.255.255.x' />"
                        + "<interface among='routerCore2 client2*' address='192.168.2.x' netmask='255.255.255.x' />"
                        + "<interface among='routerCore3 client3*' address='192.168.3.x' netmask='255.255.255.x' />"
                        + "<interface among='routerCore4 client4*' address='192.168.4.x' netmask='255.255.255.x' />"
                        + "<interface among='routerCore5 client5*' address='192.168.5.x' netmask='255.255.255.x' />"
                        + "<interface among='routerCore6 client6*' address='192.168.6.x' netmask='255.255.255.x' />"
                        
                        + "<interface host='client1*' address='192.168.1.x' netmask='255.255.255.x' />"
                        + "<interface host='client2*' address='192.168.2.x' netmask='255.255.255.x' />"
                        + "<interface host='client3*' address='192.168.3.x' netmask='255.255.255.x' />"
                        + "<interface host='client4*' address='192.168.4.x' netmask='255.255.255.x' />"
                        + "<interface host='client5*' address='192.168.5.x' netmask='255.255.255.x' />"
                        + "<interface host='client6*' address='192.168.6.x' netmask='255.255.255.x' />"
                        
                        + "<multicast-group hosts='r*' address='224.0.0.5 224.0.0.6' />"
                        + "<route hosts='client1*' destination='*' gateway='routerCore1'/>"
                        + "<route hosts='client2*' destination='*' gateway='routerCore2'/>"
                        + "<route hosts='client3*' destination='*' gateway='routerCore3'/>"
                        + "<route hosts='client4*' destination='*' gateway='routerCore4'/>"
                        + "<route hosts='client5*' destination='*' gateway='routerCore5'/>"
                        + "<route hosts='client6*' destination='*' gateway='routerCore6'/>"
                        + "</config>");

        }


        etherSwitch1: EtherSwitch {
            @display("p=241.19998,698.39996");
        }
        etherSwitch2: EtherSwitch {
            @display("p=421.19998,640.8");
        }
        etherSwitch3: EtherSwitch {
            @display("p=586.8,673.19995");
        }
        etherSwitch4: EtherSwitch {
            @display("p=1144.7999,737.99994");
        }
        etherSwitch5: EtherSwitch {
            @display("p=1663.2,795.6");
        }
        etherSwitch6: EtherSwitch {
            @display("p=2408.4,835.19995");
        }


        client101: StandardHost {
            parameters:
                @display("p=237.59999,860.39996");
        }
        client199: StandardHost {
            parameters:
                @display("p=295.19998,1490.3999");
        }


        client201: StandardHost {
            parameters:
                @display("p=341.99997,856.8");
        }
        client299: StandardHost {
            parameters:
                @display("p=471.59998,1490.3999");
        }

        client301: StandardHost {
            parameters:
                @display("p=550.8,809.99994");
        }
        client399: StandardHost {
            parameters:
                @display("p=676.8,1439.9999");
        }
        client401: StandardHost {
            parameters:
                @display("p=997.19995,899.99994");
        }
        client499: StandardHost {
            parameters:
                @display("p=1285.2,910.7999");
        }
        client501: StandardHost {
            parameters:
                @display("p=1486.7999,1011.6");
        }

        client599: StandardHost {
            parameters:
                @display("p=1864.7999,957.6");
        }

        client601: StandardHost {
            parameters:
                @display("p=1875.5999,1702.7999");
        }

        client699: StandardHost {
            parameters:
                @display("p=2430,1594.7999");
        }
    connections:


        routerCore1.ethg++ <--> NormalP <--> routerCore2.ethg++;
        routerCore2.ethg++ <--> NormalP <--> routerCore3.ethg++;
        routerCore3.ethg++ <--> NormalP <--> routerCore4.ethg++;
        routerCore4.ethg++ <--> NormalP <--> routerCore5.ethg++;
        routerCore5.ethg++ <--> NormalP <--> routerCore6.ethg++;


        etherSwitch1.ethg++ <--> NormalP <--> routerCore1.ethg++;
        etherSwitch2.ethg++ <--> NormalP <--> routerCore2.ethg++;
        etherSwitch3.ethg++ <--> NormalP <--> routerCore3.ethg++;
        etherSwitch4.ethg++ <--> NormalP <--> routerCore4.ethg++;
        etherSwitch5.ethg++ <--> NormalP <--> routerCore5.ethg++;
        etherSwitch6.ethg++ <--> NormalP <--> routerCore6.ethg++;




        //      Switch1
        etherSwitch1.ethg++ <--> NormalP <--> client101.ethg++;

        etherSwitch1.ethg++ <--> NormalP <--> client199.ethg++;

        //      Switch2        
        etherSwitch2.ethg++ <--> NormalP <--> client201.ethg++;

        etherSwitch2.ethg++ <--> NormalP <--> client299.ethg++;

        //      Switch3       
        etherSwitch3.ethg++ <--> NormalP <--> client301.ethg++;

        etherSwitch3.ethg++ <--> NormalP <--> client399.ethg++;

        //      Switch4        
        etherSwitch4.ethg++ <--> NormalP <--> client401.ethg++;

        etherSwitch4.ethg++ <--> NormalP <--> client499.ethg++;

        //      Switch5        
        etherSwitch5.ethg++ <--> NormalP <--> client501.ethg++;

        etherSwitch5.ethg++ <--> NormalP <--> client599.ethg++;

        //      Switch6    
        etherSwitch6.ethg++ <--> NormalP <--> client601.ethg++;

        etherSwitch6.ethg++ <--> NormalP <--> client699.ethg++;




}
