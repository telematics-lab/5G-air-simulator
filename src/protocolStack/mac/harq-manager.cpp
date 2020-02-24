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
 * Author: Alessandro Grassi <alessandro.grassi@poliba.it>
 * Author: Sergio Martiradonna <sergio.martiradonna@poliba.it>
 */

#include <cmath>

#include "harq-manager.h"
#include "../protocol-stack.h"
#include "../packet/packet-burst.h"
#include "../../core/eventScheduler/simulator.h"

HarqManager::HarqManager()
{
  m_maxProcesses = 8;
  m_maxRetransmissions = 4;
  m_retransmissionDelay = 0.008;
  m_txProcesses.clear ();
  m_rxProcesses.clear ();
  m_ue = nullptr;
}

HarqManager::HarqManager(UserEquipment* ue)
    : HarqManager::HarqManager()
{
  m_ue = ue;
}

void
HarqManager::SetMaxProcesses (int n_proc)
{
  m_maxProcesses = n_proc;
}

int
HarqManager::GetMaxProcesses (void)
{
  return m_maxProcesses;
}

void
HarqManager::SetMaxRetransmissions (int n_retx)
{
  m_maxRetransmissions = n_retx;
}

int
HarqManager::GetMaxRetransmissions (void)
{
  return m_maxRetransmissions;
}

void
HarqManager::SetRetransmissionDelay (double delay)
{
  m_retransmissionDelay = delay;
}

double
HarqManager::GetRetransmissionDelay(void)
{
  return m_retransmissionDelay;
}

void
HarqManager::SetUE(UserEquipment* ue)
{
  m_ue = ue;
}

UserEquipment*
HarqManager::GetUE()
{
  return m_ue;
}

int
HarqManager::GetPidForNewTx()
{
  shared_ptr<txProcess> process = nullptr;
  int pid = HARQ_NO_PID_AVAILABLE;
  for(int test_pid=0; test_pid<GetMaxProcesses(); test_pid++)
    {
      if (!TransmitProcessExists(test_pid))
        {
          pid = test_pid;
          break;
        }
    }
  return pid;
}

void
HarqManager::NewTransmission(int pid, shared_ptr<PacketScheduler::FlowToSchedule> flow)
{
  DEBUG_LOG_START_1(SIM_ENV_HARQ_DEBUG)
    cout << "HARQ_DEBUG ---- NewTransmission"
    << endl;
  DEBUG_LOG_END
  if (pid<0 || pid>=GetMaxProcesses())
    {
      cout << "Error: invalid Pid for new HARQ process (" << pid << ")" << endl;
    }
  shared_ptr<txProcess> process = make_shared<txProcess>();
  process->pid = pid;
  process->isMulticastProcess = false;
  process->numTx = 1;
  process->lastTxTime = Simulator::Init()->Now();
  process->waitingForAck = true;
  process->flows.clear();
  process->flows.push_back (flow);
  flow->SetHarqManager(this);
  m_txProcesses.insert (pair< int, shared_ptr<txProcess> > (pid,process));
}


void
HarqManager::NewMulticastTransmission(int pid, shared_ptr<PacketScheduler::FlowToSchedule> flow)
{
  shared_ptr<txProcess> process = make_shared<txProcess>();
  process->pid = pid;
  process->isMulticastProcess = true;
  process->numTx = 1;
  process->lastTxTime = Simulator::Init()->Now();
  process->waitingForAck = true;
  process->flows.clear();
  process->flows.push_back (flow);
  flow->SetHarqManager(this);
  m_txProcesses.insert (pair< int, shared_ptr<txProcess> > (pid,process));
}


void
HarqManager::Retransmission(int pid)
{
  shared_ptr<txProcess> process;
  if (TransmitProcessExists(pid)==true)
    {
      process = m_txProcesses.at(pid);
    }
  else
    {
      cout << "Error: invalid HARQ process selected (" << pid << ")" << endl;
      return;
    }

  process->numTx++;
  process->lastTxTime = Simulator::Init()->Now();
}

void
HarqManager::ReceiveAck(bool ack, int pid)
{
  shared_ptr<txProcess> process;
  if (TransmitProcessExists(pid)==true)
    {
      process = m_txProcesses.at(pid);
    }
  else
    {
      cout << "Error: received ack for non-existing HARQ process (" << pid << ")" << endl;
      return;
    }

  if (ack == true)
    {
      process->clear();
      DeleteTxProcess(pid);
    }
}

void
HarqManager::txProcess::clear()
{
  DEBUG_LOG_START_1(SIM_ENV_HARQ_DEBUG)
    cout << "HARQ_DEBUG ---- txProcess::clear"
    << endl;
  DEBUG_LOG_END
  
  waitingForAck = false;
  flows.clear ();
  isMulticastProcess = false;
  numTx = 0;
}

