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


#ifndef GNB_MAC_ENTITY_H
#define GNB_MAC_ENTITY_H

#include <list>
#include "mac-entity.h"
#include "random-access/gnb-random-access.h"
#include "../../core/idealMessages/ideal-control-messages.h"

/*
 * This class implements the MAC layer of the gNodeB device
 */

class PacketScheduler;
class DownlinkPacketScheduler;
class UplinkPacketScheduler;
class GnbRandomAccess;
class GNodeB;

class GnbMacEntity : public MacEntity
{
public:

  GnbMacEntity (void);
  virtual ~GnbMacEntity (void);

  GNodeB* GetDevice(void);

  void SetUplinkPacketScheduler (UplinkPacketScheduler* s);
  void SetDownlinkPacketScheduler (DownlinkPacketScheduler* s);
  UplinkPacketScheduler* GetUplinkPacketScheduler (void);
  DownlinkPacketScheduler* GetDownlinkPacketScheduler (void);

  void ReceiveIdealControlMessage (IdealControlMessage* msg);
  void ReceiveCqiIdealControlMessage  (CqiIdealControlMessage* msg);
  void ReceiveRiIdealControlMessage  (RankIndicatorIdealControlMessage* msg);
  void ReceivePmiIdealControlMessage  (PmiIdealControlMessage* msg);
  void ReceiveFullCsiIdealControlMessage  (FullCsiIdealControlMessage* msg);
  void SendPdcchMapIdealControlMessage  (PdcchMapIdealControlMessage* msg);
  void ReceiveSchedulingRequestIdealControlMessage (SchedulingRequestIdealControlMessage *msg);
  void ReceiveHarqAckIdealControlMessage (HarqIdealControlMessage *msg);
  void ReceiveRandomAccessPreambleIdealControlMessage (RandomAccessPreambleIdealControlMessage* msg);
  void ReceiveRandomAccessConnectionRequestIdealControlMessage (RandomAccessConnectionRequestIdealControlMessage* msg);
  GnbRandomAccess* GetRandomAccessManager(void);
  void SetRandomAccessManager(GnbRandomAccess* s);
  void SetRandomAccessType (GnbRandomAccess::RandomAccessType type);

  void SetDefaultDlTxMode (int txmode);
  int GetDefaultDlTxMode(void);

private:

  UplinkPacketScheduler* m_uplinkScheduler;
  DownlinkPacketScheduler* m_downlinkScheduler;
  GnbRandomAccess* m_gnbrandomaccess;
  RandomAccessResponseIdealControlMessage* flag;
  int m_defaultDlTxMode;
};


#endif /* GNB_MAC_ENTITY_H */
