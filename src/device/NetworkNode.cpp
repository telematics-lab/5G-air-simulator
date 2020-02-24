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



#include "NetworkNode.h"
#include "../core/eventScheduler/simulator.h"
#include "../componentManagers/NetworkManager.h"
#include "../load-parameters.h"
#include "../protocolStack/packet/Packet.h"
#include "../protocolStack/packet/packet-burst.h"
#include "stdlib.h"
#include "../phy/phy.h"
#include "../phy/gnb-phy.h"
#include "../phy/ue-phy.h"
#include "../flows/radio-bearer-instance.h"
#include "UserEquipment.h"
#include "GNodeB.h"
#include "../protocolStack/protocol-stack.h"

class UserEquipment;

NetworkNode::NetworkNode()
{
  m_cell = nullptr;
  m_protocolStack = nullptr;
  m_phy = nullptr;
  m_mobility = nullptr;
  m_multicastDest = nullptr;
}

NetworkNode::~NetworkNode()
{
  Destroy ();
}

void
NetworkNode::Destroy ()
{
  delete m_classifier;
  delete m_phy;
  delete m_protocolStack;
  delete m_mobility;
}

void
NetworkNode::SetIDNetworkNode (int id)
{
  m_idNetworkNode = id;
}

int
NetworkNode::GetIDNetworkNode (void) const
{
  return m_idNetworkNode;
}

void
NetworkNode::SetNodeType (NodeType type)
{
  m_nodeType = type;
}

NetworkNode::NodeType
NetworkNode::GetNodeType (void) const
{
  return m_nodeType;
}

void
NetworkNode::SetCell (Cell *cell)
{
  m_cell = cell;
}

Cell*
NetworkNode::GetCell (void)
{
  return m_cell;
}

void
NetworkNode::SetProtocolStack (ProtocolStack *stack)
{
  m_protocolStack = stack;
}

ProtocolStack*
NetworkNode::GetProtocolStack (void) const
{
  return m_protocolStack;
}


void
NetworkNode::SetClassifier (Classifier *classifier)
{
  m_classifier = classifier;
}

Classifier*
NetworkNode::GetClassifier (void) const
{
  return m_classifier;
}

void
NetworkNode::SetPhy (Phy *phy)
{
  m_phy = phy;
}


Phy*
NetworkNode::GetPhy (void)
{
  return m_phy;
}

void
NetworkNode::SetMobilityModel (Mobility* m)
{
  m_mobility = m;
}

Mobility*
NetworkNode::GetMobilityModel (void)
{
  return m_mobility;
}


void
NetworkNode::SetNodeState (NodeState state)
{
  m_state = state;
}

void
NetworkNode::MakeActive (void)
{
  if(GetNodeType()==TYPE_UE)
    {
      if(((UserEquipment*)this)->GetTargetNode()->GetDLScheduler()!=nullptr)
        {
          SetNodeState (STATE_ACTIVE);
          UserEquipment* ue = (UserEquipment*) this;
          ue -> SetLastActivity();
        }
      else
        {
          SetNodeState (STATE_IDLE);
        }
    }
  else
    {
      SetNodeState (STATE_ACTIVE);
    }
}

NetworkNode::NodeState
NetworkNode::GetNodeState (void) const
{
  return m_state;
}


void
NetworkNode::SendPacketBurst (shared_ptr<PacketBurst> p)
{
DEBUG_LOG_START_1(SIM_ENV_TEST_DEVICE_ON_CHANNEL)
  cout << "Node " << GetIDNetworkNode () << " sends burst" << endl;
DEBUG_LOG_END

  GetPhy ()->StartTx (p);
}

void NetworkNode::ReceivePacketBurst (shared_ptr<PacketBurst> p)
{
DEBUG_LOG_START_1(SIM_ENV_TEST_DEVICE_ON_CHANNEL)
  cout << "Node " << GetIDNetworkNode () << " receives burst" << endl;
DEBUG_LOG_END

  for (auto packet : p->GetPackets())
    {
      if (packet->GetDestinationMAC () == GetIDNetworkNode () && GetNodeType() != TYPE_MULTICAST_DESTINATION)
        {
          if (packet->GetRLCHeader () == nullptr)
            {
              // the received packet have not a RLC header (TM RLC Mode). It is not a fragment
              // --> forward it to the classifier
              GetClassifier ()->Classify (packet->Copy ());
            }
          else
            {
              //The received packet could be a fragment. Forward it to a proper RLC entity
              int rlcEntityIndex = packet->GetRLCHeader ()->GetRlcEntityIndex ();
              RadioBearerInstance* bearer =
                GetProtocolStack ()->GetRrcEntity ()->GetRadioBearer (rlcEntityIndex);

              bearer->GetRlcEntity ()->ReceptionProcedure (packet->Copy ());
            }
        }
      else if(GetNodeType() == TYPE_UE && GetMulticastDestination() != nullptr)
        {
          if(packet->GetDestinationMAC () == GetMulticastDestination()->GetIDNetworkNode ())
            {
              if (packet->GetRLCHeader () == nullptr)
                {
                  // the received packet have not a RLC header (TM RLC Mode). It is not a fragment
                  // --> forward it to the classifier
                  GetClassifier ()->Classify (packet->Copy ());
                }
              else
                {
                  //The received packet could be a fragment. Forward it to a proper RLC entity
                  int rlcEntityIndex = packet->GetRLCHeader ()->GetRlcEntityIndex ();

                  Packet* pCopy = packet->Copy ();
                  pCopy->GetIPHeader ()->SetDestinationID (GetIDNetworkNode());
                  RadioBearerInstance* bearer =
                    GetProtocolStack ()->GetRrcEntity ()->GetRadioBearer (rlcEntityIndex);

                  bearer->GetRlcEntity ()->ReceptionProcedure (pCopy);
                }
            }
        }
      else
        {
          //NOT FOR ME
        }
    }
}


void
NetworkNode::SetMulticastDestination (NetworkNode *n)
{
  m_multicastDest = n;
}

NetworkNode*
NetworkNode::GetMulticastDestination (void)
{
  return m_multicastDest;
}

