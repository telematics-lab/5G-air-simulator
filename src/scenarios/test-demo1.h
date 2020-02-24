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
 * Author: Alessandro Grassi <alessandro.grassi@poliba.it>
 */

#include "../channel/RadioChannel.h"
#include "../phy/gnb-phy.h"
#include "../phy/ue-phy.h"
#include "../core/spectrum/bandwidth-manager.h"
#include "../networkTopology/Cell.h"
#include "../protocolStack/packet/packet-burst.h"
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


static void f5g_demo1 (int argc, char *argv[])
{
  string environment(argv[2]);
  double isd = atof(argv[3]);
  double userDensity = atof(argv[4]);
  double bitrate = atof(argv[5]);
  int speed = atoi(argv[6]);
  double duration = atof(argv[7]);
  int gnbTx = atoi(argv[8]);
  int ueRx = atoi(argv[9]);
  int txMode = atoi(argv[10]);
  int seed;
  if (argc==12)
    {
      seed = atoi(argv[11]);
    }
  else
    {
      seed = -1;
    }

  double bandwidth = 20; // MHz
  int etilt; //degrees
  double penetrationLossMeanIndoor = 0; // dB
  double penetrationLossMeanOutdoor = 0; // dB
  double penetrationLossStdDev = 0; //dB
  double carrierFreq; //MHz
  double txPower = 46; // dBm
  double antennaHeight = 25; // m
  double avgBuildingHeight = 20; // m
  double antennaGain = 17; // dBi
  double antennaAttenuation = 20; // dB
  double horizontalBeamwidth3db = 65; //degrees
  int sched_type = 8; // Round Robin
  double UENoiseFigure = 9; // dB
  double handoverMargin = 3; // dB
  double cqiReportingInterval = 5; // TTI
  double shadowingStddev = 10; //dB
  int channelModel = 1;
  int nbCell = 19; // number of sites in the simulation
  int nbActiveCell = 1; // number of sites with active users
  int nbSector = 3; // number of sectors per site
  double BSNoiseFigure = 7; // dB
  bool handover = false;
  double indoorUeFraction = 0;

  double packetSize = 2; //MBytes
  double packetInterval = packetSize * 8 / bitrate; // s


  double minDist = 0.025; // in km
//  int gnbTx = 2;
//  int ueRx = 2;
//  int txMode = 4;
  double vBeamWidth = 15;
  double BSFeederLoss = 2;


//  double isd; // km
  if(environment=="suburban")
    {
      //isd = 0.25;
      etilt = 14;
      channelModel = 0;
      carrierFreq = 2000;
    }
  else if(environment=="rural")
    {
      //isd = 0.5;
      etilt = 10;
      channelModel = 0;
      carrierFreq = 800;
    }
  else
    {
      cout << "ERROR: invalid environment selected" << endl;
    }

  // define simulation times
  double flow_duration = duration-packetInterval;

  int nbApplication = 1; // number of application flows per user

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
  switch (sched_type)
    {
    case 1:
      downlink_scheduler_type = GNodeB::DLScheduler_TYPE_PROPORTIONAL_FAIR;
      cout << "Scheduler PF "<< endl;
      break;
    case 2:
      downlink_scheduler_type = GNodeB::DLScheduler_TYPE_MLWDF;
      cout << "Scheduler MLWDF "<< endl;
      break;
    case 3:
      downlink_scheduler_type = GNodeB::DLScheduler_TYPE_EXP;
      cout << "Scheduler EXP "<< endl;
      break;
    case 4:
      downlink_scheduler_type = GNodeB::DLScheduler_TYPE_FLS;
      cout << "Scheduler FLS "<< endl;
      break;
    case 5:
      downlink_scheduler_type = GNodeB::DLScheduler_EXP_RULE;
      cout << "Scheduler EXP_RULE "<< endl;
      break;
    case 6:
      downlink_scheduler_type = GNodeB::DLScheduler_LOG_RULE;
      cout << "Scheduler LOG RULE "<< endl;
      break;
    case 7:
      downlink_scheduler_type = GNodeB::DLScheduler_TYPE_MAXIMUM_THROUGHPUT;
      cout << "Scheduler MT "<< endl;
      break;
    case 8:
      downlink_scheduler_type = GNodeB::DLScheduler_TYPE_ROUND_ROBIN;
      cout << "Scheduler RR "<< endl;
      break;
    default:
      downlink_scheduler_type = GNodeB::DLScheduler_TYPE_PROPORTIONAL_FAIR;
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

          //Create GNodeB
          GNodeB* gnb = new GNodeB (i*3+j, cells->at (i));
          gnb->GetPhy ()->SetDlChannel (dlCh);
          gnb->GetPhy ()->SetUlChannel (ulCh);
          gnb->GetPhy ()->SetNoiseFigure(BSNoiseFigure);
          gnb->GetPhy ()->SetCarrierFrequency(carrierFreq);
          gnb->GetPhy ()->SetTxAntennas(gnbTx);
          gnb->GetPhy ()->SetTxPower(txPower);
          gnb->GetPhy ()->SetBandwidthManager (spectrum);
          gnb->GetPhy ()->SetHeight(antennaHeight);
          gnb->GetPhy ()->SetAverageBuildingHeight(avgBuildingHeight);
          gnb->GetProtocolStack ()->GetRrcEntity ()->GetHandoverEntity ()->GetHoManager ()->SetHandoverMargin(handoverMargin);
          // Set Antenna Parameters
          if(nbSector==1)
            {
              gnb->GetPhy ()->GetAntennaParameters ()->SetType(Phy::AntennaParameters::ANTENNA_TYPE_OMNIDIRECTIONAL);
            }
          else
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
          gnb->GetPhy ()->GetAntennaParameters ()->SetFeederLoss(BSFeederLoss);
          gnb->GetMacEntity ()->SetDefaultDlTxMode(txMode);
          ulCh->AddDevice (gnb);
          if (gnb->GetCell()->GetIdCell() < nbActiveCell)
            gnb->SetDLScheduler (downlink_scheduler_type);
          networkManager->GetGNodeBContainer ()->push_back (gnb);

        }
    }

  //Create GW
  Gateway *gw = new Gateway ();
  networkManager->GetGatewayContainer ()->push_back (gw);


  double ue_drop_radius;
  if(nbActiveCell==1)
    {
      ue_drop_radius = radius;
    }
  else if(nbActiveCell>1 && nbActiveCell<=7)
    {
      ue_drop_radius = 3*radius;
    }
  else if(nbActiveCell>7 && nbActiveCell<=19)
    {
      ue_drop_radius = 5*radius;
    }

  ue_drop_radius = isd;

  //double area = radius * radius * sqrt(3) * 1.5 * nbActiveCell; // area of the simulation
  double area = ue_drop_radius * ue_drop_radius * M_PI;
  //double area = ue_drop_radius * ue_drop_radius * M_PI - nbActiveCell * minDist * minDist * M_PI;
  int nbUE = 1;//round( area * userDensity );
  cout << "Creating " << nbUE << " user terminals" << endl;

  //Define Application Container
