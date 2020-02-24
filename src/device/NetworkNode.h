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



#ifndef NETWORKNODE_H_
#define NETWORKNODE_H_

#include <vector>
#include <cmath>
#include "../networkTopology/Cell.h"
#include "../networkTopology/Femtocell.h"
#include "IPClassifier/Classifier.h"
#include "../protocolStack/protocols/TransportProtocol.h"
#include "../mobility/Mobility.h"
#include "../mobility/RandomDirection.h"
#include "../mobility/RandomWalk.h"
#include "../mobility/RandomWaypoint.h"
#include "../mobility/ConstantPosition.h"
#include "../mobility/Manhattan.h"
#include "../mobility/LinearMovement.h"
#include "../protocolStack/protocol-stack.h"
#include "../phy/phy.h"

class PacketBurst;
class ProtocolStack;

class NetworkNode
{
public:
  enum NodeState
  {
    STATE_DETACHED,
    STATE_ACTIVE,
    STATE_IDLE
  };

  enum NodeType
  {
    TYPE_GNODEB,
    TYPE_UE,
    TYPE_GW,
    TYPE_HOME_BASE_STATION,
    TYPE_RELAY_NODE,
    TYPE_MULTICAST_DESTINATION
  };

private:
  int m_idNetworkNode;
  NodeState m_state;
  NodeType m_nodeType;

  Cell *m_cell;

  ProtocolStack *m_protocolStack;

  Classifier *m_classifier;

  Phy *m_phy;

  Mobility *m_mobility;

  NetworkNode* m_multicastDest;

public:
  NetworkNode();
  virtual ~NetworkNode();

  void Destroy ();

  void SetIDNetworkNode (int id);
  int GetIDNetworkNode (void) const;

  void SetNodeType (NodeType type);
  NodeType GetNodeType (void) const;

  void SetNodeState (NodeState state);
  virtual void MakeActive (void);
  NodeState GetNodeState (void) const;

  void SetCell (Cell *cell);
  Cell* GetCell (void);

  void SetProtocolStack (ProtocolStack *stack);
  ProtocolStack* GetProtocolStack (void) const;

  void SetClassifier (Classifier *classifier);
  Classifier* GetClassifier (void) const;

  void SetPhy (Phy *phy);
  Phy* GetPhy (void);

  void SetMulticastDestination (NetworkNode *n);
  NetworkNode* GetMulticastDestination (void);

  void SetMobilityModel (Mobility* m);
  Mobility* GetMobilityModel (void);

  void SendPacketBurst (shared_ptr<PacketBurst> p);
  void ReceivePacketBurst (shared_ptr<PacketBurst> p);
};


#endif /* NETWORKNODE_H_ */
