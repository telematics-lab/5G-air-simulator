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


#include "NetworkNode.h"
#include "UserEquipment.h"
#include "GNodeB.h"
#include "Gateway.h"
#include "../protocolStack/mac/packet-scheduler/packet-scheduler.h"
#include "../protocolStack/mac/packet-scheduler/mt-uplink-packet-scheduler.h"
#include "../protocolStack/mac/packet-scheduler/dl-pf-packet-scheduler.h"
#include "../protocolStack/mac/packet-scheduler/dl-mt-packet-scheduler.h"
#include "../protocolStack/mac/packet-scheduler/dl-mlwdf-packet-scheduler.h"
#include "../protocolStack/mac/packet-scheduler/dl-exp-packet-scheduler.h"
#include "../protocolStack/mac/packet-scheduler/dl-fls-packet-scheduler.h"
#include "../protocolStack/mac/packet-scheduler/exp-rule-downlink-packet-scheduler.h"
#include "../protocolStack/mac/packet-scheduler/log-rule-downlink-packet-scheduler.h"
#include "../protocolStack/mac/packet-scheduler/dl-rr-packet-scheduler.h"
#include "../protocolStack/mac/packet-scheduler/enhanced-uplink-packet-scheduler.h"
#include "../protocolStack/mac/packet-scheduler/roundrobin-uplink-packet-scheduler.h"
#include "../protocolStack/mac/packet-scheduler/nb-roundrobin-uplink-packet-scheduler.h"
#include "../protocolStack/mac/packet-scheduler/nb-fifo-uplink-packet-scheduler.h"
#include "../protocolStack/mac/packet-scheduler/nb-uplink-packet-scheduler.h"
#include "../phy/gnb-phy.h"
#include "../core/spectrum/bandwidth-manager.h"
#include "../protocolStack/packet/packet-burst.h"
#include "../protocolStack/mac/harq-manager.h"
#include "../componentManagers/FrameManager.h"
#include "../protocolStack/mac/random-access/gnb-random-access.h"

GNodeB::GNodeB (int idElement,
                Cell *cell)
    : GNodeB::GNodeB (idElement,
                      cell,
                      cell->GetCellCenterPosition ()->GetCoordinateX (),
                      cell->GetCellCenterPosition ()->GetCoordinateY (),
                      25) // default value for urban macro-cell scenario
{}

GNodeB::GNodeB (int idElement,
                Cell *cell,
                double posx,
                double posy)
    : GNodeB::GNodeB (idElement,
                      cell,
                      posx,
                      posy,
                      25) // default value for urban macro-cell scenario
{}

GNodeB::GNodeB (int idElement,
                Cell *cell,
                double posx,
                double posy,
                double posz)
{
  SetIDNetworkNode (idElement);
  SetNodeType(NetworkNode::TYPE_GNODEB);
  SetCell (cell);

  CartesianCoordinates *position = new CartesianCoordinates(posx, posy, posz);
  Mobility* m = new ConstantPosition ();
  m->SetAbsolutePosition (position);
  SetMobilityModel (m);
  delete position;

  m_userEquipmentRecords = new UserEquipmentRecords;

  GnbPhy *phy = new GnbPhy ();
  phy->SetDevice(this);
  SetPhy (phy);

  ProtocolStack *stack = new ProtocolStack (this);
  SetProtocolStack (stack);

  Classifier *classifier = new Classifier ();
  classifier->SetDevice (this);
  SetClassifier (classifier);
}

GNodeB::~GNodeB()
{
  Destroy ();
  m_userEquipmentRecords->clear();
  delete m_userEquipmentRecords;
}

void
GNodeB::RegisterUserEquipment (UserEquipment *UE)
{
  UserEquipmentRecord *record = new UserEquipmentRecord (UE);
  UserEquipmentRecords *UERs = GetUserEquipmentRecords ();
  UserEquipmentRecords::iterator it, ins_pos;
  ins_pos = UERs->end();
  if (UERs->size()>0)
    {
      for (it=UERs->end()-1; it>=UERs->begin(); it--)
        {
          // UEs should be inserted before multicast destinations
          if ( it == UERs->begin() )
            {
              ins_pos = it;
              break;
            }
          if ( (*it)->GetUE()->GetNodeType()==NetworkNode::TYPE_MULTICAST_DESTINATION )
            {
              ins_pos = it;
            }
          else
            {
              break;
            }
        }
    }

  UERs->insert(ins_pos,record);
  UE->SetTargetNodeRecord (record);
}

