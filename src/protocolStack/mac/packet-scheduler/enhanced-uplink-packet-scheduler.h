/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012
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
 * Author: Khaled Elsayed <khaled@ieee.org>
 */


#ifndef FME_UPLINK_PACKET_SCHEDULER_H_
#define FME_UPLINK_PACKET_SCHEDULER_H_

#include "uplink-packet-scheduler.h"

class EnhancedUplinkPacketScheduler : public UplinkPacketScheduler
{
public:
  EnhancedUplinkPacketScheduler ();
  virtual ~EnhancedUplinkPacketScheduler();

  virtual double ComputeSchedulingMetric (UserToSchedule* user, int subchannel);

  virtual void RBsAllocation ();
};

#endif /* FME_UPLINK_PACKET_SCHEDULER_H_ */
