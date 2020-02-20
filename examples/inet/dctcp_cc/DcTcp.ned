//
// author: Thomas Reschka
//

package inet.examples.inet.dctcp_cc;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.inet.Router;
//import inet.nodes.inet.SplitTCPRouter;
import inet.node.inet.StandardHost;
//import inet.nodes.inet.StandardHost;
import ned.DatarateChannel;

//import inet.linklayer.IOutputQueue;
//import inet.linklayer.queue.DropTailQueue;
//import inet.linklayer.queue.FIFOQueue;
//import inet.linklayer.queue.REDDropper;
//import inet.common.queue.RedMarker;

network DCTCPIncast
{
    parameters:
        @display("bgb=740,580");
    //        double ldelay @unit(s) = default(0.1s);
    //        double ldatarate @unit(Mbps) = default(10Mbps);
    types:
        channel NormalP extends DatarateChannel
        {
            parameters:
                //                datarate = ldatarate;
                per = 0;
                ber = 0;
        }


    submodules:
        routerCore: Router {
            parameters:
                @display("p=315,141;i=abstract/router");
        }
        client11: StandardHost {
            parameters:
                @display("p=50,100");
        }
        client12: StandardHost {
            parameters:
                @display("p=50,176");
        }
        client13: StandardHost {
            parameters:
                @display("p=32,239");
        }
        client14: StandardHost {
            parameters:
                @display("p=38,307");
        }
        client15: StandardHost {
            parameters:
                @display("p=44,386");
        }
        client16: StandardHost {
            parameters:
                @display("p=106,425");
        }
        client17: StandardHost {
            parameters:
                @display("p=172,425");
        }
        client18: StandardHost {
            parameters:
                @display("p=250,416");
        }
        client19: StandardHost {
            parameters:
                @display("p=331,425");
        }
        client110: StandardHost {
            parameters:
                @display("p=370,503");
        }
        client111: StandardHost {
            parameters:
                @display("p=411,531");
        }
        client112: StandardHost {
            parameters:
                @display("p=450,464");
        }
        client113: StandardHost {
            parameters:
                @display("p=463,404");
        }
        client114: StandardHost {
            parameters:
                @display("p=541,455");
        }
        client115: StandardHost {
            parameters:
                @display("p=598,443");
        }
        client116: StandardHost {
            parameters:
                @display("p=598,377");
        }
        client117: StandardHost {
            parameters:
                @display("p=591,307");
        }
        client118: StandardHost {
            parameters:
                @display("p=591,245");
        }
        client119: StandardHost {
            parameters:
                @display("p=591,176");
        }
        client120: StandardHost {
            parameters:
                @display("p=580,119");
        }
        configurator: Ipv4NetworkConfigurator;
        client21: StandardHost {
            parameters:
                @display("p=392,38");
        }
    connections:
        routerCore.ethg++ <--> e1: NormalP <--> client11.ethg++;
        routerCore.ethg++ <--> e2: NormalP <--> client12.ethg++;
        routerCore.ethg++ <--> e3: NormalP <--> client13.ethg++;
        routerCore.ethg++ <--> e4: NormalP <--> client14.ethg++;
        routerCore.ethg++ <--> e5: NormalP <--> client15.ethg++;
        routerCore.ethg++ <--> e6: NormalP <--> client16.ethg++;
        routerCore.ethg++ <--> e7: NormalP <--> client17.ethg++;
        routerCore.ethg++ <--> e8: NormalP <--> client18.ethg++;
        routerCore.ethg++ <--> e9: NormalP <--> client19.ethg++;
        routerCore.ethg++ <--> e10: NormalP <--> client110.ethg++;
        routerCore.ethg++ <--> e11: NormalP <--> client111.ethg++;
        routerCore.ethg++ <--> e12: NormalP <--> client112.ethg++;
        routerCore.ethg++ <--> e13: NormalP <--> client113.ethg++;
        routerCore.ethg++ <--> e14: NormalP <--> client114.ethg++;
        routerCore.ethg++ <--> e15: NormalP <--> client115.ethg++;
        routerCore.ethg++ <--> e16: NormalP <--> client116.ethg++;
        routerCore.ethg++ <--> e17: NormalP <--> client117.ethg++;
        routerCore.ethg++ <--> e18: NormalP <--> client118.ethg++;
        routerCore.ethg++ <--> e19: NormalP <--> client119.ethg++;
        routerCore.ethg++ <--> e20: NormalP <--> client120.ethg++;
        routerCore.ethg++ <--> e21: NormalP <--> client21.ethg++;
}

