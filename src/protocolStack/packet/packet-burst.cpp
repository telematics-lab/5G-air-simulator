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


#include <stdint.h>
#include <list>
#include "packet-burst.h"


PacketBurst::~PacketBurst (void)
{
  for (auto packet : m_packets )
    {
      delete packet;
    }
  m_packets.clear();
}

shared_ptr<PacketBurst>
PacketBurst::Copy (void)
{
  shared_ptr<PacketBurst> pb = make_shared<PacketBurst> ();
  for ( auto packet : GetPackets ())
    {
      pb->AddPacket (packet->Copy());
    }
  return pb;
}

void
PacketBurst::AddPacket (Packet* packet)
{
  if (packet)
    {
      m_packets.push_back (packet);
    }
}

list<Packet*>
PacketBurst::GetPackets (void) const
{
  return m_packets;
}

uint32_t
PacketBurst::GetNPackets (void) const
{
  return m_packets.size ();
}

uint32_t
PacketBurst::GetSize (void) const
{
  uint32_t size = 0;
  for ( auto packet : m_packets )
    {
      size += packet->GetSize ();
    }
  return size;
}

list<Packet* >::const_iterator
PacketBurst::Begin (void) const
{
  return m_packets.begin ();
}

list<Packet* >::const_iterator
PacketBurst::End (void) const
{
  return m_packets.end ();
}



