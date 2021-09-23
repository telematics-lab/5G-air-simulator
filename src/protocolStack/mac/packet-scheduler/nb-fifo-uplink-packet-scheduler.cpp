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

#include "../../../device/GNodeB.h"
#include "nb-fifo-uplink-packet-scheduler.h"
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

nbFifoUplinkPacketScheduler::nbFifoUplinkPacketScheduler(GnbMacEntity* mac)
{
  SetMacEntity (mac);
  CreateUsersToSchedule ();
  UserToSchedule u;
  u.m_userToSchedule = nullptr;
  u.m_averageSchedulingGrant = -1;
  u.m_dataToTransmit = -1;
  u.m_listOfAllocatedRUs.clear();
  u.m_selectedMCS = -1;

  for (unsigned long int i=0; i< mac->GetDevice ()->GetPhy ()->GetBandwidthManager ()->GetUlSubChannels ().size (); i++)
    {
      m_RUmap[0].push_back(-1);
      m_RUmap[1].push_back(-1);
      m_RUmap[2].push_back(-1);
      m_RUmap[3].push_back(-1);
      m_RUmap[4].push_back(-1);

      m_users.push_back(u);
    }
  m_queue.clear();
}

nbFifoUplinkPacketScheduler::~nbFifoUplinkPacketScheduler()
{
  Destroy ();
}

void
nbFifoUplinkPacketScheduler::RUsAllocation ()
{
  int currentSF = FrameManager::Init()->GetTTICounter();
  int ttiLength = FrameManager::Init()->getTTILength();
  GnbNbIoTRandomAccess* gnbRam = (GnbNbIoTRandomAccess*) GetMacEntity()->GetRandomAccessManager() ;

  UsersToSchedule *users = GetUsersToSchedule ();

  if (currentSF % ttiLength == 0 && !(gnbRam->isRachOpportunity()))
    {
DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG_LOG)
      cout <<"LOG_USERS TTI " << currentSF << " UE " << users->size () << endl;
DEBUG_LOG_END

      int freeTones = count(m_RUmap[0].begin(), m_RUmap[0].end(), -1);

DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG_FIFO)
      cout <<"SF: " << currentSF << " -> NB FIFO UL RUs Allocation - Users: " << users->size ();
      cout << " - Already Scheduled: " << m_RUmap[0].size() - freeTones;
      if (freeTones > 0)
        {
          cout << " - Resource(s) Available: " << freeTones << endl;
        }
      else
        {
          cout << " - Resources NOT Available! " << endl;
        }
DEBUG_LOG_END

      int tones = GetMacEntity ()->GetDevice ()->GetPhy ()->GetBandwidthManager ()->GetTones();
      bool userAlreadyScheduled;
      bool userAlreadyQueued;
      int id;

      if (m_queue.size()==0)
        {
DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG_FIFO)
          printQ();
          cout << "Queue empty! Filling..." << endl;
DEBUG_LOG_END

          for (auto scheduledUser : *users)
            {
              m_queue.push_back(* scheduledUser);
            }
        }
      else
        {
DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG_FIFO)
          cout << "Queue not empty! Checking users..." << endl;
DEBUG_LOG_END
          for (auto scheduledUser : *users)
            {
              id = scheduledUser->m_userToSchedule->GetIDNetworkNode();
              vector<UserToSchedule>::iterator i = find(m_queue.begin(), m_queue.end(), *scheduledUser);
              vector<UserToSchedule>::size_type idx = i-m_queue.begin();
              userAlreadyQueued = i != m_queue.end();
              if (!userAlreadyQueued)
                {
DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG_FIFO)
                  cout << " \t UE " << id << " not queued" << endl;
DEBUG_LOG_END
                  m_queue.push_back(* scheduledUser);
                }
              else
                {
DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG_FIFO)
                  cout << " \t UE " << id << " already queued. Updating..." << endl;
DEBUG_LOG_END
                  m_queue.at(idx) = *scheduledUser;
                }
            }
        }


DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG_FIFO)
  printQ();