network DCTCPDumbbell
{
    parameters:
        @display("bgb=740,580");
    //        double ldelay @unit(s) = default(0.1s);
    //        double ldatarate @unit(Mbps) = default(10Mbps);
    types:
        channel NormalP extends DatarateChannel
        {
            parameters:
                //                datarate = ldatarate;
                per = 0;
                ber = 0;
                delay = default(2ms);
                datarate = default(1000Mbps);
        }


    submodules:
        routerCore: Router {
            parameters:
                @display("p=277.984,140.16;i=abstract/router");
        }
        routerCore2: Router {
            parameters:
                @display("p=466.03198,140.16;i=abstract/router");
        }
        client11: StandardHost {
            parameters:
                @display("p=31.536,103.951996");
        }
        client12: StandardHost {
            parameters:
                @display("p=31.536,172.864");
        }
        client13: StandardHost {
            parameters:
                @display("p=32,239");
        }
        client14: StandardHost {
            parameters:
                @display("p=38,307");
        }
        client15: StandardHost {
            parameters:
                @display("p=44,386");
        }
        client16: StandardHost {
            parameters:
                @display("p=38,464");
        }
        client17: StandardHost {
            parameters:
                @display("p=83,518");
        }
        client18: StandardHost {
            parameters:
                @display("p=146,525");
        }
        client19: StandardHost {
            parameters:
                @display("p=204,525");
        }
        client110: StandardHost {
            parameters:
                @display("p=266,518");
        }
        client21: StandardHost {
            parameters:
                @display("p=691.456,58.399998");
        }
        client22: StandardHost {
            parameters:
                @display("p=692,128");
        }
        client23: StandardHost {
            parameters:
                @display("p=681,200");
        }
        client24: StandardHost {
            parameters:
                @display("p=692,268");
        }
        client25: StandardHost {
            parameters:
                @display("p=692,337");
        }
        client26: StandardHost {
            parameters:
                @display("p=692,418");
        }
        client27: StandardHost {
            parameters:
                @display("p=692,503");
        }
        client28: StandardHost {
            parameters:
                @display("p=623,541");
        }
        client29: StandardHost {
            parameters:
                @display("p=558,541");
        }
        client210: StandardHost {
            parameters:
                @display("p=497,541");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=367.91998,26.864");
        }
    connections:
        routerCore.ethg++ <--> e21: NormalP{datarate = 100Mbps; delay = 1ms;} <--> routerCore2.ethg++;
        
        routerCore.ethg++ <--> e1: NormalP <--> client11.ethg++;
        routerCore.ethg++ <--> e2: NormalP <--> client12.ethg++;
        routerCore.ethg++ <--> e3: NormalP <--> client13.ethg++;
        routerCore.ethg++ <--> e4: NormalP <--> client14.ethg++;
        routerCore.ethg++ <--> e5: NormalP <--> client15.ethg++;
        routerCore.ethg++ <--> e6: NormalP <--> client16.ethg++;
        routerCore.ethg++ <--> e7: NormalP <--> client17.ethg++;
        routerCore.ethg++ <--> e8: NormalP <--> client18.ethg++;
        routerCore.ethg++ <--> e9: NormalP <--> client19.ethg++;
        routerCore.ethg++ <--> e10: NormalP <--> client110.ethg++;

        routerCore2.ethg++ <--> e11: NormalP <--> client21.ethg++;
        routerCore2.ethg++ <--> e12: NormalP <--> client22.ethg++;
        routerCore2.ethg++ <--> e13: NormalP <--> client23.ethg++;
        routerCore2.ethg++ <--> e14: NormalP <--> client24.ethg++;
        routerCore2.ethg++ <--> e15: NormalP <--> client25.ethg++;
        routerCore2.ethg++ <--> e16: NormalP <--> client26.ethg++;
        routerCore2.ethg++ <--> e17: NormalP <--> client27.ethg++;
        routerCore2.ethg++ <--> e18: NormalP <--> client28.ethg++;
        routerCore2.ethg++ <--> e19: NormalP <--> client29.ethg++;
        routerCore2.ethg++ <--> e20: NormalP <--> client210.ethg++;

        
}
