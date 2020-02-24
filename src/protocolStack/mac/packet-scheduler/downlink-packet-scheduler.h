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


#ifndef DOWNLINKPACKETSCHEDULER_H_
#define DOWNLINKPACKETSCHEDULER_H_

#include "packet-scheduler.h"

class DownlinkPacketScheduler: public PacketScheduler
{
public:
  DownlinkPacketScheduler();
  virtual ~DownlinkPacketScheduler();

  void SelectFlowsToSchedule ();

  virtual void DoSchedule (void);
  virtual void DoStopSchedule (void);

  virtual void RBsAllocation ();
  virtual double ComputeSchedulingMetric (RadioBearer *bearer, double spectralEfficiency, int subChannel) = 0;

  void UpdateAverageTransmissionRate (void);

  void SetMaxUsersPerRB (int users);
  int GetMaxUsersPerRB (void);

protected:
  int m_maxUsersPerRB;
};

#endif /* DOWNLINKPACKETSCHEDULER_H_ */
