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
 * Author: Felice Paparusso <felice.paparusso@gmail.com>
 * Author: Vincenzo Fortunato <vincfort92@hotmail.it>
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


static void ItuCalibration (int argc, char *argv[])
{
  double isd = atof(argv[2]); // Km
  double minDist = atof(argv[3]); // Km
  double carrierFreq = atof(argv[4]);
  int nbUePerCell = atoi(argv[5]);
  int sched_type = atoi(argv[6]);
  int speed = atoi(argv[7]);
  double bandwidth = atof(argv[8]);
  double duration = atof(argv[9]);
  int gnbTx = atoi(argv[10]);
  int ueRx = atoi(argv[11]);
  int txMode = atoi(argv[12]);
  double txPower = atof(argv[13]);
  double pLossMean = atof(argv[14]);
  double pLossStdDev = atof(argv[15]);
  int etilt = atoi(argv[16]);
  double vBeamWidth = atof(argv[17]);
  double antennaGain = atof(argv[18]);
  double antennaHeight = atof(argv[19]);
  double avgBuildingHeight = atof(argv[20]);
  double antennaAttenuation = atof(argv[21]);
  double BSFeederLoss = atof(argv[22]);
  int channelModel = atoi(argv[23]);
  int seed;
  if (argc==25)
    {
      seed = atoi(argv[24]);
    }
  else
    {
      seed = -1;
    }

  // define simulation times
  double flow_duration = duration;

  int cqiReportingInterval = 5; // TTI
  double ueNoiseFigure = 7; // dB
  double gnbNoiseFigure = 5; // dB
  bool allowHandover = true;
  // Mobility::MobilityModel mobility = Mobility::RANDOM_DIRECTION;
  Mobility::MobilityModel mobility = Mobility::CONSTANT_POSITION;
  int nbActiveCells = 7;

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


  int nbCell = 19;
  int nbSector = 3;

  double radius = isd / sqrt(3);

  double cellArea = radius * (isd/2) * 6;
  double userDensity = nbUePerCell / cellArea;

  double ueDropRadius;
  if(nbActiveCells==1)
    {
      ueDropRadius = isd;
    }
  else if(nbActiveCells>1 && nbActiveCells<=7)
    {
      ueDropRadius = 2*isd;
    }
  else if(nbActiveCells>7 && nbActiveCells<=19)
    {
      ueDropRadius = 3*isd;
    }
  double area = ueDropRadius * ueDropRadius * M_PI;

  int nbUE = round( area * userDensity );

  //Define Application Container
  InfiniteBuffer BEApplication[nbUE];
  int beApplication = 0;
  int destinationPort = 101;
  int applicationID = 0;

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
          GNodeB* gnb = new GNodeB (i*3+j, cells->at (i));
          gnb->GetPhy ()->SetDlChannel (dlCh);
          gnb->GetPhy ()->SetUlChannel (ulCh);
          gnb->GetPhy ()->SetNoiseFigure(gnbNoiseFigure);
          gnb->GetPhy ()->SetCarrierFrequency(carrierFreq);
          gnb->GetPhy ()->SetTxAntennas(gnbTx);
          gnb->GetPhy ()->SetTxPower(txPower);
          gnb->GetPhy ()->SetBandwidthManager (spectrum);
          gnb->GetPhy ()->SetHeight(antennaHeight);
          // Set Antenna Parameters
          if(nbSector==1)
            {
              gnb->GetPhy ()->GetAntennaParameters ()->SetType(Phy::AntennaParameters::ANTENNA_TYPE_OMNIDIRECTIONAL);
            }
          else
            {
              gnb->GetPhy ()->GetAntennaParameters ()->SetType(Phy::AntennaParameters::ANTENNA_TYPE_TRI_SECTOR);
            }
          gnb->GetPhy ()->GetAntennaParameters ()->SetEtilt(etilt);
          gnb->GetPhy ()->GetAntennaParameters ()->SetVerticalBeamwidth3db(vBeamWidth);
          gnb->GetPhy ()->GetAntennaParameters ()->SetGain(antennaGain);
          gnb->GetPhy ()->GetAntennaParameters ()->SetMaxHorizontalAttenuation(antennaAttenuation);
          gnb->GetPhy ()->GetAntennaParameters ()->SetBearing(120*j);
          gnb->GetPhy ()->GetAntennaParameters ()->SetFeederLoss(BSFeederLoss);
          gnb->GetPhy ()->SetAverageBuildingHeight(avgBuildingHeight);
          gnb->GetMacEntity ()->SetDefaultDlTxMode(txMode);
          ulCh->AddDevice (gnb);
          if (gnb->GetCell()->GetIdCell() < nbActiveCells)
            {
              gnb->SetDLScheduler (downlink_scheduler_type);
            }
          networkManager->GetGNodeBContainer ()->push_back (gnb);
        }
    }

  //Create GW
  Gateway *gw = new Gateway ();
  networkManager->GetGatewayContainer ()->push_back (gw);

  //Create UEs
  int idUE = nbCell*nbSector;

  for (int n = 0; n < nbUE; n++)
    {
      //ue's random position
      double posX, posY;
      do
        {
          posX = ((2*ueDropRadius*1000)*((double)rand()/RAND_MAX)) - ueDropRadius*1000;
          posY = ((2*ueDropRadius*1000)*((double)rand()/RAND_MAX)) - ueDropRadius*1000;
        }
      while (sqrt(posX*posX + posY*posY)>ueDropRadius*1000);

      double speedDirection = (double)(rand() %360) * ((2*M_PI)/360);

      GNodeB* gnb = networkManager->GetGNodeBContainer ()->at(0);

      UserEquipment* ue = new UserEquipment (idUE,
                                             posX, posY, speed, speedDirection,
                                             cells->at (0),
                                             gnb,
                                             allowHandover,
                                             mobility
                                            );

      cout << "Created UE - id " << idUE << " position " << posX << " " << posY << endl;

      RadioChannel* dlCh = gnb->GetPhy ()->GetDlChannel ();
      RadioChannel* ulCh = gnb->GetPhy ()->GetUlChannel ();

      ue->GetPhy ()->SetDlChannel (dlCh);
      ue->GetPhy ()->SetUlChannel (ulCh);
      ue->GetPhy ()->SetNoiseFigure(ueNoiseFigure);
      ue->GetPhy ()->SetRxAntennas(ueRx);

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

          c_dl->SetPenetrationLossMean(pLossMean);
          c_dl->SetPenetrationLossStdDev(pLossStdDev);
          c_ul->SetPenetrationLossMean(pLossMean);
          c_ul->SetPenetrationLossStdDev(pLossStdDev);

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

      idUE++;

    }

  simulator->SetStop(duration);
  simulator->Run ();

}

