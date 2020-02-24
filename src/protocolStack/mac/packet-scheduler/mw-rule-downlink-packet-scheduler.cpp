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

#include "mw-rule-downlink-packet-scheduler.h"
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

MwRulePacketScheduler::MwRulePacketScheduler()
{
  SetMacEntity (nullptr);
  CreateFlowsToSchedule ();
}

MwRulePacketScheduler::~MwRulePacketScheduler()
{
  Destroy ();
}


double
MwRulePacketScheduler::ComputeSchedulingMetric (RadioBearer *bearer, double spectralEfficiency, int subChannel)
{
DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
  cout << "\t ComputeSchedulingMetric for flow "
            << bearer->GetApplication ()->GetApplicationID () << endl;
DEBUG_LOG_END

  double metric;

  if ((bearer->GetApplication ()->GetApplicationType () == Application::APPLICATION_TYPE_INFINITE_BUFFER)
      ||
      (bearer->GetApplication ()->GetApplicationType () == Application::APPLICATION_TYPE_CBR))
    {
      metric = (spectralEfficiency * 180000.)
               /
               bearer->GetAverageTransmissionRate();

DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
      cout << "\t\t non real time flow: metric = " << metric << endl;
DEBUG_LOG_END

    }
  else
    {
      QoSParameters *qos = bearer->GetQoSParameters ();
      double HOL = bearer->GetHeadOfLinePacketDelay ();
      double targetDelay = qos->GetMaxDelay ();

DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
      cout << "\t\t real time flow: HOL = " << HOL << ", target delay = " << targetDelay;
DEBUG_LOG_END

      //compute sum of HOL and average spectral efficiency
DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
      cout << "\n\t\t\t compute sum HOL for all real time flows"<< endl;
DEBUG_LOG_END

      double sumHOL = 0;
      int nbFlows = 0;

      for (auto flow : *GetFlowsToSchedule())
        {
          if (!flow->GetBearer ()->HasPackets ())
            {}
          else
            {
              if ((flow->GetBearer ()->GetApplication ()->GetApplicationType ()
                   != Application::APPLICATION_TYPE_INFINITE_BUFFER)
                  &&
                  (flow->GetBearer ()->GetApplication ()->GetApplicationType ()
                   != Application::APPLICATION_TYPE_CBR))
                {
                  sumHOL += flow->GetBearer ()->GetHeadOfLinePacketDelay ();
                  nbFlows++;

DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
                  cout << "\t\t\t --> HOL for flow " << flow->GetBearer ()->GetApplication ()->GetApplicationID ()
                            << " = " << flow->GetBearer ()->GetHeadOfLinePacketDelay ()<< endl;
DEBUG_LOG_END
                }
              else
                {
DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
                  cout << "\t\t\t --> non real time flow, go on! " << endl;
DEBUG_LOG_END
                }
            }
        }


      //COMPUTE METRIC USING EXP RULE:
      double numerator = (6/targetDelay) * HOL;
      double denominator = (1 + sqrt (sumHOL/nbFlows));
      double weight = (spectralEfficiency * 180000.)
                      /
                      bearer->GetAverageTransmissionRate();

      metric = (exp (numerator / denominator)) * weight;

DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
      cout << " --> metric = " << metric << endl;
DEBUG_LOG_END
    }

  return metric;
}

