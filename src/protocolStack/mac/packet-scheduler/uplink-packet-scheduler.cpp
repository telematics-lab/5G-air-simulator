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

#include "../../../device/GNodeB.h"
#include "uplink-packet-scheduler.h"
#include "../mac-entity.h"
#include "../../packet/Packet.h"
#include "../../packet/packet-burst.h"
#include "../../../device/UserEquipment.h"
#include "../../../flows/radio-bearer.h"
#include "../../../protocolStack/rrc/rrc-entity.h"
#include "../../../flows/application/Application.h"
#include "../../../protocolStack/mac/AMCModule.h"
#include "../../../phy/phy.h"
#include "../../../core/spectrum/bandwidth-manager.h"
#include "../../../flows/MacQueue.h"
#include "../../../utility/eesm-effective-sinr.h"
#include "../gnb-mac-entity.h"

UplinkPacketScheduler::~UplinkPacketScheduler()
{
  Destroy ();
  DeleteUsersToSchedule ();
}

void
UplinkPacketScheduler::CreateUsersToSchedule (void)
{
  m_usersToSchedule = new UsersToSchedule ();
}

void
UplinkPacketScheduler::DeleteUsersToSchedule (void)
{
  if (m_usersToSchedule != nullptr)
    {
      if (m_usersToSchedule->size () > 0)
        ClearUsersToSchedule ();
      delete m_usersToSchedule;
    }
}

void
UplinkPacketScheduler::ClearUsersToSchedule ()
{
  for (auto record : *GetUsersToSchedule())
    {
      delete record;
    }
  GetUsersToSchedule ()->clear ();
}

UplinkPacketScheduler::UsersToSchedule*
UplinkPacketScheduler::GetUsersToSchedule (void)
{
  return m_usersToSchedule;
}

void
UplinkPacketScheduler::SelectUsersToSchedule ()
{
  CreateUsersToSchedule ();

  GNodeB *node = GetMacEntity ()->GetDevice ();

DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
  cout << "UplinkPacketScheduler::SelectUsersToSchedule () "
            " users " << node->GetUserEquipmentRecords ()->size ()<< endl;

  cout << "LIST: "<< endl;
DEBUG_LOG_END

  for (auto record : *node->GetUserEquipmentRecords())
    {
      if (record->GetSchedulingRequest () > 0)
        {
          UserToSchedule* user = new UserToSchedule ();
          user->m_userToSchedule = record->GetUE ();
          user->m_dataToTransmit = record->GetSchedulingRequest ();
          user->m_listOfAllocatedRBs.clear ();
          user->m_selectedMCS = 0;
          user->m_transmittedData = 0;
          user->m_channelContition = record->GetCQI ();
          user->m_averageSchedulingGrant = record->GetSchedulingGrants ();

DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
  cout << "\t UE "
       << user->m_userToSchedule->GetIDNetworkNode()
       << " - DATA "
       << user->m_dataToTransmit
       << endl;
DEBUG_LOG_END

          GetUsersToSchedule ()->push_back (user);
        }
    }
DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
  cout << "users to be scheduled = " << GetUsersToSchedule ()->size () << endl;
DEBUG_LOG_END
}

void
UplinkPacketScheduler::DoSchedule (void)
{
DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
  cout << "Start UPLINK packet scheduler for node "
            << GetMacEntity ()->GetDevice ()->GetIDNetworkNode()<< endl;
DEBUG_LOG_END

  SelectUsersToSchedule ();

  if (GetUsersToSchedule ()->size () > 0)
    {
      RBsAllocation ();
      DoStopSchedule ();
    }

  DeleteUsersToSchedule ();
}

