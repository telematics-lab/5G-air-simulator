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


#include "packet-scheduler.h"
#include "../gnb-mac-entity.h"
#include "../../packet/Packet.h"
#include "../../packet/packet-burst.h"
#include "../../../device/NetworkNode.h"
#include "../../../device/GNodeB.h"
#include "../../../device/UserEquipment.h"
#include "../../../flows/radio-bearer.h"
#include "../../../protocolStack/rrc/rrc-entity.h"
#include "../../../flows/application/Application.h"
#include "../../../flows/MacQueue.h"
#include "../../../flows/QoS/QoSParameters.h"
#include "../../rlc/am-rlc-entity.h"

PacketScheduler::PacketScheduler()
{
  m_mac = nullptr;
  m_flowsToSchedule = nullptr;
}

PacketScheduler::~PacketScheduler()
{
  ClearFlowsToSchedule ();
  delete m_flowsToSchedule;
  m_mac = nullptr;
}

void
PacketScheduler::Destroy (void)
{
  ClearFlowsToSchedule ();
  delete m_flowsToSchedule;
}

void
PacketScheduler::SetMacEntity (GnbMacEntity* mac)
{
  m_mac = mac;
}

GnbMacEntity*
PacketScheduler::GetMacEntity (void)
{
  return m_mac;
}

void
PacketScheduler::Schedule (void)
{
  DoSchedule ();
}

void
PacketScheduler::DoSchedule (void)
{}

void
PacketScheduler::StopSchedule ()
{
  DoStopSchedule ();
}

void
PacketScheduler::DoStopSchedule ()
{}


PacketScheduler::FlowToSchedule::FlowToSchedule(RadioBearer* bearer, int dataToTransmit)
{
  m_bearer = bearer;
  m_allocatedBits = 0;
  m_transmittedData = 0;
  m_dataToTransmit = dataToTransmit;
  m_harqManager = nullptr;
  m_isHarqRetransmission = false;
  if (bearer->GetApplication ()->GetSource ()->GetNodeType () == NetworkNode::TYPE_UE)
    {
      m_ue = (UserEquipment*)bearer->GetApplication ()->GetSource ();
    }
  else if (bearer->GetApplication ()->GetDestination ()->GetNodeType () == NetworkNode::TYPE_UE ||
           bearer->GetApplication ()->GetDestination ()->GetNodeType () == NetworkNode::TYPE_MULTICAST_DESTINATION)
    {
      m_ue = (UserEquipment*)bearer->GetApplication ()->GetDestination ();
    }
  else
    {
      cout << "No UE found as source or destination for application ID " << bearer->GetApplication () << endl;
      exit(1);
    }
  m_ueRecord = m_ue->GetTargetNodeRecord();
}

PacketScheduler::FlowToSchedule::~FlowToSchedule()
{}

void
PacketScheduler::CreateFlowsToSchedule (void)
{
  m_flowsToSchedule = new FlowsToSchedule ();
}

void
PacketScheduler::DeleteFlowsToSchedule (void)
{
  ClearFlowsToSchedule ();
  delete  m_flowsToSchedule;
}

PacketScheduler::FlowsToSchedule*
PacketScheduler::GetFlowsToSchedule (void) const
{
  return m_flowsToSchedule;
}

void
PacketScheduler::ClearFlowsToSchedule ()
{
  GetFlowsToSchedule ()->clear ();
}

RadioBearer*
PacketScheduler::FlowToSchedule::GetBearer (void)
{
  return m_bearer;
}

void
PacketScheduler::FlowToSchedule::SetUe(UserEquipment* ue)
{
  m_ue = ue;
}

UserEquipment*
PacketScheduler::FlowToSchedule::GetUe(void)
{
  return m_ue;
}

void
PacketScheduler::FlowToSchedule::SetUeRecord(GNodeB::UserEquipmentRecord* record)
{
  m_ueRecord = record;
}

GNodeB::UserEquipmentRecord*
PacketScheduler::FlowToSchedule::GetUeRecord()
{
  return m_ueRecord;
}

void
PacketScheduler::FlowToSchedule::SetSpectralEfficiency (vector<double> s)
{
  m_spectralEfficiency = s;
}

