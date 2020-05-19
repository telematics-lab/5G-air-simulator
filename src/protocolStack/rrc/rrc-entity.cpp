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



#include "rrc-entity.h"
#include "../packet/Packet.h"
#include "../../device/IPClassifier/ClassifierParameters.h"
#include "../../flows/radio-bearer.h"
#include "../../flows/radio-bearer-sink.h"
#include "../../load-parameters.h"
#include "../../device/NetworkNode.h"
#include "ho/handover-entity.h"
#include"../../flows/application/Application.h"
#include"../../flows/MacQueue.h"
#include "../../device/GNodeB.h"
#include "../../device/UserEquipment.h"

RrcEntity::RrcEntity ()
{
  m_bearers = new RadioBearersContainer ();
  m_sink = new RadioBearersSinkContainer ();
  m_device = nullptr;
  m_handover = new HandoverEntity ();
}

RrcEntity::~RrcEntity ()
{
  m_bearers->clear ();
  delete m_bearers;

  m_sink->clear ();
  delete m_sink;

  delete m_handover;
}

void
RrcEntity::SetDevice (NetworkNode* d)
{
  m_device = d;
  m_handover->SetDevice (d);
}


NetworkNode*
RrcEntity::GetDevice ()
{
  return m_device;
}

RrcEntity::RadioBearersContainer*
RrcEntity::GetRadioBearerContainer (void) {
    //TODO: CHECK GD: delete italian comments
    /*
     * Sort the radio bearers basing on the rtts of the packets in the relative queues.
     * Packets in the relative queues are ordered basing on their own rtts.
     * See MacQueue::GetSortedPacketQueue()
     */
    RadioBearer* temp;
    int min;
    //TODO: CHECK GD: ERROR: THIS IS A BIG MISTAKE
    // GetDevice returns a NetworkNode because the RRC entity exists for both GNBs and UEs.
    // What happens here? why the return type of GetDevice is always casted to UserEquipment* ?
    // The condition of the if is wrong.
//    I guess m_ulSchedType should go inside the mac entity, scheduling is a mac feature
//    I commented everything for compiling
    
    //if(((UserEquipment*) GetDevice())->GetTargetNode()->m_ulSchedType == 5) {     //sort the bearers if priority scheduler is active
//        for(int i=0; i<((int)m_bearers->size())-1; i++) {
//            min = i;
//            for(int j=i+1; j<(int)m_bearers->size(); j++) {
//                if(!(m_bearers->at(j)->GetMacQueue()->IsEmpty()) && !(m_bearers->at(min)->GetMacQueue()->IsEmpty())) {
//                    double rtts1 = m_bearers->at(j)->GetMacQueue()->Peek().m_packet->GetRtts();
//                    double rtts2 = m_bearers->at(min)->GetMacQueue()->Peek().m_packet->GetRtts();
//                    if(rtts1< rtts2) //cambiare questa condizione per invertire l'ordine
//                        min = j;
//                }
//
//            }
//            temp=m_bearers->at(min);
//            m_bearers->at(min)=m_bearers->at(i);
//            m_bearers->at(i)=temp;
//        }
//    //}
    return m_bearers;
}

RrcEntity::RadioBearersSinkContainer*
RrcEntity::GetRadioBearerSinkContainer (void)
{
  return m_sink;
}


void
RrcEntity::AddRadioBearer (RadioBearer* bearer)
{
DEBUG_LOG_START_1(SIM_ENV_TEST_START_APPLICATION)
  cout << "Add radio bearer on "  << GetDevice ()->GetIDNetworkNode ()  << endl;
DEBUG_LOG_END

  m_bearers->push_back (bearer);
}

