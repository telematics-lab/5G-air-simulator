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
 */



#include "gnb-mac-entity.h"
#include "../packet/Packet.h"
#include "../packet/packet-burst.h"
#include "AMCModule.h"
#include "../../device/NetworkNode.h"
#include "packet-scheduler/packet-scheduler.h"
#include "packet-scheduler/downlink-packet-scheduler.h"
#include "packet-scheduler/uplink-packet-scheduler.h"
#include "../../device/UserEquipment.h"
#include "../../device/GNodeB.h"
#include "../../load-parameters.h"
#include "harq-manager.h"
#include "random-access/gnb-baseline-random-access.h"
#include "random-access/ue-nb-iot-random-access.h"
#include "random-access/gnb-enhanced-random-access.h"
#include "../../core/eventScheduler/simulator.h"
#include "random-access/enb-nb-iot-random-access.h"

GnbMacEntity::GnbMacEntity ()
{
  SetAmcModule (new AMCModule ());
  SetDevice (nullptr);
  m_downlinkScheduler = nullptr;
  m_uplinkScheduler = nullptr;
  m_defaultDlTxMode = 1;
  m_gnbrandomaccess = new GnbBaselineRandomAccess(this);
}


GnbMacEntity::~GnbMacEntity ()
{
  delete m_downlinkScheduler;
  delete m_uplinkScheduler;
  delete m_gnbrandomaccess;
  Destroy ();
}


void
GnbMacEntity::SetUplinkPacketScheduler (UplinkPacketScheduler* s)
{
  m_uplinkScheduler = s;
}


void
GnbMacEntity::SetDownlinkPacketScheduler (DownlinkPacketScheduler* s)
{
  m_downlinkScheduler = s;
}


UplinkPacketScheduler*
GnbMacEntity::GetUplinkPacketScheduler (void)
{
  return m_uplinkScheduler;
}


DownlinkPacketScheduler*
GnbMacEntity::GetDownlinkPacketScheduler (void)
{
  return m_downlinkScheduler;
}

GnbRandomAccess*
GnbMacEntity::GetRandomAccessManager(void)
{
  return m_gnbrandomaccess;
}

void
GnbMacEntity::SetRandomAccessManager(GnbRandomAccess* s)
{
  m_gnbrandomaccess = s;
}


void GnbMacEntity::SetRandomAccessType(GnbRandomAccess::RandomAccessType type)
{
  delete m_gnbrandomaccess;
  switch (type)
    {
    case GnbRandomAccess::RA_TYPE_BASELINE:
       m_gnbrandomaccess=new GnbBaselineRandomAccess(this);
       break;

    case GnbRandomAccess::RA_TYPE_ENHANCED:
       m_gnbrandomaccess=new GnbEnhancedRandomAccess(this);
       break;

    case GnbRandomAccess::RA_TYPE_NB_IOT:
       m_gnbrandomaccess=new GnbNbIoTRandomAccess(this);
       break;

    default:
       cout << "error : unknown random access type " << type << endl;
       exit(1);
    }
}


void
GnbMacEntity::ReceiveCqiIdealControlMessage  (CqiIdealControlMessage* msg)
{
DEBUG_LOG_START_1(SIM_ENV_TEST_CQI_FEEDBACKS)
  cout << "ReceiveIdealControlMessage (MAC) from  " << msg->GetSourceDevice ()->GetIDNetworkNode ()
            << " to " << msg->GetDestinationDevice ()->GetIDNetworkNode () << endl;
DEBUG_LOG_END

  CqiIdealControlMessage::CqiFeedbacks *cqi = msg->GetMessage ();

  UserEquipment* ue = (UserEquipment*) msg->GetSourceDevice ();
  GNodeB::UserEquipmentRecord* record = ue->GetTargetNodeRecord();
  AMCModule *amc = ue->GetProtocolStack()->GetMacEntity()->GetAmcModule();

  if (record != nullptr)
    {
      vector<int> cqiFeedback;
      for (auto feedback : *cqi)
        {
          cqiFeedback.push_back (feedback.m_cqi);
        }

DEBUG_LOG_START_1(SIM_ENV_TEST_CQI_FEEDBACKS)
      cout << "\t CQI: ";
      for (auto cqi : cqiFeedback)
        {
          cout << cqi << " ";
        }
      cout << endl;
DEBUG_LOG_END

DEBUG_LOG_START_1(SIM_ENV_AMC_MAPPING)
      cout << "\t CQI: ";
      for (auto cqi : cqiFeedback)
        {
          cout << cqi << " ";
        }
      cout << endl;

      cout << "\t MCS: ";
      for (auto cqi : cqiFeedback)
        {
          cout << amc->GetMCSFromCQI (cqi) << " ";
        }
      cout << endl;

      cout << "\t TB: ";
      for (auto cqi : cqiFeedback)
        {
          cout << amc->GetTBSizeFromMCS(
                      amc->GetMCSFromCQI (cqi)) << " ";
        }
      cout << endl;
DEBUG_LOG_END


      record->SetCQI (cqiFeedback);

    }
  else
    {
      cout << "ERROR: received cqi from unknown ue!"<< endl;
    }
}