vector<double>
PacketScheduler::FlowToSchedule::GetSpectralEfficiency (void)
{
  return m_spectralEfficiency;
}

void
PacketScheduler::FlowToSchedule::UpdateAllocatedBits (int allocatedBits)
{
  m_allocatedBits += allocatedBits;
  int availableBytes = m_allocatedBits/8;

  int transmittedPackets = ceil
                           (availableBytes/1513.0);

  m_transmittedData = availableBytes - (transmittedPackets * 8);
  if (m_transmittedData < 0)
    {
      m_transmittedData = 0;
    }
}

int
PacketScheduler::FlowToSchedule::GetAllocatedBits (void) const
{
  return m_allocatedBits;
}

int
PacketScheduler::FlowToSchedule::GetTransmittedData (void) const
{
  return m_transmittedData;
}

void
PacketScheduler::FlowToSchedule::SetDataToTransmit (int dataToTransmit)
{
  m_dataToTransmit = dataToTransmit;
}

int
PacketScheduler::FlowToSchedule::GetDataToTransmit (void) const
{
  return m_dataToTransmit;
}

vector<int>*
PacketScheduler::FlowToSchedule::GetListOfAllocatedRBs ()
{
  return &m_listOfAllocatedRBs;
}

vector<int>*
PacketScheduler::FlowToSchedule::GetListOfSelectedMCS ()
{
  return &m_listOfSelectedMCS;
}

void
PacketScheduler::FlowToSchedule::SetAssignedLayers (vector<int> layers)
{
  m_assignedLayers = layers;
}

vector<int>
PacketScheduler::FlowToSchedule::GetAssignedLayers (void)
{
  return m_assignedLayers;
}

void
PacketScheduler::FlowToSchedule::SetCqiFeedbacks (vector<int> cqiFeedbacks)
{
  m_cqiFeedbacks = cqiFeedbacks;
}

vector<int>
PacketScheduler::FlowToSchedule::GetCqiFeedbacks (void)
{
  return m_cqiFeedbacks;
}

void
PacketScheduler::FlowToSchedule::SetRiFeedback (int riFeedback)
{
  m_riFeedback = riFeedback;
}

int
PacketScheduler::FlowToSchedule::GetRiFeedback (void)
{
  return m_riFeedback;
}

void
PacketScheduler::FlowToSchedule::SetPmiFeedbacks (vector< vector<int> > pmiFeedbacks)
{
  m_pmiFeedbacks = pmiFeedbacks;
}

vector< vector<int> >
PacketScheduler::FlowToSchedule::GetPmiFeedbacks (void)
{
  return m_pmiFeedbacks;
}

void
PacketScheduler::FlowToSchedule::SetHarqManager(HarqManager* harq)
{
  m_harqManager = harq;
}

HarqManager*
PacketScheduler::FlowToSchedule::GetHarqManager(void)
{
  return m_harqManager;
}

void
PacketScheduler::FlowToSchedule::SetFullCsiFeedbacks (vector< shared_ptr<arma::cx_fmat> > fullCsiFeedbacks)
{
  m_fullCsiFeedbacks = fullCsiFeedbacks;
}

vector< shared_ptr<arma::cx_fmat> >
PacketScheduler::FlowToSchedule::GetFullCsiFeedbacks (void)
{
  return m_fullCsiFeedbacks;
}

void
PacketScheduler::FlowToSchedule::SetHarqRetransmission(bool value)
{
  m_isHarqRetransmission = value;
}

bool
PacketScheduler::FlowToSchedule::IsHarqRetransmission(void)
{
  return m_isHarqRetransmission;
}

void
PacketScheduler::FlowToSchedule::SetHarqPid(int pid)
{
  m_harqPid = pid;
}

int
PacketScheduler::FlowToSchedule::GetHarqPid(void)
{
  return m_harqPid;
}

void
PacketScheduler::FlowToSchedule::SetPacketBurst(shared_ptr<PacketBurst> pb)
{
  m_packets = pb;
}

shared_ptr<PacketBurst>
PacketScheduler::FlowToSchedule::GetPacketBurst(void)
{
  return m_packets;
}

