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


#include "MulticastDestination.h"
#include "UserEquipment.h"
#include "NetworkNode.h"
#include "GNodeB.h"
#include "HeNodeB.h"
#include "Gateway.h"
#include "../phy/multicast-destination-phy.h"
#include "CqiManager/cqi-manager.h"
#include "../core/eventScheduler/simulator.h"
#include "../componentManagers/NetworkManager.h"
#include "../protocolStack/rrc/ho/handover-entity.h"
#include "../protocolStack/rrc/ho/ho-manager.h"
#include <algorithm>

MulticastDestination::MulticastDestination (int idElement,
    Cell *cell,
    GNodeB* target)
{
  SetIDNetworkNode (idElement);
  SetNodeType(NetworkNode::TYPE_MULTICAST_DESTINATION);
  SetCell(cell);

  SetTargetNode(target);

  ProtocolStack *stack = new ProtocolStack (this);
  SetProtocolStack (stack);

  Classifier *classifier = new Classifier ();
  classifier->SetDevice (this);
  SetClassifier (classifier);
  SetNodeState(STATE_IDLE);

  //Setup Mobility Model
  Mobility *m = new ConstantPosition();
  m->SetHandover (false);
  m->SetAbsolutePosition (cell->GetCellCenterPosition());
  m->SetDevice (this);
  SetMobilityModel (m);

  /*m_timePositionUpdate = 0.001;
  Simulator::Init()->Schedule(m_timePositionUpdate,
      &MulticastDestination::UpdateUserPosition,
      this,
      Simulator::Init ()->Now());*/

  MulticastDestinationPhy *phy = new MulticastDestinationPhy ();
  phy->SetDevice(this);
  phy->SetBandwidthManager (target->GetPhy ()->GetBandwidthManager ());
  SetPhy(phy);

  SetCqiManager (nullptr);
  SetIndoorFlag (false);

  m_destinations.clear();
  m_mcs = -1;
}

MulticastDestination::~MulticastDestination()
{
  SetTargetNode(nullptr);
  Destroy ();
}

bool
MulticastDestination::IsIndoor (void)
{
  return false;
}

//Debug
void
MulticastDestination::Print (void)
{
  cout << " MulticastDestination object:"
            "\n\t m_idNetworkNode = " << GetIDNetworkNode () <<
            "\n\t idCell = " << GetCell ()->GetIdCell () <<
            "\n\t idtargetNode = " << GetTargetNode ()->GetIDNetworkNode () <<
            "\n\t m_AbsolutePosition_X = " <<  GetMobilityModel ()->GetAbsolutePosition()->GetCoordinateX()<<
            "\n\t m_AbsolutePosition_Y = " << GetMobilityModel ()->GetAbsolutePosition()->GetCoordinateY()<<
            "\n\t m_speed = " << GetMobilityModel ()->GetSpeed () <<
            "\n\t m_speedDirection = " << GetMobilityModel ()->GetSpeedDirection () <<
            endl;
}

void
MulticastDestination::AddSource(GNodeB *n)
{
  m_sources.push_back(n);
}

void
MulticastDestination::DeleteSource(GNodeB *n)
{
  m_sources.erase( find(m_sources.begin(), m_sources.end(), n) );
}

vector<GNodeB*>
MulticastDestination::GetSources()
{
  return m_sources;
}

void
MulticastDestination::SetSources(vector<GNodeB*> sources)
{
  m_sources = sources;
}

void
MulticastDestination::AddDestination(UserEquipment *n)
{
  m_destinations.push_back(n);
  GetPhy()->SetTxAntennas( min(GetPhy()->GetTxAntennas(),n->GetPhy()->GetTxAntennas()) );
  GetPhy()->SetRxAntennas( min(GetPhy()->GetRxAntennas(),n->GetPhy()->GetRxAntennas()) );

}

void
MulticastDestination::DeleteDestination(UserEquipment* n)
{
  m_destinations.erase( find(m_destinations.begin(), m_destinations.end(), n) );
}

vector<UserEquipment*>
MulticastDestination::GetDestinations()
{
  return m_destinations;
}

void
MulticastDestination::SetDestinations(vector<UserEquipment*> destinations)
{
  m_destinations = destinations;
}


void
MulticastDestination::MakeActive (void)
{
  for( auto ue : m_destinations )
    {
      ue->MakeActive();
    }
  NetworkNode::MakeActive();
}

void
MulticastDestination::SetMcs(int mcs)
{
  m_mcs = mcs;
}

int
MulticastDestination::GetMcs()
{
  return m_mcs;
}

NetworkNode::NodeState
MulticastDestination::GetNodeState (void)
{
 NetworkNode::NodeState state = STATE_ACTIVE;
  for( auto ue : m_destinations )
    {
      state = ue->GetNodeState();
    }
  return state;
}
