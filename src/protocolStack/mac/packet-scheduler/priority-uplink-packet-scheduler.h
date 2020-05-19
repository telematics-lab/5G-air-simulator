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
 * Author: Gabriele Roncella <gabriele.roncella@student.unisi.it>
 * Author: Dinesh Tamang <dinesh.tamang@student.unisi.it>
 */

#ifndef SRC_PROTOCOLSTACK_MAC_PACKET_SCHEDULER_PRIORITY_UPLINK_PACKET_SCHEDULER_H_
#define SRC_PROTOCOLSTACK_MAC_PACKET_SCHEDULER_PRIORITY_UPLINK_PACKET_SCHEDULER_H_

#include "uplink-packet-scheduler.h"

class PriorityUplinkPacketScheduler : public UplinkPacketScheduler
{
public:
    PriorityUplinkPacketScheduler (GnbMacEntity*);
    virtual ~PriorityUplinkPacketScheduler();
        
    void SelectUsersToSchedule (void);
    void DoSchedule (void);
    void RBsAllocation ();
    
    virtual double ComputeSchedulingMetric (UserToSchedule* user, int subchannel);
    
private:
    int m_index2;
};



#endif /* SRC_PROTOCOLSTACK_MAC_PACKET_SCHEDULER_PRIORITY_UPLINK_PACKET_SCHEDULER_H_ */
