//
// author: Thomas Reschka
//

package inet.examples.dmpr.fulltest;

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

//import inet.common.queue.IOutputQueue;
////import inet.linklayer.queue.DropTailQueue;
//import inet.common.queue.FifoQueue;
////import inet.common.queue.RedDropper;
//import inet.common.queue.RedMarker;
////import inet.linklayer.queue.REDDropperNUMDual;
////import inet.linklayer.queue.REDDropperNUMDual2;


import inet.visualizer.integrated.IntegratedCanvasVisualizer;


network Hexagon
{
    types:



channel NormalP extends DatarateChannel
{
    parameters:
        datarate = 10Mbps;
        delay = 1ms;
        per = 0;
        ber = 0;
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




    submodules:
        visualizer: IntegratedCanvasVisualizer {
            @display("p=500,115");
        }

        routerCore1: DmprRouter {
            parameters:
                @display("p=1000,1500;i=abstract/router");
        }
        routerCore2: DmprRouter {
            parameters:
                @display("p=1500,2000;i=abstract/router");
        }
        routerCore3: DmprRouter {
            parameters:
                @display("p=2500,2000;i=abstract/router");
        }
        routerCore4: DmprRouter {
            parameters:
                @display("p=3000,1500;i=abstract/router");
        }
        routerCore5: DmprRouter {
            parameters:
                @display("p=2500,1000;i=abstract/router");
        }
        routerCore6: DmprRouter {
            parameters:
                @display("p=1500,1000;i=abstract/router");
        }

        configurator: Ipv4NetworkConfigurator {
            parameters:
                @display("p=150,115;is=m");
                addStaticRoutes = false;
                config = default(xml("<config>"
                        + "<interface among='routerCore1 routerCore2' address='10.1.2.x' netmask='255.255.255.x' />"
                        + "<interface among='routerCore2 routerCore3' address='10.2.3.x' netmask='255.255.255.x' />"
                        + "<interface among='routerCore3 routerCore4' address='10.3.4.x' netmask='255.255.255.x' />"
                        + "<interface among='routerCore4 routerCore5' address='10.4.5.x' netmask='255.255.255.x' />"
                        + "<interface among='routerCore5 routerCore6' address='10.5.6.x' netmask='255.255.255.x' />"
                        + "<interface among='routerCore1 routerCore6' address='10.1.6.x' netmask='255.255.255.x' />"
                        
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
                        + "</config>"));

        }


        etherSwitch1: EtherSwitch {
            @display("p=311.61002,1655.81");
        }
        etherSwitch2: EtherSwitch {
            @display("p=1588.6001,2474.55");
        }
        etherSwitch3: EtherSwitch {
            @display("p=2553.9802,2462.33");
        }
        etherSwitch4: EtherSwitch {
            @display("p=3519.36,1661.92");
        }
        etherSwitch5: EtherSwitch {
            @display("p=2553.9802,659.88");
        }
        etherSwitch6: EtherSwitch {
            @display("p=1545.8301,641.55005");
        }


        client101: StandardHost {
            parameters:
                @display("p=244.40001,1209.78");
        }
        client199: StandardHost {
            parameters:
                @display("p=177.19,2089.62");
        }


        client201: StandardHost {
            parameters:
                @display("p=739.31006,2816.7102");
        }
        client299: StandardHost {
            parameters:
                @display("p=1845.2201,2816.7102");
        }

        client301: StandardHost {
            parameters:
                @display("p=3079.4402,2780.05");
        }
        client399: StandardHost {
            parameters:
                @display("p=2297.36,2810.6");
        }
        client401: StandardHost {
            parameters:
                @display("p=3806.5303,2175.1602");
        }
        client499: StandardHost {
            parameters:
                @display("p=3800.4202,1319.76");
        }
        client501: StandardHost {
            parameters:
                @display("p=3287.1802,513.24005");
        }

        client599: StandardHost {
            parameters:
                @display("p=2169.05,256.62003");
        }

        client601: StandardHost {
            parameters:
                @display("p=1765.79,293.28");
        }

        client699: StandardHost {
            parameters:
                @display("p=665.99005,409.37003");
        }
    connections:

        etherSwitch1.ethg++ <--> FastP <--> routerCore1.ethg++;
        etherSwitch2.ethg++ <--> FastP <--> routerCore2.ethg++;
        etherSwitch3.ethg++ <--> FastP <--> routerCore3.ethg++;
        etherSwitch4.ethg++ <--> FastP <--> routerCore4.ethg++;
        etherSwitch5.ethg++ <--> FastP <--> routerCore5.ethg++;
        etherSwitch6.ethg++ <--> FastP <--> routerCore6.ethg++;

        routerCore1.ethg++ <--> NormalP <--> routerCore2.ethg++;
        routerCore2.ethg++ <--> NormalP <--> routerCore3.ethg++;
        routerCore3.ethg++ <--> NormalP <--> routerCore4.ethg++;
        routerCore4.ethg++ <--> NormalP <--> routerCore5.ethg++;
        routerCore5.ethg++ <--> NormalP <--> routerCore6.ethg++;
        routerCore6.ethg++ <--> NormalP <--> routerCore1.ethg++;






        //      Switch1
        etherSwitch1.ethg++ <--> FastP <--> client101.ethg++;

        etherSwitch1.ethg++ <--> FastP <--> client199.ethg++;

        //      Switch2        
        etherSwitch2.ethg++ <--> FastP <--> client201.ethg++;

        etherSwitch2.ethg++ <--> FastP <--> client299.ethg++;

        //      Switch3       
        etherSwitch3.ethg++ <--> FastP <--> client301.ethg++;

        etherSwitch3.ethg++ <--> FastP <--> client399.ethg++;

        //      Switch4        
        etherSwitch4.ethg++ <--> FastP <--> client401.ethg++;

        etherSwitch4.ethg++ <--> FastP <--> client499.ethg++;

        //      Switch5        
        etherSwitch5.ethg++ <--> FastP <--> client501.ethg++;

        etherSwitch5.ethg++ <--> FastP <--> client599.ethg++;

        //      Switch6    
        etherSwitch6.ethg++ <--> FastP <--> client601.ethg++;

        etherSwitch6.ethg++ <--> FastP <--> client699.ethg++;

}


network Fulltest extends Hexagon
{
    parameters:

        configurator.config = xml("<config>"
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


    connections:

        routerCore1.ethg++ <--> NormalP <--> routerCore3.ethg++;
        routerCore2.ethg++ <--> NormalP <--> routerCore4.ethg++;
        routerCore3.ethg++ <--> NormalP <--> routerCore5.ethg++;
        routerCore4.ethg++ <--> NormalP <--> routerCore6.ethg++;
        routerCore5.ethg++ <--> NormalP <--> routerCore1.ethg++;
        routerCore6.ethg++ <--> NormalP <--> routerCore2.ethg++;



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
