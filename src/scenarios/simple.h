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
 */

#include "../channel/RadioChannel.h"
#include "../core/spectrum/bandwidth-manager.h"
#include "../networkTopology/Cell.h"
#include "../core/eventScheduler/simulator.h"
#include "../flows/application/InfiniteBuffer.h"
#include "../flows/QoS/QoSParameters.h"
#include "../componentManagers/FrameManager.h"
#include "../componentManagers/FlowsManager.h"
#include "../device/GNodeB.h"
#include "../phy/gnb-phy.h"
#include "../phy/ue-phy.h"
#include "../protocolStack/packet/packet-burst.h"
#include "../protocolStack/packet/Packet.h"
#include "../device/NetworkNode.h"
#include "../device/UserEquipment.h"
#include "../device/Gateway.h"
#include "../load-parameters.h"
#include "../device/CqiManager/fullband-cqi-manager.h"
#include "../device/CqiManager/wideband-cqi-manager.h"
#include "../channel/propagation-model/channel-realization.h"
#include "../channel/propagation-model/propagation-loss-model.h"
#include "../utility/CellPosition.h"


static void Simple ()
{

  // CREATE COMPONENT MANAGERS
  Simulator *simulator = Simulator::Init();
  FrameManager *frameManager = FrameManager::Init();
  NetworkManager* networkManager = NetworkManager::Init();
  FlowsManager* flowsManager = FlowsManager::Init ();


  //Create CHANNELS
  RadioChannel *dlCh = new RadioChannel ();
  RadioChannel *ulCh = new RadioChannel ();


  // CREATE SPECTRUM
  BandwidthManager* spectrum = new BandwidthManager (5, 5, 0, 0);


  // CREATE CELL
  int idCell = 0;
  int radius = 1; //km
  int minDistance = 0.0035; //km
  int posX = 0;
  int posY = 0;
  Cell* cell = networkManager->CreateCell (idCell, radius, minDistance, posX, posY);


  //Create GNodeB
  int idGnb = 1;
  int GnbTxAntennas = 1;
  int GnbRxAntennas = 1;
  GNodeB* gnb = networkManager->CreateGnodeb (idGnb, cell, posX, posY, GnbTxAntennas, GnbRxAntennas, dlCh, ulCh, spectrum);
  gnb->SetDLScheduler (GNodeB::DLScheduler_TYPE_PROPORTIONAL_FAIR);


  //Create GW
  Gateway *gw = networkManager->CreateGateway ();


  //Create UE
  int idUe = 2;
  int posX_ue = 40; //m
  int posY_ue = 0;  //m
  int speed = 3;    //km/h
  double speeDirection = 0;
  int UeTxAntennas = 1;
  int UeRxAntennas = 1;
  UserEquipment* ue = networkManager->CreateUserEquipment (idUe, posX_ue, posY_ue, speed, speeDirection, UeTxAntennas, UeRxAntennas, cell, gnb);

  //Create an Application
  QoSParameters *qos = new QoSParameters ();
  int applicationID = 0;
  int srcPort = 0;
  int dstPort = 100;
  double startTime = 0.1; //s
  double stopTime = 0.12;  //s
  Application* be = flowsManager->CreateApplication (applicationID,
                    gw, ue,
                    srcPort, dstPort, TransportProtocol::TRANSPORT_PROTOCOL_TYPE_UDP,
                    Application::APPLICATION_TYPE_INFINITE_BUFFER,
                    qos,
                    startTime, stopTime);


  simulator->SetStop(0.13);
  simulator->Run ();

}
