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

#include "am-rlc-entity.h"
#include "../packet/packet-burst.h"
#include "../../flows/radio-bearer.h"
#include "../../flows/radio-bearer-sink.h"
#include "../../flows/MacQueue.h"
#include "../../flows/application/Application.h"
#include "../../flows/application/application-sink.h"
#include "../../flows/QoS/QoSParameters.h"
#include "../../device/NetworkNode.h"
#include "amd-record.h"
#include "../../core/idealMessages/ideal-control-messages.h"
#include "../../load-parameters.h"

#define MAX_AMD_RETX 5

AmRlcEntity::AmRlcEntity()
{
  SetDevice (nullptr);
  SetRadioBearer (nullptr);
  SetRlcMode(RlcEntity::AM_RLC_MODE);
  SetRlcPduSequenceNumber (0);
  m_amStateVariables = new AmStateVariables ();
  m_sentAMDs = new vector<AmdRecord*> ();
  m_receivedAMDs = new vector<AmdRecord*> ();
}

AmRlcEntity::~AmRlcEntity()
{
  delete m_amStateVariables;
  ClearPacketList ();
}

void
AmRlcEntity::ClearPacketList (void)
{
  for (auto amdRecord : *m_sentAMDs)
    {
      delete amdRecord;
    }
  m_sentAMDs->clear ();

  for (auto amdRecord : *m_receivedAMDs)
    {
      delete amdRecord;
    }
  m_receivedAMDs->clear ();
}

void
AmRlcEntity::PrintSentAMDs (void)
{
  cout << "\t Print Sent and Unacknowledged AMDs" << endl;
  for (auto amdRecord : *m_sentAMDs)
    {
      Packet* packet = amdRecord->m_packet;
      cout << "\t\t *** pkt " << packet->GetID() << " frag " << packet->GetRLCHeader ()->GetFragmentNumber () <<
                " sn " << packet->GetRLCHeader ()->GetRlcPduSequenceNumber () <<
                " startB " << packet->GetRLCHeader ()->GetStartByte () <<
                " endB " << packet->GetRLCHeader ()->GetEndByte () << endl;

    }
}

void
AmRlcEntity::PrintReceivedAMDs (void)
{
  cout << "\t Print Received AMDs" << endl;
  for (auto amdRecord : *m_receivedAMDs)
    {
      Packet* packet = amdRecord->m_packet;
      cout << "\t\t *** pkt " << packet->GetID() << " frag " << packet->GetRLCHeader ()->GetFragmentNumber () <<
                " sn " << packet->GetRLCHeader ()->GetRlcPduSequenceNumber () <<
                " startB " << packet->GetRLCHeader ()->GetStartByte () <<
                " endB " << packet->GetRLCHeader ()->GetEndByte () << endl;

    }
}


AmStateVariables*
AmRlcEntity::GetAmStateVariables ()
{
  return m_amStateVariables;
}

vector<AmdRecord*>*
AmRlcEntity::GetSentAMDs (void)
{
  return m_sentAMDs;
}

vector<AmdRecord*>*
AmRlcEntity::GetReceivedAMDs (void)
{
  return m_receivedAMDs;
}

