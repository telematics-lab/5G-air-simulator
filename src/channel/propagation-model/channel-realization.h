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


#ifndef CHANNELREALIZATION_H_
#define CHANNELREALIZATION_H_

#include <vector>
#include "../../load-parameters.h"

class NetworkNode;
class CartesianCoordinates;

class ChannelRealization
{
public:

  enum ChannelModel
  {
    CHANNEL_MODEL_MACROCELL_URBAN,
    CHANNEL_MODEL_MACROCELL_SUB_URBAN,
    CHANNEL_MODEL_MACROCELL_RURAL,
    CHANNEL_MODEL_MICROCELL,
    CHANNEL_MODEL_FEMTOCELL_URBAN,
    CHANNEL_MODEL_3GPP_DOWNLINK,
    CHANNEL_MODEL_WINNER_DOWNLINK,
    CHANNEL_MODEL_MACROCELL_URBAN_IMT,
    CHANNEL_MODEL_MACROCELL_URBAN_IMT_3D,
    CHANNEL_MODEL_MACROCELL_RURAL_IMT,
    CHANNEL_MODEL_3GPP_CASE1,
  };

  ChannelRealization (NetworkNode* src, NetworkNode* dst, ChannelModel model, bool MBSFNRealization = false);
  ~ChannelRealization();
  void Destroy ();

  void SetSourceNode (NetworkNode* src);
  NetworkNode* GetSourceNode (void);
  void SetDestinationNode (NetworkNode* dst);
  NetworkNode* GetDestinationNode (void);


  double GetPathLoss (void);
  void SetLastUpdate (void);
  double GetLastUpdate (void);
  void SetSamplingPeriod (double sp);
  double GetSamplingPeriod (void);
  void SetShadowing (double sh);
  double GetShadowing (void);
  void SetShadowingStddev (double stddev);
  double GetShadowingStddev (void);
  void SetPenetrationLoss (double pnl);
  double GetPenetrationLoss (void);
  double GetBeamformingGain (int path);
  double GetBeamformingGain_cover (int path, int cover);
  void SetChannelModel(ChannelModel model);
  ChannelModel GetChannelModel(void);
  void SetPenetrationLossMean(double plm);
  double GetPenetrationLossMean(void);
  void SetPenetrationLossStdDev(double plsd);
  double GetPenetrationLossStdDev(void);
  void SetLoSState(bool state);
  bool GetLoSState(void);
  void SetIndoorDistance(double d);
  double GetIndoorDistance(void);
  bool isMbsfnRealization(void);

  bool NeedForUpdate (void);

  void ShortTermUpdate(void);

  vector< vector<double> > GetLoss ();
  vector< vector<float> > GetPhases ();

  typedef vector< vector<float const*> > FastFading;

  void UpdateFastFading (void);
  void SetFastFading (FastFading* ff);
  FastFading* GetFastFading ();

  bool hasFastFading();
  void enableFastFading();
  void disableFastFading();

protected:
  double m_penetrationLoss;
  double m_pathLoss;
  ChannelModel m_channelModel;

private:
  NetworkNode* m_src;
  NetworkNode* m_dst;

  double m_lastUpdate;
  double m_samplingPeriod;

  FastFading* m_fastFading;
  FastFading *m_fastFadingPhases;

  double m_shadowing;
  double m_shadowingStddev;
  double m_lastShadowingUpdate;
  double m_lastShortTermUpdate;
  bool m_isLosType;
  double m_indoorDistance;
  double m_lastPenetrationLossUpdate;
  double m_penetrationLossMean;
  double m_penetrationLossStdDev;
  int m_forceCoverShift;
  bool m_mbsfnRealization;
};

#endif /* CHANNELREALIZATION_H_ */
