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


#include "TraceBased.h"
#include "Trace/default-trace.h"
#include <cstring>
#include "../../componentManagers/NetworkManager.h"
#include "../radio-bearer.h"
#include "../../device/GNodeB.h"
#include "../../device/MulticastDestination.h"

#define MAXMTUSIZE 1490

TraceBased::TraceBased()
{
  m_sent = 0;
  m_entries = new vector<TraceEntry> ();
  LoadDefaultTrace ();
  m_frameCounter = 0;
  SetApplicationType (Application::APPLICATION_TYPE_TRACE_BASED);
}

TraceBased::~TraceBased()
{
  if (m_is_internal_trace == false)
    {
      m_entries->clear ();
      delete m_entries;
    }
}

void
TraceBased::DoStart (void)
{
  Simulator::Init()->Schedule(0.0, &TraceBased::Send, this);
}

void
TraceBased::DoStop (void)
{}

void
TraceBased::SetTraceFile(string traceFile)
{
  LoadTrace (traceFile);
  m_is_internal_trace = false;
DEBUG_LOG_START_1(SIM_ENV_APPLICATION_DEBUG)
  cout << " TRACE_BASED_DEBUG: Add file trace to Trace-Based "
            "Application Client.....  DONE" << endl;
DEBUG_LOG_END

}

void
TraceBased::LoadTrace (string traceFile)
{
  if (m_entries->size() > 0)
    {
      m_entries->clear ();
    }

  uint32_t time, index, prevTime = 0;
  uint32_t size;
  char frameType;
  TraceEntry entry;
  ifstream ifTraceFile;
  ifTraceFile.open (traceFile.c_str (), ifstream::in);

  if (!ifTraceFile.good ())
    {
      cout << " TRACE_BASED_APPLICATION ERROR BAD FILE " << traceFile.c_str () << endl;
      exit (1);
    }
  while (ifTraceFile.good ())
    {
      ifTraceFile >> index >> frameType >> time >> size;
      if (frameType == 'B' || frameType == 'PB')
        {
          entry.TimeToSend = 0;
        }
      else
        {
          entry.TimeToSend = time - prevTime;
          prevTime = time;
        }
      entry.PacketSize = size;
      entry.FrameIndex = index;
      entry.FrameType = frameType;
      m_entries->push_back (entry);
    }
  ifTraceFile.close ();
  iter = m_entries->begin ();
}

void
TraceBased::LoadInternalTrace(vector<TraceEntry>* trace)
{
  m_entries->clear ();
  delete m_entries;
  m_entries = trace;
  iter = trace->begin();
  m_is_internal_trace = true;
}


void
TraceBased::LoadDefaultTrace (void)
{
  uint32_t time, index, prevTime = 0;
  uint32_t size;
  char frameType;
  TraceEntry entry;
  for (int i = 0; i < NB_FRAME; i++)
    {
      index = i + 1;
      frameType = frameType_Tab[i];
      time = time_Tab[i];
      size = size_Tab[i];

      if (frameType == 'B')
        {
          entry.TimeToSend = 0;
        }
      else
        {
          entry.TimeToSend = time - prevTime;
          prevTime = time;
        }
      entry.PacketSize = size;
      entry.FrameIndex = index;
      entry.FrameType = frameType;
      m_entries->push_back (entry);
    }
  iter = m_entries->begin ();
  m_is_internal_trace = false;
}

void
TraceBased::ScheduleTransmit (double time)
{
  if ( (Simulator::Init()->Now () + time) < GetStopTime () )
    {
      Simulator::Init()->Schedule(time, &TraceBased::Send, this);
    }
}