void
AmRlcEntity::InsertAMDIntoReceptionList (AmdRecord* amd)
{

DEBUG_LOG_START_1(SIM_ENV_RLC_DEBUG)
  cout  << "Insert AMD Into Reception List:" << endl;
  PrintReceivedAMDs ();
DEBUG_LOG_END

  int amds = (int) m_receivedAMDs->size ();

  if (amds == 0)
    {
      DEBUG_LOG_START_1(SIM_ENV_RLC_DEBUG)
        cout << "no AMD into the list" << endl;
      DEBUG_LOG_END
      m_receivedAMDs->insert (m_receivedAMDs->begin (), amd);
      return;
    }
  
  int pduSN = amd->m_packet->GetRLCHeader ()->GetRlcPduSequenceNumber();
  
  for (int i = 0; i < amds; i++)
    {
      Packet* packet = m_receivedAMDs->at (i)->m_packet;

      if (pduSN < packet->GetRLCHeader ()->GetRlcPduSequenceNumber ())
        {
          DEBUG_LOG_START_1(SIM_ENV_RLC_DEBUG)
            cout << "insert the AMD" << endl;
          DEBUG_LOG_END
          m_receivedAMDs->insert (m_receivedAMDs->begin () + i, amd);
          return;
        }
      else if (pduSN > packet->GetRLCHeader ()->GetRlcPduSequenceNumber ()
               && i == amds - 1)
        {
          DEBUG_LOG_START_1(SIM_ENV_RLC_DEBUG)
            cout << "insert the AMD at the end of the queue" << endl;
          DEBUG_LOG_END
          m_receivedAMDs->insert (m_receivedAMDs->end (), amd);
          return;
        }
      else if (pduSN == packet->GetRLCHeader ()->GetRlcPduSequenceNumber ())
        {
          if (amd->m_packet->GetRLCHeader ()->GetEndByte () < packet->GetRLCHeader ()->GetStartByte ())
            {
              DEBUG_LOG_START_1(SIM_ENV_RLC_DEBUG)
                cout << "insert before an AMD with the same SN" << endl;
              DEBUG_LOG_END
              m_receivedAMDs->insert (m_receivedAMDs->begin () + i, amd);
              return;
            }
          else if (i == amds - 1)
            {
              DEBUG_LOG_START_1(SIM_ENV_RLC_DEBUG)
                cout << "insert the AMD at the end of the queue" << endl;
              DEBUG_LOG_END
              m_receivedAMDs->insert (m_receivedAMDs->end (), amd);
              return;
            }
        }
      else
        {}
    }

DEBUG_LOG_START_1(SIM_ENV_RLC_DEBUG)
  PrintReceivedAMDs ();
DEBUG_LOG_END

}

