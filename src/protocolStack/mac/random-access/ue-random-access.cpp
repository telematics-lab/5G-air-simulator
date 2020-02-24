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
 * Author: Cosimo Damiano Di Pace <cd.dipace@gmail.com>
 * Author: Alessandro Grassi <alessandro.grassi@poliba.it>
 */

#include "../ue-mac-entity.h"
#include "ue-random-access.h"
#include "../../../device/GNodeB.h"
#include "../../../device/UserEquipment.h"


UeRandomAccess::UeRandomAccess()
{
  m_RaProcedureActive = false;
  m_macEntity = NULL;
  m_RAProcedureTimeout = 5; //After 5 s UE initiates next attempt
  SetNbFailedAttempts(0);
  SetMaxFailedAttempts(9);
}


UeRandomAccess::UeRandomAccess(MacEntity* mac) : UeRandomAccess()
{
  m_macEntity = mac;
}


UeRandomAccess::~UeRandomAccess()
{
}


void
UeRandomAccess::SetRandomAccessType(RandomAccessType type)
{
  m_type = type;
}


UeRandomAccess::RandomAccessType
UeRandomAccess::GetUeRandomAccessType()
{
  return m_type;
}


void
UeRandomAccess::SetRaProcedureActive(bool pa)
{
  m_RaProcedureActive = pa;
}


bool
UeRandomAccess::GetRaProcedureActive()
{
  return m_RaProcedureActive;
}


void
UeRandomAccess::SetRAProcedureStartTime(double st)
{
  m_RAProcedureStartTime = st;
}


double
UeRandomAccess::GetRAProcedureStartTime()
{
  return m_RAProcedureStartTime;
}


void
UeRandomAccess::SetRAProcedureTimeout(double to)
{
  m_RAProcedureTimeout = to;
}


double
UeRandomAccess::GetRAProcedureTimeout()
{
  return m_RAProcedureTimeout;
}


void
UeRandomAccess::checkRAProcedureTimeout()
{
DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS)
  cout << "checkRAProcedureTimeout() "  << endl;
DEBUG_LOG_END

  double timeNow = Simulator::Init()->Now();

  if (m_RaProcedureActive == true && (timeNow - m_RAProcedureStartTime) < m_RAProcedureTimeout) //time not expired
    {
      Simulator::Init()->Schedule(0.001, &UeRandomAccess::checkRAProcedureTimeout, this);
    }

  else if (m_RaProcedureActive == false)
    {
      //do nothing
    }

  else if (m_RaProcedureActive == true && (timeNow - m_RAProcedureStartTime) > m_RAProcedureTimeout) //time expired
    {
      m_RaProcedureActive = false;
      Simulator::Init()->Schedule(0.001, &UeRandomAccess::StartRaProcedure, this);
    }
}


bool
UeRandomAccess::isRachOpportunity()
{
  UserEquipment* ue = (UserEquipment*)m_macEntity->GetDevice();
  return ue->GetTargetNode()->GetMacEntity()->GetRandomAccessManager()->isRachOpportunity();
}


int
UeRandomAccess::GetNbFailedAttempts(void)
{
  return m_nbFailedAttempts;
}

void
UeRandomAccess::SetNbFailedAttempts(int n)
{
  m_nbFailedAttempts = n;
}


int
UeRandomAccess::GetMaxFailedAttempts(void)
{
  return m_maxFailedAttempts;
}

void
UeRandomAccess::SetMaxFailedAttempts(int n)
{
  m_maxFailedAttempts = n;
}
