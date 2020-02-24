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
 */

#ifndef HARQMANAGER_H_
#define HARQMANAGER_H_

#include <vector>
#include <map>
#include "packet-scheduler/packet-scheduler.h"
#include "../../device/UserEquipment.h"

class HarqManager
{
public:
  HarqManager ();
  HarqManager (UserEquipment* ue);
  virtual ~HarqManager () = default;

  void SetMaxProcesses (int n_proc);
  int GetMaxProcesses (void);

  void SetMaxRetransmissions (int n_retx);
  int GetMaxRetransmissions (void);

  void SetRetransmissionDelay (double delay);
  double GetRetransmissionDelay(void);

  void SetUE(UserEquipment* ue);
  UserEquipment* GetUE();

  enum HARQ_RESPONSE
  {
    HARQ_NO_PID_AVAILABLE = -1,
    HARQ_NOT_USED = -2
  };

  struct txProcess
  {
    int pid;
    double lastTxTime;
    bool waitingForAck;
    int numTx;
    bool isMulticastProcess;
    PacketScheduler::FlowsToSchedule flows;

    void clear(void);
  };

  struct rxProcess
  {
    int numRx;
    vector< vector<double> > sinrValues;
  };

  int GetPidForNewTx();
  void NewTransmission(int pid, shared_ptr<PacketScheduler::FlowToSchedule> flow);
  void NewMulticastTransmission(int pid, shared_ptr<PacketScheduler::FlowToSchedule> flow);
  void Retransmission(int pid);
  void ReceiveAck(bool ack, int pid);
  PacketScheduler::FlowsToSchedule GetFlowsForRetransmission ();
  PacketScheduler::FlowsToSchedule GetFlowsForMulticastRetransmission ();

  bool TransmitProcessExists (int pid);
  void DeleteTxProcess (int pid);

  bool ReceiveProcessExists (int pid);
  void CreateRxProcess (int pid, vector<double> sinrValues);
  void UpdateRxProcess (int pid, vector<double> sinrValues);
  vector<double> GetCombinedSinr (int pid, vector<double> sinrValues);
  void DeleteRxProcess (int pid);

private:
  int m_maxProcesses;
  int m_maxRetransmissions;
  double m_retransmissionDelay;
  UserEquipment* m_ue;

  map< int, shared_ptr<txProcess> > m_txProcesses;
  map< int, shared_ptr<rxProcess> > m_rxProcesses;
};

#endif /* HARQMANAGER_H_ */
