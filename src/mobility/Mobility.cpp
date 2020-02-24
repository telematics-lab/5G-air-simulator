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


#include "Mobility.h"
#include "../componentManagers/NetworkManager.h"
#include "../networkTopology/Cell.h"
#include "../load-parameters.h"

Mobility::Mobility()
{
  m_AbsolutePosition = nullptr;
  m_WrapAroundPosition = nullptr;
}

Mobility::~Mobility()
{
  delete m_AbsolutePosition;
}

void
Mobility::SetDevice (NetworkNode *device)
{
  m_device = device;
}

NetworkNode*
Mobility::GetDevice (void) const
{
  return m_device;
}

void
Mobility::SetMobilityModel(MobilityModel model)
{
  m_mobilityModel = model;
}

Mobility::MobilityModel
Mobility::GetMobilityModel(void) const
{
  return m_mobilityModel;
}

void
Mobility::SetAbsolutePosition (CartesianCoordinates *position)
{
  if (position == nullptr)
    {
      m_AbsolutePosition = nullptr;
      return;
    }

  if (m_AbsolutePosition == nullptr)
    {
      m_AbsolutePosition = new CartesianCoordinates ();
    }

  m_AbsolutePosition->SetCoordinateX (position->GetCoordinateX ());
  m_AbsolutePosition->SetCoordinateY (position->GetCoordinateY ());
  m_AbsolutePosition->SetCoordinateZ (position->GetCoordinateZ ());
  m_AbsolutePosition->SetFloorHeight (position->GetFloorHeight ());
}

CartesianCoordinates*
Mobility::GetAbsolutePosition (void) const
{
  return m_AbsolutePosition;
}

CartesianCoordinates*
Mobility::GetWrapAroundPosition (CartesianCoordinates* center)
{
  if (m_AbsolutePosition == nullptr)
    {
      m_WrapAroundPosition = nullptr;
    }

  if (m_WrapAroundPosition == nullptr)
    {
      m_WrapAroundPosition = new CartesianCoordinates ();
    }

  NetworkManager* nm = NetworkManager::Init();
  double wrapDistX = nm->GetWrapAroundDistanceX();
  double wrapDistY = nm->GetWrapAroundDistanceY();

  if(NetworkManager::Init()->GetWrapAroundType()==NetworkManager::WRAPAROUND_NONE)
    {
      return m_AbsolutePosition;
    }
  else if(nm->GetWrapAroundType()==NetworkManager::WRAPAROUND_X_AXIS && wrapDistX!=0 )
    {
      *m_WrapAroundPosition = *m_AbsolutePosition;
      double currentX = m_WrapAroundPosition->GetCoordinateX();
      double centerX = center->GetCoordinateX();

      if (currentX - centerX > wrapDistX / 2)
        {
          int shifts = floor( (currentX - centerX + wrapDistX / 2) / wrapDistX );
          double newX = currentX - wrapDistX * shifts;
DEBUG_LOG_START_1(SIM_ENV_WRAPAROUND)
          cout << "WRAPAROUND X FROM " << currentX << " TO " << newX << " CENTER " << centerX << " SHIFTS " << -shifts << endl;
DEBUG_LOG_END
          m_WrapAroundPosition->SetCoordinateX(newX);
        }
      else if (currentX - centerX < -wrapDistX / 2)
        {
          int shifts = floor( (centerX - currentX + wrapDistX / 2) / wrapDistX );
          double newX = currentX + wrapDistX * shifts;
DEBUG_LOG_START_1(SIM_ENV_WRAPAROUND)
          cout << "WRAPAROUND X FROM " << currentX << " TO " << newX << " CENTER " << centerX << " SHIFTS " << shifts << endl;
DEBUG_LOG_END
          m_WrapAroundPosition->SetCoordinateX(newX);
        }
      else
        {
        }

      return m_WrapAroundPosition;
    }
  else
    {
      cout << "Error: unsupported wraparound type selected:" << nm->GetWrapAroundType() <<endl;
      exit(1);
    }
}

void
Mobility::DeleteAbsolutePosition (void)
{
  delete  m_AbsolutePosition;
}

void
Mobility::SetSpeed (int speed)
{
  m_speed = speed;
}

int
Mobility::GetSpeed (void) const
{
  return m_speed;
}

void
Mobility::SetSpeedDirection (double speedDirection)
{
  m_speedDirection = speedDirection;
}

double
Mobility::GetSpeedDirection (void) const
{
  return m_speedDirection;
}

void
Mobility::SetPositionLastUpdate (double time)
{
  m_positionLastUpdate = time;
}

double
Mobility::GetPositionLastUpdate (void) const
{
  return m_positionLastUpdate;
}

void
Mobility::SetHandover (bool handover)
{
  m_handover = handover;
}

bool
Mobility::GetHandover (void) const
{
  return m_handover;
}

void
Mobility::SetLastHandoverTime (double lastHOtime)
{
  m_handoverLastRun = lastHOtime;
}

double
Mobility::GetLastHandoverTime (void) const
{
  return m_handoverLastRun;
}

double
Mobility::GetTopologyBorder (void)
{
  int nbCell = NetworkManager::Init()->GetNbCell();

  switch (nbCell)
    {
    case 1:
      return (NetworkManager::Init()->GetCellByID (0)->GetRadius () * 1000);
      break;
    case 7:
      return ((2.6 * NetworkManager::Init()->GetCellByID (0)->GetRadius ()) * 1000);
      break;
    case 19:
      return ((4. * NetworkManager::Init()->GetCellByID (0)->GetRadius ()) * 1000);
      break;
    default:
      return 1000.0;
      break;
    }
}