shared_ptr<PacketBurst>
AmRlcEntity::TransmissionProcedure (int availableBytes)
{

DEBUG_LOG_START_1(SIM_ENV_RLC_DEBUG)
  cout << "AM RLC tx procedure for node " << GetRadioBearerInstance ()->GetSource ()->GetIDNetworkNode ()<< " bearer "<<  GetRlcEntityIndex () << " T " << Simulator::Init()->Now() << endl;
  m_amStateVariables->PrintTxVariables ();
DEBUG_LOG_END

  shared_ptr<PacketBurst> pb = make_shared<PacketBurst> ();
  RadioBearer *bearer = (RadioBearer*) GetRadioBearerInstance ();
  
  //STEP 1 CHECK FOR RE-TRANSMISSION

DEBUG_LOG_START_1(SIM_ENV_RLC_DEBUG)
  cout << "--> STEP 1:  CHECK FOR RE-TRANSMISSION" << endl;
  PrintSentAMDs ();
DEBUG_LOG_END

  int nbSentAMDs = (int) GetSentAMDs()->size();
  if (nbSentAMDs != 0)
    {
      int amdId = 0;
      while (availableBytes > 0 && amdId < nbSentAMDs)
        {
          AmdRecord* amdRecord = GetSentAMDs()->at (amdId);
          if (amdRecord->m_retx_count_timer == 0)
          {
            if (amdRecord->m_packet->GetSize () + 6 <= availableBytes) //6 = MAC  + CRC overhead
            {
              amdRecord->m_retx_count++;
              
              DEBUG_LOG_START_1(SIM_ENV_RLC_DEBUG)
                cout << "send the whole unacknowledged AMD PDU" << endl;
              DEBUG_LOG_END
              Packet* p = amdRecord->m_packet->Copy ();
              MACHeader *mac = new MACHeader (bearer->GetSource ()->GetIDNetworkNode (),
                                              bearer->GetDestination ()->GetIDNetworkNode ());
              p->AddMACHeader(mac);
              p->AddHeaderSize (3);
              
              if (_RLC_TRACING_)
              {
                cout << "TX AM_RLC SIZE " << p->GetSize () <<
                " B " << GetRlcEntityIndex () <<
                " PDU_SN " << p->GetRLCHeader ()->GetRlcPduSequenceNumber() << endl;
              }
              
              pb->AddPacket (p);
              availableBytes -= p->GetSize ();
              amdId++;
            }
            else if (availableBytes > 8) // 8 = RLC + MAC + CRC
            {
              amdRecord->m_retx_count++;
              DEBUG_LOG_START_1(SIM_ENV_RLC_DEBUG)
                cout << "there is not enough bandwidth for this AMD PDU. Do another fragmentation"
                << "--> bytes: " << availableBytes << endl;
              DEBUG_LOG_END
              Packet* p1 = amdRecord->m_packet->Copy ();
              Packet* p2 = amdRecord->m_packet;

              int sentBytes = availableBytes - 7;
              p1->GetRLCHeader ()->SetEndByte (p1->GetRLCHeader ()->GetStartByte () + sentBytes - 1);
              p1->SetSize (2 + sentBytes); //consider both RLC and sent bytes
              p2->GetRLCHeader ()->SetStartByte (p1->GetRLCHeader ()->GetStartByte () + sentBytes);
              p2->SetSize (p2->GetSize () - sentBytes); //decrease the packet size by the quota of sent bytes
              p1->GetRLCHeader ()->SetTheLatestFragment (false);

              p1->GetRLCHeader ()->SetAFragment (true);
              p2->GetRLCHeader ()->SetAFragment (true);

              MACHeader *mac = new MACHeader (bearer->GetSource ()->GetIDNetworkNode (),
                                              bearer->GetDestination ()->GetIDNetworkNode ());
              p1->AddMACHeader(mac);
              p1->AddHeaderSize (3); //CRC

              if (_RLC_TRACING_)
              {
                cout << "TX AM_RLC SIZE " << p1->GetSize () <<
                " B " << GetRlcEntityIndex () <<
                " PDU_SN " << p1->GetRLCHeader ()->GetRlcPduSequenceNumber() << endl;
              }
              
              pb->AddPacket (p1->Copy ());

              AmdRecord* newAmdRecord = new AmdRecord (p1, p1->GetRLCHeader ()->GetRlcPduSequenceNumber ());
              newAmdRecord->m_retx_count = amdRecord->m_retx_count;
              GetSentAMDs ()->insert(GetSentAMDs ()->begin() + amdId, newAmdRecord);

              DEBUG_LOG_START_1(SIM_ENV_RLC_DEBUG)
                PrintSentAMDs ();
              DEBUG_LOG_END
              availableBytes = 0;
            }
            else
            {
              availableBytes = 0;
            }
          }
          else {
            amdId++;
          }
        }
    }

  if (availableBytes <= 0 )
    return pb;

  //STEP 2 NEW TRANSMISSION
  
  MacQueue *queue = bearer->GetMacQueue ();
  
  DEBUG_LOG_START_1(SIM_ENV_RLC_DEBUG)
    cout << "--> STEP 2:  CHECK FOR NEW TRANSMISSION" << endl;
  DEBUG_LOG_END

  DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG_ARQ)
    queue->PrintQueueInfo();
  DEBUG_LOG_END
  
  if (bearer->GetApplication ()->GetApplicationType () == Application::APPLICATION_TYPE_INFINITE_BUFFER)
    {
      //CREATE PACKET FOR THE INFINITE BUFFER SOURCE
      while (true)
        {
          Packet *packet = bearer->CreatePacket (availableBytes);
          packet->GetRLCHeader ()->SetRlcEntityIndex (GetRlcEntityIndex ());

          // set sn
          int currentSN = m_amStateVariables->m_vt_s;
          packet->GetRLCHeader ()->SetRlcPduSequenceNumber (currentSN);

          //update am rlc state variables
          m_amStateVariables->m_vt_s++;

          //Add MAC header
          MACHeader *mac = new MACHeader (GetRadioBearerInstance ()->GetSource ()->GetIDNetworkNode (),
                                          GetRadioBearerInstance ()->GetDestination ()->GetIDNetworkNode ());
          packet->AddMACHeader(mac);
          packet->AddHeaderSize (3);

          if (availableBytes > 1503)
            {
              packet->SetSize (1503);
              packet->GetPacketTags ()->SetApplicationSize (1490);
              availableBytes -= 1503;

              if (_RLC_TRACING_)
                {
                  cout << "TX AM_RLC SIZE " << packet->GetSize () <<
                            " B " << GetRlcEntityIndex () <<
                            " PDU_SN " << packet->GetRLCHeader ()->GetRlcPduSequenceNumber() <<
                            " Frag " << packet->GetRLCHeader ()->IsAFragment () <<
                            " LastFrag " << packet->GetRLCHeader ()->IsTheLatestFragment() <<
                            " startB " << packet->GetRLCHeader ()->GetStartByte () <<
                            " endB " << packet->GetRLCHeader ()->GetEndByte () <<
                            endl;
                }

              pb->AddPacket (packet);
            }
          else if (availableBytes > 13)
            {
              packet->SetSize (availableBytes);
              packet->GetPacketTags ()->SetApplicationSize (availableBytes - 13);
              availableBytes = 0;

              if (_RLC_TRACING_)
                {
                  cout << "TX 2 AM_RLC SIZE " << packet->GetSize () <<
                            " B " << GetRlcEntityIndex () <<
                            " PDU_SN " << packet->GetRLCHeader ()->GetRlcPduSequenceNumber() <<
                            " Frag " << packet->GetRLCHeader ()->IsAFragment () <<
                            " LastFrag " << packet->GetRLCHeader ()->IsTheLatestFragment() <<
                            " startB " << packet->GetRLCHeader ()->GetStartByte () <<
                            " endB " << packet->GetRLCHeader ()->GetEndByte () <<
                            endl;
                }

              pb->AddPacket (packet);
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

DEBUG_LOG_START_1(SIM_ENV_RLC_DEBUG)
              cout << "SEND PACKET id " << packet->GetID() << " frag n " << packet->GetRLCHeader ()->GetFragmentNumber ()<< endl;
DEBUG_LOG_END

              //Set the id of the receiver RLC entity
              packet->GetRLCHeader ()->SetRlcEntityIndex (GetRlcEntityIndex ());
              // set sn
              int currentSN = m_amStateVariables->m_vt_s;
              packet->GetRLCHeader ()->SetRlcPduSequenceNumber (currentSN);

              //update am rlc state variables
              m_amStateVariables->m_vt_s++;

DEBUG_LOG_START_1(SIM_ENV_RLC_DEBUG)
              m_amStateVariables->PrintTxVariables ();
              cout << "___ pkt " << packet->GetID() << " frag " << packet->GetRLCHeader ()->GetFragmentNumber () <<
                        " sn " << packet->GetRLCHeader ()->GetRlcPduSequenceNumber () <<
                        " startB " << packet->GetRLCHeader ()->GetStartByte () <<
                        " endB " << packet->GetRLCHeader ()->GetEndByte () << endl;
              PrintSentAMDs ();
DEBUG_LOG_END


              AmdRecord *amdRecord = new AmdRecord (packet->Copy (), currentSN);
              GetSentAMDs ()->push_back (amdRecord);
DEBUG_LOG_START_1(SIM_ENV_RLC_DEBUG)
              PrintSentAMDs ();
DEBUG_LOG_END
              //Add MAC header
              MACHeader *mac = new MACHeader (GetRadioBearerInstance ()->GetSource ()->GetIDNetworkNode (),
                                              GetRadioBearerInstance ()->GetDestination ()->GetIDNetworkNode ());
              packet->AddMACHeader(mac);
              packet->AddHeaderSize (3);

              if (_RLC_TRACING_)
                {
                  cout << "TX AM_RLC SIZE " << packet->GetSize () <<
                            " B " << GetRlcEntityIndex () <<
                            " PDU_SN " << packet->GetRLCHeader ()->GetRlcPduSequenceNumber() << endl;
                }

              pb->AddPacket (packet);
              availableBytes -= packet->GetSize ();
            }
          else
            {
              availableBytes = 0;
            }
        }
    }
  
  for (auto amdRecord : *GetSentAMDs() ) {
    if (amdRecord->m_retx_count_timer == 1) {
      queue->SetQueueSize(queue->GetQueueSize()+amdRecord->m_packet->GetSize());
      cout<<endl;
    }
    amdRecord->m_retx_count_timer--;
  }
  
  DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG_ARQ)
    queue->PrintQueueInfo();
  DEBUG_LOG_END
  
  return pb;
}


