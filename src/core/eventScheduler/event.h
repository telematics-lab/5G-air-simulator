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

#ifndef EVENT_H_
#define EVENT_H_

#include "../../load-parameters.h"

class Event
{
public:
  Event();
  virtual ~Event() = default;

  void
  SetTimeStamp (int time);
  int
  GetTimeStamp (void) const;
  void
  SetUID (unsigned long long int uid);
  unsigned long long int
  GetUID (void) const;
  bool
  IsDeleted (void);
  void
  MarkDeleted (void);

  virtual void
  RunEvent (void) = 0;

private:
  int m_timeStamp;
  int m_uid;
  bool m_deleted;
};

class event_compare {
public:
  bool operator() (shared_ptr<Event> left, shared_ptr<Event> right)
    {
      bool result;
      int ts1 = left->GetTimeStamp();
      int ts2 = right->GetTimeStamp();
      if (ts1==ts2)
        {
          result = (left->GetUID() > right->GetUID());
        }
      else
        {
          result = (ts1 > ts2);
        }
      return result;
    }
};

#endif /* EVENT_H_ */
