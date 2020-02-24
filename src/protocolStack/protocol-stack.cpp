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


#include "protocol-stack.h"
#include "../device/NetworkNode.h"

ProtocolStack::ProtocolStack(NetworkNode* device)
{
  m_rrc = new RrcEntity ();
  m_rrc->SetDevice (device);

  m_pdcp = new PdcpEntity ();

  m_nas = new NasEntity ();
  m_nas->SetDevice (device);

  if (device->GetNodeType() == NetworkNode::TYPE_GNODEB)
    {
      m_mac = new GnbMacEntity ();
      m_mac->SetDevice(device);
    }
  else if (device->GetNodeType() == NetworkNode::TYPE_HOME_BASE_STATION)
    {
      m_mac = new HenbMacEntity ();
      m_mac->SetDevice(device);
    }

  else if (
    device->GetNodeType() == NetworkNode::TYPE_UE ||
    device->GetNodeType() == NetworkNode::TYPE_MULTICAST_DESTINATION
  )
    {
      m_mac = new UeMacEntity (device);
    }
  else
    {
      //ADD other devices
      m_mac = nullptr;
    }

  m_app = new ApplicationEntity();
}

ProtocolStack::~ProtocolStack()
{
  delete m_rrc;
  delete m_mac;
  delete m_nas;
  delete m_pdcp;
  delete m_app;
}

RrcEntity*
ProtocolStack::GetRrcEntity (void)
{
  return m_rrc;
}

MacEntity*
ProtocolStack::GetMacEntity (void)
{
  return m_mac;
}

NasEntity*
ProtocolStack::GetNasEntity (void)
{
  return m_nas;
}

PdcpEntity*
ProtocolStack::GetPdcpEntity (void)
{
  return m_pdcp;
}

ApplicationEntity*
ProtocolStack::GetApplicationEntity (void)
{
  return m_app;
}

void
ProtocolStack::SetNasEntity (NasEntity* e)
{
  m_nas = e;
}

void
ProtocolStack::SetRrcEntity (RrcEntity* e)
{
  m_rrc = e;
}

void
ProtocolStack::SetMacEntity (MacEntity* e)
{
  m_mac = e;
}

void
ProtocolStack::SetPdcpEntity (PdcpEntity* e)
{
  m_pdcp = e;
}

void
ProtocolStack::SetApplicationEntity (ApplicationEntity* e)
{
  m_app = e;
}
