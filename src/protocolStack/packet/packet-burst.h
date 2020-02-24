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


#ifndef PACKET_BURST_H
#define PACKET_BURST_H

#include <stdint.h>
#include <list>
#include <memory>
#include "Packet.h"


class PacketBurst
{
  /*
   * This class implement a burst as a list of packets
   */

public:
  PacketBurst (void) = default;
  virtual ~PacketBurst (void);

  shared_ptr<PacketBurst> Copy (void);
  void AddPacket (Packet* packet);
  list<Packet* > GetPackets (void) const;
  uint32_t GetNPackets (void) const;
  uint32_t GetSize (void) const;

  list<Packet* >::const_iterator Begin (void) const;
  list<Packet* >::const_iterator End (void) const;

private:
  list<Packet* > m_packets;
};


#endif /* PACKET_BURST */
