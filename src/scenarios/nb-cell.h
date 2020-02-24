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
 * Author: Telematics Lab <telematics-dev@poliba.it>
 * Author: Sergio Martiradonna <sergio.martiradonna@poliba.it>
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
#include "../channel/propagation-model/channel-realization.h"
#include "../phy/wideband-cqi-eesm-error-model.h"
#include "../phy/simple-error-model.h"
#include "../load-parameters.h"

#include "../device/UserEquipment.h"
#include "../device/NetworkNode.h"
#include "../device/NetworkNode.h"
#include "../device/GNodeB.h"
#include "../device/CqiManager/cqi-manager.h"
#include "../device/CqiManager/fullband-cqi-manager.h"
#include "../device/CqiManager/wideband-cqi-manager.h"
#include "../channel/propagation-model/propagation-loss-model.h"

#include "../protocolStack/mac/random-access/ue-random-access.h"
#include "../protocolStack/mac/random-access/gnb-random-access.h"
#include "../protocolStack/mac/random-access/enb-nb-iot-random-access.h"
#include "../protocolStack/mac/random-access/ue-nb-iot-random-access.h"

#include <iostream>
#include <queue>
#include <fstream>
#include <stdlib.h>
#include <cstring>
#include <math.h>


static void nbCell (int argc, char *argv[])
{
  int schedUL = atoi(argv[2]); 
  double dur = atoi(argv[3]);
  double radius = atof(argv[4]);
  //cout<< "Radius = " << radius << endl;
  int nbUE = atoi(argv[5]);
  double bandwidth = atof(argv[6]);
  int carriers = atoi(argv[7]);
  double spacing = atof(argv[8]);
  int tones = atoi(argv[9]);
  int CBR_interval = atoi(argv[10]);
  int CBR_size = atoi(argv[11]);
  int totPreambleTx = atoi(argv[12]);
  int nbCE = atoi(argv[13]);
  std::map<double, int> ceProb;
  std::map<int, int> maxPreambleTx;
  std::map<int, int> preambleRep;
  std::map<int, int> rarWindow;
  std::map<int, int> nbPre;
  std::map<int, int> rachPeriod;
  std::map<int, int> rachOffset;
  std::map<int, int> boWindow;

  for (int i=0; i<nbCE; i++)
  {
    if (i==nbCE-1)
      ceProb.insert(std::make_pair(1, i));
    else if (i==0)
      ceProb.insert(std::make_pair(atof(argv[14+i])/100,i));
    else
      ceProb.insert(std::make_pair((atof(argv[14+i])/100) + (atof(argv[13+i])/100), i));
    maxPreambleTx.insert(std::make_pair(i, atoi(argv[14+nbCE+i])));
    preambleRep.insert(std::make_pair(i, atoi(argv[14+(nbCE*2)+i])));
    rarWindow.insert(std::make_pair(i, atoi(argv[14+(nbCE*3)+i])));
    nbPre.insert(std::make_pair(i, atoi(argv[14+(nbCE*4)+i])));
    rachPeriod.insert(std::make_pair(i, atoi(argv[14+(nbCE*5)+i])));
    rachOffset.insert(std::make_pair(i, atoi(argv[14+(nbCE*6)+i])));
    boWindow.insert(std::make_pair(i, atoi(argv[14+(nbCE*7)+i])));
  }
  int seed;
  if (argc==15+(nbCE*8))
    {
      seed = atoi(argv[14+(nbCE*8)]);
    }
  else
    {
      seed = -1;
    }

    for(auto it = ceProb.cbegin(); it != ceProb.cend(); ++it)
    {
        std::cout << it->first << " " << it->second << "\n";
    }


  // define simulation times
  double duration = dur; //+ 1;
  double flow_duration = duration;

  UeRandomAccess::RandomAccessType m_UeRandomAccessType = UeRandomAccess::RA_TYPE_NB_IOT;
  GnbRandomAccess::RandomAccessType m_GnbRandomAccessType = GnbRandomAccess::RA_TYPE_NB_IOT;

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

  std::mt19937 gen(seed>=0 ? seed : time(NULL));

  cout << "Simulation with SEED = " << seed << endl;
  cout << "Duration: " << duration << " flow: " << flow_duration << endl;

  // SET FRAME STRUCTURE
  frameManager->SetFrameStructure(FrameManager::FRAME_STRUCTURE_FDD);


  //Define Application Container
  CBR CBRApplication[nbUE];
  int cbrApplication = 0;
  int destinationPort = 101;
  int applicationID = 0;


  // CREATE CELL
  Cell *cell = new Cell (0, radius, 0.005, 0, 0);

  networkManager->GetCellContainer ()->push_back (cell);

  // CREATE CHANNELS and propagation loss model
  RadioChannel *dlCh = new RadioChannel ();
  RadioChannel *ulCh = new RadioChannel ();

  // CREATE SPECTRUM
  BandwidthManager* spectrum = new BandwidthManager (bandwidth, bandwidth, 0, 0);
  spectrum->CreateNbIoTspectrum(carriers, spacing, tones);


  cout << "TTI Length: ";
  frameManager->setTTILength(tones, spacing);
  cout << frameManager->getTTILength() << "ms " << endl;

DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG_NB)
  spectrum->Print();
