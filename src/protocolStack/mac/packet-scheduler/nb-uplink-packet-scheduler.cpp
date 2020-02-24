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

#include "../../../load-parameters.h"
#include "../../../device/GNodeB.h"
#include "nb-uplink-packet-scheduler.h"
#include "../mac-entity.h"
#include "../../packet/Packet.h"
#include "../../packet/packet-burst.h"
#include "../../../device/NetworkNode.h"
#include "../../../flows/radio-bearer.h"
#include "../../../protocolStack/rrc/rrc-entity.h"
#include "../../../flows/application/Application.h"
#include "../../../protocolStack/mac/nb-AMCModule.h"
#include "../../../phy/phy.h"
#include "../../../core/spectrum/bandwidth-manager.h"
#include "../../../flows/MacQueue.h"
#include "../../../utility/eesm-effective-sinr.h"
#include "../../../componentManagers/FrameManager.h"
#include "../../../protocolStack/mac/random-access/enb-nb-iot-random-access.h"
#include <algorithm>

nbUplinkPacketScheduler::nbUplinkPacketScheduler()
{
  SetMacEntity (0);
  CreateUsersToSchedule ();
  m_users.clear();
}

nbUplinkPacketScheduler::nbUplinkPacketScheduler(GnbMacEntity* mac)
{
  SetMacEntity (mac);
  CreateUsersToSchedule ();

  UserToSchedule u;
  u.m_userToSchedule = NULL;
  u.m_averageSchedulingGrant = -1;
  u.m_dataToTransmit = -1;
  u.m_listOfAllocatedRUs.clear();
  u.m_selectedMCS = -1;

  for (int i=0; i< mac->GetDevice ()->GetPhy ()->GetBandwidthManager ()->GetUlSubChannels ().size (); i++)
    {
      m_RUmap[0].push_back(-1);
      m_RUmap[1].push_back(-1);
      m_RUmap[2].push_back(-1);
      m_RUmap[3].push_back(-1);
      m_RUmap[4].push_back(-1);

      m_users.push_back(u);
    }
  m_users.clear();
}

nbUplinkPacketScheduler::~nbUplinkPacketScheduler()
{
  Destroy ();
  DeleteUsersToSchedule ();
}

void
nbUplinkPacketScheduler::CreateUsersToSchedule (void)
{
  m_usersToSchedule = new UsersToSchedule ();
}

void
nbUplinkPacketScheduler::DeleteUsersToSchedule (void)
{
  if (m_usersToSchedule != NULL)
    {
      if (m_usersToSchedule->size () > 0)
        {
          ClearUsersToSchedule ();
        }
      delete m_usersToSchedule;
    }
}

void
nbUplinkPacketScheduler::ClearUsersToSchedule ()
{
  for (auto record : *GetUsersToSchedule ())
    {
      delete record;
    }
  GetUsersToSchedule ()->clear ();
}

nbUplinkPacketScheduler::UsersToSchedule*
nbUplinkPacketScheduler::GetUsersToSchedule (void)
{
  return m_usersToSchedule;
}

void
nbUplinkPacketScheduler::SelectUsersToSchedule ()
{
  CreateUsersToSchedule ();
  GNodeB *node = GetMacEntity ()->GetDevice ();

DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG_VERBOSE)
  cout << "SelectUsersToSchedule... "
          " Users: " << node->GetUserEquipmentRecords ()->size ()<< endl;
DEBUG_LOG_END

  for (auto record : *node->GetUserEquipmentRecords ())
    {
      if (record->GetSchedulingRequest () > 0)
        {
          UserToSchedule* user = new UserToSchedule ();
          user->m_userToSchedule = record->GetUE ();
          if (record->GetUE ()->GetActivityTimeout() - Simulator::Init()->Now() <= 0.002)
            {
              record->GetUE ()->GetMacEntity()->SendSchedulingRequest();
            }
          user->m_dataToTransmit = record->GetSchedulingRequest ();
          user->m_listOfAllocatedRUs.clear ();
          user->m_selectedMCS = 0;
          user->m_transmittedData = 0;
          user->m_averageSchedulingGrant = record->GetSchedulingGrants ();

          GetUsersToSchedule ()->push_back (user);
        }
    }
DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG_VERBOSE)
  cout << "Users to be scheduled: " << GetUsersToSchedule ()->size () << endl;
DEBUG_LOG_END
}

void
nbUplinkPacketScheduler::DoSchedule (void)
{
DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG_VERBOSE)
  cout << "Start UPLINK packet scheduler" << endl;
DEBUG_LOG_END
  SelectUsersToSchedule ();
  //int freeTones = std::count(m_RUmap[0].begin(), m_RUmap[0].end(), -1);
  if (GetUsersToSchedule ()->size () > 0)// || freeTones<m_RUmap[0].size())
    {
      RUsAllocation ();
      DoStopSchedule ();
    }
  DeleteUsersToSchedule ();
}


void
nbUplinkPacketScheduler::DoStopSchedule (void)
{
  //Finalize the allocation
  NbIoTMapIdealControlMessage *msg = new NbIoTMapIdealControlMessage ();

  for (auto user : *GetUsersToSchedule ())
    {
      if (user->m_transmittedData > 0)
        {
          //create PDCCH messages
DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG_NB)
          cout << "\t UE " << user->m_userToSchedule->GetIDNetworkNode()
               << " has transmitted " << user->m_transmittedData << " BYTE"
               << endl;
DEBUG_LOG_END
          msg->AddNewRecord (user->m_listOfAllocatedRUs.at (0),
                             user->m_userToSchedule,
                             user->m_selectedMCS, user->m_subcarrier);
          //update users informations

          GNodeB::UserEquipmentRecord* record = user->m_userToSchedule->GetTargetNodeRecord();
          record->m_schedulingRequest -= user->m_transmittedData;

          if (record->m_schedulingRequest <= 0 )
            {
              record->m_schedulingRequest = 0;
            }
DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG_NB)
          cout << "\t UE " << record->GetUE()->GetIDNetworkNode()
               << " has still " << record->m_schedulingRequest << " BYTE to transmit"
               << endl;
DEBUG_LOG_END
          record->UpdateSchedulingGrants (user->m_dataToTransmit);

        }
    }

  if (msg->GetMessage()->size () > 0)
    {
DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG_NB)
      cout << "SEND UPLINK ALLOCATION MAP MESSAGE" << endl;;
DEBUG_LOG_END
      GetMacEntity ()->GetDevice ()->GetPhy ()->SendIdealControlMessage (msg);
    }
}

void nbUplinkPacketScheduler::printMap()
{
  cout << "- ID\t";
  for (auto id : m_RUmap[0])
    {
      cout << id << "\t";
    }
  cout << endl << "- TTI\t";
  for (auto tti : m_RUmap[1])
    {
      cout << tti << "\t";
    }
  cout << endl << "- MCS\t";
  for (auto mcs : m_RUmap[2])
    {
      cout << mcs << "\t";
    }
  cout << endl << "- TBS\t";
  for (auto tbs : m_RUmap[3])
    {
      cout << tbs << "\t";
    }
  cout << endl << "- RU\t";
  for (auto ru : m_RUmap[4])
    {
      cout << ru << "\t";
    }
  cout << endl;
}

