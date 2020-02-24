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


#ifndef PROPAGATIONLOSSMODEL_H_
#define PROPAGATIONLOSSMODEL_H_

#include "../../core/spectrum/transmitted-signal.h"
#include "../../load-parameters.h"
#include <map>

class ChannelRealization;
class NetworkNode;

class PropagationLossModel
{
public:
  PropagationLossModel() = default;
  virtual ~PropagationLossModel();

  typedef pair<NetworkNode*, NetworkNode* > ChannelRealizationId_t;
  typedef map <PropagationLossModel::ChannelRealizationId_t, ChannelRealization* > ChannelRealizationMap;


  void AddChannelRealization (ChannelRealization* chRealization);
  void DelChannelRealization (NetworkNode* src, NetworkNode* dst);

  ChannelRealization* GetChannelRealization (NetworkNode* src, NetworkNode* dst);
  ChannelRealizationMap GetChannelRealizationMap (void);

  ReceivedSignal* AddLossModel (NetworkNode* src,
                                NetworkNode* dst,
                                TransmittedSignal* txSignal);
private:
  ChannelRealizationMap m_channelRealizationMap;
};

#endif /* PROPAGATIONLOSSMODEL_H_ */