vector< shared_ptr<PacketScheduler::FlowToSchedule> >* PacketScheduler::FlowToSchedule::GetMulticastClonedFlows()
{
  return &m_multicastClonedFlows;
}


void
PacketScheduler::InsertFlowToSchedule (RadioBearer* bearer, int dataToTransmit, vector<double> specEff, vector<int> cqiFeedbacks,
                                       int riFeedback, vector< vector<int> >pmiFeedbacks)
{
DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
  cout << "\t  --> selected flow: "
            << bearer->GetApplication ()->GetApplicationID ()
            << " " << dataToTransmit << endl;
DEBUG_LOG_END

  shared_ptr<FlowToSchedule> flowToSchedule = make_shared<FlowToSchedule>(bearer, dataToTransmit);
  flowToSchedule->SetSpectralEfficiency (specEff);
  flowToSchedule->SetCqiFeedbacks (cqiFeedbacks);
  flowToSchedule->SetRiFeedback (riFeedback);
  flowToSchedule->SetPmiFeedbacks (pmiFeedbacks);
  flowToSchedule->SetHarqRetransmission(false);

  GetFlowsToSchedule ()->push_back(flowToSchedule);
}

void
PacketScheduler::InsertFlowToSchedule (
  RadioBearer* bearer,
  int dataToTransmit,
  vector<double> specEff,
  vector<int> cqiFeedbacks,
  int riFeedback,
  vector< vector<int> >pmiFeedbacks,
  vector< shared_ptr<arma::cx_fmat> > fullCsiFeedbacks)
{
DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
  cout << "\t  --> selected flow: "
            << bearer->GetApplication ()->GetApplicationID ()
            << " " << dataToTransmit << endl;
DEBUG_LOG_END

  shared_ptr<FlowToSchedule> flowToSchedule = make_shared<FlowToSchedule>(bearer, dataToTransmit);
  flowToSchedule->SetSpectralEfficiency (specEff);
  flowToSchedule->SetCqiFeedbacks (cqiFeedbacks);
  flowToSchedule->SetRiFeedback (riFeedback);
  flowToSchedule->SetPmiFeedbacks (pmiFeedbacks);
  flowToSchedule->SetHarqRetransmission(false);
  flowToSchedule->SetFullCsiFeedbacks (fullCsiFeedbacks);

  GetFlowsToSchedule ()->push_back(flowToSchedule);
}

void
PacketScheduler::InsertFlowForRetransmission (shared_ptr<FlowToSchedule> flow)
{
  flow->SetHarqRetransmission(true);
  //get updated CSI information
  UserEquipment* ue = (UserEquipment*)flow->GetBearer()->GetApplication()->GetDestination();
  GNodeB::UserEquipmentRecord* ueRecord = ue->GetTargetNodeRecord();
  flow->SetFullCsiFeedbacks( ueRecord->GetChannelMatrix() );

}

void
PacketScheduler::UpdateAllocatedBits (shared_ptr<FlowToSchedule> scheduledFlow, int allocatedBits,  int allocatedRB, int selectedMCS)
{
  scheduledFlow->UpdateAllocatedBits (allocatedBits);
  scheduledFlow->GetListOfAllocatedRBs()->push_back(allocatedRB);
  scheduledFlow->GetListOfSelectedMCS()->push_back(selectedMCS);
}


void
PacketScheduler::CheckForDLDropPackets ()
{
  RrcEntity *rrc = GetMacEntity ()->GetDevice ()->GetProtocolStack ()->GetRrcEntity ();
  for (auto bearer : *rrc->GetRadioBearerContainer ())
    {
      //delete packets from queue
      bearer->GetMacQueue ()->CheckForDropPackets (
        bearer->GetQoSParameters ()->GetMaxDelay (), bearer->GetApplication ()->GetApplicationID ());

      //delete fragment waiting in AM RLC entity
      if (bearer->GetRlcEntity()->GetRlcModel() == RlcEntity::AM_RLC_MODE)
        {
          AmRlcEntity* amRlc = (AmRlcEntity*) bearer->GetRlcEntity();
          amRlc->CheckForDropPackets (
            bearer->GetQoSParameters ()->GetMaxDelay (), bearer->GetApplication ()->GetApplicationID ());
        }
    }
}
