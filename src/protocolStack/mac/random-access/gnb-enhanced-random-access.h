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
 * Author: Cosimo Damiano Di Pace <cd.dipace@gmail.com>
 * Author: Alessandro Grassi <alessandro.grassi@poliba.it>
 */

#ifndef GNB_ENHANCED_RANDOM_ACCESS_H_
#define GNB_ENHANCED_RANDOM_ACCESS_H_

#include <list>
#include <vector>
#include "gnb-random-access.h"
#include "../mac-entity.h"
#include "../../../device/NetworkNode.h"
#include "../../../device/UserEquipment.h"
#include "../../../core/idealMessages/ideal-control-messages.h"
#include "allocation-map.h"
#include <map>
#include <algorithm>


class GnbEnhancedRandomAccess : public GnbRandomAccess
{

public:
  enum PreambleFormat
  {
    Preamble_FORMAT_0,
    Preamble_FORMAT_1,
    Preamble_FORMAT_2,
    Preamble_FORMAT_3
  };

  GnbEnhancedRandomAccess();
  GnbEnhancedRandomAccess(MacEntity* mac);
  virtual ~GnbEnhancedRandomAccess();

  void SetGnbEnhancedRandomAccess();
  void GetGnbEnhancedRandomAccess();

  void SetRachReservedSubChannels();
  virtual vector<int> GetRachReservedSubChannels();
  virtual vector<int> GetCcchReservedSubChannels();
  virtual vector<int> GetReservedSubChannels();

  void setNbRARs(int n);
  int getNbRARs(void);


  virtual void ReceiveMessage1(RandomAccessPreambleIdealControlMessage *msg);
  virtual void ReceiveMessage3(RandomAccessConnectionRequestIdealControlMessage *msg);
  void CheckCollisions();
  void SendResponses();
  virtual void SendMessage2(NetworkNode *dest, int msg3time, int msg3RB);
  virtual void SendMessage4(NetworkNode *dest);
  virtual bool isRachOpportunity();

  struct preambleMessage
  {
    NetworkNode* ue;
    int preamble;
    int selectedRar;
  };

  typedef vector<preambleMessage> rachSession;

  struct rarElement
  {
    int msg1Time;
    int msg3Time;
    int msg3RB;
    NetworkNode* ue;
  };


private:
  PreambleFormat m_PreambleFormat;
  int m_ripInt; // TTIs
  int m_lastRachOpportunity; // TTIs
  int m_PreambleDuration; // TTIs
  int m_responseWindowDelay; // TTIs
  int m_responseWindowDuration; // TTIs
  AllocationMap m_RBsReservedForRach;
  map <int, rachSession> m_savedRachSessions;
  float m_maxCCCHUsage;
  AllocationMap m_RBsReservedForCCCH;
  queue <rarElement> m_rarQueue;
  int m_nbRARs;

  shared_ptr<Event> m_RachReservation;
  shared_ptr<Event> m_RarScheduling;
};

#endif /* GNB_ENHANCED_RANDOM_ACCESS_H_ */
