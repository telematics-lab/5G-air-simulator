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

static void f5g_50MbpsEverywhereThesis (int argc, char *argv[])
{
    string environment(argv[2]); // "suburban" or "rural"
    double isd = atof(argv[3]); // km
    int nbUE = atof(argv[4]); // users/km2
    int speed = atoi(argv[5]); // km/h
    double duration = atof(argv[6]); // s
    int txMode = atoi(argv[7]);
    int ulTxMode = atoi(argv[8]);
    int nbTx = atoi(argv[9]);
    int nbMu = atoi(argv[10]);
    int nbRx = atoi(argv[11]);
    int dl_sched_type = atoi(argv[12]);
    int ul_sched_type = atoi(argv[13]);
    int seed = atoi(argv[14]);
    
    
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
    double handoverMargin = 3; // dB
    double cqiReportingInterval = 5; // TTI
    double shadowingStddev = 10; //dB
    int channelModel = 1;
    int nbCell = 2; // number of sites in the simulation
    int nbActiveCell = 1; // number of sites with active users
    int nbSector = 3; // number of sectors per site
    double BSNoiseFigure = 7; // dB
    bool handover = false;
    double indoorUeFraction = 0.8;
    double minDist = 0.025; // in km
    int enbTx = nbTx;
    int ueRx = nbRx;
    double vBeamWidth = 65;
    double BSFeederLoss = 2;
    
    //the next four lines are necessary for the uplink with RACH. Defining Random Access type
    UeRandomAccess::RandomAccessType m_UeRandomAccessType;
    GnbRandomAccess::RandomAccessType m_EnbRandomAccessType;
    m_UeRandomAccessType=UeRandomAccess::RA_TYPE_BASELINE;
    m_EnbRandomAccessType=GnbRandomAccess::RA_TYPE_BASELINE;
    int dlVideoBitRate = 128;
    int ulVideoBitRate = 242;
    //Define the number of downlink applications and application parameters
    int nbdlBE = 0;
    int nbdlVoIP = 0;
    int nbdlVideo = 0;
    int nbdlCBR = 0;
    
    //Define the number of uplink applications and application parameters
    int nbulBE = 0;
    int nbulVoIP = 1;
    int nbulVideo = 0;
    int nbulCBR = 0;
    
    //  double isd; // km
    if(environment=="suburban")
    {
        //isd = 0.25;
        etilt = 14;
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
        
        Cell *c = new Cell (i, radius, minDist, center.GetCoordinateY (), center.GetCoordinateX ());
        cells->push_back (c);
        networkManager->GetCellContainer ()->push_back (c);
        
        cout << "Created Cell, id " << c->GetIdCell ()
        <<", position: " << c->GetCellCenterPosition ()->GetCoordinateX ()
        << " " << c->GetCellCenterPosition ()->GetCoordinateY ()
        << " radius " << radius << endl;
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
            GNodeB* gnb = new GNodeB (i*3+j, cells->at (i));
            gnb->SetRandomAccessType(m_EnbRandomAccessType);
            gnb->GetPhy ()->SetDlChannel (dlCh);
            gnb->GetPhy ()->SetUlChannel (ulCh);
            gnb->GetPhy ()->SetNoiseFigure(BSNoiseFigure);
            gnb->GetPhy ()->SetCarrierFrequency(carrierFreq);
            gnb->GetPhy ()->SetTxAntennas(enbTx);
            if(ulTxMode == 11)
                gnb->GetPhy ()->SetRxAntennas(enbTx);//for uplink mimo
            gnb->GetPhy ()->SetTxPower(txPower);
            gnb->GetPhy ()->SetBandwidthManager (spectrum);
            ulCh->AddDevice (gnb);	//for the uplink
            gnb->GetPhy()->SetErrorModel(errorModel); //for the uplink
            gnb->GetPhy ()->SetHeight(antennaHeight);
            gnb->GetPhy ()->SetAverageBuildingHeight(avgBuildingHeight);
            gnb->GetProtocolStack ()->GetRrcEntity ()->GetHandoverEntity ()->GetHoManager ()->SetHandoverMargin(handoverMargin);
            // Set Antenna Parameters
            if(nbSector==1)
            {
                gnb->GetPhy ()->GetAntennaParameters ()->SetType(Phy::AntennaParameters::ANTENNA_TYPE_OMNIDIRECTIONAL);
            }
            else if(nbSector==3)
            {
                gnb->GetPhy ()->GetAntennaParameters ()->SetType(Phy::AntennaParameters::ANTENNA_TYPE_TRI_SECTOR);
                gnb->GetPhy ()->GetAntennaParameters ()->SetEtilt(etilt);
                gnb->GetPhy ()->GetAntennaParameters ()->SetHorizontalBeamwidth3db(horizontalBeamwidth3db);
                gnb->GetPhy ()->GetAntennaParameters ()->SetVerticalBeamwidth3db(vBeamWidth);
                gnb->GetPhy ()->GetAntennaParameters ()->SetGain(antennaGain);
                gnb->GetPhy ()->GetAntennaParameters ()->SetMaxHorizontalAttenuation(antennaAttenuation);
                gnb->GetPhy ()->GetAntennaParameters ()->SetMaxVerticalAttenuation(antennaAttenuation);
                gnb->GetPhy ()->GetAntennaParameters ()->SetBearing(120*j);
            }
            else
            {
                cout << "Error: unsupported number of cell sectors (" << nbSector << ")" <<endl;
                exit(1);
            }
            gnb->GetPhy ()->GetAntennaParameters ()->SetFeederLoss(BSFeederLoss);
            gnb->GetMacEntity ()->SetDefaultDlTxMode(txMode);
            gnb->GetMacEntity()->SetDefaultUlTxMode(ulTxMode);	//for uplink mimo
            gnb->SetDLScheduler (downlink_scheduler_type);
            gnb->SetULScheduler(uplink_scheduler_type);	//uplink scheduler setting
            gnb->m_ulSchedType = ul_sched_type; //uplink packet scheduler
            gnb->GetDLScheduler()->SetMaxUsersPerRB(nbMu);
            networkManager->GetGNodeBContainer()->push_back (gnb);
            
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
    cout << "Creating " << nbUE*nbSector*nbCell << " user terminals" << endl;
    
    
    //Define Downlink Application Container
    InfiniteBuffer ftpApplication[nbCell*nbSector*nbdlBE*nbUE];
    TraceBased VideoApplication[nbCell*nbSector*nbdlVideo*nbUE];
    VoIP VoIPApplication[nbCell*nbSector*nbdlVoIP*nbUE];
    CBR CBRApplication[nbCell*nbSector*nbdlCBR*nbUE];
    
    //Define Uplink Application Container
    InfiniteBuffer ulftpApplication[nbCell*nbSector*nbulBE*nbUE];
    TraceBased ulVideoApplication[nbCell*nbSector*nbulVideo*nbUE];
    VoIP ulVoIPApplication[nbCell*nbSector*nbulVoIP*nbUE];
    CBR ulCBRApplication[nbCell*nbSector*nbulCBR*nbUE];
    int nrho =0;
    int ftpApplicationID = 0;
    int voipApplication = 0;
    int cbrApplication = 0;
    int videoApplication = 0;
    
    int ulvoipApplication = 0;
    int ulcbrApplication = 0;
    int ulvideoApplication = 0;
    
    double maxDelayVideo = 0.7;
    double maxDelayVoIP = 0.5;
    double maxDelayCBR = 0.01;
    int destinationPort = 101;
    int applicationID = 0;
    CartesianCoordinates center = CartesianCoordinates(0,0);
    
    int idUE = 1;//nbCell*nbSector;
    
    
    //Create UEs
    for(int k=0;k<nbCell;k++)
    {
        vector<CartesianCoordinates*> *positions = GetUniformUsersDistribution (k, nbUE);
        for(int m=0;m<nbSector;m++)
        {
            
            
            //ue's random position
            double posX, posY;
            //bool isNearToCellCenter;
            CartesianCoordinates position;
            //do
            //{
            
            double meterRadius = radius*1000;
            int intRadius = (int) meterRadius;
            double distance = (double)(rand() % intRadius);
            double theta;
            if(m==0)
                theta = (double)((rand() %121)-60) * ((2*M_PI)/(360));
            else if(m==1)
                theta = (double)((rand() %121)+60) * ((2*M_PI)/(360));
            else
                theta = (double)((rand() %121)+180) * ((2*M_PI)/(360));
            
            posX = cells->at (k)->GetCellCenterPosition()->GetCoordinateX()+distance*cos(theta);
            posY = cells->at (k)->GetCellCenterPosition()->GetCoordinateY()+distance*sin(theta);
            position = CartesianCoordinates(posX,posY);
            //isNearToCellCenter = false;
            for(int i=0; i<nbCell; i++)
            {
                double distance = cells->at(i)->GetCellCenterPosition ()->GetDistance(posX/1000,posY/1000);
                if (distance<minDist)
                {
                    //isNearToCellCenter = true;
                }
            }
            //}
            //while (sqrt(posX*posX + posY*posY) > ue_drop_radius*1000 ||
            //		/*sqrt(posX*posX + posY*posY) < minDist*1000*/
            //		!(position.GetPolarAzimut(&center)<M_PI/3 || position.GetPolarAzimut(&center)>5*M_PI/3) ||
            //		isNearToCellCenter == true);
            
            double speedDirection = (double)(rand() %360) * ((2*M_PI)/360);
            
            GNodeB* gnb = networkManager->GetGNodeBContainer()-> at(k*3+m);
            
            if(false)
            {
                nrho=1;
            }
            else if(true)
            {
                nrho =1;
            }
            else
            {
                nrho =1;
            }
            
            
            for (int n = 0; n < nbUE; n++)
            {
                for(int p=0;p<nrho;p++)
                {
                    if(p==0)
                    {
                        
                        
                        UserEquipment* ue = new UserEquipment (idUE,
                                                               posX, posY, speed, speedDirection,
                                                               cells->at (k),
                                                               gnb,
                                                               handover,
                                                               Mobility::LINEAR_MOVEMENT
                                                               //Mobility::RANDOM_DIRECTION
                                                               //	Mobility::CONSTANT_POSITION
                                                               );
                        cout << "Created UE - id " << idUE << " position " << posX << " " << posY << endl;
                        
                        
                        Simulator::Init()->Schedule(0.003, &Mobility::SetHandover, ue->GetMobilityModel(), true);
                        ue->GetProtocolStack()->GetMacEntity()->GetAmcModule()->setUseExtendedCQI(true);
                        ue->SetRandomAccessType(m_UeRandomAccessType);
                        RadioChannel* dlCh = gnb->GetPhy ()->GetDlChannel ();
                        RadioChannel* ulCh = gnb->GetPhy ()->GetUlChannel ();
                        
                        ue->GetPhy ()->SetDlChannel (dlCh);
                        ue->GetPhy ()->SetUlChannel (ulCh);
                        ue->GetPhy ()->SetNoiseFigure(UENoiseFigure);
                        ue->GetPhy ()->SetRxAntennas(ueRx);
                        if(ulTxMode == 11)
                            ue->GetPhy ()->SetTxAntennas(ueRx);//for uplink mimo
                        
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
                        
                        WidebandCqiEesmErrorModel *errorModel = new WidebandCqiEesmErrorModel ();
                        ue->GetPhy ()->SetErrorModel (errorModel);
                        
                        networkManager->GetUserEquipmentContainer ()->push_back (ue);
                        
                        // register ue to the gnb
                        gnb->RegisterUserEquipment (ue);
                        // define the channel realizations
                        GNodeB* prevEnb = nullptr;
                        ChannelRealization *prev_c_dl, *prev_c_ul;
                        for (auto gnb : *networkManager->GetGNodeBContainer())
                        {
                            ChannelRealization* c_dl = new ChannelRealization (gnb, ue, model);
                            gnb->GetPhy ()->GetDlChannel ()->GetPropagationLossModel ()->AddChannelRealization (c_dl);
                            ChannelRealization* c_ul = new ChannelRealization (ue, gnb, model);
                            gnb->GetPhy ()->GetUlChannel ()->GetPropagationLossModel ()->AddChannelRealization (c_ul);
                            
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
                            if (prevEnb != nullptr && gnb->GetCell()->GetIdCell() == prevEnb->GetCell()->GetIdCell())
                            {
                                c_dl->SetLoSState(prev_c_dl->GetLoSState());
                                c_ul->SetLoSState(prev_c_ul->GetLoSState());
                                c_dl->SetShadowing(prev_c_dl->GetShadowing());
                                c_ul->SetShadowing(prev_c_ul->GetShadowing());
                            }
                            prevEnb = gnb;
                            prev_c_dl = c_dl;
                            prev_c_ul = c_ul;
                        }
                        
                        
                        // CREATE DOWNLINK APPLICATION FOR THIS UE
                        double start_time = /*0.001*idUE*/ 0.001;
                        double duration_time = start_time + flow_duration;
                        
                        
                        
                        // *** Best Effort application
                        for (int j = 0; j < nbdlBE; j++)
                        {
                            // create application
                            ftpApplication[ftpApplicationID].SetSource (gnb);
                            ftpApplication[ftpApplicationID].SetDestination (ue);
                            ftpApplication[ftpApplicationID].SetApplicationID (applicationID);
                            ftpApplication[ftpApplicationID].SetStartTime(start_time);
                            ftpApplication[ftpApplicationID].SetStopTime(duration_time);
                            
                            
                            // create qos parameters
                            QoSParameters *qosParameters = new QoSParameters ();
                            ftpApplication[ftpApplicationID].SetQoSParameters (qosParameters);
                            
                            
                            //create classifier parameters
                            ClassifierParameters *cp = new ClassifierParameters (gnb->GetIDNetworkNode(),
                                                                                 ue->GetIDNetworkNode(),
                                                                                 0,
                                                                                 destinationPort,
                                                                                 TransportProtocol::TRANSPORT_PROTOCOL_TYPE_TCP);
                            ftpApplication[ftpApplicationID].SetClassifierParameters (cp);
                            
                            cout << "CREATED BE APPLICATION, ID " << applicationID << endl;
                            
                            //update counter
                            destinationPort++;
                            applicationID++;
                            ftpApplicationID++;
                        }
                        
                        // *** voip application
                        for (int j = 0; j < nbdlVoIP; j++)
                        {
                            // create application
                            VoIPApplication[voipApplication].SetSource (gnb);
                            VoIPApplication[voipApplication].SetDestination (ue);
                            VoIPApplication[voipApplication].SetApplicationID (applicationID);
                            VoIPApplication[voipApplication].SetStartTime(start_time);
                            VoIPApplication[voipApplication].SetStopTime(duration_time);
                            
                            // create qos parameters
                            if (downlink_scheduler_type == GNodeB::DLScheduler_TYPE_FLS)
                            {
                                QoSForFLS *qos = new QoSForFLS ();
                                qos->SetMaxDelay (maxDelayVoIP);
                                if (maxDelayVoIP == 0.1)
                                {
                                    cout << "Target Delay = 0.1 s, M = 9" << endl;
                                    qos->SetNbOfCoefficients (9);
                                }
                                else if (maxDelayVoIP == 0.08)
                                {
                                    cout << "Target Delay = 0.08 s, M = 7" << endl;
                                    qos->SetNbOfCoefficients (7);
                                }
                                else if (maxDelayVoIP == 0.06)
                                {
                                    cout << "Target Delay = 0.06 s, M = 5" << endl;
                                    qos->SetNbOfCoefficients (5);
                                }
                                else if (maxDelayVoIP == 0.04)
                                {
                                    cout << "Target Delay = 0.04 s, M = 3" << endl;
                                    qos->SetNbOfCoefficients (3);
                                }
                                else
                                {
                                    cout << "ERROR: target delay is not available"<< endl;
                                    return;
                                }
                                
                                VoIPApplication[voipApplication].SetQoSParameters (qos);
                            }
                            else if (downlink_scheduler_type == GNodeB::DLScheduler_TYPE_EXP)
                            {
                                QoSForEXP *qos = new QoSForEXP ();
                                qos->SetMaxDelay (maxDelayVoIP);
                                VoIPApplication[voipApplication].SetQoSParameters (qos);
                            }
                            else if (downlink_scheduler_type == GNodeB::DLScheduler_TYPE_MLWDF)
                            {
                                QoSForM_LWDF *qos = new QoSForM_LWDF ();
                                qos->SetMaxDelay (maxDelayVoIP);
                                VoIPApplication[voipApplication].SetQoSParameters (qos);
                            }
                            else
                            {
                                QoSParameters *qos = new QoSParameters ();
                                qos->SetMaxDelay (maxDelayVoIP);
                                VoIPApplication[voipApplication].SetQoSParameters (qos);
                            }
                            
                            
                            //create classifier parameters
                            ClassifierParameters *cp = new ClassifierParameters (gnb->GetIDNetworkNode(),
                                                                                 ue->GetIDNetworkNode(),
                                                                                 0,
                                                                                 destinationPort,
                                                                                 TransportProtocol::TRANSPORT_PROTOCOL_TYPE_UDP);
                            VoIPApplication[voipApplication].SetClassifierParameters (cp);
                            
                            cout << "CREATED VOIP APPLICATION, ID " << applicationID << endl;
                            
                            //update counter
                            destinationPort++;
                            applicationID++;
                            voipApplication++;
                        }
                        
                        // *** video application
                        for (int j = 0; j < nbdlVideo; j++)
                        {
                            // create application
                            VideoApplication[videoApplication].SetSource (gnb);
                            VideoApplication[videoApplication].SetDestination (ue);
                            VideoApplication[videoApplication].SetApplicationID (applicationID);
                            VideoApplication[videoApplication].SetStartTime(start_time);
                            VideoApplication[videoApplication].SetStopTime(duration_time);
                            
                            switch (dlVideoBitRate)
                            {
                                case 128:
                                {
                                    //VideoApplication[videoApplication].LoadInternalTrace(&foreman_h264_128k);
                                    VideoApplication[videoApplication].LoadInternalTrace(&highway_h264_128k);
                                    //                  VideoApplication[videoApplication].LoadInternalTrace(&mobile_h264_128k);
                                    cout << "  selected video @ 128k"<< endl;
                                    break;
                                }
                                case 242:
                                {
                                    VideoApplication[videoApplication].LoadInternalTrace(&foreman_h264_242k);
                                    cout << "  selected video @ 242k"<< endl;
                                    break;
                                }
                                case 440:
                                {
                                    VideoApplication[videoApplication].LoadInternalTrace(&foreman_h264_440k);
                                    cout << "  selected video @ 440k"<< endl;
                                    break;
                                }
                                default:
                                {
                                    cout << "  Unsupported video bitrate!"<< endl;
                                    exit(1);
                                }
                            }
                            
                            // create qos parameters
                            if (downlink_scheduler_type == GNodeB::DLScheduler_TYPE_FLS)
                            {
                                QoSForFLS *qos = new QoSForFLS ();
                                qos->SetMaxDelay (maxDelayVideo);
                                if (maxDelayVideo == 0.1)
                                {
                                    cout << "Target Delay = 0.1 s, M = 9" << endl;
                                    qos->SetNbOfCoefficients (9);
                                }
                                else if (maxDelayVideo == 0.08)
                                {
                                    cout << "Target Delay = 0.08 s, M = 7" << endl;
                                    qos->SetNbOfCoefficients (7);
                                }
                                else if (maxDelayVideo == 0.06)
                                {
                                    cout << "Target Delay = 0.06 s, M = 5" << endl;
                                    qos->SetNbOfCoefficients (5);
                                }
                                else if (maxDelayVideo == 0.04)
                                {
                                    cout << "Target Delay = 0.04 s, M = 3" << endl;
                                    qos->SetNbOfCoefficients (3);
                                }
                                else
                                {
                                    cout << "ERROR: target delay is not available"<< endl;
                                    return;
                                }
                                
                                VideoApplication[videoApplication].SetQoSParameters (qos);
                            }
                            else if (downlink_scheduler_type == GNodeB::DLScheduler_TYPE_EXP)
                            {
                                QoSForEXP *qos = new QoSForEXP ();
                                qos->SetMaxDelay (maxDelayVideo);
                                VideoApplication[videoApplication].SetQoSParameters (qos);
                            }
                            else if (downlink_scheduler_type == GNodeB::DLScheduler_TYPE_MLWDF)
                            {
                                QoSForM_LWDF *qos = new QoSForM_LWDF ();
                                qos->SetMaxDelay (maxDelayVideo);
                                VideoApplication[videoApplication].SetQoSParameters (qos);
                            }
                            else
                            {
                                QoSParameters *qos = new QoSParameters ();
                                qos->SetMaxDelay (maxDelayVideo);
                                VideoApplication[videoApplication].SetQoSParameters (qos);
                            }
                            
                            
                            //create classifier parameters
                            ClassifierParameters *cp = new ClassifierParameters (gnb->GetIDNetworkNode(),
                                                                                 ue->GetIDNetworkNode(),
                                                                                 0,
                                                                                 destinationPort,
                                                                                 TransportProtocol::TRANSPORT_PROTOCOL_TYPE_UDP);
                            VideoApplication[videoApplication].SetClassifierParameters (cp);
                            
                            cout << "CREATED VIDEO APPLICATION, ID " << applicationID << endl;
                            
                            //update counter
                            destinationPort++;
                            applicationID++;
                            videoApplication++;
                        }
                        
                        for (int j = 0; j < nbdlCBR; j++)
                        {
                            // create application
                            CBRApplication[cbrApplication].SetSource (gnb);
                            CBRApplication[cbrApplication].SetDestination (ue);
                            CBRApplication[cbrApplication].SetApplicationID (applicationID);
                            CBRApplication[cbrApplication].SetStartTime(start_time);
                            CBRApplication[cbrApplication].SetStopTime(duration_time);
                            CBRApplication[cbrApplication].SetInterval (0.04);
                            CBRApplication[cbrApplication].SetSize (5);
                            
                            // create qos parameters
                            QoSParameters *qosParameters = new QoSParameters ();
                            qosParameters->SetMaxDelay (maxDelayCBR);
                            
                            CBRApplication[cbrApplication].SetQoSParameters (qosParameters);
                            
                            
                            //create classifier parameters
                            ClassifierParameters *cp = new ClassifierParameters (gnb->GetIDNetworkNode(),
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
                        
                        //CREATE UPLINK APPLICATION FOR THIS UE
                        // *** Best Effort application
                        for (int j = 0; j < nbulBE; j++)
                        {
                            // create application
                            ulftpApplication[ftpApplicationID].SetSource (ue);
                            ulftpApplication[ftpApplicationID].SetDestination (gnb);
                            ulftpApplication[ftpApplicationID].SetApplicationID (applicationID);
                            ulftpApplication[ftpApplicationID].SetStartTime(start_time);
                            ulftpApplication[ftpApplicationID].SetStopTime(duration_time);
                            
                            
                            // create qos parameters
                            QoSParameters *qosParameters = new QoSParameters ();
                            ulftpApplication[ftpApplicationID].SetQoSParameters (qosParameters);
                            
                            
                            //create classifier parameters
                            ClassifierParameters *cp = new ClassifierParameters (ue->GetIDNetworkNode(),
                                                                                 gnb->GetIDNetworkNode(),
                                                                                 destinationPort,
                                                                                 0,
                                                                                 TransportProtocol::TRANSPORT_PROTOCOL_TYPE_UDP);
                            ulftpApplication[ftpApplicationID].SetClassifierParameters (cp);
                            
                            cout << "CREATED BE APPLICATION, ID " << applicationID << endl;
                            
                            //update counter
                            destinationPort++;
                            applicationID++;
                            ftpApplicationID++;
                        }
                        
                        // *** voip application
                        for (int j = 0; j < nbulVoIP; j++)
                        {
                            // create application
                            ulVoIPApplication[ulvoipApplication].SetSource (ue);
                            ulVoIPApplication[ulvoipApplication].SetDestination (gnb);
                            ulVoIPApplication[ulvoipApplication].SetApplicationID (applicationID);
                            ulVoIPApplication[ulvoipApplication].SetStartTime(start_time);
                            ulVoIPApplication[ulvoipApplication].SetStopTime(duration_time);
                            
                            // create qos parameters
                            QoSParameters *qos = new QoSParameters ();
                            qos->SetMaxDelay (maxDelayVoIP);
                            ulVoIPApplication[ulvoipApplication].SetQoSParameters (qos);
                            
                            
                            
                            //create classifier parameters
                            ClassifierParameters *cp = new ClassifierParameters (ue->GetIDNetworkNode(),
                                                                                 gnb->GetIDNetworkNode(),
                                                                                 destinationPort,
                                                                                 0,
                                                                                 TransportProtocol::TRANSPORT_PROTOCOL_TYPE_UDP);
                            ulVoIPApplication[ulvoipApplication].SetClassifierParameters (cp);
                            
                            cout << "CREATED VOIP APPLICATION, ID " << applicationID << endl;
                            
                            //update counter
                            destinationPort++;
                            applicationID++;
                            ulvoipApplication++;
                        }
                        
                        // *** video application
                        for (int j = 0; j < nbulVideo; j++)
                        {
                            // create application
                            ulVideoApplication[ulvideoApplication].SetSource (ue);
                            ulVideoApplication[ulvideoApplication].SetDestination (gnb);
                            ulVideoApplication[ulvideoApplication].SetApplicationID (applicationID);
                            ulVideoApplication[ulvideoApplication].SetStartTime(start_time);
                            ulVideoApplication[ulvideoApplication].SetStopTime(duration_time);
                            
                            switch (ulVideoBitRate)
                            {
                                case 128:
                                {
                                    //ulVideoApplication[ulvideoApplication].LoadInternalTrace(&foreman_h264_128k);
                                    //                 ulVideoApplication[ulvideoApplication].LoadInternalTrace(&highway_h264_128k);
                                    ulVideoApplication[ulvideoApplication].LoadInternalTrace(&mobile_h264_128k);
                                    cout << "  selected video @ 128k"<< endl;
                                    break;
                                }
                                case 242:
                                {
                                    ulVideoApplication[ulvideoApplication].LoadInternalTrace(&foreman_h264_242k);
                                    cout << "  selected video @ 242k"<< endl;
                                    break;
                                }
                                case 440:
                                {
                                    ulVideoApplication[ulvideoApplication].LoadInternalTrace(&foreman_h264_440k);
                                    cout << "  selected video @ 440k"<< endl;
                                    break;
                                }
                                default:
                                {
                                    cout << "  Unsupported video bitrate!"<< endl;
                                    exit(1);
                                }
                            }
                            
                            // create qos parameters
                            QoSParameters *qos = new QoSParameters ();
                            qos->SetMaxDelay (maxDelayVideo);
                            ulVideoApplication[ulvideoApplication].SetQoSParameters (qos);
                            
                            
                            //create classifier parameters
                            ClassifierParameters *cp = new ClassifierParameters (ue->GetIDNetworkNode(),
                                                                                 gnb->GetIDNetworkNode(),
                                                                                 destinationPort,
                                                                                 0,
                                                                                 TransportProtocol::TRANSPORT_PROTOCOL_TYPE_UDP);
                            ulVideoApplication[ulvideoApplication].SetClassifierParameters (cp);
                            
                            cout << "CREATED VIDEO APPLICATION, ID " << applicationID << endl;
                            
                            //update counter
                            destinationPort++;
                            applicationID++;
                            ulvideoApplication++;
                        }
                        
                        for (int j = 0; j < nbulCBR; j++)
                        {
                            // create application
                            ulCBRApplication[ulcbrApplication].SetSource (ue);
                            ulCBRApplication[ulcbrApplication].SetDestination (gnb);
                            ulCBRApplication[ulcbrApplication].SetApplicationID (applicationID);
                            ulCBRApplication[ulcbrApplication].SetStartTime(start_time);
                            ulCBRApplication[ulcbrApplication].SetStopTime(duration_time);
                            ulCBRApplication[ulcbrApplication].SetInterval (0.04);
                            ulCBRApplication[ulcbrApplication].SetSize (5000);
                            
                            // create qos parameters
                            QoSParameters *qosParameters = new QoSParameters ();
                            qosParameters->SetMaxDelay (maxDelayCBR);
                            
                            ulCBRApplication[ulcbrApplication].SetQoSParameters (qosParameters);
                            
                            
                            //create classifier parameters
                            ClassifierParameters *cp = new ClassifierParameters (ue->GetIDNetworkNode(),
                                                                                 gnb->GetIDNetworkNode(),
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
                                                                cells->at (k),
                                                                gnb,
                                                                handover,
                                                                Mobility::LINEAR_MOVEMENT
                                                                //Mobility::RANDOM_DIRECTION
                                                                //	Mobility::CONSTANT_POSITION
                                                                );
                        
                        
                        cout << "Created UE - id " << idUE*100 << " position " << posX << " " << posY << endl;
                        //		Simulator::Init()->Schedule(0.003, &Mobility::SetHandover, ue2->GetMobilityModel(), false);
                        ue2->GetProtocolStack()->GetMacEntity()->GetAmcModule()->setUseExtendedCQI(true);
                        ue2->SetRandomAccessType(m_UeRandomAccessType);
                        RadioChannel* dlCh2 = gnb->GetPhy ()->GetDlChannel ();
                        RadioChannel* ulCh2 = gnb->GetPhy ()->GetUlChannel ();
                        
                        ue2->GetPhy ()->SetDlChannel (dlCh2);
                        ue2->GetPhy ()->SetUlChannel (ulCh2);
                        ue2->GetPhy ()->SetNoiseFigure(UENoiseFigure);
                        ue2->GetPhy ()->SetRxAntennas(ueRx);
                        if(ulTxMode == 11)
                        {
                            ue2->GetPhy ()->SetTxAntennas(ueRx);//for uplink mimo
                        }
                        if((double)rand()/RAND_MAX < indoorUeFraction)
                        {
                            ue2->SetIndoorFlag(true);
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
                        
                        // register ue to the gnb
                        //gnb->RegisterUserEquipment (ue2);
                        
                        
                        
                        // define the channel realizations
                        /*	ENodeB* prevEnb2 = nullptr;
                         ChannelRealization *prev_c_dl2, *prev_c_ul2;
                         for (auto gnb : *networkManager->GetENodeBContainer())
                         {
                         ChannelRealization* c_dl2 = new ChannelRealization (gnb, ue2, model);
                         gnb->GetPhy ()->GetDlChannel ()->GetPropagationLossModel ()->AddChannelRealization (c_dl2);
                         ChannelRealization* c_ul2 = new ChannelRealization (ue2, gnb, model);
                         gnb->GetPhy ()->GetUlChannel ()->GetPropagationLossModel ()->AddChannelRealization (c_ul2);
                         
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
                         if (prevEnb2 != nullptr && gnb->GetCell()->GetIdCell() == prevEnb2->GetCell()->GetIdCell())
                         {
                         c_dl2->SetLoSState(prev_c_dl2->GetLoSState());
                         c_ul2->SetLoSState(prev_c_ul2->GetLoSState());
                         c_dl2->SetShadowing(prev_c_dl2->GetShadowing());
                         c_ul2->SetShadowing(prev_c_ul2->GetShadowing());
                         }
                         prevEnb2 = gnb;
                         prev_c_dl2 = c_dl2;
                         prev_c_ul2 = c_ul2;
                         }*/
                        
                        
                    }
                }
                
                idUE++;
            }
            
        }
    }
    simulator->SetStop(duration);
    simulator->Run ();
}