void
AmRlcEntity::ReceptionProcedure (Packet* p)
{
  DEBUG_LOG_START_1(SIM_ENV_RLC_DEBUG)
  cout << "AM RLC rx procedure for node " << GetRadioBearerInstance ()->GetDestination ()->GetIDNetworkNode ()<< " bearer "<<  GetRlcEntityIndex () << " T " << Simulator::Init()->Now() << endl;
  m_amStateVariables->PrintRxVariables ();
  cout << "___ pkt " << p->GetID() << " frag " << p->GetRLCHeader ()->GetFragmentNumber () <<
  " sn " << p->GetRLCHeader ()->GetRlcPduSequenceNumber () <<
  " startB " << p->GetRLCHeader ()->GetStartByte () <<
  " endB " << p->GetRLCHeader ()->GetEndByte () << endl;
  DEBUG_LOG_END
  
  if (_RLC_TRACING_)
    {
      cout << "RX AM_RLC SIZE " << p->GetSize () <<
                " B " << GetRlcEntityIndex () <<
                " PDU_SN " << p->GetRLCHeader ()->GetRlcPduSequenceNumber() << endl;
    }

  int seqNumber = p->GetRLCHeader ()->GetRlcPduSequenceNumber ();

  /* NOT IMPLEMENTED YET
  if (seqNumber < amsState->m_vr_r || seqNumber >= amsState->m_vr_mr)
    {
    //discard the received packet and finish the rx procedure
    ReceptionProcedureEnd ();
    return;
    }
  */

  if (seqNumber >= m_amStateVariables->m_vr_h)
    {
      m_amStateVariables->m_vr_h = seqNumber + 1;
      m_amStateVariables->m_vr_mr = m_amStateVariables->m_vr_h + m_amStateVariables->m_am_window_size;
    }
  
  DEBUG_LOG_START_1(SIM_ENV_RLC_DEBUG)
    PrintReceivedAMDs ();
    m_amStateVariables->PrintRxVariables();
  DEBUG_LOG_END

  //create an ack
  //if (!bearer->GetApplication ()->GetSourceApplication ()->GetApplicationType () == Application::APPLICATION_TYPE_INFINITE_BUFFER)
  //{
  ArqRlcIdealControlMessage msg;
  msg.SetAck (seqNumber);
  msg.SetStartByte (p->GetRLCHeader ()->GetStartByte ());
  msg.SetEndByte (p->GetRLCHeader ()->GetEndByte ());
  SendArqRlcIdealControlMessage (msg);
  //}

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
  else if (p->GetRLCHeader ()->IsAFragment ())
    {
DEBUG_LOG_START_1(SIM_ENV_RLC_DEBUG)
      cout << "\t received a fragment " << endl;
DEBUG_LOG_END
      AmdRecord *amdRecord = new AmdRecord (p, seqNumber);
      InsertAMDIntoReceptionList (amdRecord);

DEBUG_LOG_START_1(SIM_ENV_RLC_DEBUG)
      PrintReceivedAMDs ();
DEBUG_LOG_END
      ReceptionProcedureEnd ();
    }
}

