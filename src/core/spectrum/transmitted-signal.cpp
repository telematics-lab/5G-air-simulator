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


#include "transmitted-signal.h"
#include "bandwidth-manager.h"
#include <iostream>

TransmittedSignal::TransmittedSignal()
{
  m_values.resize(1);
  m_isMBSFNSignal = false;
}

void
TransmittedSignal::SetValues (vector< vector<double> > values)
{
  m_values = values;
}

vector< vector<double> >
TransmittedSignal::GetValues (void)
{
  return m_values;
}

void
TransmittedSignal::SetPhases (vector< vector<float> > phases)
{
  m_phases = phases;
}

vector< vector<float> >
TransmittedSignal::GetPhases (void)
{
  return m_phases;
}

TransmittedSignal*
TransmittedSignal::Copy (void)
{
  TransmittedSignal* txSignal = new TransmittedSignal ();
  txSignal->SetValues (GetValues ());
  txSignal->SetPhases (GetPhases ());

  return txSignal;
}


bool
TransmittedSignal::GetIsMBSFNSignal(void)
{
  return m_isMBSFNSignal;
}

void
TransmittedSignal::SetIsMBSFNSignal(bool isMBSFNSignal)
{
  m_isMBSFNSignal = isMBSFNSignal;
}
