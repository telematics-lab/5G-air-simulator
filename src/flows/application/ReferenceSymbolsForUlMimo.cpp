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
 * Author: Gabriele Roncella <gabriele.roncella@student.unisi.it>
 * Author: Dinesh Tamang <dinesh.tamang@student.unisi.it>
 */

#include "ReferenceSymbolsForUlMimo.h"
#include <cstdlib>
#include "../../componentManagers/NetworkManager.h"
#include "../radio-bearer.h"

ReferenceSymbolsForUlMimo::ReferenceSymbolsForUlMimo()
{
    SetApplicationType (Application::APPLICATION_TYPE_REFERENCESYMBOLSFORULMIMO);
}

void
ReferenceSymbolsForUlMimo::DoStart (void)
{
    Simulator::Init()->Schedule(0.0, &ReferenceSymbolsForUlMimo::Send, this);
}

void
ReferenceSymbolsForUlMimo::DoStop (void)
{
}

void
ReferenceSymbolsForUlMimo::ScheduleTransmit (double time)
{
    if ( (Simulator::Init()->Now () + time) < GetStopTime () )
    {
        Simulator::Init()->Schedule(time, &ReferenceSymbolsForUlMimo::Send, this);
    }
}

void
ReferenceSymbolsForUlMimo::Send (void)
{
    //CREATE A NEW PACKET (ADDING UDP, IP and PDCP HEADERS)
    Packet *packet = new Packet ();
    int uid = Simulator::Init()->GetUID ();
    
    packet->SetID(uid);
    packet->SetTimeStamp (Simulator::Init()->Now ());
    packet->SetSize (GetSize ());
    
    PacketTAGs *tags = new PacketTAGs ();
    tags->SetApplicationType(PacketTAGs::APPLICATION_TYPE_CBR);
    tags->SetApplicationSize (packet->GetSize ());
    packet->SetPacketTags(tags);
    
    
    UDPHeader *udp = new UDPHeader (GetClassifierParameters ()->GetSourcePort (),
                                    GetClassifierParameters ()->GetDestinationPort ());
    packet->AddUDPHeader (udp);
    
    IPHeader *ip = new IPHeader (GetClassifierParameters ()->GetSourceID (),
                                 GetClassifierParameters ()->GetDestinationID ());
    packet->AddIPHeader (ip);
    
    PDCPHeader *pdcp = new PDCPHeader ();
    packet->AddPDCPHeader (pdcp);
    
    Trace (packet);
    
    GetRadioBearer()->Enqueue (packet);
    
    ScheduleTransmit (GetInterval ());
    
}


int
ReferenceSymbolsForUlMimo::GetSize (void) const
{
    return m_size;
}

void
ReferenceSymbolsForUlMimo::SetSize(int size)
{
    m_size = size;
}
void
ReferenceSymbolsForUlMimo::SetInterval(double interval)
{
    m_interval = interval;
}

double
ReferenceSymbolsForUlMimo::GetInterval (void) const
{
    return m_interval;
}
