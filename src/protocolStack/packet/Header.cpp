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


#include "Header.h"

void
Header::SetProtocolHeader (ProtocolHeader protocolHeader)
{
  m_protocolHeader = protocolHeader;
}

Header::ProtocolHeader
Header::GetProtocolHeader (void) const
{
  return m_protocolHeader;
}

void
Header::SetHeaderSize (int size)
{
  m_size = size;
}

int
Header::GetHeaderSize (void) const
{
  return m_size;
}



/*
 * Transport Protocol HEADER
 */

void
TransportHeader::SetSourcePort (int port)
{
  m_sourcePort = port;
}

int
TransportHeader::GetSourcePort (void) const
{
  return m_sourcePort;
}

void
TransportHeader::SetDestinationPort (int port)
{
  m_destinationPort = port;
}

int
TransportHeader::GetDestinationPort (void) const
{
  return m_destinationPort;
}

void
TransportHeader::SetProtocolType (TransportProtocol::TransportProtocolType protocol)
{
  m_protocolType = protocol;
}

TransportProtocol::TransportProtocolType
TransportHeader::GetProtocolType (void) const
{
  return m_protocolType;
}


/*
 * UDP HEADER
 */
UDPHeader::UDPHeader(int sourcePort, int destinationPort)
{
  SetSourcePort (sourcePort);
  SetDestinationPort (destinationPort);
  SetProtocolHeader (Header::HEADER_UDP);
  SetHeaderSize (8);
  SetProtocolType (TransportProtocol::TRANSPORT_PROTOCOL_TYPE_UDP);
}


/*
 * IP HEADER
 */
IPHeader::IPHeader()
{
  SetProtocolHeader (Header::HEADER_IP);
  SetHeaderSize (20);
}

IPHeader::IPHeader(int sourceID, int destinationID)
    : IPHeader::IPHeader()
{
  m_sourceID = sourceID;
  m_destinationID = destinationID;
}

void
IPHeader::SetSourceID (int id)
{
  m_sourceID = id;
}

int
IPHeader::GetSourceID (void) const
{
  return m_sourceID;
}

void
IPHeader::SetDestinationID (int id)
{
  m_destinationID = id;
}

int
IPHeader::GetDestinationID (void) const
{
  return m_destinationID;
}



/*
 * PDCP HEADER
 */
PDCPHeader::PDCPHeader()
{
  SetProtocolHeader (Header::HEADER_PDCP);
  SetHeaderSize (2);
}




/*
 * RLC HEADER
 */
RLCHeader::RLCHeader()
{
  SetProtocolHeader (Header::HEADER_RLC);
  SetHeaderSize (2);
  m_isAFragment = false;
  m_isTheLatestFragment = true;
  m_fragmentNumber = 0;
  m_rlcEntityIndex = -1;
  m_startByte = 0;
  m_endByte = 0;
}

bool
RLCHeader::IsTheLatestFragment ()
{
  return m_isTheLatestFragment;
}

bool
RLCHeader::IsAFragment ()
{
  return m_isAFragment;
}

void
RLCHeader::SetTheLatestFragment (bool flag)
{
  m_isTheLatestFragment = flag;
}

void
RLCHeader::SetAFragment (bool flag)
{
  m_isAFragment = flag;
}

void
RLCHeader::SetFragmentNumber (int n)
{
  m_fragmentNumber = n;
}

int
RLCHeader::GetFragmentNumber (void)
{
  return m_fragmentNumber;
}

void
RLCHeader::SetRlcEntityIndex (int i)
{
  m_rlcEntityIndex = i;
}

int
RLCHeader::GetRlcEntityIndex (void)
{
  return m_rlcEntityIndex;
}

void
RLCHeader::SetRlcPduSequenceNumber (int i)
{
  m_rlcPduSequenceNumber = i;
}

int
RLCHeader::GetRlcPduSequenceNumber (void)
{
  return m_rlcPduSequenceNumber;
}

void
RLCHeader::SetStartByte (int b)
{
  m_startByte = b;
}

void
RLCHeader::SetEndByte (int b)
{
  m_endByte = b;
}

int
RLCHeader::GetStartByte (void)
{
  return m_startByte;
}

int RLCHeader::GetEndByte (void)
{
  return m_endByte;
}

/*
 * MAC HEADER
 */
MACHeader::MACHeader()
    : MACHeader::MACHeader(0, 0)
{}

MACHeader::MACHeader(int sourceMAC, int destinationMAC)
{
  SetProtocolHeader (Header::HEADER_MAC);
  SetHeaderSize (3);

  m_macSourceID = sourceMAC;
  m_macDestinationID = destinationMAC;
}

void
MACHeader::SetMACsourceID (int id)
{
  m_macSourceID = id;
}

int
MACHeader::GetMACsourceID (void) const
{
  return m_macSourceID;
}

void
MACHeader::SetMACdestinationID (int id)
{
  m_macDestinationID = id;
}

int
MACHeader::GetMACdestinationID (void) const
{
  return m_macDestinationID;
}




