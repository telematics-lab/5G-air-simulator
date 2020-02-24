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



#include "rrc-entity.h"
#include "../packet/Packet.h"
#include "../../device/IPClassifier/ClassifierParameters.h"
#include "../../flows/radio-bearer.h"
#include "../../flows/radio-bearer-sink.h"
#include "../../load-parameters.h"
#include "../../device/NetworkNode.h"
#include "ho/handover-entity.h"

RrcEntity::RrcEntity ()
{
  m_bearers = new RadioBearersContainer ();
  m_sink = new RadioBearersSinkContainer ();
  m_device = nullptr;
  m_handover = new HandoverEntity ();
}

RrcEntity::~RrcEntity ()
{
  m_bearers->clear ();
  delete m_bearers;

  m_sink->clear ();
  delete m_sink;

  delete m_handover;
}

void
RrcEntity::SetDevice (NetworkNode* d)
{
  m_device = d;
  m_handover->SetDevice (d);
}


NetworkNode*
RrcEntity::GetDevice ()
{
  return m_device;
}

RrcEntity::RadioBearersContainer*
RrcEntity::GetRadioBearerContainer (void)
{
  return m_bearers;
}

RrcEntity::RadioBearersSinkContainer*
RrcEntity::GetRadioBearerSinkContainer (void)
{
  return m_sink;
}


void
RrcEntity::AddRadioBearer (RadioBearer* bearer)
{
DEBUG_LOG_START_1(SIM_ENV_TEST_START_APPLICATION)
  cout << "Add radio bearer on "  << GetDevice ()->GetIDNetworkNode ()  << endl;
DEBUG_LOG_END

  m_bearers->push_back (bearer);
}

void
RrcEntity::DelRadioBearer (RadioBearer* bearer)
{
  RadioBearersContainer* newContainer = new RadioBearersContainer ();
  for (auto b : *GetRadioBearerContainer ())
    {
      if (b->GetRlcEntity ()->GetRlcEntityIndex () != bearer->GetRlcEntity ()->GetRlcEntityIndex ())
        {
          newContainer->push_back (b);
        }
    }
  m_bearers->clear ();
  delete m_bearers;
  m_bearers = newContainer;
}

void
RrcEntity::AddRadioBearerSink (RadioBearerSink* bearer)
{
DEBUG_LOG_START_1(SIM_ENV_TEST_START_APPLICATION)
  cout << "Add sink radio bearer on "  << GetDevice ()->GetIDNetworkNode () << endl;
DEBUG_LOG_END

  m_sink->push_back (bearer);
}

void
RrcEntity::DelRadioBearerSink (RadioBearerSink* bearer)
{
  RadioBearersSinkContainer* newContainer = new RadioBearersSinkContainer ();
  for (auto b : *GetRadioBearerSinkContainer ())
    {
      if (b->GetRlcEntity ()->GetRlcEntityIndex () != bearer->GetRlcEntity ()->GetRlcEntityIndex ())
        {
          newContainer->push_back (b);
        }
    }
  m_sink->clear ();
  delete m_sink;
  m_sink = newContainer;
}

RadioBearer*
RrcEntity::GetRadioBearer (ClassifierParameters* ipc)
{
  for (auto bearer : *GetRadioBearerContainer ())
    {
      if (bearer->GetClassifierParameters ()->GetSourcePort () == ipc->GetSourcePort ()
          &&
          bearer->GetClassifierParameters ()->GetDestinationPort () == ipc->GetDestinationPort ())
        {
          return bearer;
        }
    }
  return 0;
}

RadioBearerSink*
RrcEntity::GetRadioBearerSink (ClassifierParameters* ipc)
{
  for (auto bearer : *GetRadioBearerSinkContainer ())
    {
      if (bearer->GetClassifierParameters ()->GetSourcePort () == ipc->GetSourcePort ()
          &&
          bearer->GetClassifierParameters ()->GetDestinationPort () == ipc->GetDestinationPort ())
        {
          return bearer;
        }
    }
  return 0;
}


RadioBearerInstance*
RrcEntity::GetRadioBearer (int rlcIndex)
{
  for (auto bearer : *GetRadioBearerSinkContainer ())
    {
      if (bearer->GetRlcEntity ()->GetRlcEntityIndex () == rlcIndex)
        {
          return bearer;
        }
    }

  for (auto bearer : *GetRadioBearerContainer ())
    {
      if (bearer->GetRlcEntity ()->GetRlcEntityIndex () == rlcIndex)
        {
          return bearer;
        }
    }

  return 0;
}


void
RrcEntity::SetHandoverEntity (HandoverEntity* h)
{
  m_handover = h;
}

HandoverEntity*
RrcEntity::GetHandoverEntity (void)
{
  return m_handover;
}
