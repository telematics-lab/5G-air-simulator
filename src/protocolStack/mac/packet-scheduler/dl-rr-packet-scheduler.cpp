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

#include "dl-rr-packet-scheduler.h"
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

DL_RR_PacketScheduler::DL_RR_PacketScheduler()
{
  SetMacEntity (nullptr);
  CreateFlowsToSchedule ();
  m_selectedFlowIndex = 0;
  m_lastUpdate = 0;
}

DL_RR_PacketScheduler::~DL_RR_PacketScheduler()
{
  Destroy ();
}


double
DL_RR_PacketScheduler::ComputeSchedulingMetric (RadioBearer *bearer, double spectralEfficiency, int subChannel)
{
  /*
   * For the RR scheduler the metric is computed
   * as follows:
   *
   * 1 for the selected flow
   * 0 for other flows
   *
   * The selected flow changes every subframe
   */

  FlowsToSchedule* flows = GetFlowsToSchedule();
  if(Simulator::Init()->Now() > m_lastUpdate)
    {
      m_selectedFlowIndex += m_maxUsersPerRB;
      if(m_selectedFlowIndex >= (int)flows->size())
        {
          m_selectedFlowIndex = 0;
        }
      m_lastUpdate = Simulator::Init()->Now();
    }

  double metric = 0;

  for (int i = m_selectedFlowIndex; i < m_selectedFlowIndex+m_maxUsersPerRB; i++)
    {
      if(flows->at( i % (int)flows->size() )->GetBearer() == bearer)
        {
          metric = 1;
        }
    }

  return metric;
}

