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


#ifndef RRC_ENTITY_H
#define RRC_ENTITY_H

#include <list>
#include <vector>
#include "../../load-parameters.h"

class ClassifierParameters;
class RadioBearer;
class RadioBearerSink;
class RadioBearerInstance;
class NetworkNode;
class HandoverEntity;

/*
 * This class implements the RRC entity
 */
class RrcEntity
{
public:

  RrcEntity (void);
  virtual ~RrcEntity (void);

  void SetDevice (NetworkNode* d);
  NetworkNode* GetDevice ();

  typedef vector<RadioBearer* > RadioBearersContainer;
  typedef vector<RadioBearerSink* > RadioBearersSinkContainer;


  RadioBearersContainer* GetRadioBearerContainer (void);
  RadioBearersSinkContainer* GetRadioBearerSinkContainer (void);


  void AddRadioBearer (RadioBearer* bearer);
  void DelRadioBearer (RadioBearer* bearer);


  void AddRadioBearerSink (RadioBearerSink* bearer);
  void DelRadioBearerSink (RadioBearerSink* bearer);


  RadioBearer* GetRadioBearer (ClassifierParameters* ipc);
  RadioBearerSink* GetRadioBearerSink (ClassifierParameters* ipc);
  RadioBearerInstance* GetRadioBearer (int rlcIndex);


  void SetHandoverEntity (HandoverEntity* h);
  HandoverEntity* GetHandoverEntity (void);

private:
  RadioBearersContainer* m_bearers;
  RadioBearersSinkContainer* m_sink;
  NetworkNode* m_device;
  HandoverEntity* m_handover;
};


#endif /* RRC_ENTITY_H */