void
AmRlcEntity::ReceptionProcedureEnd ()
{
DEBUG_LOG_START_1(SIM_ENV_RLC_DEBUG)
  cout << "AM RLC ReceptionProcedureEnd " << endl;
  PrintReceivedAMDs ();
DEBUG_LOG_END

  if (m_receivedAMDs->size () == 0) return;

  int currentPacket = -1;
  int expectedNextByte = 0;

  list<int> packetIdToDelete;

  for (auto amdRecord : *m_receivedAMDs)
    {
      Packet* packet = amdRecord->m_packet;

      if (packet->GetID () != currentPacket && packet->GetRLCHeader ()->GetStartByte () == 0)
        {
          DEBUG_LOG_START_1(SIM_ENV_RLC_DEBUG)
            cout << "\t\t !! first fragment of the packet" << endl;
          DEBUG_LOG_END
          currentPacket = packet->GetID ();
          expectedNextByte = packet->GetRLCHeader ()->GetEndByte () + 1;
        }
      else if (packet->GetID () != currentPacket && packet->GetRLCHeader ()->GetStartByte () != 0)
        {
          DEBUG_LOG_START_1(SIM_ENV_RLC_DEBUG)
            cout << "\t\t !! fragment of a new packet, but not the first" << endl;
          DEBUG_LOG_END
          expectedNextByte = 0;
        }
      else if (packet->GetID () == currentPacket && packet->GetRLCHeader ()->GetStartByte () ==  expectedNextByte)
        {
          if (packet->GetRLCHeader ()->IsTheLatestFragment ())
            {
              DEBUG_LOG_START_1(SIM_ENV_RLC_DEBUG)
                cout << "\t\t !! consecutive fragment --> the packet can be reassembled" << endl;
              DEBUG_LOG_END
              RadioBearerSink *bearer = (RadioBearerSink*) GetRadioBearerInstance ();
              Packet* p = packet->Copy ();
              p->SetSize (packet->GetRLCHeader ()->GetEndByte () + 8);
              bearer->Receive (p);

              packetIdToDelete.push_front(packet->GetID ());

              expectedNextByte = 0;
            }
          else
            {
              DEBUG_LOG_START_1(SIM_ENV_RLC_DEBUG)
                cout << "\t\t !! consecutive fragment --> we expect another fragment" << endl;
              DEBUG_LOG_END
              expectedNextByte = packet->GetRLCHeader ()->GetEndByte () + 1;
            }
        }
    }


  //DELETE REASSEMBLED PACKETS!
DEBUG_LOG_START_1(SIM_ENV_RLC_DEBUG)
  cout << "\t\t !! DELETE REASSEMBLED PACKETS" << endl;
DEBUG_LOG_END

  for (auto id : packetIdToDelete)
    {
      vector <AmdRecord*> *newAmdList = new vector <AmdRecord*>;

      for (auto amdRecord : *m_receivedAMDs)
        {
          if (amdRecord->m_packet->GetID () == id)
            {
              delete amdRecord;
            }
          else
            {
              newAmdList->push_back (amdRecord);
            }
        }
      m_receivedAMDs->clear();
      delete m_receivedAMDs;
      m_receivedAMDs = newAmdList;
    }


  //DELETE PACKETS or AMDs WHOSE DEADLINE is EXPIRED !
DEBUG_LOG_START_1(SIM_ENV_RLC_DEBUG)
  cout << "\t\t !! DELETE PACKETS or AMDs WHOSE DEADLINE is EXPIRED !" << endl;
DEBUG_LOG_END

  int currentpacketId = -1;
  vector <AmdRecord*> *newAmdList = new vector <AmdRecord*>;
  for (auto amdRecord : *m_receivedAMDs)
    {
      double delay = amdRecord->m_packet->GetTimeStamp () + GetRadioBearerInstance ()->GetQoSParameters ()->GetMaxDelay ();
      if ((delay + 0.01) < Simulator::Init()->Now())
        {
          //trace for statistical purposes
          Packet *pp = amdRecord->m_packet;
          if (pp->GetID () !=  currentpacketId)
            {
              currentpacketId = amdRecord->m_packet->GetID ();

              if (_RLC_TRACING_)
                {
                  cout << "DROP_RX_AM_RLC";
                  if (pp->GetPacketTags()->GetApplicationType() == PacketTAGs::APPLICATION_TYPE_VOIP)
                    cout << " VOIP";
                  else if (pp->GetPacketTags()->GetApplicationType() == PacketTAGs::APPLICATION_TYPE_TRACE_BASED)
                    cout << " VIDEO";
                  else if (pp->GetPacketTags()->GetApplicationType() == PacketTAGs::APPLICATION_TYPE_CBR)
                    cout << " CBR";
                  else if (pp->GetPacketTags()->GetApplicationType() == PacketTAGs::APPLICATION_TYPE_INFINITE_BUFFER)
                    cout << " INF_BUF";
                  else if (pp->GetPacketTags()->GetApplicationType() == PacketTAGs::APPLICATION_TYPE_FTP2)
                    cout << " FTP";
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
            }
          delete amdRecord;
        }
      else
        {
          newAmdList->push_back (amdRecord);
        }
    }
  m_receivedAMDs->clear();
  delete m_receivedAMDs;
  m_receivedAMDs = newAmdList;



DEBUG_LOG_START_1(SIM_ENV_RLC_DEBUG)
  PrintReceivedAMDs ();
DEBUG_LOG_END
}

void
AmRlcEntity::SendArqRlcIdealControlMessage (ArqRlcIdealControlMessage msg)
{
DEBUG_LOG_START_1(SIM_ENV_RLC_DEBUG)
  cout << "AM RLC -- SendArqRlcIdealControlMessage, node " << GetRadioBearerInstance ()->GetDestination ()->GetIDNetworkNode () << " bearer "<<  GetRlcEntityIndex () << endl;
  cout << "\t ack " << msg.GetAck() << ", startB " << msg.GetStartByte () << ", endB " << msg.GetEndByte () << endl;
DEBUG_LOG_END

  //send AM RLC ACK to the receiver RLC under ideal control channel
  RadioBearerSink* thisBearer = (RadioBearerSink*) GetRadioBearerInstance ();
  ApplicationSink *thisApplication = (ApplicationSink*) thisBearer->GetApplication ();
  RadioBearer* remoteBearer = thisApplication->GetSourceApplication ()->GetRadioBearer ();

  AmRlcEntity* remoteRlc = (AmRlcEntity*) remoteBearer->GetRlcEntity();
  remoteRlc->ReceiveArqRlcIdealControlMessage (msg);
}

void
AmRlcEntity::ReceiveArqRlcIdealControlMessage (ArqRlcIdealControlMessage msg)
{
DEBUG_LOG_START_1(SIM_ENV_RLC_DEBUG)
  cout << "AM RLC -- ReceiveArqRlcIdealControlMessage, node " << GetRadioBearerInstance ()->GetSource ()->GetIDNetworkNode () << " bearer "<<  GetRlcEntityIndex () << endl;
  cout << "\t ack " << msg.GetAck() << ", startB " << msg.GetStartByte () << ", endB " << msg.GetEndByte () << endl;
  PrintSentAMDs ();
DEBUG_LOG_END

  //delete AMD PDU from m_sentAMDs
  vector<AmdRecord*>::iterator it;
  for (it = m_sentAMDs->begin(); it != m_sentAMDs->end(); it++)
    {
      AmdRecord *amdRecord = (*it);
      if (amdRecord->m_packet->GetRLCHeader ()->GetRlcPduSequenceNumber () == msg.GetAck()
          &&
          amdRecord->m_packet->GetRLCHeader ()->GetStartByte () == msg.GetStartByte ())
        {
          m_sentAMDs->erase (it);
          delete amdRecord;

DEBUG_LOG_START_1(SIM_ENV_RLC_DEBUG)
          cout << " || deleted AMD ||"<< endl;
          PrintSentAMDs ();
DEBUG_LOG_END
          return;
        }

    }
}

void
AmRlcEntity::CheckForDropPackets (double maxDelay, int bearerID)
{
DEBUG_LOG_START_1(SIM_ENV_RLC_DEBUG)
  cout << "AmRlcEntity::CheckForDropPackets " << endl;
  PrintSentAMDs ();
DEBUG_LOG_END

  int currentpacket = -1;
  double now = Simulator::Init()->Now();
  vector <AmdRecord*> *newAmdlist = new vector <AmdRecord*> ();

  for (auto amdRecord : *m_sentAMDs)
    {
      double HOL = now - amdRecord->m_packet->GetTimeStamp ();

      if (HOL > maxDelay || amdRecord->m_retx_count > MAX_AMD_RETX)
        {
          if (currentpacket != amdRecord->m_packet->GetID ())
            {
              currentpacket = amdRecord->m_packet->GetID ();

              if (_RLC_TRACING_)
                {
                  /*
                   * TRACE
                   */
                  cout << "DROP";
                  if (amdRecord->m_packet->GetPacketTags()->GetApplicationType() == PacketTAGs::APPLICATION_TYPE_VOIP)
                    cout << " VOIP";
                  else if (amdRecord->m_packet->GetPacketTags()->GetApplicationType() == PacketTAGs::APPLICATION_TYPE_TRACE_BASED)
                    cout << " VIDEO";
                  else if (amdRecord->m_packet->GetPacketTags()->GetApplicationType() == PacketTAGs::APPLICATION_TYPE_CBR)
                    cout << " CBR";
                  else if (amdRecord->m_packet->GetPacketTags()->GetApplicationType() == PacketTAGs::APPLICATION_TYPE_INFINITE_BUFFER)
                    cout << " INF_BUF";
                  else
                    cout << " UNKNOW";

                  cout << " ID "<< amdRecord->m_packet->GetID()
                            << " B " << GetRlcEntityIndex ();
                  if (amdRecord->m_packet->GetPacketTags() != nullptr
                      && amdRecord->m_packet->GetPacketTags()->GetApplicationType() ==
                      PacketTAGs::APPLICATION_TYPE_TRACE_BASED)
                    {
                      cout << " FRAME " <<  amdRecord->m_packet->GetPacketTags()->GetFrameNuber()
                                << " START " << amdRecord->m_packet->GetPacketTags()->GetStartByte()
                                << " END " << amdRecord->m_packet->GetPacketTags()->GetEndByte();
                    }
                  cout  <<  endl;
                }
            }
          delete amdRecord;
        }
      else
        {
          newAmdlist->push_back (amdRecord);
        }
    }

  m_sentAMDs->clear();
  delete m_sentAMDs;
  m_sentAMDs = newAmdlist;

DEBUG_LOG_START_1(SIM_ENV_RLC_DEBUG)
  PrintSentAMDs ();
DEBUG_LOG_END
}

int
AmRlcEntity::GetSizeOfUnaknowledgedAmd (void)
{
  int size = 0;
  for (auto amdRecord : *m_sentAMDs)
    {
      size += amdRecord->m_packet->GetSize () + 5; //add also MAC and CRC overhead
    }
  return size;
}
