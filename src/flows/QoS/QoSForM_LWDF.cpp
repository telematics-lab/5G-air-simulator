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


#include "QoSForM_LWDF.h"
#include <iostream>

QoSForM_LWDF::QoSForM_LWDF()
{
  m_dropProbability = 0.005;
}

QoSForM_LWDF::QoSForM_LWDF(double targetDelay)
    : QoSForM_LWDF::QoSForM_LWDF()
{
  SetMaxDelay (targetDelay);
}

void
QoSForM_LWDF::SetDropPorbability (double probability)
{
  m_dropProbability = probability;
}

double
QoSForM_LWDF::GetDropProbability (void) const
{
  return m_dropProbability;
}


//Debug
void
QoSForM_LWDF::Print ()
{
}
