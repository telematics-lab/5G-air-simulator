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


#include "PacketTAGs.h"

PacketTAGs::PacketTAGs()
    : PacketTAGs::PacketTAGs(PacketTAGs::APPLICATION_TYPE_INFINITE_BUFFER, 0,  0, 0)
{}

PacketTAGs::PacketTAGs(PacketTAGs::ApplicationType type,
                       int frameNumber,
                       int startFrame,
                       int endFrame)
{
  m_type = type;
  m_frameNumber = frameNumber;
  m_startByte = startFrame;
  m_endBytes = endFrame;
}

void
PacketTAGs::SetFrameNumber (int frameNumber)
{
  m_frameNumber = frameNumber;
}

int
PacketTAGs::GetFrameNuber (void) const
{
  return m_frameNumber;
}

void
PacketTAGs::SetStartByte (int startByte)
{
  m_startByte = startByte;
}

int
PacketTAGs::GetStartByte (void) const
{
  return m_startByte;
}

void
PacketTAGs::SetEndByte (int endByte)
{
  m_endBytes = endByte;
}

int
PacketTAGs::GetEndByte (void) const
{
  return m_endBytes;
}

void
PacketTAGs::SetApplicationType (PacketTAGs::ApplicationType type)
{
  m_type = type;
}

PacketTAGs::ApplicationType
PacketTAGs::GetApplicationType (void)
{
  return m_type;
}

void
PacketTAGs::SetApplicationSize (int s)
{
  m_applicationSize = s;
}

int
PacketTAGs::GetApplicationSize ()
{
  return m_applicationSize;
}
