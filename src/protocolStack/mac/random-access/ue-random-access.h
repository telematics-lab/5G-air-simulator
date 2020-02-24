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

#ifndef UE_RANDOM_ACCESS_H_
#define UE_RANDOM_ACCESS_H_
#include <vector>
#include "../../../device/NetworkNode.h"
#include "../ue-mac-entity.h"
#include "../../../core/eventScheduler/simulator.h"


class UeRandomAccess
{
public:
  enum RandomAccessType
  {
    RA_TYPE_BASELINE,
    RA_TYPE_ENHANCED,
    RA_TYPE_NB_IOT
  };

  UeRandomAccess();
  UeRandomAccess(MacEntity* mac);
  virtual ~UeRandomAccess();

  void SetRandomAccessType(UeRandomAccess::RandomAccessType type);
  RandomAccessType GetUeRandomAccessType();

  void SetRaProcedureActive(bool pa);
  bool GetRaProcedureActive();

  void SetRAProcedureStartTime(double st);
  double GetRAProcedureStartTime();
  void SetRAProcedureTimeout(double to);
  double GetRAProcedureTimeout();
  void checkRAProcedureTimeout();
  bool isRachOpportunity();
  int GetMaxFailedAttempts(void);
  void SetMaxFailedAttempts(int n);

  void SetMotivationRequest(RandomAccessConnectionRequestIdealControlMessage::MotivationRequest(m_moti));

  virtual void StartRaProcedure() = 0;
  virtual void ReStartRaProcedure() = 0;
  virtual void ReceiveMessage2(int) = 0;
  virtual void ReceiveMessage4() = 0;

protected:
  bool m_RaProcedureActive;
  MacEntity* m_macEntity;
  RandomAccessType m_type;
  double m_RAProcedureStartTime;
  double m_RAProcedureTimeout;
  int m_nbFailedAttempts;
  int m_maxFailedAttempts;

  int GetNbFailedAttempts(void);
  void SetNbFailedAttempts(int n);

private:
};

#endif /* UE_RANDOM_ACCESS_H_ */
