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


#ifndef TRANSMITTEDSIGNAL_H_
#define TRANSMITTEDSIGNAL_H_

#include <vector>
#include "../../load-parameters.h"

class BandwidthManager;

class TransmittedSignal
{
public:
  TransmittedSignal();
  virtual ~TransmittedSignal() = default;

  void SetValues (vector< vector<double> > values);
  vector< vector<double> > GetValues (void);

  void SetPhases (vector< vector<float> > phases);
  vector< vector<float> > GetPhases (void);

  TransmittedSignal* Copy (void);

  bool GetIsMBSFNSignal(void);
  void SetIsMBSFNSignal(bool isMBSFNSignal);

private:
  vector< vector<double> > m_values; //transmitted power for each MIMO path and sub-carrier
  vector< vector<float> > m_phases; //phase shift of received signal for each MIMO path and sub-carrier
  bool m_isMBSFNSignal;
};

/*
 * The meaning of the values of TransmittedSignal is: one value per subchannel and per TX antenna;
 * The meaning of the values of ReceivedSignal is: one value per subchannel and per MIMO path;
 * Otherwise, the two classes are the same.
 */
typedef TransmittedSignal ReceivedSignal;

#endif /* TRANSMITTEDSIGNAL_H_ */
