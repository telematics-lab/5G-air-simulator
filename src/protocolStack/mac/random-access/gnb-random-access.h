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

#ifndef GNB_RANDOM_ACCESS_H_
#define GNB_RANDOM_ACCESS_H_
#include <vector>
#include <map>
#include "../../../core/idealMessages/ideal-control-messages.h"

class NetworkNode;
class MacEntity;

class GnbRandomAccess
{
public:
  enum RandomAccessType
  {
    RA_TYPE_BASELINE,
    RA_TYPE_ENHANCED,
    RA_TYPE_NB_IOT
  };

  GnbRandomAccess();
  GnbRandomAccess(MacEntity* mac);
  virtual ~GnbRandomAccess();

  void SetGnbRandomAccessType(RandomAccessType type);
  RandomAccessType GetGnbRandomAccessType();

  virtual vector<int> GetReservedSubChannels() = 0;
  virtual void ReceiveMessage1( RandomAccessPreambleIdealControlMessage *msg) = 0;
  virtual void ReceiveMessage3(RandomAccessConnectionRequestIdealControlMessage *msg) = 0;
  virtual void SendMessage4(NetworkNode* dest) = 0;
  virtual bool isRachOpportunity() = 0;

protected:
  MacEntity* m_macEntity;
  RandomAccessType m_type;
private:

};

#endif /* GNB_RANDOM_ACCESS_H_ */
