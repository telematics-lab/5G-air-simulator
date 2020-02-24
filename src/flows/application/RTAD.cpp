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
 * Author: Alessandro Grassi <alessandro.grassi@poliba.it>
 */



#include "RTAD.h"
#include <cstdlib>
#include "../../componentManagers/NetworkManager.h"
#include "../radio-bearer.h"

#define MAXMTUSIZE 1490

RTAD::RTAD()
{
  SetApplicationType (Application::APPLICATION_TYPE_FTP2);
  m_generationInterval = 0.036;
}

void
RTAD::DoStart (void)
{
  Simulator::Init()->Schedule(0.0, &RTAD::Send, this);
}

void
RTAD::DoStop (void)
{
}

void
RTAD::ScheduleTransmit (double time)
{
  if ( (Simulator::Init()->Now () + time) < GetStopTime () )
    {
      Simulator::Init()->Schedule(time, &RTAD::Send, this);
    }
}

void
RTAD::Send (void)
{
  int dataOfBurstAlreadySent = 0;

  SetSize();

  for (int i = 0; i < GetSize() / MAXMTUSIZE; i++)
    {
      //CREATE A NEW PACKET (ADDING UDP, IP and PDCP HEADERS)
      Packet *packet = new Packet ();
      int uid = Simulator::Init()->GetUID ();

      packet->SetID(uid);
      packet->SetTimeStamp (Simulator::Init()->Now ());
      packet->SetSize (MAXMTUSIZE);

      Trace (packet);

      PacketTAGs *tags = new PacketTAGs ();
      tags->SetApplicationType(PacketTAGs::APPLICATION_TYPE_FTP2);
      tags->SetStartByte(dataOfBurstAlreadySent);
      tags->SetEndByte(dataOfBurstAlreadySent+MAXMTUSIZE-1);
      tags->SetApplicationSize (packet->GetSize ());
      dataOfBurstAlreadySent+=MAXMTUSIZE;
      packet->SetPacketTags(tags);


      UDPHeader *udp = new UDPHeader (GetClassifierParameters ()->GetSourcePort (),
                                      GetClassifierParameters ()->GetDestinationPort ());
      packet->AddUDPHeader (udp);

      IPHeader *ip = new IPHeader (GetClassifierParameters ()->GetSourceID (),
                                   GetClassifierParameters ()->GetDestinationID ());
      packet->AddIPHeader (ip);

      PDCPHeader *pdcp = new PDCPHeader ();
      packet->AddPDCPHeader (pdcp);

      GetRadioBearer()->Enqueue (packet);
    }

  uint16_t sizetosend = GetSize() % MAXMTUSIZE;

  if (sizetosend > 0)
    {
      //CREATE A NEW PACKET (ADDING UDP, IP and PDCP HEADERS)
      Packet *packet = new Packet ();
      int uid = Simulator::Init()->GetUID ();

      packet->SetID(uid);
      packet->SetTimeStamp (Simulator::Init()->Now ());
      packet->SetSize (sizetosend);

      Trace (packet);

      PacketTAGs *tags = new PacketTAGs ();
      tags->SetApplicationType(PacketTAGs::APPLICATION_TYPE_FTP2);
      tags->SetStartByte(dataOfBurstAlreadySent);
      tags->SetEndByte(dataOfBurstAlreadySent+sizetosend-1);
      tags->SetApplicationSize (packet->GetSize ());
      dataOfBurstAlreadySent+=sizetosend;
      packet->SetPacketTags(tags);


      UDPHeader *udp = new UDPHeader (GetClassifierParameters ()->GetSourcePort (),
                                      GetClassifierParameters ()->GetDestinationPort ());
      packet->AddUDPHeader (udp);

      IPHeader *ip = new IPHeader (GetClassifierParameters ()->GetSourceID (),
                                   GetClassifierParameters ()->GetDestinationID ());
      packet->AddIPHeader (ip);

      PDCPHeader *pdcp = new PDCPHeader ();
      packet->AddPDCPHeader (pdcp);

      GetRadioBearer()->Enqueue (packet);
    }

  double interval = -log((double )rand()/RAND_MAX)*GetInterval ();
  //round to milliseconds
  interval = round(interval*1000)/1000;

  ScheduleTransmit (interval);

}


int
RTAD::GetSize (void) const
{
  return m_size;
}

void
RTAD::SetSize()
{
  m_size = (double)rand()/RAND_MAX*3*1024 + 3*1024;
}

double
RTAD::GetInterval (void) const
{
  return m_generationInterval;
}

