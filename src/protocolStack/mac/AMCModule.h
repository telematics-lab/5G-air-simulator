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



#ifndef AMCModule_H_
#define AMCModule_H_

#include <vector>
#include "../../load-parameters.h"
/*
 *  Adaptive Modulation And Coding Scheme
 */

class AMCModule
{
public:
  AMCModule();
  virtual ~AMCModule();

  int
  GetCQIFromEfficiency (double sinr);
  int
  GetMCSIndexFromEfficiency(double spectralEfficiency);
  int
  GetMCSFromCQI (int cqi);
  int
  GetCQIFromMCS (int mcs);
  int
  GetTBSizeFromMCS (int mcs);
  int
  GetTBSizeFromMCS (int mcs, int nbRBs);
  int
  GetTBSizeFromMCS (int mcs, int nbRBs, int nbLayers);
  int
  GetTBSizeFromMCS (int mcs1, int mcs2, int nbRBs, int rank);
  double
  GetEfficiencyFromCQI (int cqi);
  int
  GetCQIFromSinr (double sinr);
  double
  GetSinrFromCQI (int cqi);
  int
  GetModulationOrderFromMCS(int mcs);
  int
  GetMCSFromSinrVector(const vector<double> &sinr);

  vector<int> CreateCqiFeedbacks (vector<double> sinr);
  bool getUseExtendedCQI (void);
  void setUseExtendedCQI (bool t);

private:
  bool m_useExtendedCQI;
  double* SINRForCQIIndex;
};

#endif /* AMCModule_H_ */
