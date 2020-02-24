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


#ifndef UMRLCENTITY_H_
#define UMRLCENTITY_H_

#include "rlc-entity.h"
#include <vector>

class PacketBurst;

class UmRlcEntity: public RlcEntity
{
public:
  UmRlcEntity();
  virtual ~UmRlcEntity();

  virtual shared_ptr<PacketBurst> TransmissionProcedure (int availableBytes);
  virtual void ReceptionProcedure (Packet* p);

  void ClearIncomingPackets (void);

private:
  vector <Packet*> m_incomingPacket;
};

#endif /* UMRLCENTITY_H_ */
