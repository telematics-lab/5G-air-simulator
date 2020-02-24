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


#include "dl-exp-packet-scheduler.h"
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
#include "../../../flows/QoS/QoSForEXP.h"
#include "../../../flows/MacQueue.h"

DL_EXP_PacketScheduler::DL_EXP_PacketScheduler()
{
  SetMacEntity (nullptr);
  CreateFlowsToSchedule ();
}

DL_EXP_PacketScheduler::~DL_EXP_PacketScheduler()
{
  Destroy ();
}

void
DL_EXP_PacketScheduler::DoSchedule ()
{
DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
  cout << "Start DL packet scheduler for node "
            << GetMacEntity ()->GetDevice ()->GetIDNetworkNode()<< endl;
DEBUG_LOG_END

  UpdateAverageTransmissionRate ();
  CheckForDLDropPackets ();
  SelectFlowsToSchedule ();
  ComputeAW ();

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
DL_EXP_PacketScheduler::ComputeSchedulingMetric (RadioBearer *bearer, double spectralEfficiency, int subChannel)
{
  /*
   * For the EXP scheduler the metric is computed
   * as follows:
   *                -log(dropProbability)/targetDelay * HOL - aW
   * metric = exp ( --------------------------------------------) * availableRate/averageRate
   *                            1 - sqrt (aW)
   */

  double metric;

  if ((bearer->GetApplication ()->GetApplicationType () == Application::APPLICATION_TYPE_INFINITE_BUFFER)
      ||
      (bearer->GetApplication ()->GetApplicationType () == Application::APPLICATION_TYPE_CBR))
    {
      metric = (spectralEfficiency * 180000.)
               /
               bearer->GetAverageTransmissionRate();
    }
  else
    {

      QoSForEXP *qos = (QoSForEXP*) bearer->GetQoSParameters ();

      double HOL = bearer->GetHeadOfLinePacketDelay ();
      double alfa = -log10(qos->GetDropProbability()) / qos->GetMaxDelay ();
      double avgAW = GetAW ();
      double AW = alfa * HOL;

      if (AW < 0.000001)
        AW=0;

      double AW_avgAW = AW - avgAW;

      if (AW_avgAW < 0.000001)
        AW_avgAW=0;

      metric = exp ( AW_avgAW /
                     (1 + sqrt (GetAW ())) )
               *
               ((spectralEfficiency * 180000.)
                /
                bearer->GetAverageTransmissionRate());
    }

  return metric;
}


void
DL_EXP_PacketScheduler::ComputeAW ()
{
DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
  cout << "ComputeAW" << endl;
DEBUG_LOG_END

  m_aW = 0;
  int nbFlow = 0;
  for (auto flow : *GetFlowsToSchedule())
    {
      RadioBearer *bearer = flow->GetBearer ();

      if (bearer->HasPackets ())
        {
          if ((bearer->GetApplication ()->GetApplicationType () == Application::APPLICATION_TYPE_TRACE_BASED)
              ||
              (bearer->GetApplication ()->GetApplicationType () == Application::APPLICATION_TYPE_VOIP))
            {
              QoSForEXP *qos = (QoSForEXP*) bearer->GetQoSParameters ();
              double aWi =  - (log10 (qos->GetDropProbability())
                               /
                               qos->GetMaxDelay ());
              double HOL = bearer->GetHeadOfLinePacketDelay ();
              aWi = aWi * HOL;
              m_aW += aWi;
              nbFlow++;
            }
        }
    }

  m_aW = m_aW/nbFlow;

  if (m_aW < 0.000001)
    m_aW=0;
}

double
DL_EXP_PacketScheduler::GetAW (void) const
{
  return m_aW;
}