void
GNodeB::DeleteUserEquipment (UserEquipment *UE)
{
  UserEquipmentRecords *new_records = new UserEquipmentRecords ();

  for (auto record : *GetUserEquipmentRecords())
    {
      if (record->GetUE ()->GetIDNetworkNode () != UE->GetIDNetworkNode ())
        {
          //records->erase(iter);
          //break;
          new_records->push_back (record);
        }
      else
        {
          if (UE->GetTargetNodeRecord() == record)
            {
              UE->SetTargetNodeRecord (NULL);
            }
          delete record;
        }
    }

  m_userEquipmentRecords->clear ();
  delete m_userEquipmentRecords;
  m_userEquipmentRecords = new_records;
}

int
GNodeB::GetNbOfUserEquipmentRecords (void)
{
  return GetUserEquipmentRecords ()->size();
}

void
GNodeB::CreateUserEquipmentRecords (void)
{
  m_userEquipmentRecords = new UserEquipmentRecords ();
}

void
GNodeB::DeleteUserEquipmentRecords (void)
{
  m_userEquipmentRecords->clear ();
  delete m_userEquipmentRecords;
}

GNodeB::UserEquipmentRecords*
GNodeB::GetUserEquipmentRecords (void)
{
  return m_userEquipmentRecords;
}

GNodeB::UserEquipmentRecord*
GNodeB::GetUserEquipmentRecord (int idUE)
{
  for (auto record : *GetUserEquipmentRecords())
    {
      if (record->GetUE ()->
          GetIDNetworkNode () == idUE)
        {
          return record;
        }
    }
  return nullptr;
}

GnbMacEntity*
GNodeB::GetMacEntity(void) const
{
  return (GnbMacEntity*)GetProtocolStack()->GetMacEntity();
}

GNodeB::UserEquipmentRecord::UserEquipmentRecord ()
{
  m_UE = nullptr;
  //Create initial CQI values:
  m_cqiAvailable = false;
  m_cqiFeedback.clear ();
  m_uplinkChannelStatusIndicator.clear ();
  m_schedulingRequest = 0;
  m_averageSchedulingGrants = 1;
  if (_harq_active_)
    SetHarqManager (new HarqManager ());
  else
    SetHarqManager (nullptr);
}

GNodeB::UserEquipmentRecord::~UserEquipmentRecord ()
{
  m_cqiFeedback.clear ();
  m_uplinkChannelStatusIndicator.clear();
  if (m_harqManager != nullptr)
    {
      delete m_harqManager;
    }
}

GNodeB::UserEquipmentRecord::UserEquipmentRecord (UserEquipment *UE)
{
  m_UE = UE;
  BandwidthManager *s = m_UE->GetPhy ()->GetBandwidthManager ();

  int nbRbs = (int) s->GetDlSubChannels ().size ();
  m_cqiFeedback.clear ();
  for (int i = 0; i < nbRbs; i++ )
    {
      m_cqiFeedback.push_back (10);
    }

  nbRbs = (int) s->GetUlSubChannels ().size ();
  m_uplinkChannelStatusIndicator.clear ();
  for (int i = 0; i < nbRbs; i++ )
    {
      m_uplinkChannelStatusIndicator.push_back (10.);
    }

  m_schedulingRequest = 0;
  m_averageSchedulingGrants = 1;
  m_DlTxMode = UE->GetTargetNode()->GetMacEntity ()->GetDefaultDlTxMode ();
  if (UE->GetNodeType() == NetworkNode::TYPE_MULTICAST_DESTINATION
      || !_harq_active_)
    {
      SetHarqManager (nullptr);
    }
  else
    {
      SetHarqManager (new HarqManager (UE));
    }
  if (FrameManager::Init()->MbsfnEnabled()==true && UE->GetNodeType()==NetworkNode::TYPE_MULTICAST_DESTINATION)
    {
      m_DlTxMode = 1;
    }
}

void
GNodeB::UserEquipmentRecord::SetUE (UserEquipment *UE)
{
  m_UE = UE;
}

UserEquipment*
GNodeB::UserEquipmentRecord::GetUE (void) const
{
  return m_UE;
}

