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

#include "TwinCBR.h"
#include <cstdlib>
#include "../../componentManagers/NetworkManager.h"
#include "../radio-bearer.h"
#include "../QoS/QoSParameters.h"
#include"../../device/NetworkNode.h"
#include"../../device/UserEquipment.h"
#include <cstdlib>
#include "../../componentManagers/FlowsManager.h"
#include "../radio-bearer-sink.h"


TwinCBR::TwinCBR()
{
    m_interval = 0.040;
    m_stopFlow = false;
    m_size = 1000;
    SetApplicationType (Application::APPLICATION_TYPE_TWIN_CBR);
    
}

void
TwinCBR::DoStart (void)
{
    Simulator::Init()->Schedule(0.0, &TwinCBR::Send, this);
}

void
TwinCBR::DoStop (void)
{
}

void
TwinCBR::ScheduleTransmit (double time)
{
    
    Simulator::Init()->Schedule(time, &TwinCBR::Send, this);
    
}


void
TwinCBR::Send (void)
{
    //CREATE A NEW PACKET (ADDING UDP, IP and PDCP HEADERS)
    if(!Application::GetStopFlow())
    {
        Packet *packet = new Packet ();
        int uid = Simulator::Init()->GetUID ();
        
        packet->SetID(uid);
        packet->SetTimeStamp (Simulator::Init()->Now ());
        packet->SetSize (GetSize ());
        Trace (packet);
        
        PacketTAGs *tags = new PacketTAGs ();
        tags->SetApplicationType(PacketTAGs::APPLICATION_TYPE_TWIN_CBR);
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
        
        ScheduleTransmit (GetInterval ());
        Application::SetInterval(m_interval);
        
    }
    else
    {
    } //Do Nothing
    
}


int
TwinCBR::GetSize (void) const
{
    return m_size;
}

void
TwinCBR::SetSize(int size)
{
    m_size = size;
}

void
TwinCBR::SetInterval(double interval)
{
    m_interval = interval;
}

double
TwinCBR::GetInterval (void) const
{
    return m_interval;
}
