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


#ifndef AMRLCENTITY_H_
#define AMRLCENTITY_H_

#include "rlc-entity.h"
#include "am-state-variables.h"
#include <vector>

class PacketBurst;
class Packet;
class AmdRecord;
class ArqRlcIdealControlMessage;

class AmRlcEntity: public RlcEntity
{
public:
  AmRlcEntity();
  virtual ~AmRlcEntity();

  virtual shared_ptr<PacketBurst> TransmissionProcedure (int availableBytes);
  virtual void ReceptionProcedure (Packet* p);
  void ReceptionProcedureEnd ();

  AmStateVariables* GetAmStateVariables ();

  vector<AmdRecord*>* GetSentAMDs (void);
  vector<AmdRecord*>* GetReceivedAMDs (void);

  void InsertAMDIntoReceptionList (AmdRecord* amd);

  void SendArqRlcIdealControlMessage (ArqRlcIdealControlMessage msg);
  void ReceiveArqRlcIdealControlMessage (ArqRlcIdealControlMessage msg);

  void ClearPacketList (void);

  void PrintSentAMDs (void);
  void PrintReceivedAMDs (void);

  void CheckForDropPackets (double maxDelay, int bearerID);

  int GetSizeOfUnaknowledgedAmd (void);

private:
  AmStateVariables* m_amStateVariables;
  vector<AmdRecord*> *m_sentAMDs;
  vector <AmdRecord*> *m_receivedAMDs;
};

#endif /* AMRLCENTITY_H_ */
