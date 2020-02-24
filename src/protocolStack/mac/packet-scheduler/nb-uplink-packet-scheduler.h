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

#ifndef NB_UPLINKPACKETSCHEDULER_H_
#define NB_UPLINKPACKETSCHEDULER_H_

#include "uplink-packet-scheduler.h"

class nbUplinkPacketScheduler: public UplinkPacketScheduler {
public:
  nbUplinkPacketScheduler();
  nbUplinkPacketScheduler(GnbMacEntity* mac);
  virtual ~nbUplinkPacketScheduler();
  virtual double ComputeSchedulingMetric (UserToSchedule* user, int subchannel){return 0;};

  struct UserToSchedule
    {
      UserEquipment* m_userToSchedule;
      int m_transmittedData; //bytes
      int m_dataToTransmit; //bytes
      double m_averageSchedulingGrant; // in bytes
      int m_subcarrier;

      vector<int> m_listOfAllocatedRUs;
      int m_selectedMCS;

      bool operator==(const UserToSchedule& u) const
        {
          return (m_userToSchedule->GetIDNetworkNode() == u.m_userToSchedule->GetIDNetworkNode());
        }
    };

  typedef vector<UserToSchedule*> UsersToSchedule;

  void CreateUsersToSchedule (void);
  void DeleteUsersToSchedule (void);
  void ClearUsersToSchedule ();
  UsersToSchedule* GetUsersToSchedule (void);

  void SelectUsersToSchedule (void);

  void DoSchedule (void);
  void DoStopSchedule (void);

  virtual void RUsAllocation () = 0;
  void printMap (void);

private:
  UsersToSchedule* m_usersToSchedule;
  vector< vector<int> > m_RUmap = vector< vector<int> >(5);
  vector<UserToSchedule> m_users;
};

#endif /* NB_UPLINKPACKETSCHEDULER_H_ */
