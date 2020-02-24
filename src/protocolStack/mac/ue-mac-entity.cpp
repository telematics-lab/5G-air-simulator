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
 * Author: Alessandro Grassi <alessandro.grassi@poliba.it>
 * Author: Sergio Martiradonna <sergio.martiradonna@poliba.it>
 */


#include "ue-mac-entity.h"
#include "../packet/Packet.h"
#include "../packet/packet-burst.h"
#include "AMCModule.h"
#include "nb-AMCModule.h"
#include "../../core/idealMessages/ideal-control-messages.h"
#include "../../device/NetworkNode.h"
#include "../../device/UserEquipment.h"
#include "../../phy/phy.h"
#include "../../protocolStack/rrc/rrc-entity.h"
#include "../../flows/radio-bearer.h"
#include "../../flows/application/Application.h"
#include "../../flows/MacQueue.h"
#include "../../flows/QoS/QoSParameters.h"
#include "../rlc/am-rlc-entity.h"
#include "harq-manager.h"
#include "random-access/ue-baseline-random-access.h"
#include "random-access/ue-enhanced-random-access.h"
#include "random-access/ue-nb-iot-random-access.h"


UeMacEntity::UeMacEntity (NetworkNode* d)
{
  SetAmcModule (new AMCModule ());
  SetDevice (d);
  if (GetDevice()->GetNodeType() != NetworkNode::TYPE_MULTICAST_DESTINATION && _harq_active_)
      SetHarqManager (new HarqManager (GetDevice()));
  else
      SetHarqManager (nullptr);
  m_schedulingRequestInterval = 0.001;
  if (GetDevice()->GetNodeType() == NetworkNode::TYPE_MULTICAST_DESTINATION)
      m_uerandomaccess = nullptr;
  else
	  m_uerandomaccess = new UeBaselineRandomAccess(this);

}

UeMacEntity::~UeMacEntity ()
{
  delete m_harqManager;
  delete m_uerandomaccess;
  Destroy ();
}

double
UeMacEntity::GetSchedulingRequestInterval (void)
{
  return m_schedulingRequestInterval;
}

void
UeMacEntity::SetSchedulingRequestInterval (double t)
{
  m_schedulingRequestInterval = t;
}

UeRandomAccess*
UeMacEntity::GetRandomAccessManager (void)
{
  return m_uerandomaccess;
}

void UeMacEntity::SetRandomAccessType(UeRandomAccess::RandomAccessType type)
{
  delete m_uerandomaccess;
  switch (type)
    {
    case UeRandomAccess::RA_TYPE_BASELINE:
      m_uerandomaccess=new UeBaselineRandomAccess(this);
      break;

    case UeRandomAccess::RA_TYPE_ENHANCED:
      m_uerandomaccess=new UeEnhancedRandomAccess(this);
      break;

    case UeRandomAccess::RA_TYPE_NB_IOT:
      m_uerandomaccess=new UeNbIoTRandomAccess(this);
      break;

    default:
      cout << "error : unknown random access type " << type << endl;
      exit(1);
    }
}

void
UeMacEntity::SetRandomAccessManager(UeRandomAccess* s)
{
  m_uerandomaccess = s;
}

void
UeMacEntity::SendSchedulingRequest ()
{

  CheckForDropPackets ();

  //compute the buffer status report
  int bufferStaturReport = 0;
  RrcEntity *rrc = GetDevice ()->GetProtocolStack ()->GetRrcEntity ();

  if (rrc->GetRadioBearerContainer ()->size() > 0)
    {
      for (auto bearer : *rrc->GetRadioBearerContainer ())
        {
          if (bearer->GetApplication()->GetApplicationType ()
              != Application::APPLICATION_TYPE_INFINITE_BUFFER)
            {
              bufferStaturReport += bearer->GetQueueSize ();
            }
          else
            {
              bufferStaturReport += 10000000;
            }
        }
    }

  UserEquipment* thisNode = GetDevice ();
  //send the message
  if(thisNode->GetNodeState() == NetworkNode::STATE_ACTIVE)
    {
      //create the message
      SchedulingRequestIdealControlMessage *msg = new SchedulingRequestIdealControlMessage ();
      msg->SetSourceDevice (thisNode);
      msg->SetDestinationDevice (thisNode->GetTargetNode ());
      msg->SetBufferStatusReport (bufferStaturReport);
      GetDevice ()->GetPhy ()->SendIdealControlMessage (msg);
      thisNode ->SetLastActivity();
    }



}


