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



#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "../../core/eventScheduler/simulator.h"
#include "../../protocolStack/packet/Packet.h"
#include "../../protocolStack/protocols/TransportProtocol.h"
#include "../../load-parameters.h"
#include "../../device/IPClassifier/ClassifierParameters.h"
#include "../../protocolStack/packet/PacketTAGs.h"


#include <iostream>
#include <stdint.h>

class NetworkNode;
class ClassifierParameters;
class RadioBearer;
class QoSParameters;
class ApplicationSink;
class RadioBearerSink;

class Application
{
public:
  enum ApplicationType
  {
    APPLICATION_TYPE_VOIP,
    APPLICATION_TYPE_TRACE_BASED,
    APPLICATION_TYPE_INFINITE_BUFFER,
    APPLICATION_TYPE_CBR,
    APPLICATION_TYPE_WEB,
    APPLICATION_TYPE_FTP2,
    APPLICATION_TYPE_EXTERNAL_SOURCE
  };

  Application();
  virtual ~Application() = default;

  void SetApplicationType (ApplicationType applicationType);
  ApplicationType GetApplicationType (void) const;

  void SetClassifierParameters (ClassifierParameters* cp);
  ClassifierParameters* GetClassifierParameters (void);

  void SetQoSParameters (QoSParameters* qos);
  QoSParameters* GetQoSParameters (void);

  NetworkNode* GetSource (void);
  void SetSource (NetworkNode *source);
  NetworkNode* GetDestination (void);
  void SetDestination (NetworkNode *destination);
  int GetSourcePort (void) const;
  void SetSourcePort (int port);
  int GetDestinationPort (void) const;
  void SetDestinationPort (int port);
  TransportProtocol::TransportProtocolType GetTransportProtocol (void) const;
  void SetTransportProtocol (TransportProtocol::TransportProtocolType protocol);

  vector<NetworkNode*> GetTransmittingNodes(void);
  void SetTransmittingNodes(vector<NetworkNode*> nodes);
  void AddTransmittingNode(NetworkNode* node);

  vector<RadioBearer*> GetRadioBearers(void);
  void SetRadioBearers(vector<RadioBearer*> bearers);
  void AddRadioBearer(RadioBearer* bearer);

  void SetStartTime (double time);
  double GetStartTime (void) const;
  void SetStopTime (double time);
  double GetStopTime (void) const;

  void Start (void);
  void Stop (void);
  virtual void DoStart (void) = 0;
  virtual void DoStop (void) = 0;
  void enqueue (Packet* p);

  void SetApplicationID (int id);
  int GetApplicationID (void);

  RadioBearer* GetRadioBearer (void);

  void Trace (Packet* packet);

  //Debug
  void Print (void);

private:
  ApplicationType m_applicationType;

  NetworkNode *m_source;
  NetworkNode *m_destination;
  int m_sourcePort;
  int m_destinationPort;
  TransportProtocol::TransportProtocolType m_transportProtocol;

  vector<NetworkNode*> m_transmittingNodes; // for multicast applications
  vector<RadioBearer*> m_radioBearers; // for multicast applications

  ClassifierParameters* m_classifierParameters;
  QoSParameters *m_qosParameters;

  RadioBearer* m_radioBearer;
  RadioBearerSink *m_bearerSink;
  ApplicationSink *m_applicationSink;

  double m_startTime;
  double m_stopTime;

  int m_applicationID;
};

#endif /* APPLICATION_H_ */
