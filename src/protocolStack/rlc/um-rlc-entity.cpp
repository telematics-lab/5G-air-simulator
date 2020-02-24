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


#include "um-rlc-entity.h"
#include "../packet/packet-burst.h"
#include "../../flows/radio-bearer.h"
#include "../../flows/radio-bearer-sink.h"
#include "../../flows/MacQueue.h"
#include "../../flows/application/Application.h"
#include "../../device/NetworkNode.h"
#include "../../load-parameters.h"


UmRlcEntity::UmRlcEntity()
{
  SetDevice (nullptr);
  SetRadioBearer (nullptr);
  SetRlcPduSequenceNumber (0);
  SetRlcMode(RlcEntity::UM_RLC_MODE);
}

UmRlcEntity::~UmRlcEntity()
{
  ClearIncomingPackets ();
}

shared_ptr<PacketBurst>
UmRlcEntity::TransmissionProcedure (int availableBytes)
{
DEBUG_LOG_START_1(SIM_ENV_RLC_DEBUG)
  cout << "UM RLC tx procedure for node " << GetRadioBearerInstance ()->GetSource ()->GetIDNetworkNode ()<< endl;
DEBUG_LOG_END

  shared_ptr<PacketBurst> pb = make_shared<PacketBurst>();

  RadioBearer *bearer = (RadioBearer*) GetRadioBearerInstance ();
  MacQueue *queue = bearer->GetMacQueue ();

  if (bearer->GetApplication ()->GetApplicationType () == Application::APPLICATION_TYPE_INFINITE_BUFFER)
    {
      //CREATE PACKET FOR THE INFINITE BUFFER SOURCE
      while (true)
        {
          Packet *packet = bearer->CreatePacket (availableBytes);

          //Set the id of the receiver RLC entity
          packet->GetRLCHeader ()->SetRlcEntityIndex (GetRlcEntityIndex ());

          packet->GetRLCHeader ()->SetRlcPduSequenceNumber (GetRlcPduSequenceNumber ());
          int newSequenceNumber = GetRlcPduSequenceNumber () + 1;
          SetRlcPduSequenceNumber (newSequenceNumber);

          //Add MAC header
          MACHeader *mac = new MACHeader (GetRadioBearerInstance ()->GetSource ()->GetIDNetworkNode (),
                                          GetRadioBearerInstance ()->GetDestination ()->GetIDNetworkNode ());
          packet->AddMACHeader(mac);
          packet->AddHeaderSize (3); //CRC

          if (availableBytes > 1503)
            {
              packet->SetSize (1503);
              packet->GetPacketTags ()->SetApplicationSize (1490);
              //Set the id of the receiver RLC entity
              packet->GetRLCHeader ()->SetRlcEntityIndex (GetRlcEntityIndex ());
              packet->GetRLCHeader ()->SetRlcPduSequenceNumber (GetRlcPduSequenceNumber ());
              int newSequenceNumber = GetRlcPduSequenceNumber () + 1;
              SetRlcPduSequenceNumber (newSequenceNumber);

              availableBytes -= 1503;
              pb->AddPacket (packet);

              if (_RLC_TRACING_)
                {
                  cout << "TX UM_RLC SIZE" << packet->GetSize () <<
                            " B " << GetRlcEntityIndex () <<
                            " PDU_SN " << packet->GetRLCHeader ()->GetRlcPduSequenceNumber() << endl;
                }
            }
          else if (availableBytes > 13)
            {
              packet->SetSize (availableBytes);
              packet->GetPacketTags ()->SetApplicationSize (availableBytes - 13);
              //Set the id of the receiver RLC entity
              packet->GetRLCHeader ()->SetRlcEntityIndex (GetRlcEntityIndex ());
              packet->GetRLCHeader ()->SetRlcPduSequenceNumber (GetRlcPduSequenceNumber ());
              int newSequenceNumber = GetRlcPduSequenceNumber () + 1;
              SetRlcPduSequenceNumber (newSequenceNumber);

              availableBytes = 0;
              pb->AddPacket (packet);

              if (_RLC_TRACING_)
                {
                  cout << "TX UM_RLC SIZE" << packet->GetSize () <<
                            " B " << GetRlcEntityIndex () <<
                            " PDU_SN " << packet->GetRLCHeader ()->GetRlcPduSequenceNumber() << endl;
                }

              break;
            }
          else
            {
              availableBytes = 0;
              break;
            }
        }
    }
  else
    {
      while (availableBytes > 0 && !queue->IsEmpty ())
        {
          Packet* packet = queue->GetPacketToTramsit (availableBytes);

          if (packet != nullptr)
            {
              //Set the id of the receiver RLC entity
              packet->GetRLCHeader ()->SetRlcEntityIndex (GetRlcEntityIndex ());
              packet->GetRLCHeader ()->SetRlcPduSequenceNumber (GetRlcPduSequenceNumber ());
              int newSequenceNumber = GetRlcPduSequenceNumber () + 1;
              SetRlcPduSequenceNumber (newSequenceNumber);

              if (_RLC_TRACING_)
                {
                  cout << "TX UM_RLC SIZE" << packet->GetSize () <<
                            " B " << GetRlcEntityIndex () <<
                            " PDU_SN " << packet->GetRLCHeader ()->GetRlcPduSequenceNumber() << endl;
                }

              //Add MAC header
              MACHeader *mac = new MACHeader (GetRadioBearerInstance ()->GetSource ()->GetIDNetworkNode (),
                                              GetRadioBearerInstance ()->GetDestination ()->GetIDNetworkNode ());
              packet->AddMACHeader(mac);
              packet->AddHeaderSize (3); //CRC

              pb->AddPacket (packet);
              availableBytes -= packet->GetSize ();
            }
          else
            {
              availableBytes = 0;
            }
        }
    }

  return pb;
}


