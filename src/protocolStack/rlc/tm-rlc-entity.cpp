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

#include "tm-rlc-entity.h"
#include "../packet/packet-burst.h"
#include "../../flows/radio-bearer.h"
#include "../../flows/radio-bearer-sink.h"
#include "../../flows/MacQueue.h"
#include "../../flows/application/Application.h"
#include "../../device/NetworkNode.h"
#include "../../load-parameters.h"


TmRlcEntity::TmRlcEntity()
{
  SetDevice (nullptr);
  SetRadioBearer (nullptr);
  SetRlcPduSequenceNumber (0);
  SetRlcMode(RlcEntity::TM_RLC_MODE);
}

shared_ptr<PacketBurst>
TmRlcEntity::TransmissionProcedure (int availableBytes)
{
DEBUG_LOG_START_1(SIM_ENV_RLC_DEBUG)
  cout << "TM RLC tx procedure for node " << GetRadioBearerInstance ()->GetSource ()->GetIDNetworkNode ()<< endl;
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

          //Add MAC header
          MACHeader *mac = new MACHeader (GetRadioBearerInstance ()->GetSource ()->GetIDNetworkNode (),
                                          GetRadioBearerInstance ()->GetDestination ()->GetIDNetworkNode ());
          packet->AddMACHeader(mac);
          packet->AddHeaderSize (3); //CRC

          if (availableBytes > 1503)
            {
              packet->SetSize (1503);
              packet->GetPacketTags ()->SetApplicationSize (1490);
              availableBytes -= 1503;
              pb->AddPacket (packet);

              if (_RLC_TRACING_)
                {
                  cout << "TX TM_RLC SIZE " << packet->GetSize () <<
                            " B " << GetRlcEntityIndex () << endl;
                }
            }
          else if (availableBytes > 13)
            {
              packet->SetSize (availableBytes);
              packet->GetPacketTags ()->SetApplicationSize (availableBytes - 13);
              availableBytes = 0;
              pb->AddPacket (packet);

              if (_RLC_TRACING_)
                {
                  cout << "TX TM_RLC SIZE " << packet->GetSize () <<
                            " B " << GetRlcEntityIndex () << endl;
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
          Packet* packet = queue->Peek ().GetPacket ();

          if ((packet->GetSize () + 6) <= availableBytes)
            {
              if (_RLC_TRACING_)
                {
                  cout << "TX TM_RLC SIZE " << packet->GetSize () <<
                            " B " << GetRlcEntityIndex () << endl;
                }

              Packet *p = packet->Copy ();

              queue->UpdateQueueSize (-p->GetSize ());
              queue->Dequeue ();

              //Add MAC header
              MACHeader *mac = new MACHeader (GetRadioBearerInstance ()->GetSource ()->GetIDNetworkNode (),
                                              GetRadioBearerInstance ()->GetDestination ()->GetIDNetworkNode ());
              p->AddMACHeader(mac);
              p->AddHeaderSize(3); //Add CRC Size
              pb->AddPacket (p);
              availableBytes -= p->GetSize ();
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
TmRlcEntity::ReceptionProcedure (Packet* p)
{
DEBUG_LOG_START_1(SIM_ENV_RLC_DEBUG)
  cout << "TM RLC rx procedure for node " << GetRadioBearerInstance ()->GetSource ()->GetIDNetworkNode ()<< endl;
DEBUG_LOG_END
  if (_RLC_TRACING_)
    {
      cout << "RX TM_RLC SIZE " << p->GetSize () <<
                " B " << GetRlcEntityIndex () << endl;
    }

  RadioBearerSink *bearer = (RadioBearerSink*) GetRadioBearerInstance ();
  bearer->Receive (p);
}
