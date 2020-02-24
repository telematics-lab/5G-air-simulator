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

#ifndef WIDEBANDCQIMANAGER_H_
#define WIDEBANDCQIMANAGER_H_

/*
 * This class provides an implementation of the
 * Wideband CQI reporting using the EEMS alghrithm
 * to compute the effective SINR
 */

#include "cqi-manager.h"

class WidebandCqiManager: public CqiManager
{
public:
  WidebandCqiManager() = default;
  virtual ~WidebandCqiManager() = default;

  virtual vector<int> CalculateCqiFeedbacks (vector<double> sinr);
};

#endif /* WIDEBANDCQIMANAGER_H_ */
