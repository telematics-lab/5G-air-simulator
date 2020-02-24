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


#ifndef MACQUEUE_H_
#define MACQUEUE_H_

#include "../protocolStack/packet/Packet.h"

#include <deque>
#include <stdint.h>

class MacQueue
{
private:
  struct QueueElement
  {
    QueueElement (void);
    QueueElement (Packet *packet);
    virtual ~QueueElement();
    QueueElement operator= (const QueueElement &obj);
    QueueElement (const QueueElement &obj);
    Packet *m_packet;
    bool m_fragmentation;
    int m_fragmentNumber;
    int m_fragmentOffset;

    //Can be used for HARQ process
    bool m_lastFragment;
    int m_tempFragmentNumber;
    int m_tempFragmentOffset;

    Packet*
    GetPacket(void);

    int
    GetSize (void) const;
    double
    GetTimeStamp(void) const;

    void
    SetFragmentation (bool flag);
    bool
    GetFragmentation(void) const;

    void
    SetFragmentNumber (int fragmentNumber);
    int
    GetFragmentNumber (void) const;
    void
    SetFragmentOffset (int fragmentOffset);
    int
    GetFragmentOffset (void) const;

    void
    SetTempFragmentNumber (int fragmentNumber);
    int
    GetTempFragmentNumber (void) const;
    void
    SetTempFragmentOffset (int fragmentOffset);
    int
    GetTempFragmentOffset (void) const;
  };

  typedef deque<QueueElement> PacketQueue;
  PacketQueue *m_queue;
  int m_maxSize;
  int m_queueSize;
  int m_nbDataPackets;

public:
  MacQueue();
  virtual ~MacQueue();

  void
  SetMaxSize (int maxSize);
  int
  GetMaxSize (void) const;
  void
  SetQueueSize (int size);
  void
  UpdateQueueSize (int packetSize);
  int
  GetQueueSize (void) const;
  int
  GetQueueSizeWithMACHoverhead (void) const;
  void
  UpdateNbDataPackets (void);
  int
  GetNbDataPackets (void) const;

  PacketQueue*
  GetPacketQueue (void) const;

  bool
  Enqueue (Packet *packet);
  QueueElement
  Peek (void) const;
  bool
  IsEmpty (void) const;

  Packet*
  GetPacketToTramsit (int availableBytes);
  void
  Dequeue ();

  void
  ModifyPacketSourceID (int id);

  void
  CheckForDropPackets (double maxDelay,
                       int bearerID);
  int GetByte (int byte); //for FLS scheduler


  void PrintQueueInfo (void);
};

#endif /* MACQUEUE_H_ */
