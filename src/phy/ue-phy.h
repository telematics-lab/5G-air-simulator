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


#ifndef UE_PHY_H_
#define UE_PHY_H_

#include "phy.h"
#include <vector>
#include <armadillo>

class IdealControlMessage;
class UserEquipment;

class UePhy :public Phy
{
public:
  UePhy();
  virtual ~UePhy();

  virtual void DoSetBandwidthManager (void);

  virtual void StartTx (shared_ptr<PacketBurst> p);
  virtual void StartRx (shared_ptr<PacketBurst> p, TransmittedSignal* txSignal);

  virtual void CreateCqiFeedbacks (vector<double> sinr);
  UserEquipment* GetDevice(void);

  vector<double>& GetMeasuredMBSFNSinr (void);
  int GetRankForRiFeedback();

  virtual void SendIdealControlMessage (IdealControlMessage *msg);
  virtual void ReceiveIdealControlMessage (IdealControlMessage *msg);

  void SendReferenceSymbols (void);
  void SetTxSignalForReferenceSymbols (void);
  TransmittedSignal* GetTxSignalForReferenceSymbols (void);

protected:
  vector<double> m_sinrForCQI;
  int m_rankForRiFeedback;

private:
  vector<double> m_measuredMBSFNSinr;
  vector< vector<int> > m_pmiForFeedback;
  vector< shared_ptr<arma::cx_fmat> > m_fullCsiFeedback;
  int m_harqPidForRx;
  int m_harqPidForTx;
  TransmittedSignal* m_txSignalForReferenceSymbols;

  vector<int> m_channelsForTx;
  vector<int> m_mcsIndexForTx;

  vector<int> m_channelsForRx;
  vector<int> m_mcsIndexForRx;
  int m_rankForRx;
  vector< vector<int> > m_pmiForRx;
  vector<int> m_assignedLayers;
  vector< shared_ptr<arma::cx_fmat> > m_precodingMatricesForRx;
  vector< shared_ptr<arma::cx_fmat> > m_channelMatricesForSrtaPi;
};

#endif /* UE_PHY_H_ */
