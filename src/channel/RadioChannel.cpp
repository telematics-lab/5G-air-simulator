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


#include "RadioChannel.h"
#include "../core/spectrum/transmitted-signal.h"
#include "../protocolStack/packet/packet-burst.h"
#include "../device/NetworkNode.h"
#include "../device/MulticastDestination.h"
#include "../device/CqiManager/cqi-manager.h"
#include "../phy/phy.h"
#include "../phy/multicast-destination-phy.h"
#include "../core/eventScheduler/simulator.h"
#include "../componentManagers/FrameManager.h"
#include "../load-parameters.h"
#include "propagation-model/propagation-loss-model.h"

RadioChannel::RadioChannel()
{
  m_attachedDevices = new vector<NetworkNode*> ();
  m_propagationLossModel = new PropagationLossModel ();
}

RadioChannel::~RadioChannel()
{
  m_attachedDevices->clear ();
  delete m_attachedDevices;
  delete m_propagationLossModel;
}

void
RadioChannel::StartTx (shared_ptr<PacketBurst> p, TransmittedSignal* txSignal, NetworkNode* src)
{
DEBUG_LOG_START_1(SIM_ENV_TEST_DEVICE_ON_CHANNEL)
  cout << "RadioChannel::StartTx ch " << GetChannelId () << endl;
DEBUG_LOG_END

  Simulator::Init()->Schedule(0.001,
                              &RadioChannel::StartRx,
                              this,
                              p,
                              txSignal,
                              src);
DEBUG_LOG_START_1(SIM_ENV_TRANSMISSION_DEBUG)
  cout << "   =======  channel  =======" << endl;
DEBUG_LOG_END

}

void
RadioChannel::StartRx (shared_ptr<PacketBurst> p, TransmittedSignal* txSignal, NetworkNode* src)
{
DEBUG_LOG_START_1(SIM_ENV_TEST_DEVICE_ON_CHANNEL)
  cout << "RadioChannel::StartRx ch " << GetChannelId () << endl;
DEBUG_LOG_END

  for (auto dst : *GetDevices())
    {

DEBUG_LOG_START_1(SIM_ENV_TEST_DEVICE_ON_CHANNEL)
      cout << "\t Node " << dst->GetIDNetworkNode () << " is attached" << endl;
DEBUG_LOG_END

      //APPLY THE PROPAGATION LOSS MODEL
      ReceivedSignal* rxSignal;
      if (m_propagationLossModel != nullptr && dst->GetNodeType() != NetworkNode::TYPE_MULTICAST_DESTINATION )
        {
DEBUG_LOG_START_1(SIM_ENV_TEST_DEVICE_ON_CHANNEL)
          cout << "RadioChannel::StartRx add propagation loss" << endl;
DEBUG_LOG_END
          rxSignal = GetPropagationLossModel ()->AddLossModel (src, dst, txSignal);

//if(rxSignal->GetIsMBSFNSignal())cout << "MBSFN signal in radio-channel" << endl;
        }
      else
        {
          rxSignal = txSignal->Copy ();
        }

      //DELIVERY THE BURST OF PACKETS
      if(dst->GetNodeType() != NetworkNode::TYPE_MULTICAST_DESTINATION)
        {
          dst->GetPhy ()->StartRx (p->Copy (), rxSignal);
        }
      else
        {
          delete rxSignal;
          MulticastDestinationPhy* destPhy = (MulticastDestinationPhy*)dst->GetPhy();
          if (FrameManager::Init()->MbsfnEnabled()==true)
            {
              if (FrameManager::Init()->isMbsfnSubframe()==true)
                {
                  //destPhy->CreateCqiFeedbacks();
                }
            }
          else
            {
              MulticastDestination* mdst = (MulticastDestination*)dst;
              if (mdst->GetCqiManager()->NeedToSendFeedbacks())
                {
                  destPhy->CreateCqiFeedbacks();
                }
            }
        }
    }

}

void
RadioChannel::AddDevice (NetworkNode* d)
{
DEBUG_LOG_START_1(SIM_ENV_TEST_DEVICE_ON_CHANNEL)
  cout << "RadioChannel " << m_channelId << " ADD Node " << d->GetIDNetworkNode ()<< endl;
DEBUG_LOG_END

  vector<NetworkNode*>::iterator it, ins_pos;
  ins_pos = m_attachedDevices->end();
  if (m_attachedDevices->size()>0)
    {
      for (it=m_attachedDevices->end()-1; it>=m_attachedDevices->begin(); it--)
        {
          // UEs shoul be inserted before multicast destinations
          if ( (*it)->GetNodeType()==NetworkNode::TYPE_MULTICAST_DESTINATION )
            {
              ins_pos = it;
            }
          else
            {
              break;
            }
        }
    }
  m_attachedDevices->insert(ins_pos,d);
}

void
RadioChannel::DelDevice (NetworkNode* d)
{
DEBUG_LOG_START_1(SIM_ENV_TEST_DEVICE_ON_CHANNEL)
  cout << "RadioChannel " << m_channelId << " DEL Node " << d->GetIDNetworkNode ()<< endl;
DEBUG_LOG_END

  vector<NetworkNode*> *new_list = new vector<NetworkNode*> ();
  for (auto node : *GetDevices ())
    {
      if (node->GetIDNetworkNode () != d->GetIDNetworkNode ())
        {
          new_list->push_back (node);
        }
    }

  m_attachedDevices->clear ();
  delete m_attachedDevices;
  m_attachedDevices = new_list;
}

bool
RadioChannel::IsAttached (NetworkNode* d)
{
  for (auto node : *GetDevices ())
    {
      if (node->GetIDNetworkNode () == d->GetIDNetworkNode ())
        {
DEBUG_LOG_START_1(SIM_ENV_TEST_DEVICE_ON_CHANNEL)
          cout << "RadioChannel find Node " << d->GetIDNetworkNode ()<< endl;
DEBUG_LOG_END
          return true;
        }
    }
  return false;
}

vector<NetworkNode*>*
RadioChannel::GetDevices (void)
{
  return m_attachedDevices;
}

void
RadioChannel::SetPropagationLossModel (PropagationLossModel* m)
{
  m_propagationLossModel = m;
}

PropagationLossModel*
RadioChannel::GetPropagationLossModel (void)
{
  return m_propagationLossModel;
}

void
RadioChannel::SetChannelId (int id)
{
  m_channelId = id;
}

int
RadioChannel::GetChannelId (void)
{
  return m_channelId;
}
