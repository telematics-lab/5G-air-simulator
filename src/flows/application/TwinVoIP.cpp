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
 * Author: Dinesh Tamang <dinesh.tamang@student.unisi.it>
 */

#include "TwinVoIP.h"
#include <cstdlib>
#include "../../componentManagers/NetworkManager.h"
#include "../../componentManagers/FlowsManager.h"
#include "../QoS/QoSParameters.h"
#include "../radio-bearer.h"
#include "../radio-bearer-sink.h"
#include <math.h>
#include"../../device/NetworkNode.h"
#include"../../device/UserEquipment.h"

TwinVoIP::TwinVoIP()
{
    /*
     * G729 codec generates during the ON period a packet with
     * a fixed size (20 bytes). We must add the RTP header (12 bytes)
     */
    m_size = 32; //application + RTP header
    //m_firstTimeFlag = true;
    
    
    //m_lastPacketCreationTime;
    //m_firstTimeFlag;
    m_stopFlow = false;
    m_interval = 0.010;
    SetApplicationType (Application::APPLICATION_TYPE_TWIN_VOIP);
}

void
TwinVoIP::DoStart (void)
{
    Simulator::Init()->Schedule(0.0, &TwinVoIP::Send, this);
}

void
TwinVoIP::DoStop (void)
{}

void
TwinVoIP::ScheduleTransmit (double time)
{
    
    if (!(Application::GetStopFlow()))
    {
        Simulator::Init()->Schedule(time, &TwinVoIP::Send, this);
    }
    else
    {}
    
    /*
     int nbDlTwinBearer=0;
     int nbUlTwinBearer=0;
     bool stop = false;
     
     if(GetDestination()->GetNodeType()==NetworkNode::TYPE_UE && GetSource()->GetNodeType()==NetworkNode::TYPE_ENODEB)
     {
     // if downlink
     NetworkNode* dst = GetDestination();
     NetworkNode* src = GetSource();
     UserEquipment* twin;
     //ENodeB* enb;
     twin = (UserEquipment*) dst ;
     //enb = (ENodeB*) src;
     RrcEntity* enbRrc = twin->GetTargetNode()->GetProtocolStack()->GetRrcEntity();
     RrcEntity* twinRrc = twin->GetProtocolStack()->GetRrcEntity();
     
     
     RadioBearer* bearerX = GetRadioBearer();
     if(bearerX->GetDestination()->GetNodeType()==NetworkNode::TYPE_UE) // downlink
     {
     if(!bearerX->HasPackets())
     {
     int dstPort = bearerX->GetClassifierParameters()->GetDestinationPort();
     enbRrc->DelRadioBearer(bearerX);
     
     vector<RadioBearerSink* > *dlSinkToDelete = new vector<RadioBearerSink* > ();
     
     for(auto sink: *twinRrc->GetRadioBearerSinkContainer())
     {
     if(sink->GetClassifierParameters()->GetDestinationPort()==dstPort)
     {
     dlSinkToDelete->push_back(sink);
     }
     }
     for (auto sink : *dlSinkToDelete)
     {
     twinRrc->DelRadioBearerSink (sink);
     }
     dlSinkToDelete->clear ();
     delete dlSinkToDelete;
     }
     
     if(enbRrc->GetRadioBearerContainer()->size()>0)
     {
     for(auto bearer: *enbRrc->GetRadioBearerContainer())
     {
     if(bearer->GetDestination()->GetIDNetworkNode()==twin->GetIDNetworkNode())
     {
     nbDlTwinBearer++;
     }
     }
     
     }
     
     
     
     }
     else if (GetDestination()->GetNodeType()==NetworkNode::TYPE_ENODEB && GetSource()->GetNodeType()==NetworkNode::TYPE_UE)
     {
     // if uplink
     NetworkNode* dst = GetDestination();
     NetworkNode* src = GetSource();
     UserEquipment* twin;
     //ENodeB* enb;
     twin = (UserEquipment*) src ;
     //enb = (ENodeB*) dst;
     
     RadioBearer* bearerX = GetRadioBearer();
     
     if(!bearerX->HasPackets())
     {
     int dstPort = bearerX->GetClassifierParameters()->GetDestinationPort();
     twinRrc->DelRadioBearer (bearerX);
     vector<RadioBearerSink* > *ulSinkToDelete = new vector<RadioBearerSink* > ();
     
     
     for (auto sink : *enbRrc->GetRadioBearerSinkContainer ())
     {
     if (sink->GetClassifierParameters()->GetDestinationPort()== dstPort)
     {
     ulSinkToDelete->push_back (sink);
     }
     }
     for (auto sink : *ulSinkToDelete)
     {
     enbRrc->DelRadioBearerSink (sink);
     }
     
     ulSinkToDelete->clear ();
     delete ulSinkToDelete;
     }
     
     if(enbRrc->GetRadioBearerSinkContainer()->size()>0)
     {
     for(auto bearer: *enbRrc->GetRadioBearerSinkContainer())
     {
     if(bearer->GetSource()->GetIDNetworkNode()==twin->GetIDNetworkNode())
     {
     nbUlTwinBearer++;
     }
     }
     
     }
     }
     if(nbDlTwinBearer==0&&nbUlTwinBearer==0)
     {
     stop =true;
     
     UserEquipment* ue = NetworkManager::Init()->GetTwinUserEquipmentByID(twin->GetIDNetworkNode());
     
     
     NetworkManager::Init()->HandoverProcedure(Simulator::Init()->Now(),ue, ue->GetTargetNode(), twin->GetTargetNode());
     //    NetworkManager::Init()->TransferBearerInfo(ue,twin->GetTargetNode());
     //NetworkManager::Init()->TransferBearerInfo(twin,twin->GetTargetNode());
     }
     if(stop==false)
     {
     Simulator::Init()->Schedule(0.001, &TwinVoIP::ScheduleTransmit, this,0.001);
     }
     }
     
     
     }*/
    
}


