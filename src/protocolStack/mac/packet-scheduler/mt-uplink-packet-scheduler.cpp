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

#include "mt-uplink-packet-scheduler.h"
#include "../mac-entity.h"
#include "../../packet/Packet.h"
#include "../../packet/packet-burst.h"
#include "../../../device/NetworkNode.h"
#include "../../../flows/radio-bearer.h"
#include "../../../protocolStack/rrc/rrc-entity.h"
#include "../../../flows/application/Application.h"
#include "../../../device/GNodeB.h"
#include "../../../protocolStack/mac/AMCModule.h"
#include "../../../phy/phy.h"
#include "../../../core/spectrum/bandwidth-manager.h"
#include "../../../core/idealMessages/ideal-control-messages.h"
#include "../../../flows/QoS/QoSParameters.h"
#include "../../../flows/MacQueue.h"

MaximumThroughputUplinkPacketScheduler::MaximumThroughputUplinkPacketScheduler()
{
  SetMacEntity (nullptr);
  CreateUsersToSchedule ();
}

MaximumThroughputUplinkPacketScheduler::~MaximumThroughputUplinkPacketScheduler()
{
  Destroy ();
}

double
MaximumThroughputUplinkPacketScheduler::ComputeSchedulingMetric (UserToSchedule* user, int subchannel)
{
  double metric;

  int channelCondition = user->m_channelContition.at (subchannel);
  AMCModule* amc = user->m_userToSchedule->GetProtocolStack()->GetMacEntity()->GetAmcModule();
  double spectralEfficiency = amc->GetSinrFromCQI (channelCondition);

  metric = spectralEfficiency * 180000;

  return metric;
}
