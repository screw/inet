//
// Copyright (C) OpenSim Ltd.
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

package inet.visualizer.integrated;

import inet.visualizer.contract.IDataLinkVisualizer;
import inet.visualizer.contract.IEnergyStorageVisualizer;
import inet.visualizer.contract.IIeee80211Visualizer;
import inet.visualizer.contract.IInfoVisualizer;
import inet.visualizer.contract.IIntegratedVisualizer;
import inet.visualizer.contract.IInterfaceTableVisualizer;
import inet.visualizer.contract.ILinkBreakVisualizer;
import inet.visualizer.contract.IMediumVisualizer;
import inet.visualizer.contract.IMobilityVisualizer;
import inet.visualizer.contract.INetworkConnectionVisualizer;
import inet.visualizer.contract.INetworkNodeVisualizer;
import inet.visualizer.contract.INetworkRouteVisualizer;
import inet.visualizer.contract.IPacketDropVisualizer;
import inet.visualizer.contract.IPhysicalEnvironmentVisualizer;
import inet.visualizer.contract.IPhysicalLinkVisualizer;
import inet.visualizer.contract.IQueueVisualizer;
import inet.visualizer.contract.IRadioVisualizer;
import inet.visualizer.contract.IRoutingTableVisualizer;
import inet.visualizer.contract.ISceneVisualizer;
import inet.visualizer.contract.IStatisticVisualizer;
import inet.visualizer.contract.ITracingObstacleLossVisualizer;
import inet.visualizer.contract.ITransportConnectionVisualizer;
import inet.visualizer.contract.ITransportRouteVisualizer;