void
TwinVoIP::Send (void)
{
    
    m_stateON = GetState();
    m_stateDuration = GetStateDuration();
    m_endState = GetEndState();
    
    if (!m_stateON)
    {
        m_stateON = true;
        //start state ON
        double random = rand() %10000;
        m_stateDuration = -3*log(1-((double)random/10000));
        m_endState = Simulator::Init()->Now () + m_stateDuration;
        
        DEBUG_LOG_START_1(SIM_ENV_APPLICATION_DEBUG)
        cout << " VoIP_DEBUG - Start ON Period, "
        "\n\t Time = " << Simulator::Init()->Now ()
        << "\n\t state ON Duration = " << m_stateDuration
        << "\n\t end ON state = " <<  m_endState << endl;
        DEBUG_LOG_END
        
        SetState(m_stateON);
        SetStateDuration(m_stateDuration);
        SetEndState(m_endState);
    }
    
    //CREATE A NEW PACKET (ADDING UDP, IP and PDCP HEADERS)
    Packet *packet = new Packet ();
    int uid = Simulator::Init()->GetUID ();
    
    packet->SetID(uid);
    
    packet->SetTimeStamp (Simulator::Init()->Now ());
    packet->SetSize (GetSize ());
    Trace (packet);
    Application::SetLastPacketCreationTime(Simulator::Init()->Now());
    
    PacketTAGs *tags = new PacketTAGs ();
    tags->SetApplicationType(PacketTAGs::APPLICATION_TYPE_TWIN_VOIP);
    tags->SetApplicationSize (packet->GetSize ());
    packet->SetPacketTags(tags);
    
    double maxDelay = GetQoSParameters()->GetMaxDelay(); //for priority scheduling
    packet->SetRtts(maxDelay); //for priority scheduling
    
    UDPHeader *udp = new UDPHeader (GetClassifierParameters ()->GetSourcePort (),
                                    GetClassifierParameters ()->GetDestinationPort ());
    packet->AddUDPHeader (udp);
    
    IPHeader *ip = new IPHeader (GetClassifierParameters ()->GetSourceID (),
                                 GetClassifierParameters ()->GetDestinationID ());
    packet->AddIPHeader (ip);
    
    PDCPHeader *pdcp = new PDCPHeader ();
    packet->AddPDCPHeader (pdcp);
    
    GetRadioBearer()->Enqueue (packet);
    
    
    if (Simulator::Init()->Now () <= m_endState)
    {
        ScheduleTransmit (m_interval);
    }
    else
    {
        //schedule OFF Period
        m_stateON = false;
        double random = rand() %10000;
        m_stateDuration = -2.23*log(1-((double)random/10000));
        if (m_stateDuration > 6.9)
        {
            m_stateDuration = 6.9;
        }
        
        DEBUG_LOG_START_1(SIM_ENV_APPLICATION_DEBUG)
        cout << " VoIP_DEBUG - Start OFF Period, "
        "\n\t Time = " << Simulator::Init()->Now ()
        << "\n\t state OFF Duration = " << m_stateDuration << endl;
        DEBUG_LOG_END
        
        ScheduleTransmit (m_stateDuration);
        SetState(m_stateON);
        SetStateDuration(m_stateDuration);
        SetEndState(m_endState);
    }
}


int
TwinVoIP::GetSize (void) const
{
    return m_size;
}