void
TraceBased::Send (void)
{

  int dataOfFrameAlreadySent = 0;

  while (true)
    {
      for (int i = 0; i < (int)(iter->PacketSize) / MAXMTUSIZE; i++)
        {
          //CREATE A NEW PACKET (ADDING UDP, IP and PDCP HEADERS)
          Packet *packet = new Packet ();
          int uid = Simulator::Init()->GetUID ();

          packet->SetID(uid);
          packet->SetTimeStamp (Simulator::Init()->Now ());
          packet->SetSize (MAXMTUSIZE);
          if(GetDestination()->GetNodeState() == NetworkNode::STATE_ACTIVE)
            {
          Trace (packet);
            }
          PacketTAGs *tags = new PacketTAGs ();
          tags->SetApplicationType(PacketTAGs::APPLICATION_TYPE_TRACE_BASED);
          tags->SetFrameNumber(GetFrameCounter());
          tags->SetStartByte(dataOfFrameAlreadySent);
          tags->SetEndByte(dataOfFrameAlreadySent+MAXMTUSIZE-1);
          tags->SetApplicationSize (packet->GetSize ());
          dataOfFrameAlreadySent+=MAXMTUSIZE;
          packet->SetPacketTags(tags);


          UDPHeader *udp = new UDPHeader (GetClassifierParameters ()->GetSourcePort (),
                                          GetClassifierParameters ()->GetDestinationPort ());
          packet->AddUDPHeader (udp);

          IPHeader *ip = new IPHeader (GetClassifierParameters ()->GetSourceID (),
                                       GetClassifierParameters ()->GetDestinationID ());
          packet->AddIPHeader (ip);

          PDCPHeader *pdcp = new PDCPHeader ();
          packet->AddPDCPHeader (pdcp);
          if(GetDestination()->GetNodeState() == NetworkNode::STATE_ACTIVE || GetDestination ()->GetNodeType() == NetworkNode::TYPE_MULTICAST_DESTINATION)
            {
              GetRadioBearer()->Enqueue (packet);
            }
        }

      uint32_t sizetosend = (iter->PacketSize) % MAXMTUSIZE;

      if (sizetosend > 0)
        {
          //CREATE A NEW PACKET (ADDING UDP, IP and PDCP HEADERS)
          Packet *packet = new Packet ();
          int uid = Simulator::Init()->GetUID ();

          packet->SetID(uid);
          packet->SetTimeStamp (Simulator::Init()->Now ());
          packet->SetSize (sizetosend);
          if (GetDestination ()->GetNodeType() == NetworkNode::TYPE_MULTICAST_DESTINATION)
          {
            MulticastDestination* dest = (MulticastDestination*) GetDestination();
            if(dest->GetNodeState() == NetworkNode::STATE_ACTIVE)
            {
            Trace (packet);
            }
          }
          else
          {
            if(GetDestination()->GetNodeState() == NetworkNode::STATE_ACTIVE)
            {
              Trace (packet);
            }
          }
          PacketTAGs *tags = new PacketTAGs ();
          tags->SetApplicationType(PacketTAGs::APPLICATION_TYPE_TRACE_BASED);
          tags->SetFrameNumber(GetFrameCounter());
          tags->SetStartByte(dataOfFrameAlreadySent);
          tags->SetEndByte(dataOfFrameAlreadySent+sizetosend-1);
          tags->SetApplicationSize (packet->GetSize ());
          dataOfFrameAlreadySent+=sizetosend;
          packet->SetPacketTags(tags);


          UDPHeader *udp = new UDPHeader (GetClassifierParameters ()->GetSourcePort (),
                                          GetClassifierParameters ()->GetDestinationPort ());
          packet->AddUDPHeader (udp);

          IPHeader *ip = new IPHeader (GetClassifierParameters ()->GetSourceID (),
                                       GetClassifierParameters ()->GetDestinationID ());
          packet->AddIPHeader (ip);

          PDCPHeader *pdcp = new PDCPHeader ();
          packet->AddPDCPHeader (pdcp);
          if(GetDestination()->GetNodeState() == NetworkNode::STATE_ACTIVE || GetDestination ()->GetNodeType() == NetworkNode::TYPE_MULTICAST_DESTINATION)
            {
              GetRadioBearer()->Enqueue (packet);
            }
        }

      iter++;
      m_frameCounter++;
      dataOfFrameAlreadySent = 0;

      if (iter == m_entries->end ())
        {
          iter = m_entries->begin ();
        }

      if (iter->TimeToSend != 0)
        {
          ScheduleTransmit ((iter->TimeToSend)*0.001);
          break;
        }
    }
}

void
TraceBased::UpdateFrameCounter (void)
{
  m_frameCounter++;
}

int
TraceBased::GetFrameCounter (void)
{
  return m_frameCounter;
}

void
TraceBased::PrintTrace(void)
{
  cout << "Print Trace "<< endl;

  int n=0;
  for (auto entry : *m_entries)
    {
      cout << entry.FrameIndex << " " << entry.FrameType
                << " " << entry.TimeToSend << " " << entry.PacketSize << endl;
      n++;
      if (n==10)
        return;
    }
}

void
TraceBased::SetBurstSize(int bytes)
{
  m_burstSize = bytes;
}

int
TraceBased::GetBurstSize(void)
{
  return m_burstSize;
}
