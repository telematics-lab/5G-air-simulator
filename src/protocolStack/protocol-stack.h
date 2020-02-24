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


#ifndef CONTROL_PLANE_PROTOCOL_STACK_H_
#define CONTROL_PLANE_PROTOCOL_STACK_H_

#include "../load-parameters.h"
#include "packet/Packet.h"
#include "packet/Header.h"
#include "packet/PacketTAGs.h"
#include "protocols/TransportProtocol.h"
#include <vector>
#include "nas/nas-entity.h"
#include "rrc/rrc-entity.h"
#include "rlc/rlc-entity.h"
#include "mac/mac-entity.h"
#include "mac/gnb-mac-entity.h"
#include "mac/henb-mac-entity.h"
#include "mac/ue-mac-entity.h"
#include "pdcp/pdcp-entity.h"
#include "application/application-entity.h"


class ProtocolStack
{
public:
  ProtocolStack(NetworkNode* device);
  virtual ~ProtocolStack();

  NasEntity* GetNasEntity (void);
  RrcEntity* GetRrcEntity (void);
  MacEntity* GetMacEntity (void);
  PdcpEntity* GetPdcpEntity (void);
  ApplicationEntity* GetApplicationEntity (void);

  void  SetNasEntity (NasEntity* e);
  void  SetRrcEntity (RrcEntity* e);
  void  SetMacEntity (MacEntity* e);
  void  SetPdcpEntity (PdcpEntity* e);
  void  SetApplicationEntity (ApplicationEntity* e);

private:

  NasEntity* m_nas; //NAS entity for this node
  RrcEntity* m_rrc; //RRC entity for this node
  PdcpEntity* m_pdcp; //PDCP Entity
  MacEntity* m_mac; //MAC entity for this node
  ApplicationEntity* m_app; //Application entity for this node
};

#endif /* CONTROL_PLANE_PROTOCOL_STACK_H_ */
