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
 * Author: Sergio Martiradonna <sergio.martiradonna@poliba.it>
 */


#ifndef RADIOBEARER_H_
#define RADIOBEARER_H_

#include "radio-bearer-instance.h"

class NetworkNode;
class ClassifierParameters;
class Application;
class MacQueue;
class QoSParameters;
class Packet;
class RlcEntity;

class RadioBearer : public RadioBearerInstance
{
public:
  RadioBearer();
  RadioBearer(Application* a);
  virtual ~RadioBearer();

  MacQueue* GetMacQueue (void);

  void SetApplication (Application* a);
  Application* GetApplication (void);

  void UpdateTransmittedBytes (int bytes);
  int GetTransmittedBytes (void) const;
  void ResetTransmittedBytes (void);
  void UpdateAverageTransmissionRate ();
  double GetAverageTransmissionRate (void) const;
  void SetLastUpdate (void);
  double GetLastUpdate (void) const;


  void Enqueue (Packet *packet);
  bool HasPackets (void);

  Packet* CreatePacket (int bytes);

  void CheckForDropPackets ();

  int GetQueueSize (void);
  double GetHeadOfLinePacketDelay (void);
  int GetHeadOfLinePacketSize (void);
  int GetByte (int byte); //for FLS scheduler

private:
  Application* m_application;

  MacQueue *m_macQueue;

  // Scheduler Information
  double m_averageTransmissionRate;
  int m_transmittedBytes;
  double m_lastUpdate;
};

#endif /* RADIOBEARER_H_ */
