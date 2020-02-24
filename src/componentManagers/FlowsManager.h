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


#ifndef FLOWSMANAGER_H_
#define FLOWSMANAGER_H_

#include <iostream>
#include <queue>
#include "../protocolStack/protocols/TransportProtocol.h"
#include "../flows/application/Application.h"

class ClassifierParameters;
class QoSParameters;
class NetworkNode;


class FlowsManager
{
public:

private:

  FlowsManager() = default;
  static FlowsManager *ptr;

public:
  virtual ~FlowsManager() = default;

  static FlowsManager*
  Init (void)
  {
    if (ptr==nullptr)
      {
        ptr = new FlowsManager;
      }
    return ptr;
  }

  Application*  CreateApplication (int applicationID,
                                   NetworkNode* src, NetworkNode* dst,
                                   int srcPort, int destPort,
                                   TransportProtocol::TransportProtocolType protocol,
                                   Application::ApplicationType type,
                                   QoSParameters* qos,
                                   double startTime, double duration);
};

#endif /* FLOWSMANAGER_H_ */
