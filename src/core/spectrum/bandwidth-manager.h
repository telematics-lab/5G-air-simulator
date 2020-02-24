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


#ifndef BandwidthManager_H_
#define BandwidthManager_H_

#include <vector>
#include "../../load-parameters.h"

/*
 * This class models the bandwidth used for the transmission
 * in a particular cell.
 * See information on TR 36.101 - Table 5.5-1
 */
class BandwidthManager
{
public:
  BandwidthManager() = default;
  BandwidthManager(double ulBw, double dlBw, int ulOffset, int dlOffset, bool tddTrue=false);
  virtual ~BandwidthManager() = default;

  void SetDlSubChannels (vector<double> s);
  vector<double> GetDlSubChannels (void);

  void SetUlSubChannels (vector<double> s);
  vector<double> GetUlSubChannels (void);


  void SetOperativeSubBand (int s);
  int GetOperativeSubBand (void);

  void SetUlBandwidth (double b);
  void SetDlBandwidth (double b);
  void SetUlOffsetBw (int o);
  void SetDlOffsetBw (int o);

  double GetUlBandwidth (void);
  double GetDlBandwidth (void);
  int GetUlOffsetBw (void);
  int GetDlOffsetBw (void);


  BandwidthManager* Copy ();

  void Print (void);

  int GetMaxNbIoTcarriers(void);
  vector<int> GetNbIoTrb(void);
  void CreateNbIoTspectrum(int c, double s, int t);

  double GetRUsBandwidth() const;
  int GetNbOfNbIoTcarriers() const;
  double GetSubcarrierSpacing() const;
  int GetTones() const;


private:
  vector<double> m_dlSubChannels;
  vector<double> m_ulSubChannels;

  int m_operativeSubBand;

  double m_ulBandwidth;
  double m_dlBandwidth;

  int m_ulOffsetBw;
  int m_dlOffsetBw;

  float m_subcarrierSpacing;
  bool NbIoTenabled;
  int m_nbOfNbIoTcarriers;
  vector<double> m_NbIoTchannels;
  int m_tones;
};

#endif /* BandwidthManager_H_ */
