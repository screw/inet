//
// author: Marcel Marek
//

package inet.examples.dmpr.stability;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.ethernet.EtherSwitch;
import inet.node.inet.Router;
import inet.node.dmpr.DmprRouter;
//import inet.nodes.inet.Router;
//import inet.node.ospfv2.OspfRouter;
import inet.node.inet.StandardHost;
//import inet.nodes.inet.StandardHost;
import ned.DatarateChannel;

//import inet.linklayer.queue.REDDropper;

import inet.common.queue.IOutputQueue;
//import inet.linklayer.queue.DropTailQueue;
import inet.common.queue.FifoQueue;
//import inet.common.queue.RedDropper;
import inet.common.queue.RedMarker;
//import inet.linklayer.queue.REDDropperNUMDual;
//import inet.linklayer.queue.REDDropperNUMDual2;


import inet.visualizer.integrated.IntegratedCanvasVisualizer;



//module RedMarkerQueue like IOutputQueue
//{
//    gates:
//        input in;
//        output out;
//
//    submodules:
//        red: RedMarker;
//        fifo: FifoQueue;
//    connections:
//        in --> red.in[0];
//        red.out[0] --> fifo.in++;
//        fifo.out --> out;
//}


channel NormalP extends DatarateChannel
{
    parameters:
        datarate = 10Mbps;
        delay = default(1ms);
        per = 0;
        ber = 0;
}
channel NormalP3 extends NormalP
{
	parameters:
	    delay = 3ms;   
}
channel FastP extends DatarateChannel
{
    parameters:
        datarate = 100Mbps;
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



network Stability
{
    parameters:
        @display("bgb=4166.738,3051.564,white;bgl=8");
    //        double ldelay @unit(s) = default(0.1s);
    //            double ldatarate @unit(Mbps) = default(10Mbps);
    types:



    submodules:
        visualizer: IntegratedCanvasVisualizer {
            @display("p=500,115");
        }

        routerCore1: DmprRouter {
            parameters:
                @display("p=1026.4801,995.93005;i=abstract/router");
        }
        routerCore2: DmprRouter {
            parameters:
                @display("p=2945.02,995.93005;i=abstract/router");
        }
        routerCore3: DmprRouter {
            parameters:
                @display("p=1026.4801,2077.4001;i=abstract/router");
        }
        routerCore4: DmprRouter {
            parameters:
                @display("p=2938.9102,2077.4001;i=abstract/router");
        }
        routerCore5: DmprRouter {
            parameters:
                @display("p=1509.17,1545.8301;i=abstract/router");
        }
        routerCore6: DmprRouter {
            parameters:
                @display("p=2450.11,1545.8301;i=abstract/router");
        }

        configurator: Ipv4NetworkConfigurator {
            parameters:
                @display("p=150,115;is=m");
                addStaticRoutes = false;
                config = default(xml("<config>"
                        + "<interface among='routerCore1 routerCore2' address='10.1.2.x' netmask='255.255.255.x' />"
                        + "<interface among='routerCore1 routerCore5' address='10.1.5.x' netmask='255.255.255.x' />"
                        + "<interface among='routerCore3 routerCore4' address='10.3.4.x' netmask='255.255.255.x' />"
                        + "<interface among='routerCore3 routerCore5' address='10.3.5.x' netmask='255.255.255.x' />"
                        + "<interface among='routerCore5 routerCore6' address='10.5.6.x' netmask='255.255.255.x' />"
                        + "<interface among='routerCore2 routerCore6' address='10.2.6.x' netmask='255.255.255.x' />"
                        + "<interface among='routerCore4 routerCore6' address='10.4.6.x' netmask='255.255.255.x' />"
                        
                        + "<interface among='routerCore1 sender1*' address='192.168.1.x' netmask='255.255.255.x' />"
                        + "<interface among='routerCore2 receiver1*' address='192.168.2.x' netmask='255.255.255.x' />"
                        + "<interface among='routerCore3 sender2*' address='192.168.3.x' netmask='255.255.255.x' />"
                        + "<interface among='routerCore4 receiver2*' address='192.168.4.x' netmask='255.255.255.x' />"
                        
                        + "<interface host='sender1*' address='192.168.1.x' netmask='255.255.255.x' />"
                        + "<interface host='receiver1*' address='192.168.2.x' netmask='255.255.255.x' />"
                        + "<interface host='sender2*' address='192.168.3.x' netmask='255.255.255.x' />"
                        + "<interface host='receiver2*' address='192.168.4.x' netmask='255.255.255.x' />"
                        
                        + "<multicast-group hosts='r*' address='224.0.0.5 224.0.0.6' />"
                        + "<route hosts='sender1*' destination='*' gateway='routerCore1'/>"
                        + "<route hosts='receiver1*' destination='*' gateway='routerCore2'/>"
                        + "<route hosts='sender2*' destination='*' gateway='routerCore3'/>"
                        + "<route hosts='receiver2*' destination='*' gateway='routerCore4'/>"
                        + "</config>"));

        }


        sender1: StandardHost {
            parameters:
                @display("p=342.16,995.93005");
        }

        receiver1: StandardHost {
            parameters:
                @display("p=3708.7703,995.93005");
        }

        sender2: StandardHost {
            parameters:
                @display("p=342.16,2077.4001");
        }

        receiver2: StandardHost {
            parameters:
                @display("p=3757.6501,2077.4001");
        }

    connections:

        sender1.ethg++ <--> FastP <--> routerCore1.ethg++;
        receiver1.ethg++ <--> FastP <--> routerCore2.ethg++;
        sender2.ethg++ <--> FastP <--> routerCore3.ethg++;
        receiver2.ethg++ <--> FastP <--> routerCore4.ethg++;

		// 1 - 1
        routerCore5.ethg++ <--> NormalP <--> routerCore6.ethg++;
        // 2 - 2	
        routerCore1.ethg++ <--> NormalP{delay=3ms;} <--> routerCore2.ethg++;
        routerCore3.ethg++ <--> NormalP <--> routerCore5.ethg++;
        routerCore4.ethg++ <--> NormalP <--> routerCore6.ethg++;
        // 3 - 3
        routerCore1.ethg++ <--> NormalP <--> routerCore5.ethg++;
        routerCore6.ethg++ <--> NormalP <--> routerCore2.ethg++;        
        routerCore3.ethg++ <--> NormalP3 <--> routerCore4.ethg++;
        
        
        
        routerCore6.ethg++ <--> NormalP <--> routerCore4.ethg++;
        

}







