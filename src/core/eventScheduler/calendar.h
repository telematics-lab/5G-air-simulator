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



#ifndef CALENDAR_H_
#define CALENDAR_H_

#include "event.h"
#include "../../load-parameters.h"


class Calendar
{
public:
  Calendar();
  virtual ~Calendar();

  typedef map< int, queue<shared_ptr<Event> > > Events;

  Events*
  GetEvents (void);

  void
  InsertEvent (shared_ptr<Event> newEvent);
  bool
  IsEmpty (void);
  shared_ptr<Event>
  GetEvent (void);
  void
  RemoveEvent (void);


private:
  Events *m_events;
};

#endif /* CALENDAR_H_ */
