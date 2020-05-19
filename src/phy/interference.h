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

#include "phy.h"
#include "../device/GNodeB.h"
#include<map>

#ifndef INTERFERENCE_H_
#define INTERFERENCE_H_

class UserEquipment;
class GNodeB;

class Interference
{
public:
  Interference();
  virtual ~Interference() = default;

  double ComputeInterference (UserEquipment *ue, bool MBSFNConstructiveInterference=false);
  double ComputeDopplerInterference (int speed, Phy::WaveformType type);
    
  map<UserEquipment*,double> ComputeUplinkInterference (UserEquipment *ue); //for uplink mimo
  map<GNodeB*,double> ComputeDownlinkInterference (UserEquipment *ue, bool MBSFNConstructiveInterference=false); //for uplink mimo

private:
  bool m_isLosType;

  // each pair contains corresponding speed+interference values
  vector< pair<double, double> > m_OFDMInterferenceValues;
  vector< pair<double, double> > m_POFDMInterferenceValues;
};

#endif /* INTERFERENCE_H_ */
