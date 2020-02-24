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


#include "dl-mlwdf-packet-scheduler.h"
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
#include "../../../flows/QoS/QoSForM_LWDF.h"
#include "../../../flows/MacQueue.h"

DL_MLWDF_PacketScheduler::DL_MLWDF_PacketScheduler()
{
  SetMacEntity (nullptr);
  CreateFlowsToSchedule ();
}

DL_MLWDF_PacketScheduler::~DL_MLWDF_PacketScheduler()
{
  Destroy ();
}


void
DL_MLWDF_PacketScheduler::DoSchedule ()
{
DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
  cout << "Start DL packet scheduler for node "
            << GetMacEntity ()->GetDevice ()->GetIDNetworkNode()<< endl;
DEBUG_LOG_END

  UpdateAverageTransmissionRate ();
  CheckForDLDropPackets ();
  SelectFlowsToSchedule ();

  if (GetFlowsToSchedule ()->size() == 0)
    {}
  else
    {
      RBsAllocation ();
    }

  StopSchedule ();
  ClearFlowsToSchedule ();
}

double
DL_MLWDF_PacketScheduler::ComputeSchedulingMetric (RadioBearer *bearer, double spectralEfficiency, int subChannel)
{
  /*
   * For the M-LWDF scheduler the metric is computed
   * as follows:
   *
   * metric = -log(dropProbability)/targetDelay *
   *          * HOL * availableRate/averageRate
   */

  double metric;

  if ((bearer->GetApplication ()->GetApplicationType () == Application::APPLICATION_TYPE_INFINITE_BUFFER)
      ||
      (bearer->GetApplication ()->GetApplicationType () == Application::APPLICATION_TYPE_CBR))
    {
      metric = (spectralEfficiency * 180000.)
               /
               bearer->GetAverageTransmissionRate();

DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
      cout << "METRIC: " << bearer->GetApplication ()->GetApplicationID ()
                << " " << spectralEfficiency
                << " " << bearer->GetAverageTransmissionRate ()
                << " --> " << metric
                << endl;
DEBUG_LOG_END

    }
  else
    {

      QoSForM_LWDF *qos = (QoSForM_LWDF*) bearer->GetQoSParameters ();

      double a = (-log10 (qos->GetDropProbability())) / qos->GetMaxDelay ();
      double HOL = bearer->GetHeadOfLinePacketDelay ();

      metric = (a * HOL)
               *
               ((spectralEfficiency * 180000.)
                /
                bearer->GetAverageTransmissionRate ());

DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
      cout << "METRIC: " << bearer->GetApplication ()->GetApplicationID ()
                << " " << a
                << " " << Simulator::Init()->Now()
                << " " << bearer->GetMacQueue()->Peek().GetTimeStamp()
                << " " << HOL
                << " " << spectralEfficiency
                << " " << bearer->GetAverageTransmissionRate ()
                << " --> " << metric
                << endl;
DEBUG_LOG_END

    }

  return metric;
}