void
RrcEntity::DelRadioBearer (RadioBearer* bearer)
{
    //TODO: CHECK GD I TRIED to rewrite this method - check if it works! I guess it can be further improved (the if could be moved inside for)
    RadioBearersContainer* newContainer = new RadioBearersContainer ();
    UserEquipment* ue;
    if(bearer->GetSource()->GetNodeType()==NetworkNode::TYPE_GNODEB || bearer->GetSource()->GetNodeType()==NetworkNode::TYPE_GW)
        ue = (UserEquipment*) bearer->GetDestination();
    else if (bearer->GetSource()->GetNodeType()==NetworkNode::TYPE_UE)
        ue = (UserEquipment*) bearer->GetSource();
    else
        cout<< "ERROR" ;
        exit(1);
    
    if (ue->IsTwin()) {
        for (auto b : *GetRadioBearerContainer ())
            if (b->GetDestination()->GetIDNetworkNode() != bearer->GetDestination()->GetIDNetworkNode())
                newContainer->push_back (b);
    }
    else {
        for (auto b : *GetRadioBearerContainer ())
            if (b->GetRlcEntity ()->GetRlcEntityIndex () != bearer->GetRlcEntity ()->GetRlcEntityIndex ())
                newContainer->push_back (b);
    }

m_bearers->clear ();
delete m_bearers;
m_bearers = newContainer;
}

void
RrcEntity::AddRadioBearerSink (RadioBearerSink* bearer)
{
DEBUG_LOG_START_1(SIM_ENV_TEST_START_APPLICATION)
  cout << "Add sink radio bearer on "  << GetDevice ()->GetIDNetworkNode () << endl;
DEBUG_LOG_END

  m_sink->push_back (bearer);
}

void
RrcEntity::DelRadioBearerSink (RadioBearerSink* bearer)
{
  RadioBearersSinkContainer* newContainer = new RadioBearersSinkContainer ();
  for (auto b : *GetRadioBearerSinkContainer ())
    {
      if (b->GetRlcEntity ()->GetRlcEntityIndex () != bearer->GetRlcEntity ()->GetRlcEntityIndex ())
        {
          newContainer->push_back (b);
        }
    }
  m_sink->clear ();
  delete m_sink;
  m_sink = newContainer;
}

RadioBearer*
RrcEntity::GetRadioBearer (ClassifierParameters* ipc)
{
  for (auto bearer : *GetRadioBearerContainer ())
    {
      if (bearer->GetClassifierParameters ()->GetSourcePort () == ipc->GetSourcePort ()
          &&
          bearer->GetClassifierParameters ()->GetDestinationPort () == ipc->GetDestinationPort ())
        {
          return bearer;
        }
    }
  return 0;
}

RadioBearerSink*
RrcEntity::GetRadioBearerSink (ClassifierParameters* ipc)
{
  for (auto bearer : *GetRadioBearerSinkContainer ())
    {
      if (bearer->GetClassifierParameters ()->GetSourcePort () == ipc->GetSourcePort ()
          &&
          bearer->GetClassifierParameters ()->GetDestinationPort () == ipc->GetDestinationPort ())
        {
          return bearer;
        }
    }
  return 0;
}


RadioBearerInstance*
RrcEntity::GetRadioBearer (int rlcIndex)
{
  for (auto bearer : *GetRadioBearerSinkContainer ())
    {
      if (bearer->GetRlcEntity ()->GetRlcEntityIndex () == rlcIndex)
        {
          return bearer;
        }
    }

  for (auto bearer : *GetRadioBearerContainer ())
    {
      if (bearer->GetRlcEntity ()->GetRlcEntityIndex () == rlcIndex)
        {
          return bearer;
        }
    }

  return 0;
}


void
RrcEntity::SetHandoverEntity (HandoverEntity* h)
{
  m_handover = h;
}

HandoverEntity*
RrcEntity::GetHandoverEntity (void)
{
  return m_handover;
}

void
RrcEntity::CopyRadioBearer (RadioBearer* bearer) {
    RadioBearersContainer* newContainer = new RadioBearersContainer();
    
    for(auto b : *GetRadioBearerContainer())
        if(b->GetDestination()==bearer->GetDestination())
            newContainer->push_back(b);
    
    newContainer->push_back(bearer);
    m_bearers->clear();
    delete m_bearers;
    m_bearers= newContainer;
}
