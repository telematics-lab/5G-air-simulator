/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2020 TELEMATICS LAB, Politecnico di Bari
 *
 * This file is part of 5G-air-simulator
 *
 * 5G-air-simulator is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation;
 *
 * 5G-air-simulator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 5G-air-simulator; if not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Gabriele Roncella <gabriele.roncella@student.unisi.it>
 * Author: Dinesh Tamang <dinesh.tamang@student.unisi.it>
 */

#include "../channel/RadioChannel.h"
#include "../phy/gnb-phy.h"
#include "../phy/ue-phy.h"
#include "../core/spectrum/bandwidth-manager.h"
#include "../networkTopology/Cell.h"
#include "../protocolStack/packet/packet-burst.h"
#include "../protocolStack/mac/packet-scheduler/downlink-packet-scheduler.h"
#include "../protocolStack/mac/AMCModule.h"
#include "../protocolStack/packet/Packet.h"
#include "../protocolStack/rrc/ho/handover-entity.h"
#include "../protocolStack/rrc/ho/power-based-ho-manager.h"
#include "../protocolStack/rrc/ho/twin-model-ho-manager.h"
#include "../core/eventScheduler/simulator.h"
#include "../flows/application/InfiniteBuffer.h"
#include "../flows/application/VoIP.h"
#include "../flows/application/CBR.h"
#include "../flows/application/FTP2.h"
#include "../flows/application/TraceBased.h"
#include "../device/IPClassifier/ClassifierParameters.h"
#include "../flows/QoS/QoSParameters.h"
#include "../flows/QoS/QoSForEXP.h"
#include "../flows/QoS/QoSForFLS.h"
#include "../flows/QoS/QoSForM_LWDF.h"
#include "../componentManagers/FrameManager.h"
#include "../utility/RandomVariable.h"
#include "../utility/UsersDistribution.h"
#include "../load-parameters.h"
#include <iostream>
#include <queue>
#include <fstream>
#include <stdlib.h>
#include <cstring>

