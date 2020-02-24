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



#ifndef CLASSIFIER_H_
#define CLASSIFIER_H_

#include "../../protocolStack/packet/Packet.h"

class ClassifierParameters;
class NetworkNode;

class Classifier
{
private:

public:
  Classifier();
  virtual ~Classifier() = default;

  void SetDevice (NetworkNode* d);
  NetworkNode* GetDevice (void);

  void Classify (Packet* p);


private:
  NetworkNode* m_device;
};

#endif /* CLASSIFIER_H_ */