void
GnbMacEntity::ReceiveIdealControlMessage (IdealControlMessage* msg)
{
  switch(msg->GetMessageType ())
    {
    case IdealControlMessage::CQI_FEEDBACKS:
DEBUG_LOG_START_1(SIM_ENV_TEST_CQI_FEEDBACKS)
      cout << "Receive CQI feedbacks from  " << msg->GetSourceDevice ()->GetIDNetworkNode ()
           << " to " << msg->GetDestinationDevice ()->GetIDNetworkNode () << endl;
DEBUG_LOG_END
      ReceiveCqiIdealControlMessage ( (CqiIdealControlMessage*)msg );
      break;

    case IdealControlMessage::SCHEDULING_REQUEST:
      ReceiveSchedulingRequestIdealControlMessage ( (SchedulingRequestIdealControlMessage*)msg );
      break;

    case IdealControlMessage::RI_FEEDBACK:
      ReceiveRiIdealControlMessage ( (RankIndicatorIdealControlMessage*)msg );
      break;

    case IdealControlMessage::PMI_FEEDBACK:
      ReceivePmiIdealControlMessage ( (PmiIdealControlMessage*)msg );
      break;

    case IdealControlMessage::FULLCSI_FEEDBACK:
      ReceiveFullCsiIdealControlMessage ( (FullCsiIdealControlMessage*)msg );
      break;

    case IdealControlMessage::HARQ_ACK:
      ReceiveHarqAckIdealControlMessage ( (HarqIdealControlMessage*)msg );
      break;

    case IdealControlMessage::RA_PREAMBLE:
    {
    	UeMacEntity* ueMac = (UeMacEntity*) msg->GetSourceDevice ()->GetProtocolStack()->GetMacEntity();
    	UeNbIoTRandomAccess* ueRam = (UeNbIoTRandomAccess*) ueMac->GetRandomAccessManager();
    	int ceClass = ueRam->GetCEClassDynamic();

    	DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS_PREAMBLE)
			  cout 	<< " T " << Simulator::Init()->Now() << " UE " << msg->GetSourceDevice ()->GetIDNetworkNode ()
					<< " CE " << ceClass << " PREAMBLE " << ((RandomAccessPreambleIdealControlMessage*)msg)->GetPreamble() <<  endl;
		DEBUG_LOG_END

		DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS)
		  	  cout << "RANDOM_ACCESS RECEIVE_MSG1 FROM_UE " << msg->GetSourceDevice ()->GetIDNetworkNode () << " T " << Simulator::Init()->Now() << endl;
		DEBUG_LOG_END

      ReceiveRandomAccessPreambleIdealControlMessage ( (RandomAccessPreambleIdealControlMessage*)msg );
      break;
    }
    case IdealControlMessage::RA_CONNECTION_REQUEST:
      ReceiveRandomAccessConnectionRequestIdealControlMessage((RandomAccessConnectionRequestIdealControlMessage*)msg);
      break;

    default:
      cout << "Error in GnbMacEntity::ReceiveIdealControlMessage(): unknown message type received ("
           << msg->GetMessageType () << ")" << endl;
      exit(1);
      break;
    }

}