static void f5g_50MbpsEverywhereHandover (int argc, char *argv[])
{
    string environment(argv[2]); // "suburban" or "rural"
    double isd = atof(argv[3]); // km
    int nbUE = atof(argv[4]); // users/km2
    int speed = atoi(argv[5]); // km/h
    double duration = atof(argv[6]); // sec
    int txMode = atoi(argv[7]);  // dl tx mode
    int ulTxMode = atoi(argv[8]); // ul tx mode
    int nbTx = atoi(argv[9]);  // nb of Tx antennas
    int nbMu = atoi(argv[10]); // nb of Max users per RB
    int nbRx = atoi(argv[11]); // nb of Rx antennas
    int dl_sched_type = atoi(argv[12]); // dl scheduling
    int ul_sched_type = atoi(argv[13]); // ul scheduling
    
    
    double thresholdBefore = atoi(argv[14]); // used when twinmodel is activated
    double thresholdAfter = atoi(argv[15]); // used when twinmodel is activated
    int handoverManager = atoi(argv[16]);  // Set to 1 to activate -->twinmodel HO else -->classical powerbased is set for any input
    int seed = atoi(argv[17]);
    int nbdlCBR = atoi(argv[18]);
    int nbulCBR = atoi(argv[19]);
    
    
    double bandwidth = 20; // MHz
    int etilt; //degrees
    double penetrationLossMeanIndoor = 0; // dB
    double penetrationLossMeanOutdoor = 0; // dB
    double penetrationLossStdDev = 0; //dB
    double carrierFreq; //MHz
    double txPower = 46; // dBm
    double antennaHeight = 25; // m
    double avgBuildingHeight = 20; // m
    double antennaGain = 8; // dBi
    double antennaAttenuation = 20; // dB
    double horizontalBeamwidth3db = 65; //degrees
    double UENoiseFigure = 9; // dB
    
    double cqiReportingInterval = 1; // TTI
    double shadowingStddev = 0; //dB
    int channelModel = 1;
    int nbCell = 2; // number of sites in the simulation
    int nbActiveCell = 2; // number of sites with active users
    int nbSector = 3; // number of sectors per site
    double BSNoiseFigure = 7; // dB
    bool handover = true;
    double indoorUeFraction = 0.80;
    double minDist = 0.025; // in km
    int enbTx = nbTx;
    int ueRx = nbRx;
    double vBeamWidth = 65;
    double BSFeederLoss = 2;
    double interval=0.040;
    
    //the next four lines are necessary for the uplink with RACH. Defining Random Access type
    UeRandomAccess::RandomAccessType m_UeRandomAccessType;
    GnbRandomAccess::RandomAccessType m_EnbRandomAccessType;
    m_UeRandomAccessType=UeRandomAccess::RA_TYPE_BASELINE;
    m_EnbRandomAccessType=GnbRandomAccess::RA_TYPE_BASELINE;
    int dlVideoBitRate = 128;
    int ulVideoBitRate = 242;
    //Define the number of downlink applications and application parameters
    
    
    
    //Define the number of uplink applications and application parameters
    int nbulBE = 0;
    int nbulVoIP = 0;
    int nbulVideo = 0;
    
    
    //  double isd; // km
    if(environment=="suburban")
    {
        //isd = 0.25;
        etilt = 10;
        channelModel = 4;
        carrierFreq = 2000;
    }
    else if(environment=="rural")
    {
        //isd = 0.5;
        etilt = 10;
        channelModel = 4;
        carrierFreq = 800;
    }
    else
    {
        cout << "ERROR: invalid environment selected" << endl;
    }
    
    // define simulation times
    double flow_duration = duration;
    
    // define channel model
    ChannelRealization::ChannelModel model;
    
    switch(channelModel)
    {
        case 0:
            model = ChannelRealization::CHANNEL_MODEL_MACROCELL_URBAN_IMT;
            break;
        case 1:
            model = ChannelRealization::CHANNEL_MODEL_MACROCELL_RURAL_IMT;
            break;
        case 2:
            model = ChannelRealization::CHANNEL_MODEL_3GPP_CASE1;
            break;
        case 3:
            model = ChannelRealization::CHANNEL_MODEL_MACROCELL_RURAL;
            break;
        case 4:
            model = ChannelRealization::CHANNEL_MODEL_MACROCELL_URBAN_IMT_3D;
            break;
        default:
            model = ChannelRealization::CHANNEL_MODEL_MACROCELL_URBAN;
            break;
    }
    
    // CREATE COMPONENT MANAGER
    Simulator *simulator = Simulator::Init();
    FrameManager *frameManager = FrameManager::Init();
    NetworkManager* networkManager = NetworkManager::Init();
    FlowsManager* flowsManager = FlowsManager::Init();
    
    // CONFIGURE SEED
    if (seed >= 0)
    {
        srand (seed);
    }
    else
    {
        srand (time(nullptr));
    }
    cout << "Simulation with SEED = " << seed << endl;
    
    double radius = isd / sqrt(3); // define cell radius from inter site distance
    
    // SET SCHEDULING ALLOCATION SCHEME
    GNodeB::DLSchedulerType downlink_scheduler_type;
    switch (dl_sched_type)
    {
        case 1:
            downlink_scheduler_type = GNodeB::DLScheduler_TYPE_PROPORTIONAL_FAIR;
            cout << "Downlink Scheduler PF "<< endl;
            break;
        case 2:
            downlink_scheduler_type = GNodeB::DLScheduler_TYPE_MLWDF;
            cout << "Downlink Scheduler MLWDF "<< endl;
            break;
        case 3:
            downlink_scheduler_type = GNodeB::DLScheduler_TYPE_EXP;
            cout << "Downlink Scheduler EXP "<< endl;
            break;
        case 4:
            downlink_scheduler_type = GNodeB::DLScheduler_TYPE_FLS;
            cout << "Downlink Scheduler FLS "<< endl;
            break;
        case 5:
            downlink_scheduler_type = GNodeB::DLScheduler_EXP_RULE;
            cout << "Downlink Scheduler EXP_RULE "<< endl;
            break;
        case 6:
            downlink_scheduler_type = GNodeB::DLScheduler_LOG_RULE;
            cout << "Downlink Scheduler LOG RULE "<< endl;
            break;
        case 7:
            downlink_scheduler_type = GNodeB::DLScheduler_TYPE_MAXIMUM_THROUGHPUT;
            cout << "Downlink Scheduler MT "<< endl;
            break;
        case 8:
            downlink_scheduler_type = GNodeB::DLScheduler_TYPE_ROUND_ROBIN;
            cout << "Downlink Scheduler RR "<< endl;
            break;
        default:
            downlink_scheduler_type = GNodeB::DLScheduler_TYPE_PROPORTIONAL_FAIR;
            break;
    }
    
    // SET UPLINK SCHEDULING ALLOCATION SCHEME
    GNodeB::ULSchedulerType uplink_scheduler_type;
    switch (ul_sched_type)
    {
        case 1:
            uplink_scheduler_type = GNodeB::ULScheduler_TYPE_MAXIMUM_THROUGHPUT;
            cout << "Uplink Scheduler MT "<< endl;
            break;
        case 2:
            uplink_scheduler_type = GNodeB::ULScheduler_TYPE_ROUNDROBIN;
            cout << "Uplink Scheduler RR "<< endl;
            break;
        case 3:
            uplink_scheduler_type = GNodeB::ULScheduler_TYPE_FME;
            cout << "Uplink Scheduler FME "<< endl;
            break;
        case 5:
            uplink_scheduler_type = GNodeB::ULScheduler_TYPE_PRIORITY_SCHED;
            cout << "Uplink Scheduler PRIORITY_URLLC "<< endl;
            break;
        default:
            uplink_scheduler_type = GNodeB::ULScheduler_TYPE_MAXIMUM_THROUGHPUT;
            break;
    }
    
    // SET FRAME STRUCTURE
    frameManager->SetFrameStructure(FrameManager::FRAME_STRUCTURE_FDD);
    
    
    // CREATE CELLS
    vector <Cell*> *cells = new vector <Cell*>;
    for (int i = 0; i < nbCell; i++)
    {
        CartesianCoordinates center =
        GetCartesianCoordinatesForCell(i, radius * 1000.);
        
        Cell *c = new Cell (i, radius, minDist, center.GetCoordinateX (), center.GetCoordinateY ());
        cells->push_back (c);
        networkManager->GetCellContainer ()->push_back (c);
        
        cout << "Created Cell, id " << c->GetIdCell ()
        <<", position: " << c->GetCellCenterPosition ()->GetCoordinateX ()
        << " " << c->GetCellCenterPosition ()->GetCoordinateY ()
        << " radius " << radius*1000 << endl;
    }
    
    for (int i = 0; i < nbCell; i++)
    {
        for (int j=0; j<nbSector; j++)
        {
            // CREATE CHANNELS and propagation loss model
            RadioChannel *dlCh = new RadioChannel ();
            RadioChannel *ulCh = new RadioChannel ();
            
            // CREATE SPECTRUM
            BandwidthManager* spectrum = new BandwidthManager (bandwidth, bandwidth, 0, 0);
            
            // Set error model
            WidebandCqiEesmErrorModel *errorModel = new WidebandCqiEesmErrorModel ();
            //Create ENodeB
            GNodeB* enb = new GNodeB (i*3+j, cells->at (i));
            enb->SetRandomAccessType(m_EnbRandomAccessType);
            enb->GetPhy ()->SetDlChannel (dlCh);
            enb->GetPhy ()->SetUlChannel (ulCh);
            enb->GetPhy ()->SetNoiseFigure(BSNoiseFigure);
            enb->GetPhy ()->SetCarrierFrequency(carrierFreq);
            enb->GetPhy ()->SetTxAntennas(enbTx);
            if(ulTxMode == 11)
                enb->GetPhy ()->SetRxAntennas(enbTx);//for uplink mimo
            enb->GetPhy ()->SetTxPower(txPower);
            enb->GetPhy ()->SetBandwidthManager (spectrum);
            ulCh->AddDevice (enb);	//for the uplink
            enb->GetPhy()->SetErrorModel(errorModel); //for the uplink
            enb->GetPhy ()->SetHeight(antennaHeight);
            enb->GetPhy ()->SetAverageBuildingHeight(avgBuildingHeight);
            if(handoverManager==1)
            {
                TwinModelHoManager* ho = new TwinModelHoManager();
                ho->SetHandoverThresholdAfter(thresholdAfter);
                ho->SetHandoverThresholdBefore(thresholdBefore);
                enb->GetProtocolStack()->GetRrcEntity()->GetHandoverEntity()->SetHoManager(ho);
                enb->GetProtocolStack()->GetRrcEntity()->GetHandoverEntity()->SetDetachTime(0);
        
            }
            else
            {
                HoManager* ho = new PowerBasedHoManager();
                enb->GetProtocolStack()->GetRrcEntity()->GetHandoverEntity()->SetHoManager(ho);
                enb->GetProtocolStack()->GetRrcEntity()->GetHandoverEntity()->SetDetachTime(0.030);
                enb->GetProtocolStack ()->GetRrcEntity ()->GetHandoverEntity ()->GetHoManager ()->SetHandoverThresholdBefore(thresholdBefore);
                
            }
            
            
            
            
            // Set Antenna Parameters
            if(nbSector==1)
            {
                enb->GetPhy ()->GetAntennaParameters ()->SetType(Phy::AntennaParameters::ANTENNA_TYPE_OMNIDIRECTIONAL);
            }
            else if(nbSector==3)
            {
                enb->GetPhy ()->GetAntennaParameters ()->SetType(Phy::AntennaParameters::ANTENNA_TYPE_TRI_SECTOR);
                enb->GetPhy ()->GetAntennaParameters ()->SetEtilt(etilt);
                enb->GetPhy ()->GetAntennaParameters ()->SetHorizontalBeamwidth3db(horizontalBeamwidth3db);
                enb->GetPhy ()->GetAntennaParameters ()->SetVerticalBeamwidth3db(vBeamWidth);
                enb->GetPhy ()->GetAntennaParameters ()->SetGain(antennaGain);
                enb->GetPhy ()->GetAntennaParameters ()->SetMaxHorizontalAttenuation(antennaAttenuation);
                enb->GetPhy ()->GetAntennaParameters ()->SetMaxVerticalAttenuation(antennaAttenuation);
                enb->GetPhy ()->GetAntennaParameters ()->SetBearing(120*j);
            }
            else
            {
                cout << "Error: unsupported number of cell sectors (" << nbSector << ")" <<endl;
                exit(1);
            }
            enb->GetPhy ()->GetAntennaParameters ()->SetFeederLoss(BSFeederLoss);
            enb->GetMacEntity ()->SetDefaultDlTxMode(txMode);
            enb->GetMacEntity()->SetDefaultUlTxMode(ulTxMode);	//for uplink mimo
            enb->SetDLScheduler (downlink_scheduler_type);
            enb->SetULScheduler(uplink_scheduler_type);	//uplink scheduler setting
            enb->m_ulSchedType = ul_sched_type; //uplink packet scheduler
            enb->GetDLScheduler()->SetMaxUsersPerRB(nbMu);
            networkManager->GetGNodeBContainer ()->push_back (enb);
            
        }
    }
    
    //Create GW
    Gateway *gw = new Gateway ();
    networkManager->GetGatewayContainer ()->push_back (gw);
    
    
    double ue_drop_radius;
    if(nbActiveCell==1)
    {
        ue_drop_radius = isd;
    }
    else if(nbActiveCell>1 && nbActiveCell<=7)
    {
        ue_drop_radius = 2*isd;
    }
    else if(nbActiveCell>7 && nbActiveCell<=19)
    {
        ue_drop_radius = 3*isd;
    }
    
    double area = ue_drop_radius * ue_drop_radius * M_PI;
    
    cout << "Creating " << nbUE << " user terminals" << endl;
    
    
    //Define Downlink Application Container
    CBR CBRApplication[nbCell*nbSector*nbdlCBR*nbUE];
    
    CBR ulCBRApplication[nbCell*nbSector*nbulCBR*nbUE];
    
    int nrho =2;
    int cbrApplication = 0;
    int ulcbrApplication = 0;
    
    
    
    double maxDelayCBR = 0.8; // delay Constraints
    
    int destinationPort = 101;
    int applicationID = 0;
    CartesianCoordinates center = CartesianCoordinates(0,0);
    
    int idUE = 6;
    
    
    //ue's random position
    double posX, posY;
    bool isNearToCellCenter;
    CartesianCoordinates position;
    
    double meterRadius = radius*1000;
    int intRadius = (int) meterRadius;
    
    posX = cells->at (0)->GetCellCenterPosition()->GetCoordinateX();//distance*cos(theta);
    posY = cells->at (0)->GetCellCenterPosition()->GetCoordinateY()+250;//distance*sin(theta);
    position = CartesianCoordinates(posX,posY);
    
    
    double speedDirection = 90;
    
    GNodeB* enb = networkManager->GetGNodeBContainer()->at(1);
    
    
    
    for (int n = 0; n<nbUE; n++)
    {
        
        
        for(int p=0;p<nrho;p++)
        {
            if(p==0)
            {
                
                UserEquipment* ue = new UserEquipment (idUE,
                                                       posX, posY, speed, speedDirection,
                                                       cells->at (0),
                                                       enb,
                                                       false,
                                                       Mobility::LINEAR_MOVEMENT
                                                       
                                                       );
                cout << "Created UE - id " << idUE << " position " << posX << " " << posY << endl;
                
                
                Simulator::Init()->Schedule(0.50, &Mobility::SetHandover, ue->GetMobilityModel(), true);
                ue->GetProtocolStack()->GetMacEntity()->GetAmcModule()->setUseExtendedCQI(true);
                ue->SetRandomAccessType(m_UeRandomAccessType);
                
                RadioChannel* dlCh = enb->GetPhy ()->GetDlChannel ();
                RadioChannel* ulCh = enb->GetPhy ()->GetUlChannel ();
                ue->SetTwinTransmittingFlag(true);
                ue->GetPhy ()->SetDlChannel (dlCh);
                ue->GetPhy ()->SetUlChannel (ulCh);
                ue->GetPhy ()->SetNoiseFigure(UENoiseFigure);
                ue->GetPhy ()->SetRxAntennas(ueRx);
                if(ulTxMode == 11)
                {
                    ue->GetPhy ()->SetTxAntennas(ueRx);//for uplink mimo
                    
                }
                
                
                if((double)rand()/RAND_MAX < indoorUeFraction)
                {
                    ue->SetIndoorFlag(true);
                }
                
                WidebandCqiManager *cqiManager = new WidebandCqiManager ();
                //FullbandCqiManager *cqiManager = new FullbandCqiManager ();
                cqiManager->SetCqiReportingMode (CqiManager::PERIODIC);
                cqiManager->SetReportingInterval (cqiReportingInterval);
                cqiManager->SetDevice (ue);
                ue->SetCqiManager (cqiManager);
                ue->SetTwinFlag(false); //used when twinmodel is activated
                //ue->SetTwinTransmittingFlag(false); //used when twinmodel is activated
                if(handoverManager==1)
                {
                    ue->SethasTwin(true);
                }
                else
                {
                    ue->SethasTwin(false);
                }
                
                WidebandCqiEesmErrorModel *errorModel = new WidebandCqiEesmErrorModel ();
                ue->GetPhy ()->SetErrorModel (errorModel);
                
                networkManager->GetUserEquipmentContainer ()->push_back (ue);
                
                // register ue to the enb
                enb->RegisterUserEquipment (ue);
                // define the channel realizations
                GNodeB* prevEnb = nullptr;
                ChannelRealization *prev_c_dl, *prev_c_ul;
                for (auto enb : *networkManager->GetGNodeBContainer())
                {
                    ChannelRealization* c_dl = new ChannelRealization (enb, ue, model);
                    enb->GetPhy ()->GetDlChannel ()->GetPropagationLossModel ()->AddChannelRealization (c_dl);
                    ChannelRealization* c_ul = new ChannelRealization (ue, enb, model);
                    enb->GetPhy ()->GetUlChannel ()->GetPropagationLossModel ()->AddChannelRealization (c_ul);
                    
                    c_dl->SetShadowingStddev(shadowingStddev);
                    c_ul->SetShadowingStddev(shadowingStddev);
                    
                    if(ue->IsIndoor())
                    {
                        c_dl->SetPenetrationLossMean(penetrationLossMeanIndoor);
                        c_ul->SetPenetrationLossMean(penetrationLossMeanIndoor);
                    }
                    else
                    {
                        c_dl->SetPenetrationLossMean(penetrationLossMeanOutdoor);
                        c_ul->SetPenetrationLossMean(penetrationLossMeanOutdoor);
                    }
                    c_dl->SetPenetrationLossStdDev(penetrationLossStdDev);
                    c_ul->SetPenetrationLossStdDev(penetrationLossStdDev);
                    
                    // syncronize LoS state and shadowing for eNBs at the same site
                    if (prevEnb != nullptr && enb->GetCell()->GetIdCell() == prevEnb->GetCell()->GetIdCell())
                    {
                        c_dl->SetLoSState(prev_c_dl->GetLoSState());
                        c_ul->SetLoSState(prev_c_ul->GetLoSState());
                        c_dl->SetShadowing(prev_c_dl->GetShadowing());
                        c_ul->SetShadowing(prev_c_ul->GetShadowing());
                    }
                    prevEnb = enb;
                    prev_c_dl = c_dl;
                    prev_c_ul = c_ul;
                }
                
                
                // CREATE DOWNLINK APPLICATION FOR THIS UE
                double start_time = 0.020;
                double duration_time = start_time + flow_duration-1;
                
                
                
                for (int j = 0; j < nbdlCBR; j++)
                {
                    // create application
                    CBRApplication[cbrApplication].SetSource (enb);
                    CBRApplication[cbrApplication].SetDestination (ue);
                    CBRApplication[cbrApplication].SetApplicationID (applicationID);
                    CBRApplication[cbrApplication].SetStartTime(start_time);
                    CBRApplication[cbrApplication].SetStopTime(duration_time);
                    CBRApplication[cbrApplication].SetInterval (interval);
                    CBRApplication[cbrApplication].SetSize (1000);
                    
                    // create qos parameters
                    QoSParameters *qosParameters = new QoSParameters ();
                    qosParameters->SetMaxDelay (maxDelayCBR);
                    
                    CBRApplication[cbrApplication].SetQoSParameters (qosParameters);
                    
                    
                    //create classifier parameters
                    ClassifierParameters *cp = new ClassifierParameters (enb->GetIDNetworkNode(),
                                                                         ue->GetIDNetworkNode(),
                                                                         0,
                                                                         destinationPort,
                                                                         TransportProtocol::TRANSPORT_PROTOCOL_TYPE_UDP);
                    CBRApplication[cbrApplication].SetClassifierParameters (cp);
                    
                    cout << "CREATED CBR APPLICATION, ID " << applicationID << endl;
                    
                    //update counter
                    destinationPort++;
                    applicationID++;
                    cbrApplication++;
                }
                
                
                
                
                for (int j = 0; j < nbulCBR; j++)
                {
                    // create application
                    ulCBRApplication[ulcbrApplication].SetSource (ue);
                    ulCBRApplication[ulcbrApplication].SetDestination (enb);
                    ulCBRApplication[ulcbrApplication].SetApplicationID (applicationID);
                    ulCBRApplication[ulcbrApplication].SetStartTime(start_time);
                    ulCBRApplication[ulcbrApplication].SetStopTime(duration_time);
                    ulCBRApplication[ulcbrApplication].SetInterval (interval);
                    ulCBRApplication[ulcbrApplication].SetSize (1000);
                    
                    // create qos parameters
                    QoSParameters *qosParameters = new QoSParameters ();
                    qosParameters->SetMaxDelay (maxDelayCBR);
                    
                    ulCBRApplication[ulcbrApplication].SetQoSParameters (qosParameters);
                    
                    
                    //create classifier parameters
                    ClassifierParameters *cp = new ClassifierParameters (ue->GetIDNetworkNode(),
                                                                         enb->GetIDNetworkNode(),
                                                                         destinationPort,
                                                                         0,
                                                                         TransportProtocol::TRANSPORT_PROTOCOL_TYPE_UDP);
                    ulCBRApplication[ulcbrApplication].SetClassifierParameters (cp);
                    
                    cout << "CREATED CBR APPLICATION, ID " << applicationID << endl;
                    
                    //update counter
                    destinationPort++;
                    applicationID++;
                    ulcbrApplication++;
                }
                
            }
            else
            {
                
                UserEquipment* ue2 = new UserEquipment (idUE*100,
                                                        posX, posY, speed, speedDirection,
                                                        cells->at (0),
                                                        enb,
                                                        true,
                                                        Mobility::LINEAR_MOVEMENT
                                                        );
                
                
                cout << "Created UE - id " << idUE*100 << " position " << posX << " " << posY << endl;
                //Simulator::Init()->Schedule(0.003, &Mobility::SetHandover, ue2->GetMobilityModel(), false);
                ue2->GetProtocolStack()->GetMacEntity()->GetAmcModule()->setUseExtendedCQI(true);
                ue2->SetRandomAccessType(m_UeRandomAccessType);
                RadioChannel* dlCh2 = enb->GetPhy ()->GetDlChannel ();
                RadioChannel* ulCh2 = enb->GetPhy ()->GetUlChannel ();
                
                
                ue2->GetPhy()->SetTxPower(11.5);
                ue2->GetPhy ()->SetDlChannel (dlCh2);
                ue2->GetPhy ()->SetUlChannel (ulCh2);
                ue2->GetPhy ()->SetNoiseFigure(UENoiseFigure);
                ue2->GetPhy ()->SetRxAntennas(ueRx);
                ue2->SetTwinFlag(true); //used when twinmodel is activated
                ue2->SetTwinTransmittingFlag(false); //used when twinmodel is activated
                if(ulTxMode == 11)
                {
                    ue2->GetPhy ()->SetTxAntennas(ueRx);//for uplink mimo
                }
                if((double)rand()/RAND_MAX < indoorUeFraction)
                {
                    ue2->SetIndoorFlag(true);
                }
                
                
                if(handoverManager==1) //used when twinmodel is activated
                {
                    ue2->SethasTwin(true);
                }
                else
                {
                    ue2->SethasTwin(false);
                }
                
                WidebandCqiManager *cqiManager = new WidebandCqiManager ();
                //FullbandCqiManager *cqiManager = new FullbandCqiManager ();
                cqiManager->SetCqiReportingMode (CqiManager::PERIODIC);
                cqiManager->SetReportingInterval (cqiReportingInterval);
                cqiManager->SetDevice (ue2);
                ue2->SetCqiManager (cqiManager);
                
                WidebandCqiEesmErrorModel *errorModel = new WidebandCqiEesmErrorModel ();
                ue2->GetPhy ()->SetErrorModel (errorModel);
                
                networkManager->GetUserEquipmentContainer ()->push_back (ue2);
                
                // register ue to the enb
                enb->RegisterUserEquipment (ue2);
                
                
                
                // define the channel realizations
                GNodeB* prevEnb2 = nullptr;
                ChannelRealization *prev_c_dl2, *prev_c_ul2;
                for (auto enb : *networkManager->GetGNodeBContainer())
                {
                    ChannelRealization* c_dl2 = new ChannelRealization (enb, ue2, model);
                    enb->GetPhy ()->GetDlChannel ()->GetPropagationLossModel ()->AddChannelRealization (c_dl2);
                    ChannelRealization* c_ul2 = new ChannelRealization (ue2, enb, model);
                    enb->GetPhy ()->GetUlChannel ()->GetPropagationLossModel ()->AddChannelRealization (c_ul2);
                    
                    c_dl2->SetShadowingStddev(shadowingStddev);
                    c_ul2->SetShadowingStddev(shadowingStddev);
                    
                    if(ue2->IsIndoor())
                    {
                        c_dl2->SetPenetrationLossMean(penetrationLossMeanIndoor);
                        c_ul2->SetPenetrationLossMean(penetrationLossMeanIndoor);
                    }
                    else
                    {
                        c_dl2->SetPenetrationLossMean(penetrationLossMeanOutdoor);
                        c_ul2->SetPenetrationLossMean(penetrationLossMeanOutdoor);
                    }
                    c_dl2->SetPenetrationLossStdDev(penetrationLossStdDev);
                    c_ul2->SetPenetrationLossStdDev(penetrationLossStdDev);
                    
                    // syncronize LoS state and shadowing for eNBs at the same site
                    if (prevEnb2 != nullptr && enb->GetCell()->GetIdCell() == prevEnb2->GetCell()->GetIdCell())
                    {
                        c_dl2->SetLoSState(prev_c_dl2->GetLoSState());
                        c_ul2->SetLoSState(prev_c_ul2->GetLoSState());
                        c_dl2->SetShadowing(prev_c_dl2->GetShadowing());
                        c_ul2->SetShadowing(prev_c_ul2->GetShadowing());
                    }
                    prevEnb2 = enb;
                    prev_c_dl2 = c_dl2;
                    prev_c_ul2 = c_ul2;
                }
                
                
            }
        }
        
        idUE++;
        
    }
    simulator->SetStop(duration);
    simulator->Run ();
}

