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




#include "calendar.h"

#include <iostream>

Calendar::Calendar()
{
  m_events = new Events;
}

Calendar::~Calendar()
{
  delete m_events;
}

Calendar::Events*
Calendar::GetEvents (void)
{
  return m_events;
}

void
Calendar::InsertEvent (shared_ptr<Event> newEvent)
{
  (*m_events)[newEvent->GetTimeStamp()].push(newEvent);
}

bool
Calendar::IsEmpty (void)
{
  return GetEvents ()->empty();
}

shared_ptr<Event>
Calendar::GetEvent (void)
{
  if (IsEmpty ())
    return nullptr;

  shared_ptr<Event> event = GetEvents ()->begin()->second.front ();
  return event;
}

void
Calendar::RemoveEvent (void)
{
  shared_ptr<Event>event = GetEvents ()->begin()->second.front();
  m_events->begin()->second.pop();
  if (m_events->begin()->second.empty())
    {
      m_events->erase(m_events->begin());
    }
}