void
UeMacEntity::ScheduleNbUplinkTransmission (int mcs, int ru)
{
  int availableBytes = (GetNbAmcModule()->GetTBSizeFromMCS(mcs, ru))/8;
DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
  cout << "SCHEDULE UL TRANSMISSION UE " << GetDevice()->GetIDNetworkNode()
       << " BYTE " << availableBytes
       << endl;
DEBUG_LOG_END
  shared_ptr<PacketBurst> pb = make_shared<PacketBurst> ();
  RrcEntity *rrc = GetDevice ()->GetProtocolStack ()->GetRrcEntity ();

  if (rrc->GetRadioBearerContainer ()->size() > 0)
    {
      for ( auto bearer : *rrc->GetRadioBearerContainer () )
        {
          if (availableBytes > 0)
            {
              RlcEntity *rlc = bearer->GetRlcEntity ();
              shared_ptr<PacketBurst> pb2 = rlc->TransmissionProcedure (availableBytes);
              if (pb2->GetNPackets () > 0)
                {
                  for (auto packet : pb2->GetPackets ())
                    {
                      pb->AddPacket (packet->Copy ());
                    }
                }
              availableBytes -= pb2->GetSize ();
            }
        }
      GetDevice ()->SendPacketBurst (pb);
    }
}


void
UeMacEntity::ScheduleUplinkTransmission (int nbPrb, int mcs)
{
  int availableBytes = GetAmcModule ()->GetTBSizeFromMCS (mcs, nbPrb)/8;

  shared_ptr<PacketBurst> pb = make_shared<PacketBurst> ();
  RrcEntity *rrc = GetDevice ()->GetProtocolStack ()->GetRrcEntity ();

  if (rrc->GetRadioBearerContainer ()->size() > 0)
    {
      for (auto bearer : *rrc->GetRadioBearerContainer ())
        {
          if (availableBytes > 0)
            {
              RlcEntity *rlc = bearer->GetRlcEntity ();
              shared_ptr<PacketBurst> pb2 = rlc->TransmissionProcedure (availableBytes);
              if (pb2->GetNPackets () > 0)
                {
                  for (auto packet : pb2->GetPackets())
                    {
                      pb->AddPacket (packet->Copy ());
                    }
                }
              availableBytes -= pb2->GetSize ();
            }
        }

      GetDevice ()->SendPacketBurst (pb);
    }
}

void
UeMacEntity::ReceiveIdealControlMessage (IdealControlMessage* msg)
{
  if (msg->GetMessageType () == IdealControlMessage::RA_RESPONSE)
    {
      ReceiveRandomAccessResponseIdealControlMessage ( (RandomAccessResponseIdealControlMessage*)msg );
    }
    else if (msg->GetMessageType()== IdealControlMessage::RA_CONNECTION_RESOLUTION)
    {
      ReceiveRandomAccessConnectionResolutionIdealControlMessage ( (RandomAccessConnectionResolutionIdealControlMessage*)msg );
    }
  else
    {
      cout << "Error in UeMacEntity::ReceiveIdealControlMessage(): unknown message type received" << endl;
      exit(1);
    }
}

void
UeMacEntity::ReceiveRandomAccessResponseIdealControlMessage(RandomAccessResponseIdealControlMessage* msg)
{
  GetRandomAccessManager()->ReceiveMessage2( msg->getMsg3Time() );
}

void
UeMacEntity::ReceiveRandomAccessConnectionResolutionIdealControlMessage(RandomAccessConnectionResolutionIdealControlMessage* msg)
{
  GetRandomAccessManager()->ReceiveMessage4();
}

void
UeMacEntity::CheckForDropPackets (void)
{
  RrcEntity *rrc = GetDevice ()->GetProtocolStack ()->GetRrcEntity ();

  for (auto bearer : *rrc->GetRadioBearerContainer())
    {
      if (bearer->GetMacQueue()->GetNbDataPackets()>0)
        {
          //delete packets from queue
          bearer->GetMacQueue ()->CheckForDropPackets (
          bearer->GetQoSParameters ()->GetMaxDelay (), bearer->GetApplication ()->GetApplicationID ());
        }

      //delete fragment waiting in AM RLC entity
      if (bearer->GetRlcEntity()->GetRlcModel() == RlcEntity::AM_RLC_MODE)
        {
          AmRlcEntity* amRlc = (AmRlcEntity*) bearer->GetRlcEntity();
          amRlc->CheckForDropPackets (
            bearer->GetQoSParameters ()->GetMaxDelay (), bearer->GetApplication ()->GetApplicationID ());
        }
    }
}

void
UeMacEntity::SetHarqManager (HarqManager* harqManager)
{
  m_harqManager = harqManager;
}

HarqManager*
UeMacEntity::GetHarqManager (void)
{
  return m_harqManager;
}


UserEquipment*
UeMacEntity::GetDevice(void)
{
  NetworkNode* device = ((MacEntity*)this)->GetDevice();
  return (UserEquipment*)device;
}