bool
GNodeB::UserEquipmentRecord::CqiAvailable()
{
  return m_cqiAvailable;
}

void
GNodeB::UserEquipmentRecord::SetCQI (vector<int> cqi)
{
  m_cqiAvailable = true;
  m_cqiFeedback = cqi;
}

vector<int>
GNodeB::UserEquipmentRecord::GetCQI (void) const
{
  return m_cqiFeedback;
}

void
GNodeB::UserEquipmentRecord::SetRI(int ri)
{
  m_riFeedback = ri;
}

int
GNodeB::UserEquipmentRecord::GetRI (void) const
{
  return m_riFeedback;
}

void
GNodeB::UserEquipmentRecord::SetPMI(vector< vector<int> > pmi)
{
  m_pmiFeedback = pmi;
}

vector< vector<int> >
GNodeB::UserEquipmentRecord::GetPMI (void) const
{
  return m_pmiFeedback;
}

void
GNodeB::UserEquipmentRecord::SetChannelMatrix(vector< shared_ptr<arma::cx_fmat> > channelMatrix)
{
  m_channelMatrix = channelMatrix;
}

vector< shared_ptr<arma::cx_fmat> >
GNodeB::UserEquipmentRecord::GetChannelMatrix(void)
{
  return m_channelMatrix;
}


void
GNodeB::UserEquipmentRecord::SetDlTxMode(int txMode)
{
  m_DlTxMode = txMode;
}

int
GNodeB::UserEquipmentRecord::GetDlTxMode()
{
  return m_DlTxMode;
}

void
GNodeB::UserEquipmentRecord::SetHarqManager (HarqManager* harqManager)
{
  m_harqManager = harqManager;
}

HarqManager*
GNodeB::UserEquipmentRecord::GetHarqManager (void)
{
  return m_harqManager;
}

int
GNodeB::UserEquipmentRecord::GetSchedulingRequest (void)
{
  return m_schedulingRequest;
}

void
GNodeB::UserEquipmentRecord::SetSchedulingRequest (int r)
{
  m_schedulingRequest = r;
}

void
GNodeB::UserEquipmentRecord::UpdateSchedulingGrants (int b)
{
  m_averageSchedulingGrants = (0.9 * m_averageSchedulingGrants) + (0.1 * b);
}

int
GNodeB::UserEquipmentRecord::GetSchedulingGrants (void)
{
  return m_averageSchedulingGrants;
}

void
GNodeB::UserEquipmentRecord::SetUlMcs (int mcs)
{
  m_ulMcs = mcs;
}

int
GNodeB::UserEquipmentRecord::GetUlMcs (void)
{
  return m_ulMcs;
}

void
GNodeB::UserEquipmentRecord::SetUplinkChannelStatusIndicator (vector<double> vet)
{
  m_uplinkChannelStatusIndicator = vet;
}

vector<double>
GNodeB::UserEquipmentRecord::GetUplinkChannelStatusIndicator (void) const
{
  return m_uplinkChannelStatusIndicator;
}

void
GNodeB::SetDLScheduler (GNodeB::DLSchedulerType type)
{
  GnbMacEntity *mac = GetMacEntity ();
  DownlinkPacketScheduler *scheduler;
  switch (type)
    {
    case GNodeB::DLScheduler_TYPE_PROPORTIONAL_FAIR:
      scheduler = new  DL_PF_PacketScheduler ();
      scheduler->SetMacEntity (mac);
      mac->SetDownlinkPacketScheduler (scheduler);
      break;

    case GNodeB::DLScheduler_TYPE_FLS:
      scheduler = new  DL_FLS_PacketScheduler ();
      scheduler->SetMacEntity (mac);
      mac->SetDownlinkPacketScheduler (scheduler);
      break;

    case GNodeB::DLScheduler_TYPE_EXP:
      scheduler = new  DL_EXP_PacketScheduler ();
      scheduler->SetMacEntity (mac);
      mac->SetDownlinkPacketScheduler (scheduler);
      break;

    case GNodeB::DLScheduler_TYPE_MLWDF:
      scheduler = new  DL_MLWDF_PacketScheduler ();
      scheduler->SetMacEntity (mac);
      mac->SetDownlinkPacketScheduler (scheduler);
      break;

    case GNodeB::DLScheduler_EXP_RULE:
      scheduler = new  ExpRuleDownlinkPacketScheduler ();
      scheduler->SetMacEntity (mac);
      mac->SetDownlinkPacketScheduler (scheduler);
      break;

    case GNodeB::DLScheduler_LOG_RULE:
      scheduler = new  LogRuleDownlinkPacketScheduler ();
      scheduler->SetMacEntity (mac);
      mac->SetDownlinkPacketScheduler (scheduler);
      break;

    case GNodeB::DLScheduler_TYPE_MAXIMUM_THROUGHPUT:
      scheduler = new  DL_MT_PacketScheduler ();
      scheduler->SetMacEntity (mac);
      mac->SetDownlinkPacketScheduler (scheduler);
      break;

    case GNodeB::DLScheduler_TYPE_ROUND_ROBIN:
      scheduler = new  DL_RR_PacketScheduler ();
      scheduler->SetMacEntity (mac);
      mac->SetDownlinkPacketScheduler (scheduler);
      break;

    default:
      cout << "ERROR: invalid scheduler type" << endl;
      exit(1);
      break;
    }
}