DEBUG_LOG_END


  GNodeB::ULSchedulerType uplink_scheduler_type;
  switch (schedUL)
    {
    case 0:
      uplink_scheduler_type = GNodeB::ULScheduler_TYPE_NB_IOT_FIFO;
      cout << "Scheduler NB FIFO "<< endl;
      break;
    case 1:
      uplink_scheduler_type = GNodeB::ULScheduler_TYPE_NB_IOT_ROUNDROBIN;
      cout << "Scheduler NB RR "<< endl;
      break;
    default:
      uplink_scheduler_type = GNodeB::ULScheduler_TYPE_NB_IOT_FIFO;
      cout << "Scheduler NB FIFO "<< endl;
      break;
    }

  //Create GNodeB
  GNodeB* gnb = new GNodeB (1, cell, 0, 0);
  gnb->SetRandomAccessType(m_GnbRandomAccessType);
  gnb->GetPhy ()->SetDlChannel (dlCh);
  gnb->GetPhy ()->SetUlChannel (ulCh);
  gnb->GetPhy ()->SetBandwidthManager (spectrum);
  ulCh->AddDevice (gnb);
  gnb->SetDLScheduler (GNodeB::DLScheduler_TYPE_PROPORTIONAL_FAIR);
  gnb->SetULScheduler(uplink_scheduler_type);

  networkManager->GetGNodeBContainer ()->push_back (gnb);
  cout << "Created gNB - id 1 position (0;0)"<< endl;

  GnbNbIoTRandomAccess* gnbRam = (GnbNbIoTRandomAccess*) gnb->GetMacEntity()->GetRandomAccessManager() ;

  gnbRam->SetMaxPreambleTx(maxPreambleTx);
  gnbRam->SetPreambleRep(preambleRep);
  gnbRam->SetRarWindow(rarWindow);
  gnbRam->SetSubcarriers(nbPre);
  gnbRam->SetResPeriodicity(rachPeriod);
  gnbRam->SetTimeOffset(rachOffset);
  gnbRam->SetBackoff(boWindow);

  //Create UEs
  int idUE = 2;
  double speedDirection = 0;

  double posX = 0;
  double posY = 0;

  int nbOfZones;
  if (tones==1)
    {
      nbOfZones=11;
    }
  else
    {
      nbOfZones=14;
    }
  int zone;
  double zoneWidth = radius*1000 / nbOfZones;
  double edges[nbOfZones+1];
  for (int i=0; i <= nbOfZones; i++)
    {
      edges[i]= i*zoneWidth;
    }

  int high, low, sign;
  double distance;

  std::uniform_real_distribution<> spaDis(0.0, zoneWidth);
  std::uniform_int_distribution<> zoneDis(0, nbOfZones-1);
  std::uniform_int_distribution<> sig(1, 2);
  std::uniform_real_distribution<> timeDis(0.0, (double) CBR_interval);

  cout << "CBR interval is " << (double) CBR_interval << endl;

  for (int i = 0; i < nbUE; i++)
    {
      zone = zoneDis(gen);
      cout << "ZONE " << zone;
      low = edges[nbOfZones - 1 - zone];
      cout << " LOW EDGE " << low;

      distance = spaDis(gen) + (double) low;

      cout << " DISTANCE " << distance;
      cout << endl;

      sign = (sig(gen) % 2) * 2 - 1;
      posX=distance / sqrt(2) * sign;
      sign = (sig(gen) % 2) * 2 - 1;
      posY=distance / sqrt(2) * sign;

      UserEquipment* ue = new UserEquipment (idUE,
                                             posX, posY, 0, speedDirection,
                                             cell,
                                             gnb,
                                             0, //handover false!
                                             Mobility::CONSTANT_POSITION);

      cout << "Created UE - id " << idUE << " position " << posX << " " << posY << endl;

      ue->SetRandomAccessType(m_UeRandomAccessType);
      ue->GetPhy ()->SetDlChannel (dlCh);
      ue->GetPhy ()->SetUlChannel (ulCh);



      double propDist=distance/(1000*radius);
      std::map<double, int>::iterator it;
      it = ceProb.upper_bound(propDist);
      UeMacEntity* ueMac = (UeMacEntity*) ue->GetProtocolStack()->GetMacEntity();
      UeNbIoTRandomAccess* ueRam =(UeNbIoTRandomAccess*) ueMac->GetRandomAccessManager();
      ueRam->SetCEClassStatic(it->second);
      ueRam->SetCEClassDynamic(it->second);
      ueRam->SetMaxFailedAttempts(totPreambleTx);

      DEBUG_LOG_START_1(SIM_ENV_CE_CLASS_LOG)
      	  cout << "CE Class is " << ueRam->GetCEClassStatic()<< " and proportional dist is " << propDist << endl;
      DEBUG_LOG_END



      networkManager->GetUserEquipmentContainer ()->push_back (ue);

      // register ue to the gnb
      gnb->RegisterUserEquipment (ue);


      gnb->GetPhy()->GetUlChannel()->SetPropagationLossModel(NULL);

DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG_LOG)
      CartesianCoordinates* userPosition = ue->GetMobilityModel()->GetAbsolutePosition();
      CartesianCoordinates* cellPosition = cell->GetCellCenterPosition();
      double distance = userPosition->GetDistance(cellPosition)/1000;

      double zoneWidth = (double) (radius/nbOfZones);
      double edges[nbOfZones+1];
      for (int i=0; i <= nbOfZones; i++)
        {
          edges[i]= i*zoneWidth;
        }
      if (distance >= edges[nbOfZones])
        {
          zone = 0;
        }
      for (int i= 0; i < nbOfZones; i++)
        {
          if (distance >= edges[i] && distance <= edges[i+1])
            {
              zone=nbOfZones-1-i;
            }
        }
      cout << "LOG_ZONE UE " << idUE
           << " DISTANCE " << distance
           << " WIDTH " << zoneWidth
           << " ZONE " << zone
           << endl;