//  FTP2 ftpApplication[nbApplication*nbUE];
  InfiniteBuffer ftpApplication[nbApplication*nbUE];
  int ftpApplicationID = 0;
  int destinationPort = 101;
  int applicationID = 0;

  int idUE = nbCell*nbSector;

  //Create UEs
  for (int n = 0; n < nbUE; n++)
    {
      //ue's random position
      double posX, posY;
      /* bool isNearToCellCenter;
       do
         {
           posX = (2*ue_drop_radius*1000)*((double)rand()/RAND_MAX) - ue_drop_radius*1000;
           posY = (2*ue_drop_radius*1000)*((double)rand()/RAND_MAX) - ue_drop_radius*1000;
           isNearToCellCenter = false;
           for(int i=0; i<nbCell; i++)
             {
               double distance = cells->at(i)->GetCellCenterPosition ()->GetDistance(posX/1000,posY/1000);
               if (distance<minDist)
                 {
                   isNearToCellCenter = true;
                 }
             }
         }
       while (sqrt(posX*posX + posY*posY) > ue_drop_radius*1000*0.95 ||
              isNearToCellCenter == true);*/

      posX = minDist*1000;
      posY = 0;
      double speedDirection = 0;//(double)(rand() %360) * ((2*M_PI)/360);

      GNodeB* gnb = networkManager->GetGNodeBContainer ()->at(0);

      UserEquipment* ue = new UserEquipment (idUE,
                                             posX, posY, speed, speedDirection,
                                             cells->at (0),
                                             gnb,
                                             handover,
                                             //Mobility::RANDOM_WALK
                                             Mobility::LINEAR_MOVEMENT
                                             //Mobility::CONSTANT_POSITION
                                            );

      cout << "Created UE - id " << idUE << " position " << posX << " " << posY << endl;

      RadioChannel* dlCh = gnb->GetPhy ()->GetDlChannel ();
      RadioChannel* ulCh = gnb->GetPhy ()->GetUlChannel ();

      ue->GetPhy ()->SetDlChannel (dlCh);
      ue->GetPhy ()->SetUlChannel (ulCh);
      ue->GetPhy ()->SetNoiseFigure(UENoiseFigure);
      ue->GetPhy ()->SetRxAntennas(ueRx);
      if((double)rand()/RAND_MAX < indoorUeFraction)
        {
          ue->SetIndoorFlag(true);
        }

      ue->SetIndoorFlag(false);

      WidebandCqiManager *cqiManager = new WidebandCqiManager ();
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
      GNodeB* prevGnb = nullptr;
      ChannelRealization *prev_c_dl, *prev_c_ul;
      for (auto gnb : *networkManager->GetGNodeBContainer ())
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

          // syncronize LoS state and shadowing for gNBs at the same site
          if (prevGnb != nullptr && gnb->GetCell()->GetIdCell() == prevGnb->GetCell()->GetIdCell())
            {
              c_dl->SetLoSState(prev_c_dl->GetLoSState());
              c_ul->SetLoSState(prev_c_ul->GetLoSState());
              c_dl->SetShadowing(prev_c_dl->GetShadowing());
              c_ul->SetShadowing(prev_c_ul->GetShadowing());
            }
          prevGnb = gnb;
          prev_c_dl = c_dl;
          prev_c_ul = c_ul;
        }


      // CREATE DOWNLINK APPLICATION FOR THIS UE
      double start_time = 0.001*idUE + 0.01;
      double duration_time = start_time + flow_duration;

      // *** ftp application
      for (int j = 0; j < nbApplication; j++)
        {
          // create application
          ftpApplication[ftpApplicationID].SetSource (gw);
          ftpApplication[ftpApplicationID].SetDestination (ue);
          ftpApplication[ftpApplicationID].SetApplicationID (applicationID);
          ftpApplication[ftpApplicationID].SetStartTime(start_time);
          ftpApplication[ftpApplicationID].SetStopTime(duration_time);
//          ftpApplication[ftpApplicationID].SetAverageInterval(packetInterval);
//          ftpApplication[ftpApplicationID].SetSize(packetSize*1024*1024);


          // create qos parameters
          QoSParameters *qosParameters = new QoSParameters ();
          ftpApplication[ftpApplicationID].SetQoSParameters (qosParameters);


          //create classifier parameters
          ClassifierParameters *cp = new ClassifierParameters (gw->GetIDNetworkNode(),
              ue->GetIDNetworkNode(),
              0,
              destinationPort,
              TransportProtocol::TRANSPORT_PROTOCOL_TYPE_UDP);
          ftpApplication[ftpApplicationID].SetClassifierParameters (cp);

          cout << "CREATED BE APPLICATION, ID " << applicationID << endl;

          //update counter
          destinationPort++;
          applicationID++;
          ftpApplicationID++;
        }
      idUE++;
    }

  simulator->SetStop(duration);
  simulator->Run ();
}