DownlinkPacketScheduler*
GNodeB::GetDLScheduler (void) const
{
  return GetMacEntity ()->GetDownlinkPacketScheduler ();
}

void
GNodeB::SetULScheduler (ULSchedulerType type)
{
  GnbMacEntity *mac = GetMacEntity ();
  UplinkPacketScheduler *scheduler;
  switch (type)
    {
    case GNodeB::ULScheduler_TYPE_MAXIMUM_THROUGHPUT:
      scheduler = new MaximumThroughputUplinkPacketScheduler ();
      scheduler->SetMacEntity (mac);
      mac->SetUplinkPacketScheduler (scheduler);
      break;
    case GNodeB::ULScheduler_TYPE_FME:
      scheduler = new EnhancedUplinkPacketScheduler();
      scheduler->SetMacEntity (mac);
      mac->SetUplinkPacketScheduler (scheduler);
      break;
    case GNodeB::ULScheduler_TYPE_ROUNDROBIN:
      scheduler = new RoundRobinUplinkPacketScheduler ();
      scheduler->SetMacEntity (mac);
      mac->SetUplinkPacketScheduler (scheduler);
      break;
    case GNodeB::ULScheduler_TYPE_NB_IOT_FIFO:
      scheduler = new nbFifoUplinkPacketScheduler (mac);
      mac->SetUplinkPacketScheduler (scheduler);
      break;
    case GNodeB::ULScheduler_TYPE_NB_IOT_ROUNDROBIN:
      scheduler = new nbRoundRobinUplinkPacketScheduler (mac);
      mac->SetUplinkPacketScheduler (scheduler);
      break;

    default:
      cout << "ERROR: invalid scheduler type" << endl;
      exit(1);
    }
}

UplinkPacketScheduler*
GNodeB::GetULScheduler (void) const
{
  return GetMacEntity ()->GetUplinkPacketScheduler ();
}

void
GNodeB::ResourceBlocksAllocation (void)
{
  DownlinkResourceBlockAllocation ();
  UplinkResourceBlockAllocation ();
}

void
GNodeB::UplinkResourceBlockAllocation (void)
{
  if (GetULScheduler () != nullptr && GetNbOfUserEquipmentRecords () > 0)
    {
      GetULScheduler ()->Schedule();
    }
}

void
GNodeB::DownlinkResourceBlockAllocation (void)
{
  if (GetDLScheduler () != nullptr && GetNbOfUserEquipmentRecords () > 0)
    {
      GetDLScheduler ()->Schedule();
    }
  else
    {
      //send only reference symbols
      //PacketBurst *pb = new PacketBurst ();
      //SendPacketBurst (pb);
    }
}

void
GNodeB::SetRandomAccessType(GnbRandomAccess::RandomAccessType type)
{
  GetMacEntity ()->SetRandomAccessType(type);
}


//Debug
void
GNodeB::Print (void)
{
  cout << " GNodeB object:"
            "\n\t m_idNetworkNode = " << GetIDNetworkNode () <<
            "\n\t m_idCell = " << GetCell ()->GetIdCell () <<
            "\n\t Served Users: " <<
            endl;

  for (auto record : *GetUserEquipmentRecords())
    {
      cout << "\t\t idUE = " << record->GetUE ()->
                GetIDNetworkNode () << endl;
    }
}
