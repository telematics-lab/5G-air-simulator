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
#include "../device/IPClassifier/ClassifierParameters.h"
#include "../device/MulticastDestination.h"
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


static void TestMbsfn (int argc, char *argv[])
{
  double isd = atof(argv[2]);
  double minDist = atof(argv[3]);
  double carrierFreq = atof(argv[4]);
  int nbUE = atoi(argv[5]);
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
  double antennaAttenuation = atof(argv[20]);
  double BSFeederLoss = atof(argv[21]);
  int channelModel = atoi(argv[22]);
  int videoBitRate = atoi(argv[23]);
  double maxDelay = atof(argv[24]);
  int mbsfnPattern = atoi(argv[25]);
  int seed;
  if (argc==27)
    {
      seed = atoi(argv[26]);
    }
  else
    {
      seed = -1;
    }

  nbUE = nbUE * 4;

  // define simulation times
  double flow_duration = duration-1;

  int nbCell = 37;
  int nbSector = 1;

  bool create_be=true;
  bool create_mc=true;

  int nbBE = 1;

  int frame_struct = 1; //FDD

  // define cell radius from inter site distance
  double radius = isd / sqrt(3);

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
  frameManager->SetMbsfnPattern(mbsfnPattern);

  //Create GW
  Gateway *gw = new Gateway ();
  networkManager->GetGatewayContainer ()->push_back (gw);

  //Define Application Container
  InfiniteBuffer BEApplication[nbBE*nbSector*nbUE];
  TraceBased MCApplication;
//FTP2 BEApplication[nbBE*nbSector*nbUE];
  int beApplication = 0;
  int destinationPort = 101;
  int applicationID = 0;

  int idUE = nbCell*nbSector;
  int idMulticast = 10000;
  MulticastDestination* mc;
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
          RadioChannel *dlMcCh = new RadioChannel ();
          RadioChannel *ulCh = new RadioChannel ();


          // CREATE SPECTRUM
          BandwidthManager* spectrum = new BandwidthManager (bandwidth, bandwidth, 0, 0);



          //Create GNodeB
          GNodeB* gnb = new GNodeB (i*3+j, cells->at (i)/*, 0, 0*/);
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
          gnb->GetMacEntity ()->SetDefaultDlTxMode(txMode);
          ulCh->AddDevice (gnb);
          if (gnb->GetCell()->GetIdCell() < 1)
            gnb->SetDLScheduler (downlink_scheduler_type);
          networkManager->GetGNodeBContainer ()->push_back (gnb);

          if (gnb->GetCell()->GetIdCell() < 1 && create_mc)
            {
              mc = new MulticastDestination(idMulticast,cells->at (i),gnb);
              gnb->RegisterUserEquipment (mc);
              mc->GetPhy ()->SetRxAntennas(ueRx);
              //WidebandCqiManager *cqiManager = new WidebandCqiManager ();
              FullbandCqiManager *cqiManager = new FullbandCqiManager ();
              cqiManager->SetCqiReportingMode (CqiManager::PERIODIC);
              cqiManager->SetReportingInterval (5);
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
//                    MCApplication.LoadInternalTrace(&highway_h264_128k);
//                    MCApplication.LoadInternalTrace(&mobile_h264_128k);
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
          if (gnb->GetCell()->GetIdCell() == 0 && create_mc)
            {
              gnb->RegisterUserEquipment(mc);
              mc->AddSource(gnb);
              MCApplication.AddTransmittingNode(gnb);
            }
        }
    }
  double ue_drop_radius=radius*2;

  //Create multicast UEs
  if(create_mc)
    for (int n = 0; n < nbUE; n++)
      {
        //ue's random position
        double posX, posY;
        do
          {
            posX = ((2*ue_drop_radius*1000)*((double)rand()/RAND_MAX)) - ue_drop_radius*1000;
            posY = ((2*ue_drop_radius*1000)*((double)rand()/RAND_MAX)) - ue_drop_radius*1000;
          }
        while (sqrt(posX*posX + posY*posY)>ue_drop_radius*1000);
        //linear arrangement
        /* double distance = (isd-2*minDist)*1000/(nbUE-1)*n+minDist;
         double angle = 60/57.2957795;
         double posX = distance * cos(angle);
         double posY = distance * sin(angle);*/

        double speedDirection = (double)(rand() %360) * ((2*M_PI)/360);

        GNodeB* gnb = networkManager->GetGNodeBContainer ()->at(0);

        UserEquipment* ue = new UserEquipment (idUE,
                                               posX, posY, speed, speedDirection,
                                               cells->at (0),
                                               gnb,
                                               1, //handover
                                               //Mobility::RANDOM_DIRECTION
                                               Mobility::CONSTANT_POSITION
                                              );

        cout << "Created UE - id " << idUE << " position " << posX << " " << posY << endl;

        Simulator::Init()->Schedule(0.005, &Mobility::SetHandover, ue->GetMobilityModel(), false);

        RadioChannel* dlCh = gnb->GetPhy ()->GetDlChannel ();
        RadioChannel* dlMcCh = gnb->GetPhy ()->GetDlMcChannel ();
        RadioChannel* ulCh = gnb->GetPhy ()->GetUlChannel ();
        ue->GetPhy ()->SetDlChannel (dlCh);
        dlCh->AddDevice(ue);
        ue->GetPhy ()->SetDlMcChannel (dlMcCh);
        dlMcCh->AddDevice(ue);
        ue->GetPhy ()->SetUlChannel (ulCh);
        ulCh->AddDevice(ue);

        ue->GetPhy ()->SetNoiseFigure(7);
        ue->GetPhy ()->SetRxAntennas(ueRx);

        //WidebandCqiManager *cqiManager = new WidebandCqiManager ();
        FullbandCqiManager *cqiManager = new FullbandCqiManager ();
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
        // define the channel realizations
        GNodeB* prevGnb = nullptr;
        ChannelRealization *prev_c_dl, *prev_c_mcdl, *prev_c_ul;
        for ( auto gnb : *networkManager->GetGNodeBContainer () )
          {
            ChannelRealization* c_dl = new ChannelRealization (gnb, ue, model);
            gnb->GetPhy ()->GetDlChannel ()->GetPropagationLossModel ()->AddChannelRealization (c_dl);
            ChannelRealization* c_mcdl = new ChannelRealization (gnb, ue, model, true);
            gnb->GetPhy ()->GetDlMcChannel ()->GetPropagationLossModel ()->AddChannelRealization (c_mcdl);
            ChannelRealization* c_ul = new ChannelRealization (ue, gnb, model);
            gnb->GetPhy ()->GetUlChannel ()->GetPropagationLossModel ()->AddChannelRealization (c_ul);

            c_dl->SetPenetrationLossMean(pLossMean);
            c_dl->SetPenetrationLossStdDev(pLossStdDev);
            c_mcdl->SetPenetrationLossMean(pLossMean);
            c_mcdl->SetPenetrationLossStdDev(pLossStdDev);
            c_ul->SetPenetrationLossMean(pLossMean);
            c_ul->SetPenetrationLossStdDev(pLossStdDev);

            // syncronize LoS state and shadowing for gNBs at the same site
            if (prevGnb != nullptr && gnb->GetCell()->GetIdCell() == prevGnb->GetCell()->GetIdCell())
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
  //Create BE UEs
  if(create_be)
    for (int n = nbUE; n < nbUE*2; n++)
      {
        //ue's random position
        double posX, posY;
        do
          {
            posX = ((2*ue_drop_radius*1000)*((double)rand()/RAND_MAX)) - ue_drop_radius*1000;
            posY = ((2*ue_drop_radius*1000)*((double)rand()/RAND_MAX)) - ue_drop_radius*1000;
          }
        while (sqrt(posX*posX + posY*posY)>ue_drop_radius*1000);
        //linear arrangement
        /* double distance = (isd-2*minDist)*1000/(nbUE-1)*n+minDist;
         double angle = 60/57.2957795;
         double posX = distance * cos(angle);
         double posY = distance * sin(angle);*/

        double speedDirection = (double)(rand() %360) * ((2*M_PI)/360);

        GNodeB* gnb = networkManager->GetGNodeBContainer ()->at(0);

        UserEquipment* ue = new UserEquipment (idUE,
                                               posX, posY, speed, speedDirection,
                                               cells->at (0),
                                               gnb,
                                               1, //handover
                                               //Mobility::RANDOM_DIRECTION
                                               Mobility::CONSTANT_POSITION
                                              );

        cout << "Created UE - id " << idUE << " position " << posX << " " << posY << endl;

        Simulator::Init()->Schedule(0.005, &Mobility::SetHandover, ue->GetMobilityModel(), false);

        RadioChannel* dlCh = gnb->GetPhy ()->GetDlChannel ();
        RadioChannel* dlMcCh = gnb->GetPhy ()->GetDlMcChannel ();
        RadioChannel* ulCh = gnb->GetPhy ()->GetUlChannel ();

        ue->GetPhy ()->SetDlChannel (dlCh);
        dlCh->AddDevice(ue);
        ue->GetPhy ()->SetDlMcChannel (dlCh);
        dlMcCh->AddDevice(ue);
        ue->GetPhy ()->SetUlChannel (ulCh);
        ulCh->AddDevice(ue);

        ue->GetPhy ()->SetNoiseFigure(7);
        ue->GetPhy ()->SetRxAntennas(ueRx);

        FullbandCqiManager *cqiManager = new FullbandCqiManager ();
        cqiManager->SetCqiReportingMode (CqiManager::PERIODIC);
        cqiManager->SetReportingInterval (5);
        cqiManager->SetDevice (ue);
        ue->SetCqiManager (cqiManager);

        WidebandCqiEesmErrorModel *errorModel = new WidebandCqiEesmErrorModel ();
        ue->GetPhy ()->SetErrorModel (errorModel);

        networkManager->GetUserEquipmentContainer ()->push_back (ue);

        //ue->SetMulticastDestination (mc);
        //mc->AddDestination(ue);
        // register ue to the gnb
        gnb->RegisterUserEquipment (ue);
        // define the channel realizations
        GNodeB* prevGnb = nullptr;
        ChannelRealization *prev_c_dl, *prev_c_mcdl, *prev_c_ul;
        for ( auto gnb : *networkManager->GetGNodeBContainer () )
          {
            ChannelRealization* c_dl = new ChannelRealization (gnb, ue, model);
            gnb->GetPhy ()->GetDlChannel ()->GetPropagationLossModel ()->AddChannelRealization (c_dl);
            ChannelRealization* c_mcdl = new ChannelRealization (gnb, ue, model, true);
            gnb->GetPhy ()->GetDlMcChannel ()->GetPropagationLossModel ()->AddChannelRealization (c_mcdl);
            ChannelRealization* c_ul = new ChannelRealization (ue, gnb, model);
            gnb->GetPhy ()->GetUlChannel ()->GetPropagationLossModel ()->AddChannelRealization (c_ul);

            c_dl->SetPenetrationLossMean(pLossMean);
            c_dl->SetPenetrationLossStdDev(pLossStdDev);
            c_mcdl->SetPenetrationLossMean(pLossMean);
            c_mcdl->SetPenetrationLossStdDev(pLossStdDev);
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

        // *** be application
        for (int j = 0; j < nbBE; j++)
          {
            // create application
            BEApplication[beApplication].SetSource (gw);
            BEApplication[beApplication].SetDestination (ue);
            BEApplication[beApplication].SetApplicationID (applicationID);
            BEApplication[beApplication].SetStartTime(start_time);
            BEApplication[beApplication].SetStopTime(duration_time);
//BEApplication[beApplication].SetAverageInterval(3.2);
//BEApplication[beApplication].SetInterval(0.2);
//BEApplication[beApplication].SetSize(2*1024*1024);



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