PacketScheduler::FlowsToSchedule
HarqManager::GetFlowsForRetransmission ()
{
  PacketScheduler::FlowsToSchedule flows;
  list<RadioBearer*> bearers;
  map< int, shared_ptr<txProcess> >::iterator p = m_txProcesses.begin();
  while (p != m_txProcesses.end())
    {
      shared_ptr<txProcess> process = p->second;
DEBUG_LOG_START_1(SIM_ENV_HARQ_DEBUG)
      cout << "HARQ_DEBUG ---- GetFlowsForRetransmission"
                << " ue " << m_ue->GetIDNetworkNode()
                << ", pid " << p->first
                << ", num_tx " << process->numTx
                << ", is waiting for ack " << process->waitingForAck
                << endl;
DEBUG_LOG_END
      if (process->numTx >= GetMaxRetransmissions())
        {
          process->clear();
          p = m_txProcesses.erase(p);
        }
      else if (process->waitingForAck == true && Simulator::Init()->Now() - process->lastTxTime > m_retransmissionDelay)
        {
          // avoid scheduling multiple packets from the same bearer
          bool duplicate_bearers = false;
          for (auto flow : process->flows)
            {
              if (find(bearers.begin(),bearers.end(),flow->GetBearer()) != bearers.end())
                {
                  duplicate_bearers = true;
                }
            }

          if (duplicate_bearers == false)
            {
              flows.insert(flows.end(), process->flows.begin(), process->flows.end());
              for (auto flow : process->flows)
                {
                  bearers.push_back(flow->GetBearer());
                }
              if (process->isMulticastProcess == true)
                {
                  p = m_txProcesses.erase(p);
                }
              else
                {
                  ++p;
                }
            }
          else
            {
              ++p;
            }
        }
      else
        {
          ++p;
        }
    }
  for (auto f:flows)
    {
      f->SetHarqRetransmission(true);
    }
  return flows;
}

PacketScheduler::FlowsToSchedule
HarqManager::GetFlowsForMulticastRetransmission ()
{
  PacketScheduler::FlowsToSchedule flows;
  for (auto p:m_txProcesses)
    {
      shared_ptr<txProcess> process = p.second;
      if (process->numTx >= GetMaxRetransmissions())
        {
          process->clear();
          DeleteTxProcess(process->pid);
        }
      else if (process->waitingForAck == true && Simulator::Init()->Now() - process->lastTxTime > m_retransmissionDelay)
        {
          if (process->isMulticastProcess == true)
            {
              flows.insert(flows.end(), process->flows.begin(), process->flows.end());
              DeleteTxProcess(process->pid);
            }
        }
    }
  for (auto f:flows)
    {
      f->SetHarqRetransmission(true);
    }
  return flows;
}


bool
HarqManager::TransmitProcessExists (int pid)
{
  return m_txProcesses.count (pid);
}


void
HarqManager::DeleteTxProcess (int pid)
{
  m_txProcesses.erase(pid);
}



bool
HarqManager::ReceiveProcessExists (int pid)
{
  return m_rxProcesses.count (pid);
}


void
HarqManager::CreateRxProcess (int pid, vector<double> sinrValues)
{
  DEBUG_LOG_START_1(SIM_ENV_HARQ_DEBUG)
    cout << "HARQ_DEBUG ---- CreateRxProcess"
    << endl;
  DEBUG_LOG_END
  if (ReceiveProcessExists(pid) == true)
    {
      cout << "HARQ manager error: deleting an existing process to create a new one" << endl;
      DeleteRxProcess (pid);
    }
  shared_ptr<rxProcess> process = make_shared<rxProcess>();
  process->sinrValues.push_back (sinrValues);
  process->numRx = 1;
  m_rxProcesses.insert (pair< int, shared_ptr<rxProcess> > (pid,process));
}


void
HarqManager::UpdateRxProcess (int pid, vector<double> sinrValues)
{
  
  DEBUG_LOG_START_1(SIM_ENV_HARQ_DEBUG)
    cout << "HARQ_DEBUG ---- UpdateRxProcess"
    << endl;
  DEBUG_LOG_END
  shared_ptr<rxProcess> process = m_rxProcesses.at(pid);
  process->sinrValues.push_back (sinrValues);
  process->numRx++;
  if (process->numRx == GetMaxRetransmissions())
    {
      DeleteRxProcess(pid);
    }
}


vector<double>
HarqManager::GetCombinedSinr (int pid, vector<double> sinrValues)
{
  const float combiningLoss = 0.93;
  int nbOfRBs = (int) sinrValues.size();
  if (!ReceiveProcessExists(pid))
    {
      cout << "HARQ manager error: receive process does not exist!" << endl;
      return sinrValues;
    }
  shared_ptr<rxProcess> process = m_rxProcesses.at(pid);
  vector<double> sinrValues_sum, sinrValues_final;
  sinrValues_sum.resize(nbOfRBs);
  sinrValues_final.resize(nbOfRBs);
  for(int rb=0; rb<nbOfRBs; rb++)
    {
      sinrValues_sum.at(rb) = pow(10,sinrValues.at(rb)/10);
    }
  for(int n=0; n<(int)process->sinrValues.size(); n++)
    {
      for(int rb=0; rb<nbOfRBs; rb++)
        {
          sinrValues_sum.at(rb) += pow(10,process->sinrValues.at(n).at(rb)/10);
        }
    }
  for(int rb=0; rb<nbOfRBs; rb++)
    {
      sinrValues_final.at(rb) = 10*log10( sinrValues_sum.at(rb) * combiningLoss );
    }
  return sinrValues_final;
}

void
HarqManager::DeleteRxProcess (int pid)
{
  m_rxProcesses.erase(pid);
}