void
UplinkPacketScheduler::RBsAllocation ()
{
DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
  cout << " ---- UL RBs Allocation";
DEBUG_LOG_END

  GnbRandomAccess* randomAccessManager = GetMacEntity ()->GetRandomAccessManager ();

  vector<int> reservedRBs = randomAccessManager->GetReservedSubChannels ();


  UsersToSchedule *users = GetUsersToSchedule ();
  int nbOfRBs = GetMacEntity ()->GetDevice ()->GetPhy ()->GetBandwidthManager ()->GetUlSubChannels ().size ();

  //create a matrix of flow metrics
  vector< vector<double> > metrics;
  metrics.resize(nbOfRBs);
  for (int i = 0; i < nbOfRBs; i++)
    {
      metrics.at(i).resize(users->size ());
      for (int j = 0; j < (int)users->size (); j++)
        {
          metrics.at(i).at(j) = ComputeSchedulingMetric (users->at (j), i);
        }
    }

DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
  cout << ", available RBs " << nbOfRBs << ", users " << users->size () << endl;
  for (int ii = 0; ii < (int)users->size (); ii++)
    {
      cout << "\t metrics for user "
                << users->at (ii)->m_userToSchedule->GetIDNetworkNode ();
      for (int jj = 0; jj < nbOfRBs; jj++)
        {
          cout << " " << metrics[jj][ii];
        }
      cout << endl;
    }
DEBUG_LOG_END

  //RBs allocation
  int s = 0;
  vector<bool>* isScheduled = new vector<bool>(users->size ());
  fill(isScheduled->begin(), isScheduled->end(), false);
  while (s < nbOfRBs)
    {
      if(find(reservedRBs.begin(), reservedRBs.end(), s)!=reservedRBs.end())
        {
          s++;
          continue;
        }

      double targetMetric = 0;
      bool RBIsAllocated = false;
      UserToSchedule* scheduledUser;
      int scheduledUserIndex;

      for (int k = 0; k < (int)users->size (); k++)
        {
          if (metrics.at(s).at(k) > targetMetric && isScheduled->at(k) == false)
            {
              targetMetric = metrics.at(s).at(k);
              RBIsAllocated = true;
              scheduledUser = users->at (k);
              scheduledUserIndex = k;
            }
        }

      if (RBIsAllocated)
        {
          int dataToTransmit = scheduledUser->m_dataToTransmit;
          int availableRBs = nbOfRBs - s;
          AMCModule* amc = scheduledUser->m_userToSchedule->GetProtocolStack()->GetMacEntity()->GetAmcModule();
          vector<double> sinrs;
          for (auto c : scheduledUser->m_channelContition)
            {
              sinrs.push_back (amc->GetSinrFromCQI (c));
            }
          int mcs = amc->GetMCSFromSinrVector (sinrs);
          int tbs = (amc->GetTBSizeFromMCS (mcs, availableRBs)) / 8;

          if (tbs <= dataToTransmit)
            {
              scheduledUser->m_transmittedData = tbs;

              for (int ss = s; ss < (s+ availableRBs); ss++)
                {
                  scheduledUser->m_listOfAllocatedRBs.push_back (ss);
                }
              s = nbOfRBs;
              scheduledUser->m_selectedMCS = mcs;
            }
          else
            {
              int rbsNeeded = (ceil) ((float)scheduledUser->m_dataToTransmit /
                                       (amc->GetTBSizeFromMCS (mcs, 1) / 8));
              scheduledUser->m_transmittedData = amc->GetTBSizeFromMCS (mcs, rbsNeeded) / 8;

              for (int ss = s; ss < (s+rbsNeeded); ss++)
                {
                  scheduledUser->m_listOfAllocatedRBs.push_back (ss); // XXX it doesn't describe correctly assigned PRBs
                }
              s += rbsNeeded;
              scheduledUser->m_selectedMCS = mcs;
              isScheduled->at(scheduledUserIndex) = true;
            }
        }
      else
        {
          s++;
        }
    }
  delete isScheduled;
}

void
UplinkPacketScheduler::DoStopSchedule (void)
{
  //Finalize the allocation
  PdcchMapIdealControlMessage *pdcchMsg = new PdcchMapIdealControlMessage ();

  for (auto user : *GetUsersToSchedule())
    {
      if (user->m_transmittedData > 0)
        {
DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG_NB)
          cout << "\t UE " << user->m_userToSchedule->GetIDNetworkNode()
               << " has transmitted " << user->m_transmittedData << " BYTE"
               << endl;
DEBUG_LOG_END

          int harqPid = 0;
          //create PDCCH messages
          for (int rb = 0; rb < (int)user->m_listOfAllocatedRBs.size (); rb++ )
            {
              pdcchMsg->AddNewRecord (PdcchMapIdealControlMessage::UPLINK,
                                      user->m_listOfAllocatedRBs.at (rb),
                                      user->m_userToSchedule,
                                      user->m_selectedMCS,
                                      harqPid);
            }

          //update users informations
          GNodeB::UserEquipmentRecord* record = user->m_userToSchedule->GetTargetNodeRecord();
          record->m_schedulingRequest -= user->m_transmittedData;
          if (record->m_schedulingRequest < 0 )
            {
              record->m_schedulingRequest = 0;
            }
          record->UpdateSchedulingGrants (user->m_dataToTransmit);
DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG_NB)
          cout << "\t UE " << user->m_userToSchedule->GetIDNetworkNode ()
               << " has still " << record->m_schedulingRequest << " BYTE to transmit"
               << endl;
DEBUG_LOG_END

        }
    }

  if (pdcchMsg->GetMessage()->size () > 0)
    {
DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG_NB)
      cout << "SEND UPLINK ALLOCATION MAP MESSAGE" << endl;
DEBUG_LOG_END
      GetMacEntity ()->GetDevice ()->GetPhy ()->SendIdealControlMessage (pdcchMsg);
    }
}
