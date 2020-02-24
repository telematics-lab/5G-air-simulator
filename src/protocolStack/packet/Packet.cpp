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

#include "Packet.h"
#include "../../core/eventScheduler/simulator.h"
#include <iostream>

Packet::Packet()
{
  m_UDPHeader = nullptr;
  m_IPHeader = nullptr;
  m_PDCPHeader = nullptr;
  m_RLCHeader = nullptr;
  m_MACHeader = nullptr;
  m_tags = nullptr;

  m_id = 0;
  m_timeStamp = 0;
  m_size = 0;
}

Packet::~Packet()
{
  delete m_UDPHeader;
  delete m_IPHeader;
  delete m_PDCPHeader;
  delete m_RLCHeader;
  delete m_MACHeader;
  delete m_tags;
}

void
Packet::SetTimeStamp (double time)
{
  m_timeStamp = time;
}

double
Packet::GetTimeStamp (void) const
{
  return m_timeStamp;
}

void
Packet::SetSize (int size)
{
  m_size = size;
}

int
Packet::GetSize (void) const
{
  return m_size;
}

void
Packet::AddHeaderSize (int size)
{
  m_size += size;
}

void
Packet::UpdatePacketSize (void)
{}

void
Packet::AddUDPHeader (UDPHeader *header)
{
  m_UDPHeader = header;
  AddHeaderSize (8);
}

UDPHeader*
Packet::GetUDPHeader (void) const
{
  return m_UDPHeader;
}

void
Packet::AddIPHeader (IPHeader *header)
{
  m_IPHeader = header;
  AddHeaderSize (20); //Header size fo IPv4
}

IPHeader*
Packet::GetIPHeader (void)
{
  return  m_IPHeader;
}

void
Packet::AddPDCPHeader (PDCPHeader *header)
{
  m_PDCPHeader = header;

  //ROHC Headers Compression: form 40 bytes to 3 bytes
  int newPacketSize = GetSize () - 28 + 3;
  SetSize (newPacketSize);

  AddHeaderSize (2);
}

PDCPHeader*
Packet::GetPDCPHeader (void) const
{
  return  m_PDCPHeader;

}

void
Packet::AddRLCHeader (RLCHeader *header)
{
  m_RLCHeader = header;
  AddHeaderSize (header->GetHeaderSize());
}

RLCHeader*
Packet::GetRLCHeader (void) const
{
  return m_RLCHeader;
}

void
Packet::AddMACHeader (MACHeader *header)
{
  m_MACHeader = header;
  AddHeaderSize(header->GetHeaderSize());
}

MACHeader*
Packet::GetMACHeader (void) const
{
  return m_MACHeader;
}


int
Packet::GetSourceID ()
{
  return GetIPHeader ()->GetSourceID ();
}

int
Packet::GetDestinationID ()
{
  return GetIPHeader ()->GetDestinationID ();
}

int
Packet::GetSourcePort ()
{
  return GetUDPHeader ()->GetSourcePort();
}

int
Packet::GetDestinationPort ()
{
  return GetUDPHeader ()->GetDestinationPort();
}

int
Packet::GetSourceMAC ()
{
  return GetMACHeader()->GetMACsourceID();
}

int
Packet::GetDestinationMAC ()
{
  return GetMACHeader()->GetMACdestinationID();
}

void
Packet::SetID (int uid)
{
  m_id = uid;
}

int
Packet::GetID (void)
{
  return m_id;
}

PacketTAGs*
Packet::GetPacketTags (void) const
{
  return m_tags;
}

void
Packet::SetPacketTags (PacketTAGs* tags)
{
  m_tags = tags;
}

Packet*
Packet::Copy (void)
{
  Packet *p = new Packet ();
  p->m_id = m_id;
  p->m_timeStamp = m_timeStamp;

  if (m_UDPHeader != nullptr)
    {
      UDPHeader *udp = new UDPHeader (GetSourcePort(), GetDestinationPort());
      p->AddUDPHeader(udp);
    }

  if (m_IPHeader != nullptr)
    {
      IPHeader *ip = new IPHeader (GetSourceID(), GetDestinationID());
      p->AddIPHeader(ip);
    }

  if (m_PDCPHeader != nullptr)
    {
      PDCPHeader *pdcp = new PDCPHeader ();
      p->AddPDCPHeader (pdcp);
    }

  if (m_RLCHeader != nullptr)
    {
      RLCHeader *rlc = new RLCHeader ();

      if (GetRLCHeader ()->IsAFragment ())
        rlc->SetAFragment (true);
      if (!GetRLCHeader ()->IsTheLatestFragment ())
        rlc->SetTheLatestFragment (false);

      rlc->SetFragmentNumber (GetRLCHeader ()->GetFragmentNumber());
      rlc->SetRlcEntityIndex (GetRLCHeader ()->GetRlcEntityIndex ());

      rlc->SetRlcPduSequenceNumber (GetRLCHeader ()->GetRlcPduSequenceNumber());
      rlc->SetStartByte (GetRLCHeader ()->GetStartByte ());
      rlc->SetEndByte (GetRLCHeader ()->GetEndByte ());

      p->AddRLCHeader (rlc);
    }

  if (m_MACHeader != nullptr)
    {
      MACHeader *mac = new MACHeader (GetSourceMAC (), GetDestinationMAC ());
      p->AddMACHeader(mac);
    }

  if (m_tags != nullptr)
    {
      PacketTAGs *tags = new PacketTAGs ();
      tags->SetApplicationType(GetPacketTags()->GetApplicationType());
      tags->SetFrameNumber (GetPacketTags ()->GetFrameNuber ());
      tags->SetStartByte (GetPacketTags ()->GetStartByte ());
      tags->SetEndByte (GetPacketTags ()->GetEndByte ());
      tags->SetApplicationSize (GetPacketTags ()->GetApplicationSize ());
      p->SetPacketTags(tags);
    }

  p->m_size = m_size;

  return p;
}

//Debug
void
Packet::Print ()
{
  cout <<
            " ********** \n **Packet:"
            "\n  ** ID " <<  GetID () <<
            " timeStamp " <<  GetTimeStamp () <<
            "\n  ** size: " << GetSize() <<
            "\n  ** sourcePort "  << GetSourcePort() <<
            " destinationPort " << GetDestinationPort() <<
            "\n  ** srcIP " << GetSourceID() <<
            " dstIP " << GetDestinationID() <<
            "\n  ** srcMAC " << GetSourceMAC() <<
            " dstMAC " << GetDestinationMAC() << endl;

  if (m_RLCHeader->IsAFragment())
    cout << "  ** IsAFragment TRUE ";
  else
    cout << "  ** IsAFragment FALSE ";

  if (m_RLCHeader->IsTheLatestFragment())
    cout << " IsTheLatestFragment TRUE ";
  else
    cout << " IsTheLatestFragment FALSE ";

  cout << "\n **********"  << endl;
}
