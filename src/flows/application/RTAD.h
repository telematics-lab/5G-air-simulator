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
 * Author: Alessandro Grassi <alessandro.grassi@poliba.it>
 */

/*
 * Generate packets with fixed interarrival time of 36 ms and
 * random size uniformly distributed between 3 KBytes and 6 KBytes,
 * according to METIS Real-Time Application Driven traffic model
 * (METIS deliverable D6.1)
 */


#ifndef RTAD_H_
#define RTAD_H_

#include "Application.h"

class RTAD : public Application
{
public:
  RTAD();
  virtual ~RTAD() = default;

  virtual void DoStart (void);
  virtual void DoStop (void);

  void
  ScheduleTransmit (double time);
  void
  Send (void);

  int
  GetSize (void) const;
  double
  GetInterval (void) const;

private:
  void
  SetSize();

  double m_generationInterval;
  int m_size;
};

#endif /* RTAD_H_ */