//
// This module integrates all canvas visualizers into a single module. It
// supports disabling any submodule visualizer by providing an empty string as
// its type.
//
// @see ~IntegratedOsgVisualizer, ~IntegratedVisualizer, ~IIntegratedVisualizer.
//
module IntegratedMultiCanvasVisualizer like IIntegratedVisualizer
{
    parameters:
        @display("i=block/app");
        int numSceneVisualizers = default(1);
        int numPhysicalEnvironmentVisualizers = default(1);
        int numNetworkNodeVisualizers = default(1);
        int numNetworkConnectionVisualizers = default(1);
        int numMobilityVisualizers = default(1);
        int numStatisticVisualizers = default(1);
        int numInfoVisualizers = default(1);
        int numEnergyStorageVisualizers = default(1);
        int numMediumVisualizers = default(1);
        int numObstacleLossVisualizers = default(1);
        int numPacketDropVisualizers = default(1);
        int numQueueVisualizers = default(1);
        int numRadioVisualizers = default(1);
        int numPhysicalLinkVisualizers = default(1);
        int numInterfaceTableVisualizers = default(1);
        int numDataLinkVisualizers = default(1);
        int numLinkBreakVisualizers = default(1);
        int numIeee80211Visualizers = default(1);
        int numNetworkRouteVisualizers = default(1);
        int numRoutingTableVisualizers = default(1);
        int numTransportRouteVisualizers = default(1);
        int numTransportConnectionVisualizers = default(1);
        string sceneVisualizerType = default("SceneCanvasVisualizer");
        string physicalEnvironmentVisualizerType = default("PhysicalEnvironmentCanvasVisualizer");
        string networkNodeVisualizerType = default("NetworkNodeCanvasVisualizer");
        string networkConnectionVisualizerType = default("NetworkConnectionCanvasVisualizer");
        string mobilityVisualizerType = default("MobilityCanvasVisualizer");
        string statisticVisualizerType = default("StatisticCanvasVisualizer");
        string infoVisualizerType = default("InfoCanvasVisualizer");
        string energyStorageVisualizerType = default("EnergyStorageCanvasVisualizer");
        string mediumVisualizerType = default("MediumCanvasVisualizer");
        string obstacleLossVisualizerType = default("TracingObstacleLossCanvasVisualizer");
        string packetDropVisualizerType = default("PacketDropCanvasVisualizer");
        string queueVisualizerType = default("QueueCanvasVisualizer");
        string radioVisualizerType = default("RadioCanvasVisualizer");
        string physicalLinkVisualizerType = default("PhysicalLinkCanvasVisualizer");
        string interfaceTableVisualizerType = default("InterfaceTableCanvasVisualizer");
        string dataLinkVisualizerType = default("DataLinkCanvasVisualizer");
        string linkBreakVisualizerType = default("LinkBreakCanvasVisualizer");
        string ieee80211VisualizerType = default("Ieee80211CanvasVisualizer");
        string networkRouteVisualizerType = default("NetworkRouteCanvasVisualizer");
        string routingTableVisualizerType = default("RoutingTableCanvasVisualizer");
        string transportRouteVisualizerType = default("TransportRouteCanvasVisualizer");
        string transportConnectionVisualizerType = default("TransportConnectionCanvasVisualizer");
        *.networkNodeVisualizerModule = default(absPath(".networkNodeVisualizer[0]"));
    submodules:
        sceneVisualizer[numSceneVisualizers]: <sceneVisualizerType> like ISceneVisualizer if sceneVisualizerType != "" {
            parameters:
                @display("p=150,100,row,100");
        }
        physicalEnvironmentVisualizer[numPhysicalEnvironmentVisualizers]: <physicalEnvironmentVisualizerType> like IPhysicalEnvironmentVisualizer if physicalEnvironmentVisualizerType != "" {
            parameters:
                @display("p=150,200,row,100");
        }
        networkNodeVisualizer[numNetworkNodeVisualizers]: <networkNodeVisualizerType> like INetworkNodeVisualizer if networkNodeVisualizerType != "" {
            parameters:
                @display("p=150,300,row,100");
        }
        networkConnectionVisualizer[numNetworkConnectionVisualizers]: <networkConnectionVisualizerType> like INetworkConnectionVisualizer if networkConnectionVisualizerType != "" {
            parameters:
                @display("p=150,400,row,100");
        }
        mobilityVisualizer[numMobilityVisualizers]: <mobilityVisualizerType> like IMobilityVisualizer if mobilityVisualizerType != "" {
            parameters:
                @display("p=150,500,row,100");
        }
        statisticVisualizer[numStatisticVisualizers]: <statisticVisualizerType> like IStatisticVisualizer if statisticVisualizerType != "" {
            parameters:
                @display("p=150,600,row,100");
        }
        infoVisualizer[numInfoVisualizers]: <infoVisualizerType> like IInfoVisualizer if infoVisualizerType != "" {
            parameters:
                @display("p=150,700,row,100");
        }
        energyStorageVisualizer[numEnergyStorageVisualizers]: <energyStorageVisualizerType> like IEnergyStorageVisualizer if energyStorageVisualizerType != "" {
            parameters:
                @display("p=150,800,row,100");
        }
        mediumVisualizer[numMediumVisualizers]: <mediumVisualizerType> like IMediumVisualizer if mediumVisualizerType != "" {
            parameters:
                @display("p=150,900,row,100");
        }
        obstacleLossVisualizer[numObstacleLossVisualizers]: <obstacleLossVisualizerType> like ITracingObstacleLossVisualizer if obstacleLossVisualizerType != "" {
            parameters:
                @display("p=150,1000,row,100");
        }
        packetDropVisualizer[numPacketDropVisualizers]: <packetDropVisualizerType> like IPacketDropVisualizer if packetDropVisualizerType != "" {
            parameters:
                @display("p=150,1100,row,100");
        }
        queueVisualizer[numQueueVisualizers]: <queueVisualizerType> like IQueueVisualizer if queueVisualizerType != "" {
            parameters:
                @display("p=450,100,row,100");
        }
        radioVisualizer[numRadioVisualizers]: <radioVisualizerType> like IRadioVisualizer if radioVisualizerType != "" {
            parameters:
                @display("p=450,200,row,100");
        }
        physicalLinkVisualizer[numPhysicalLinkVisualizers]: <physicalLinkVisualizerType> like IPhysicalLinkVisualizer if physicalLinkVisualizerType != "" {
            parameters:
                @display("p=450,300,row,100");
        }
        interfaceTableVisualizer[numInterfaceTableVisualizers]: <interfaceTableVisualizerType> like IInterfaceTableVisualizer if interfaceTableVisualizerType != "" {
            parameters:
                @display("p=450,400,row,100");
        }
        dataLinkVisualizer[numDataLinkVisualizers]: <dataLinkVisualizerType> like IDataLinkVisualizer if dataLinkVisualizerType != "" {
            parameters:
                @display("p=450,500,row,100");
        }
        linkBreakVisualizer[numLinkBreakVisualizers]: <linkBreakVisualizerType> like ILinkBreakVisualizer if linkBreakVisualizerType != "" {
            parameters:
                @display("p=450,600,row,100");
        }
        ieee80211Visualizer[numIeee80211Visualizers]: <ieee80211VisualizerType> like IIeee80211Visualizer if ieee80211VisualizerType != "" {
            parameters:
                @display("p=450,700,row,25,row,100");
        }
        networkRouteVisualizer[numNetworkRouteVisualizers]: <networkRouteVisualizerType> like INetworkRouteVisualizer if networkRouteVisualizerType != "" {
            parameters:
                @display("p=450,800,row,100");
        }
        routingTableVisualizer[numRoutingTableVisualizers]: <routingTableVisualizerType> like IRoutingTableVisualizer if routingTableVisualizerType != "" {
            parameters:
                @display("p=450,900,row,100");
        }
        transportRouteVisualizer[numTransportRouteVisualizers]: <transportRouteVisualizerType> like ITransportRouteVisualizer if transportRouteVisualizerType != "" {
            parameters:
                @display("p=450,1000,row,100");
        }
        transportConnectionVisualizer[numTransportConnectionVisualizers]: <transportConnectionVisualizerType> like ITransportConnectionVisualizer if transportConnectionVisualizerType != "" {
            parameters:
                @display("p=450,1100,row,100");
        }
}