DEBUG_LOG_END

      //CREATE UPLINK APPLICATION FOR THIS UE
      double start_time = .001 + timeDis(gen);
      double duration_time = flow_duration - 0.001;

      // *** cbr application
      // create application
      CBRApplication[cbrApplication].SetSource (ue);
      CBRApplication[cbrApplication].SetDestination (gnb);
      CBRApplication[cbrApplication].SetApplicationID (applicationID);
      CBRApplication[cbrApplication].SetStartTime(start_time);
      CBRApplication[cbrApplication].SetStopTime(duration_time);

      CBRApplication[cbrApplication].SetInterval ((double) CBR_interval);
      CBRApplication[cbrApplication].SetSize (CBR_size);

//------------------------------------------------------------------------------------------------------------- create qos parameters????

      QoSParameters *qosParameters = new QoSParameters ();
      qosParameters->SetMaxDelay (flow_duration);
      CBRApplication[cbrApplication].SetQoSParameters (qosParameters);


      //create classifier parameters
      ClassifierParameters *cp = new ClassifierParameters (ue->GetIDNetworkNode(),
                                                           gnb->GetIDNetworkNode(),
                                                           0,
                                                           destinationPort,
                                                           TransportProtocol::TRANSPORT_PROTOCOL_TYPE_UDP);
      CBRApplication[cbrApplication].SetClassifierParameters (cp);

      cout << "CREATED CBR APPLICATION, ID " << applicationID << endl;

      //update counter
      //destinationPort++;
      applicationID++;
      cbrApplication++;
      idUE++;
    }

  simulator->SetStop(duration);
  simulator->Run ();
}
