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

#include "roundrobin-uplink-packet-scheduler.h"
#include "../mac-entity.h"
#include "../../packet/Packet.h"
#include "../../packet/packet-burst.h"
#include "../../../device/NetworkNode.h"
#include "../../../flows/radio-bearer.h"
#include "../../../protocolStack/rrc/rrc-entity.h"
#include "../../../flows/application/Application.h"
#include "../../../device/GNodeB.h"
#include "../../../protocolStack/mac/AMCModule.h"
#include "../../../phy/phy.h"
#include "../../../core/spectrum/bandwidth-manager.h"
#include "../../../core/idealMessages/ideal-control-messages.h"
#include "../../../flows/QoS/QoSParameters.h"
#include "../../../flows/MacQueue.h"
#include "../../../utility/eesm-effective-sinr.h"

RoundRobinUplinkPacketScheduler::RoundRobinUplinkPacketScheduler()
{
  SetMacEntity (nullptr);
  CreateUsersToSchedule ();

  m_roundRobinId = 0;
}

RoundRobinUplinkPacketScheduler::~RoundRobinUplinkPacketScheduler()
{
  Destroy ();
}

double
RoundRobinUplinkPacketScheduler::ComputeSchedulingMetric (UserToSchedule* user, int subchannel)
{
  double metric;

  int channelCondition = user->m_channelContition.at (subchannel);
  AMCModule* amc = user->m_userToSchedule->GetProtocolStack()->GetMacEntity()->GetAmcModule();
  double spectralEfficiency = amc->GetSinrFromCQI (channelCondition);

  metric = spectralEfficiency * 180000;

  return metric;
}


void
RoundRobinUplinkPacketScheduler::RBsAllocation ()
{
DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
  cout << " ---- RR UL RBs Allocation";
DEBUG_LOG_END

  UsersToSchedule *users = GetUsersToSchedule ();
  int nbOfRBs = GetMacEntity ()->GetDevice ()->GetPhy ()->GetBandwidthManager ()->GetUlSubChannels ().size ();


  //RBs allocation
  int nbPrbToAssign = 5;
  int stop_nbOfRBs = nbOfRBs;
  if ((nbOfRBs / (int)users->size ()) > nbPrbToAssign)
    {
      nbPrbToAssign = ceil (nbOfRBs / users->size ());
      stop_nbOfRBs = nbPrbToAssign * users->size ();
    }


DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
  cout << "  PRB to assign " << nbOfRBs
            << ", PRB for user " << nbPrbToAssign << endl;
DEBUG_LOG_END

  int s = 0;
  while (s < stop_nbOfRBs)
    {
      if (m_roundRobinId >= (int)users->size ()) m_roundRobinId = 0; //restart again from the beginning

      UserToSchedule* scheduledUser = users->at (m_roundRobinId);

DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG_NB)
      cout << "\t UE " << scheduledUser->m_userToSchedule->GetIDNetworkNode()
           << " has  " << scheduledUser->m_dataToTransmit << " byte to transmit"
           << endl;
DEBUG_LOG_END

      AMCModule* amc = scheduledUser->m_userToSchedule->GetProtocolStack()->GetMacEntity()->GetAmcModule();
      vector<double> sinrs;
      for (int i = 0; i < nbPrbToAssign; i++)
        {
          double chCondition = scheduledUser->m_channelContition.at (s+i);
          double sinr = chCondition;
          sinrs.push_back (sinr);
          scheduledUser->m_listOfAllocatedRBs.push_back (s+i);
        }

      int mcs = amc->GetMCSFromSinrVector (sinrs);
      int tbs = (
                  (amc->GetTBSizeFromMCS (mcs, nbPrbToAssign))
                  / 8);

      scheduledUser->m_transmittedData = tbs;
      scheduledUser->m_selectedMCS = mcs;

      s = s + nbPrbToAssign;
      m_roundRobinId++;
    }
}
