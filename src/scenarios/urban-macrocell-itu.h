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
#include "../device/IPClassifier/ClassifierParameters.h"
#include "../flows/QoS/QoSParameters.h"
#include "../componentManagers/FrameManager.h"
#include "../load-parameters.h"
#include <iostream>
#include <queue>
#include <fstream>
#include <stdlib.h>
#include <cstring>


static void UrbanMacrocellItu (int argc, char *argv[])
{
  int nbUePerCell = atoi(argv[2]);
  double duration = atof(argv[3]);
  int seed;
  if (argc==5)
    {
      seed = atoi(argv[4]);
    }
  else
    {
      seed = -1;
    }

  double isd = 0.5; // in km
  double minDist = 0.025; // in km
  double carrierFreq = 2000; //in MHz
  int speed = 30; // kh/h
  double bandwidth = 10; // MHz
  int gnbTx = 1;
  int ueRx = 2;
  int txMode = 1;
  double txPower = 46; // dBm
  double pLossMean = 9; // dB
  double pLossStdDev = 5; // dB
  int etilt = 12; // degrees
  double vBeamWidth = 15; // degrees
  double antennaGain = 17; // dBi
  double antennaHeight = 25; // m
  double avgBuildingHeight = 20; // m
  double antennaAttenuation = 20; // dB
  double BSFeederLoss = 2; // dB
  double flow_duration = duration;
  int cqiReportingInterval = 5; // TTI
  double ueNoiseFigure = 7; // dB
  double gnbNoiseFigure = 5; // dB
  bool allowHandover = true;
  // Mobility::MobilityModel mobility = Mobility::RANDOM_DIRECTION;
  Mobility::MobilityModel mobility = Mobility::CONSTANT_POSITION;
  int nbActiveCells = 7;

  // define channel model
  ChannelRealization::ChannelModel model = ChannelRealization::CHANNEL_MODEL_MACROCELL_URBAN_IMT;

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
  GNodeB::DLSchedulerType downlink_scheduler_type = GNodeB::DLScheduler_TYPE_ROUND_ROBIN;
  cout << "Scheduler RR "<< endl;

  // SET FRAME STRUCTURE
  frameManager->SetFrameStructure(FrameManager::FRAME_STRUCTURE_FDD);

  int nbCell = 19;
  int nbSector = 3;
  double radius = isd / sqrt(3);

  double cellArea = (radius/2) * (isd/2) * 6;
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

