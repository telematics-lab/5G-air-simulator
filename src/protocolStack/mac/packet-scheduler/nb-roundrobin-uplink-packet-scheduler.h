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
 * Author: Sergio Martiradonna <sergio.martiradonna@poliba.it>
 */

#ifndef NB_ROUNDROBIN_UPLINK_PACKET_SCHEDULER_H_
#define NB_ROUNDROBIN_UPLINK_PACKET_SCHEDULER_H_

#include "nb-uplink-packet-scheduler.h"

class nbRoundRobinUplinkPacketScheduler : public nbUplinkPacketScheduler {
public:
  nbRoundRobinUplinkPacketScheduler (GnbMacEntity* mac);
  virtual ~nbRoundRobinUplinkPacketScheduler();
  void printMap();
  void RUsAllocation ();
  void printQ();

private:
  int m_roundRobinId;
  vector< vector<int> > m_RUmap = vector< vector<int> >(5);
  vector<UserToSchedule> m_users;
  vector<UserToSchedule>  m_queue;
};

#endif /* NB_ROUNDROBIN_UPLINK_PACKET_SCHEDULER_H_ */
