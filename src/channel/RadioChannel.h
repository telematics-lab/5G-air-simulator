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


#ifndef RADIO_CHANNEL_H_
#define RADIO_CHANNEL_H_

#include <vector>
#include <memory>
#include "../load-parameters.h"

class NetworkNode;
class PacketBurst;
class TransmittedSignal;
class PropagationLossModel;

class RadioChannel
{
public:
  RadioChannel();
  virtual ~RadioChannel();

  void StartTx (shared_ptr<PacketBurst> p, TransmittedSignal* txSignal, NetworkNode* src);
  void StartRx (shared_ptr<PacketBurst> p, TransmittedSignal* psd, NetworkNode* src);

  void AddDevice (NetworkNode* d);
  void DelDevice (NetworkNode* d);
  bool IsAttached (NetworkNode* d);

  vector<NetworkNode*>* GetDevices (void);

  void SetPropagationLossModel (PropagationLossModel* m);
  PropagationLossModel* GetPropagationLossModel (void);

  void SetChannelId (int id);
  int GetChannelId (void);

private:
  vector<NetworkNode*> *m_attachedDevices; //list of devices attached to the channel

  PropagationLossModel* m_propagationLossModel;

  int m_channelId;
};

#endif /* RADIO_CHANNEL_H_ */
