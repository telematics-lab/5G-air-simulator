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
#include "../core/eventScheduler/simulator.h"
#include "../flows/application/InfiniteBuffer.h"
#include "../flows/application/VoIP.h"
#include "../flows/application/CBR.h"
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


static void TestMultiCellTriSector (int argc, char *argv[])
{
  double radius = atof(argv[2]);
  int nbUE = atoi(argv[3]);
  int sched_type = atoi(argv[4]);
  int speed = atoi(argv[5]);
  double bandwidth = atof(argv[6]);
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

  // define simulation times
  double flow_duration = duration;

  int nbBE = 1;

  int frame_struct = 1;

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
  FrameManager::FrameStructure frame_structure;
  switch (frame_struct)
     {
    case 1:
      frame_structure = FrameManager::FRAME_STRUCTURE_FDD;
      break;
    case 2:
      frame_structure = FrameManager::FRAME_STRUCTURE_TDD;
      break;
    default:
      cout << "Error: invalid frame structure specified!" << endl;
      exit(1);
    }
  frameManager->SetFrameStructure(frame_structure);


  int nbCell = 19;
  int nbSector = 3;

  //Define Application Container
  InfiniteBuffer BEApplication[nbBE*nbSector*nbUE];
  int beApplication = 0;
  int destinationPort = 101;
  int applicationID = 0;


  int idUE = nbCell*nbSector;

  // CREATE CELLS
  vector <Cell*> *cells = new vector <Cell*>;
  for (int i = 0; i < nbCell; i++)
    {
      CartesianCoordinates center =
        GetCartesianCoordinatesForCell(i, radius * 1000.);

      Cell *c = new Cell (i, radius, 0.035, center.GetCoordinateX (), center.GetCoordinateY ());
      cells->push_back (c);
      networkManager->GetCellContainer ()->push_back (c);

      cout << "Created Cell, id " << c->GetIdCell ()
                <<", position: " << c->GetCellCenterPosition ()->GetCoordinateX ()
                << " " << c->GetCellCenterPosition ()->GetCoordinateY () << endl;
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
          GNodeB* gnb = new GNodeB (i*3+j, cells->at (i)/*, 0, 0*/);
          gnb->GetPhy ()->SetDlChannel (dlCh);
          gnb->GetPhy ()->SetUlChannel (ulCh);
          gnb->GetPhy ()->SetTxAntennas(gnbTx);
          gnb->GetPhy ()->SetBandwidthManager (spectrum);
          gnb->GetPhy ()->GetAntennaParameters ()->SetType(Phy::AntennaParameters::ANTENNA_TYPE_TRI_SECTOR);
          gnb->GetPhy ()->GetAntennaParameters ()->SetGain(17);
          gnb->GetPhy ()->GetAntennaParameters ()->SetBearing(120*j);
          gnb->GetMacEntity ()->SetDefaultDlTxMode(txMode);
          ulCh->AddDevice (gnb);
          gnb->SetDLScheduler (downlink_scheduler_type);
          networkManager->GetGNodeBContainer ()->push_back (gnb);

        }
    }

  //Create GW
  Gateway *gw = new Gateway ();
  networkManager->GetGatewayContainer ()->push_back (gw);


  //Create UEs
  for (int n = 0; n < nbUE; n++)
    {
      //ue's random position
      int maxXY = radius * 1000; // in metres
      double posX = (double)rand()/RAND_MAX;
      posX = 0.95 *
             (((2*radius*1000)*posX) - (radius*1000));
      double posY = (double)rand()/RAND_MAX;
      posY = 0.95 *
             (((2*radius*1000)*posY) - (radius*1000));
      double speedDirection = (double)(rand() %360) * ((2*M_PI)/360);

      GNodeB* gnb = networkManager->GetGNodeBContainer ()->at(0);

      UserEquipment* ue = new UserEquipment (idUE,
                                             posX, posY, speed, speedDirection,
                                             cells->at (0),
                                             gnb,
                                             1,
                                             Mobility::RANDOM_DIRECTION);

      cout << "Created UE - id " << idUE << " position " << posX << " " << posY << endl;

      RadioChannel* dlCh = gnb->GetPhy ()->GetDlChannel ();
      RadioChannel* ulCh = gnb->GetPhy ()->GetUlChannel ();

      ue->GetPhy ()->SetDlChannel (dlCh);
      ue->GetPhy ()->SetUlChannel (ulCh);
      ue->GetPhy ()->SetRxAntennas(ueRx);

      FullbandCqiManager *cqiManager = new FullbandCqiManager ();
      cqiManager->SetCqiReportingMode (CqiManager::PERIODIC);
      cqiManager->SetReportingInterval (1);
      cqiManager->SetDevice (ue);
      ue->SetCqiManager (cqiManager);

      WidebandCqiEesmErrorModel *errorModel = new WidebandCqiEesmErrorModel ();
      ue->GetPhy ()->SetErrorModel (errorModel);

      networkManager->GetUserEquipmentContainer ()->push_back (ue);

      // register ue to the gnb
      gnb->RegisterUserEquipment (ue);
      // define the channel realization
      ChannelRealization* c_dl = new ChannelRealization (gnb, ue, ChannelRealization::CHANNEL_MODEL_MACROCELL_URBAN);
      gnb->GetPhy ()->GetDlChannel ()->GetPropagationLossModel ()->AddChannelRealization (c_dl);
      ChannelRealization* c_ul = new ChannelRealization (ue, gnb, ChannelRealization::CHANNEL_MODEL_MACROCELL_URBAN);
      gnb->GetPhy ()->GetUlChannel ()->GetPropagationLossModel ()->AddChannelRealization (c_ul);


      // CREATE DOWNLINK APPLICATION FOR THIS UE
      double start_time = 0.001*idUE + 0.01;
      double duration_time = start_time + flow_duration;

      // *** be application
      for (int j = 0; j < nbBE; j++)
        {
          // create application
          BEApplication[beApplication].SetSource (gw);
          BEApplication[beApplication].SetDestination (ue);
          BEApplication[beApplication].SetApplicationID (applicationID);
          BEApplication[beApplication].SetStartTime(start_time);
          BEApplication[beApplication].SetStopTime(duration_time);


          // create qos parameters
          QoSParameters *qosParameters = new QoSParameters ();
          BEApplication[beApplication].SetQoSParameters (qosParameters);


          //create classifier parameters
          ClassifierParameters *cp = new ClassifierParameters (gw->GetIDNetworkNode(),
              ue->GetIDNetworkNode(),
              0,
              destinationPort,
              TransportProtocol::TRANSPORT_PROTOCOL_TYPE_UDP);
          BEApplication[beApplication].SetClassifierParameters (cp);

          cout << "CREATED BE APPLICATION, ID " << applicationID << endl;

          //update counter
          destinationPort++;
          applicationID++;
          beApplication++;
        }


      idUE++;

    }

  simulator->SetStop(duration);
  simulator->Run ();

}

