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


#include "handover-entity.h"
#include "../../../device/NetworkNode.h"
#include "../../../device/UserEquipment.h"
#include "ho-manager.h"
#include "position-based-ho-manager.h"
#include "power-based-ho-manager.h"

HandoverEntity::HandoverEntity()
{
  m_device = nullptr;
  m_hoManager = new PowerBasedHoManager ();
  m_detachTime = 0.030;
}

HandoverEntity::~HandoverEntity()
{
  delete m_hoManager;
}

void
HandoverEntity::SetDevice (NetworkNode* d)
{
  m_device = d;
}

NetworkNode*
HandoverEntity::GetDevice ()
{
  return m_device;
}

void
HandoverEntity::SetHoManager (HoManager *h)
{
  m_hoManager = h;
}

HoManager*
HandoverEntity::GetHoManager (void)
{
  return m_hoManager;
}

void
HandoverEntity::SetDetachTime (double t)
{
  m_detachTime = t;
}

double
HandoverEntity::GetDetachTime (void)
{
  return m_detachTime;
}


bool
HandoverEntity::CheckHandoverNeed (UserEquipment* ue)
{
  return GetHoManager ()->CheckHandoverNeed (ue);
}