DEBUG_LOG_END

  if (count(m_RUmap[0].begin(), m_RUmap[0].end(), -1)>0)
    {
      for ( auto scheduledUser : m_queue )
        {
          id = scheduledUser.m_userToSchedule->GetIDNetworkNode();
          userAlreadyScheduled = find(m_RUmap[0].begin(), m_RUmap[0].end(), id) != m_RUmap[0].end();

          if (!userAlreadyScheduled)
            {
              if (std::count(m_RUmap[0].begin(), m_RUmap[0].end(), -1)>0)
                {
                  int dataToTransmit = scheduledUser.m_dataToTransmit;
                  CartesianCoordinates* userPosition = scheduledUser.m_userToSchedule->GetMobilityModel()->GetAbsolutePosition();
                  CartesianCoordinates* cellPosition = scheduledUser.m_userToSchedule->GetCell()->GetCellCenterPosition();
                  double cellRadius = scheduledUser.m_userToSchedule->GetCell()->GetRadius();
                  double distance = userPosition->GetDistance(cellPosition);
                  int mcs = GetMacEntity ()->GetNbAmcModule ()->GetMCSfromDistance (distance, cellRadius*1000, tones);

DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG_FIFO)
                  cout << " \t Assigning resources to user " << id << endl;
                  cout << " \t User Distance = " << distance << endl;
                  cout << " \t Data to transmit [byte] = "<< dataToTransmit << endl;
                  cout << " \t MCS = " << mcs;
DEBUG_LOG_END

                  int nru = GetMacEntity ()->GetNbAmcModule ()->GetNbOfRUsFromSize(mcs, dataToTransmit*8);
                  int tbs = (GetMacEntity ()->GetNbAmcModule ()->GetTBSizeFromMCS (mcs, nru)) / 8;

DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG_FIFO)
                  cout << " - TBS [byte] = " << tbs << " - NRU = " << nru << endl;
DEBUG_LOG_END

                  vector<int>::iterator i = find(m_RUmap[0].begin(), m_RUmap[0].end(), -1); //find first free subcarrier
                  vector<int>::size_type idx = i-m_RUmap[0].begin();

                  m_RUmap[0][idx] = id;
                  m_RUmap[1][idx] = nru;
                  m_RUmap[2][idx] = mcs;
                  m_RUmap[3][idx] = tbs;
                  m_RUmap[4][idx] = nru;
                  m_users[idx] = scheduledUser;
                  scheduledUser.m_transmittedData = 0;
                }
            }
          else
            {

DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG_FIFO)
              cout << "\t User " << scheduledUser.m_userToSchedule->GetIDNetworkNode() <<" has been already scheduled." << endl;
DEBUG_LOG_END
            }
        }
DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG_FIFO)
      printMap();
DEBUG_LOG_END
    }
  else
    {
DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG_FIFO)
      cout << "- Resources NOT Available" << endl;
DEBUG_LOG_END
    }

DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG_LOG)
      cout << "LOG_RESOURCES TTI " << currentSF
           << " SUBCARRIERS " << m_RUmap[1].size()-std::count(m_RUmap[0].begin(), m_RUmap[0].end(), -1)
           << endl;
DEBUG_LOG_END

      for (unsigned long int i=0; i<m_RUmap[1].size(); i++)
        {
          if (m_RUmap[1][i] >= 0)
            {
              if (--m_RUmap[1][i] <= 0)
                {
                  id = m_users[i].m_userToSchedule->GetIDNetworkNode();
                  if (m_users[i].m_dataToTransmit - m_RUmap[3][i] <=0)
                    {
                      m_queue.erase(find(m_queue.begin(), m_queue.end(), m_users[i]));
DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG_FIFO)
                      cout << "\t UE " << id <<" removed from Queue" << endl;
DEBUG_LOG_END
                    }
                  for (auto u : *users )
                    {
                      if (u->m_userToSchedule->GetIDNetworkNode() == id)
                        {
                          u->m_transmittedData = m_RUmap[3][i];
                          u->m_selectedMCS = m_RUmap[2][i];
                          u->m_listOfAllocatedRUs.push_back(m_RUmap[4][i]);
                          u->m_subcarrier = i;
                        }
                    }

                  UserToSchedule us;
                  us.m_userToSchedule = NULL;
                  us.m_averageSchedulingGrant = -1;
                  us.m_dataToTransmit = -1;
                  us.m_listOfAllocatedRUs.clear();
                  us.m_selectedMCS = -1;
                  us.m_subcarrier = -1;

                  m_users[i] = us;
                  m_RUmap[0][i] = -1;
                  m_RUmap[1][i] = -1;
                  m_RUmap[2][i] = -1;
                  m_RUmap[3][i] = -1;
                  m_RUmap[4][i] = -1;
                }
            }
        }

DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG_FIFO)
      cout << endl;
      cout << "SF " << currentSF;
      printQ();
      printMap();
DEBUG_LOG_END
    }
}


void nbFifoUplinkPacketScheduler::printMap()
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


void nbFifoUplinkPacketScheduler::printQ()
{
  int id;
  cout << " QUEUE = ";
  for (auto scheduledUser : m_queue)
    {
      id = scheduledUser.m_userToSchedule->GetIDNetworkNode();
      cout << id << " ";
    }
  cout << endl;
}
