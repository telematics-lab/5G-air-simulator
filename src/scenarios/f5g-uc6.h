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
#include "../flows/application/FTP2.h"
#include "../flows/application/TraceBased.h"
#include "../flows/application/ExternalSource.h"
#include "../device/IPClassifier/ClassifierParameters.h"
#include "../device/MulticastDestination.h"
#include "../flows/QoS/QoSParameters.h"
#include "../flows/QoS/QoSForEXP.h"
#include "../flows/QoS/QoSForFLS.h"
#include "../protocolStack/mac/harq-manager.h"
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


static void f5g_BroadcastServices (int argc, char *argv[])
{
  string environment(argv[2]); // "suburban" or "rural"
  double isd = atof(argv[3]); // in km
  int userDensity = atoi(argv[4]);
  int mbsfnPattern = atoi(argv[5]);
  double duration = atof(argv[6]);
  int mcs = atoi(argv[7]);
  int use_harq = atoi(argv[8]);
  int seed;
  if (argc==10)
    {
      seed = atoi(argv[9]);
    }
  else
    {
      seed = -1;
    }

  double minDist = 0.025;     // in km
  double bandwidth = 20;  // in MHz
  int ueRx = 8;
  int gnbTx;
  int txMode;
  if (use_harq==0)
    {
      // assume simpler transmission to speed up simulation
      gnbTx = 1;
      txMode = 1;
    }
  else
    {
      gnbTx = 8;
      txMode = 9;
    }
  double txPower = 46; // dBm

  double pLossMean = 20; // dB - check
  double pLossStdDev = 0;
  int etilt;

  double vBeamWidth = 65;
  double antennaGain = 15; // dBi
  double avgBuildingHeight = 30; // m
  double antennaHeight = avgBuildingHeight + 15; // m
  double antennaAttenuation = 20; // dB
  double BSFeederLoss = 2; // dB
  double carrierFreq;
  int videoBitRate = 17000; // kbps
  //int videoBitRate = 440; // kbps
  double maxDelay = 0.1; // s
  int nbActiveCell = 1; // number of sites with active users
  double flow_duration = duration-1;
  bool handoverEnabled = true;
  Mobility::MobilityModel mobilityModel = Mobility::CONSTANT_POSITION; //Mobility::RANDOM_DIRECTION;

  int nbCell = 37;
  int nbSector = 3;

  bool create_be=true;
  bool create_mc=true;

  if(environment=="suburban" || environment=="urban")
    {
      etilt = 14;
      carrierFreq = 2000;
    }
  else if(environment=="rural")
    {
      etilt = 10;
      carrierFreq = 800;
    }
  else
    {
      cout << "ERROR: invalid environment selected" << endl;
      exit(EXIT_FAILURE);
    }
  //vector<double> speed_thresholds = {0.0, 0.65, 0.8, 0.95, 0.975};
  //vector<double> speed_values = {0, 30, 120, 250, 500};
  vector<double> speed_thresholds = {0.0, 0.65, 0.975};
  vector<double> speed_values = {3, 30, 120};

  // define cell radius from inter site distance
  double radius = isd / sqrt(3);

  // define channel model
  ChannelRealization::ChannelModel model = ChannelRealization::CHANNEL_MODEL_MACROCELL_RURAL;

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
      srand (time(NULL));
    }
  cout << "Simulation with SEED = " << seed << endl;

  // SET SCHEDULING ALLOCATION SCHEME
  GNodeB::DLSchedulerType downlink_scheduler_type = GNodeB::DLScheduler_TYPE_ROUND_ROBIN;

  // SET FRAME STRUCTURE
  frameManager->SetFrameStructure(FrameManager::FRAME_STRUCTURE_FDD);
  frameManager->SetMbsfnPattern(mbsfnPattern);

  //Create GW
  Gateway *gw = new Gateway ();
  networkManager->GetGatewayContainer ()->push_back (gw);

  //Define Application Container
  TraceBased MCApplication;
  int destinationPort = 101;

  int idUE = nbCell*nbSector;
  int idMulticast = 10000;
  MulticastDestination* mc = NULL;

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
                << " " << c->GetCellCenterPosition ()->GetCoordinateY () << endl;
    }

  for (int i = 0; i < nbCell; i++)
    {
      for (int j=0; j<nbSector; j++)
        {

          // CREATE CHANNELS and propagation loss model
          RadioChannel *dlCh = new RadioChannel ();
          RadioChannel *dlMcCh = new RadioChannel ();
          RadioChannel *ulCh = new RadioChannel ();
          dlCh->SetChannelId (1000+(i*nbSector+j));
          ulCh->SetChannelId (2000+(i*nbSector+j));
          dlMcCh->SetChannelId (3000+(i*nbSector+j));
          
          // CREATE SPECTRUM
          BandwidthManager* spectrum = new BandwidthManager (bandwidth, bandwidth, 0, 0);

          //Create GNodeB
          GNodeB* gnb = new GNodeB (i*nbSector+j, cells->at (i)/*, 0, 0*/);
          gnb->GetPhy ()->SetDlChannel (dlCh);
          gnb->GetPhy ()->SetDlMcChannel (dlMcCh);
          gnb->GetPhy ()->SetUlChannel (ulCh);
          gnb->GetPhy ()->SetNoiseFigure(5);
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
          else if(nbSector==3)
            {
              gnb->GetPhy ()->GetAntennaParameters ()->SetType(Phy::AntennaParameters::ANTENNA_TYPE_TRI_SECTOR);
            }
          else
            {
              cout << "Error: unsupported number of cell sectors (" << nbSector << ")" <<endl;
              exit(1);
            }
          gnb->GetPhy ()->GetAntennaParameters ()->SetEtilt(etilt);
          gnb->GetPhy ()->GetAntennaParameters ()->SetVerticalBeamwidth3db(vBeamWidth);
          gnb->GetPhy ()->GetAntennaParameters ()->SetGain(antennaGain);
          gnb->GetPhy ()->GetAntennaParameters ()->SetMaxHorizontalAttenuation(antennaAttenuation);
          gnb->GetPhy ()->GetAntennaParameters ()->SetBearing(120*j);
          gnb->GetPhy ()->GetAntennaParameters ()->SetFeederLoss(BSFeederLoss);
          gnb->GetMacEntity ()->SetDefaultDlTxMode(txMode);
          ulCh->AddDevice (gnb);
          if (gnb->GetCell()->GetIdCell() < nbActiveCell)
          //if (gnb->GetCell()->GetIdCell() == 0 || gnb->GetCell()->GetIdCell() == 1 || gnb->GetCell()->GetIdCell() == 7)
            {
              gnb->SetDLScheduler (downlink_scheduler_type);
            }
          networkManager->GetGNodeBContainer ()->push_back (gnb);

          if (mc == NULL && create_mc)
            {
              mc = new MulticastDestination(idMulticast,cells->at (i),gnb);
              mc -> SetMcs(mcs);
              gnb->RegisterUserEquipment (mc);
              mc->GetTargetNodeRecord()->SetDlTxMode(1);
              mc->GetPhy ()->SetRxAntennas(ueRx);
              WidebandCqiManager *cqiManager = new WidebandCqiManager ();
              //FullbandCqiManager *cqiManager = new FullbandCqiManager ();
              cqiManager->SetCqiReportingMode (CqiManager::PERIODIC);
              cqiManager->SetReportingInterval (40);
              cqiManager->SetDevice (mc);
              mc->SetCqiManager (cqiManager);

              // create multicast application
              double start_time = 0.05;
              double duration_time = start_time + flow_duration;
              MCApplication.SetDestination (mc);
              MCApplication.SetApplicationID (idMulticast);
              MCApplication.SetStartTime(start_time);
              MCApplication.SetStopTime(duration_time);

              switch (videoBitRate)
                {
                  case 128:
                  {
                    MCApplication.LoadInternalTrace(&foreman_h264_128k);
                    cout << " selected video @ 128k " << endl;
                    MCApplication.SetBurstSize(11569);
                    break;
                  }
                  case 242:
                  {
                    MCApplication.LoadInternalTrace(&foreman_h264_242k);
                    cout << " selected video @ 242k"<< endl;
                    MCApplication.SetBurstSize(14608);
                    break;
                  }
                  case 440:
                  {
                    MCApplication.LoadInternalTrace(&foreman_h264_440k);
                    cout << " selected video @ 440k"<< endl;
                    MCApplication.SetBurstSize(22563);
                    break;
                  }
                  case 8000:
                  {
                    MCApplication.LoadInternalTrace(&trailer_h264_8000k);
                    cout << " selected video @ 8000k"<< endl;
                    MCApplication.SetBurstSize(232708);
                    break;
                  }
                  case 17000:
                  {
                    MCApplication.LoadInternalTrace(&trailer_h264_17000k);
                    cout << " selected video @ 17000k"<< endl;
                    MCApplication.SetBurstSize(494505);
                    break;
                  }
                  default:
                  {
                    cout << "Error: cannot find video trace!"<< endl;
                    exit(1);
                    break;
                  }
                }

              //QoSParameters *qosParameters = new QoSParameters ();
              QoSParameters *qosParameters = new QoSForFLS ();
              qosParameters->SetMaxDelay (maxDelay);
              MCApplication.SetQoSParameters (qosParameters);

              ClassifierParameters *cp = new ClassifierParameters (gw->GetIDNetworkNode(),
                  mc->GetIDNetworkNode(),
                  0,
                  destinationPort,
                  TransportProtocol::TRANSPORT_PROTOCOL_TYPE_UDP);
              MCApplication.SetClassifierParameters (cp);

              MCApplication.SetSource (gw);

              idMulticast++;
            }
          if (gnb->GetCell()->GetIdCell() < nbActiveCell && create_mc)
          //if ((gnb->GetCell()->GetIdCell() == 0 || gnb->GetCell()->GetIdCell() == 1 || gnb->GetCell()->GetIdCell() == 7) && create_mc)
            {
              gnb->RegisterUserEquipment(mc);
              mc->AddSource(gnb);
              MCApplication.AddTransmittingNode(gnb);
            }
        }
    }


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
    else
    {
        cout << "Error: unsupported number of active cells" <<endl;
        exit(1);
    }

  double area = ue_drop_radius * ue_drop_radius * M_PI;
  int nbUE = round( area * userDensity );
  cout << "Creating " << nbUE << " user terminals" << endl;

  //Create multicast UEs
  if(create_mc)
    for (int n = 0; n < nbUE; n++)
      {
        double posX, posY;
        do
          {
            posX = ((2*ue_drop_radius*1000)*((double)rand()/RAND_MAX)) - ue_drop_radius*1000;
            posY = ((2*ue_drop_radius*1000)*((double)rand()/RAND_MAX)) - ue_drop_radius*1000;
          }
        while (sqrt(posX*posX + posY*posY)>ue_drop_radius*1000);

        int speed = speed_values.at(0);
        for(int i=0; i<(int)speed_thresholds.size(); i++)
          {
            if(n >= speed_thresholds.at(i)*nbUE)
              {
                speed = speed_values.at(i);
              }
          }
        double speedDirection = (double)(rand() %360) * ((2*M_PI)/360);

        GNodeB* gnb = networkManager->GetGNodeBContainer ()->at(0);

        UserEquipment* ue = new UserEquipment (idUE,
                                               posX, posY, speed, speedDirection,
                                               cells->at (0),
                                               gnb,
                                               handoverEnabled,
                                               mobilityModel
                                              );

        cout << "Created UE - id " << idUE << " position " << posX << " " << posY << endl;

        Simulator::Init()->Schedule(0.005, &Mobility::SetHandover, ue->GetMobilityModel(), false);
        if(use_harq==0)
          {
            delete ue->GetMacEntity()->GetHarqManager();
            ue->GetMacEntity()->SetHarqManager(NULL);
          }
        RadioChannel* dlCh = gnb->GetPhy ()->GetDlChannel ();
        RadioChannel* ulCh = gnb->GetPhy ()->GetUlChannel ();
        RadioChannel* dlMcCh = gnb->GetPhy ()->GetDlMcChannel ();

        ue->GetPhy ()->SetDlChannel (dlCh);
        dlCh->AddDevice(ue);
        ue->GetPhy ()->SetUlChannel (ulCh);
        ulCh->AddDevice(ue);
        ue->GetPhy ()->SetDlMcChannel (dlMcCh);
        dlMcCh->AddDevice(ue);

        ue->GetPhy ()->SetNoiseFigure(9);
        ue->GetPhy ()->SetRxAntennas(ueRx);

        WidebandCqiManager *cqiManager = new WidebandCqiManager ();
        //FullbandCqiManager *cqiManager = new FullbandCqiManager ();
        cqiManager->SetCqiReportingMode (CqiManager::PERIODIC);
        cqiManager->SetReportingInterval (5);
        cqiManager->SetDevice (ue);
        ue->SetCqiManager (cqiManager);

        WidebandCqiEesmErrorModel *errorModel = new WidebandCqiEesmErrorModel ();
        ue->GetPhy ()->SetErrorModel (errorModel);

        networkManager->GetUserEquipmentContainer ()->push_back (ue);
        ue->SetMulticastDestination (mc);

        mc->AddDestination(ue);
        // register ue to the gnb
        gnb->RegisterUserEquipment (ue);

        //ExternalSource Creation
        ExternalSource* es = new ExternalSource();
        int ExternalSourceID = 5;
        double start_time = 0.01;
        es->SetSource (gw);
        es->SetDestination(ue);
        es->SetApplicationID (ExternalSourceID);
        es->SetStartTime(start_time);

        // define the channel realizations
        GNodeB* prevGnb = NULL;
        ChannelRealization *prev_c_dl, *prev_c_ul, *prev_c_mcdl;
        for ( auto gnb : *networkManager->GetGNodeBContainer () )
          {
            ChannelRealization* c_dl = new ChannelRealization (gnb, ue, model, false);
            gnb->GetPhy ()->GetDlChannel ()->GetPropagationLossModel ()->AddChannelRealization (c_dl);
            ChannelRealization* c_mcdl = new ChannelRealization (gnb, ue, model, true);
            gnb->GetPhy ()->GetDlMcChannel ()->GetPropagationLossModel ()->AddChannelRealization (c_mcdl);
            ChannelRealization* c_ul = new ChannelRealization (ue, gnb, model, false);
            gnb->GetPhy ()->GetUlChannel ()->GetPropagationLossModel ()->AddChannelRealization (c_ul);

            c_dl->SetPenetrationLossMean(pLossMean);
            c_dl->SetPenetrationLossStdDev(pLossStdDev);
            c_mcdl->SetPenetrationLossMean(pLossMean);
            c_mcdl->SetPenetrationLossStdDev(pLossStdDev);
            c_ul->SetPenetrationLossMean(pLossMean);
            c_ul->SetPenetrationLossStdDev(pLossStdDev);

            // syncronize LoS state and shadowing for gNBs at the same site
            if (prevGnb != NULL && gnb->GetCell()->GetIdCell() == prevGnb->GetCell()->GetIdCell())
              {
                c_dl->SetLoSState(prev_c_dl->GetLoSState());
                c_mcdl->SetLoSState(prev_c_mcdl->GetLoSState());
                c_ul->SetLoSState(prev_c_ul->GetLoSState());
                c_dl->SetShadowing(prev_c_dl->GetShadowing());
                c_mcdl->SetShadowing(prev_c_mcdl->GetShadowing());
                c_ul->SetShadowing(prev_c_ul->GetShadowing());
              }
            prevGnb = gnb;
            prev_c_dl = c_dl;
            prev_c_mcdl = c_mcdl;
            prev_c_ul = c_ul;
          }

        idUE++;

      }

  simulator->SetStop(duration);
  simulator->Run ();

}