void
UmRlcEntity::ReceptionProcedure (Packet* p)
{
DEBUG_LOG_START_1(SIM_ENV_RLC_DEBUG)
  cout << "UM RLC rx procedure for node " << GetRadioBearerInstance ()->GetDestination ()->GetIDNetworkNode ()<< endl;
  cout << "RECEIVE PACKET id " << p->GetID() << " frag n " << p->GetRLCHeader ()->GetFragmentNumber ()<< endl;
DEBUG_LOG_END

  if (_RLC_TRACING_)
    {
      cout << "RX UM_RLC SIZE " << p->GetSize () <<
                " B " << GetRlcEntityIndex () <<
                " PDU_SN " << p->GetRLCHeader ()->GetRlcPduSequenceNumber() << endl;
    }

  if (m_incomingPacket.size() > 0 && p->GetID () != m_incomingPacket.at (0)->GetID ())
    {
DEBUG_LOG_START_1(SIM_ENV_RLC_DEBUG)
      cout << "received a new packet, delete enqueued fragments"<< endl;
DEBUG_LOG_END

      Packet *pp = m_incomingPacket.at (0);

      if (_RLC_TRACING_)
        {
          cout << "DROP_RX_UM_RLC";
          if (pp->GetPacketTags()->GetApplicationType() == PacketTAGs::APPLICATION_TYPE_VOIP)
            cout << " VOIP";
          else if (pp->GetPacketTags()->GetApplicationType() == PacketTAGs::APPLICATION_TYPE_TRACE_BASED)
            cout << " VIDEO";
          else if (pp->GetPacketTags()->GetApplicationType() == PacketTAGs::APPLICATION_TYPE_CBR)
            cout << " CBR";
          else if (pp->GetPacketTags()->GetApplicationType() == PacketTAGs::APPLICATION_TYPE_INFINITE_BUFFER)
            cout << " INF_BUF";
          else
            cout << " UNKNOW";

          cout << " ID "<< pp->GetID()
                    << " B " << GetRlcEntityIndex ();

          if (pp->GetPacketTags() != nullptr
              && pp->GetPacketTags()->GetApplicationType() ==
              PacketTAGs::APPLICATION_TYPE_TRACE_BASED)
            {
              cout << " FRAME " <<  pp->GetPacketTags()->GetFrameNuber()
                        << " START " << pp->GetPacketTags()->GetStartByte()
                        << " END " << pp->GetPacketTags()->GetEndByte();
            }
          cout  <<  endl;
        }

      ClearIncomingPackets ();
    }

  //The received packet is not a fragment
  if (!p->GetRLCHeader ()->IsAFragment ())
    {
DEBUG_LOG_START_1(SIM_ENV_RLC_DEBUG)
      cout << "\t received a packet " << endl;
DEBUG_LOG_END


      RadioBearerSink *bearer = (RadioBearerSink*) GetRadioBearerInstance ();
      bearer->Receive (p);
    }

  //The received packet is a fragment
  if (p->GetRLCHeader ()->IsAFragment () && !p->GetRLCHeader ()->IsTheLatestFragment())
    {

DEBUG_LOG_START_1(SIM_ENV_RLC_DEBUG)
      cout << "\t received a fragment " << endl;
DEBUG_LOG_END
      m_incomingPacket.push_back (p);
    }

  //The received packet is the latest fragment
  if (p->GetRLCHeader ()->IsAFragment () && p->GetRLCHeader ()->IsTheLatestFragment())
    {

DEBUG_LOG_START_1(SIM_ENV_RLC_DEBUG)
      cout << "\t received the latest fragment " << endl;
DEBUG_LOG_END
      m_incomingPacket.push_back (p);

      //check if all fragment have been received
      int numberOfPackets = p->GetRLCHeader ()->GetFragmentNumber () + 1;

      if ((int)m_incomingPacket.size () == numberOfPackets)
        {
          RadioBearerSink *bearer = (RadioBearerSink*) GetRadioBearerInstance ();
          bearer->Receive (p->Copy ());

          ClearIncomingPackets ();
        }
      else
        {

DEBUG_LOG_START_1(SIM_ENV_RLC_DEBUG)
          cout << "list of fragment incomplete -> delete all!"<< endl;
DEBUG_LOG_END

          if (_RLC_TRACING_)
            {
              cout << "DROP_RX_UM_RLC";
              if (p->GetPacketTags()->GetApplicationType() == PacketTAGs::APPLICATION_TYPE_VOIP)
                cout << " VOIP";
              else if (p->GetPacketTags()->GetApplicationType() == PacketTAGs::APPLICATION_TYPE_TRACE_BASED)
                cout << " VIDEO";
              else if (p->GetPacketTags()->GetApplicationType() == PacketTAGs::APPLICATION_TYPE_CBR)
                cout << " CBR";
              else if (p->GetPacketTags()->GetApplicationType() == PacketTAGs::APPLICATION_TYPE_INFINITE_BUFFER)
                cout << " INF_BUF";
              else
                cout << " UNKNOW";

              cout << " ID "<< p->GetID()
                        << " B " << GetRlcEntityIndex ();

              if (p->GetPacketTags() != nullptr
                  && p->GetPacketTags()->GetApplicationType() ==
                  PacketTAGs::APPLICATION_TYPE_TRACE_BASED)
                {
                  cout << " FRAME " <<  p->GetPacketTags()->GetFrameNuber()
                            << " START " << p->GetPacketTags()->GetStartByte()
                            << " END " << p->GetPacketTags()->GetEndByte();
                }
              cout  <<  endl;
            }

          ClearIncomingPackets ();
        }
    }
}

void
UmRlcEntity::ClearIncomingPackets (void)
{
  for (auto packet : m_incomingPacket)
    {
      delete packet;
    }
  m_incomingPacket.clear ();
}
