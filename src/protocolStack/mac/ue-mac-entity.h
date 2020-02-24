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


#ifndef UE_MAC_ENTITY_H
#define UE_MAC_ENTITY_H

#include <list>
#include "mac-entity.h"
#include "random-access/ue-random-access.h"

/*
 * This class implements the MAC layer of the UE device
 */
class UeMacEntity : public MacEntity
{
public:

  UeMacEntity () = delete;
  UeMacEntity (NetworkNode* d);
  virtual ~UeMacEntity (void);

  UserEquipment* GetDevice(void);

  void SendSchedulingRequest (void);
  void ScheduleUplinkTransmission (int nbPrb, int mcs);
  void ScheduleNbUplinkTransmission (int ru, int mcs);

  double GetSchedulingRequestInterval (void);
  void SetSchedulingRequestInterval (double t);

  void SetHarqManager (HarqManager* harqManager);
  HarqManager* GetHarqManager (void);

  UeRandomAccess* GetRandomAccessManager (void);
  void SetRandomAccessManager(UeRandomAccess* s);
  void SetRandomAccessType(UeRandomAccess::RandomAccessType type);
  void CheckForDropPackets (void);
  void ReceiveIdealControlMessage (IdealControlMessage* msg);
  void ReceiveRandomAccessResponseIdealControlMessage (RandomAccessResponseIdealControlMessage* msg);
  void ReceiveRandomAccessConnectionResolutionIdealControlMessage (RandomAccessConnectionResolutionIdealControlMessage* msg);

private:
  double m_schedulingRequestInterval; //seconds
  HarqManager* m_harqManager;
  UeRandomAccess* m_uerandomaccess;

};



#endif /* UE_MAC_ENTITY_H */