void
GnbMacEntity::ReceiveRandomAccessPreambleIdealControlMessage(RandomAccessPreambleIdealControlMessage* msg)
{
  GetRandomAccessManager()->ReceiveMessage1(msg);
}


void
GnbMacEntity::ReceiveRandomAccessConnectionRequestIdealControlMessage(RandomAccessConnectionRequestIdealControlMessage* msg)
{
  GetRandomAccessManager()->ReceiveMessage3(msg);
}


void
GnbMacEntity::ReceiveRiIdealControlMessage (RankIndicatorIdealControlMessage* msg)
{
  int ri = msg->GetRI ();

  UserEquipment* ue = (UserEquipment*) msg->GetSourceDevice ();
  GNodeB::UserEquipmentRecord* record = ue->GetTargetNodeRecord();

  if (record != nullptr)
    {
DEBUG_LOG_START_1(SIM_ENV_TEST_RI_FEEDBACK)
      cout << "\t RI: " << ri << endl;
DEBUG_LOG_END
      record->SetRI (ri);
    }
  else
    {
      cout << "ERROR: received ri from unknown ue!"<< endl;
    }
}

void
GnbMacEntity::ReceivePmiIdealControlMessage (PmiIdealControlMessage* msg)
{
  vector< vector<int> > pmi = msg->GetPMI ();

  UserEquipment* ue = (UserEquipment*) msg->GetSourceDevice ();
  GNodeB::UserEquipmentRecord* record = ue->GetTargetNodeRecord();

  if (record != nullptr)
    {
DEBUG_LOG_START_1(SIM_ENV_TEST_PMI_FEEDBACK)
      cout << "\t PMI: " << pmi << endl;
DEBUG_LOG_END
      record->SetPMI (pmi);
    }
  else
    {
      cout << "ERROR: received pmi from unknown ue!"<< endl;
    }
}

void
GnbMacEntity::ReceiveFullCsiIdealControlMessage  (FullCsiIdealControlMessage* msg)
{
  vector< shared_ptr<arma::cx_fmat> > channelMatrix = msg->GetChannelMatrix();

  UserEquipment* ue = (UserEquipment*) msg->GetSourceDevice ();
  GNodeB::UserEquipmentRecord* record = ue->GetTargetNodeRecord();

  if (record != nullptr)
    {
      record->SetChannelMatrix (channelMatrix);
    }
  else
    {
      cout << "ERROR: received csi from unknown ue!"<< endl;
    }
}

void
GnbMacEntity::SendPdcchMapIdealControlMessage  (PdcchMapIdealControlMessage* msg)
{
}


void
GnbMacEntity::ReceiveSchedulingRequestIdealControlMessage (SchedulingRequestIdealControlMessage* msg)
{
  UserEquipment* ue = (UserEquipment*) msg->GetSourceDevice ();
  GNodeB::UserEquipmentRecord* record = ue->GetTargetNodeRecord();

  int bufferStatusReport = msg->GetBufferStatusReport ();

  if (record != nullptr)
    {
      record->SetSchedulingRequest (bufferStatusReport);
    }
  else
    {
      cout << "ERROR: received cqi from unknown ue!"<< endl;
    }
}

void
GnbMacEntity::ReceiveHarqAckIdealControlMessage (HarqIdealControlMessage *msg)
{
  UserEquipment* ue = (UserEquipment*) msg->GetSourceDevice ();
  GNodeB::UserEquipmentRecord* record = ue->GetTargetNodeRecord();
  HarqManager* harq = record->GetHarqManager ();
  harq->ReceiveAck (msg->GetAck (), msg->GetPid ());
}

void
GnbMacEntity::SetDefaultDlTxMode (int txmode)
{
  m_defaultDlTxMode = txmode;
}

int
GnbMacEntity::GetDefaultDlTxMode(void)
{
  return m_defaultDlTxMode;
}


GNodeB*
GnbMacEntity::GetDevice(void)
{
  NetworkNode* node = ((MacEntity*)this)->GetDevice();
  return (GNodeB*)node;
}
