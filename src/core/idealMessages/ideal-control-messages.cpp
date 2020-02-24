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


#include "ideal-control-messages.h"
#include "../../device/NetworkNode.h"


IdealControlMessage::IdealControlMessage (void)
  : m_source (0),
    m_destination (0)
{
}


void
IdealControlMessage::SetSourceDevice (NetworkNode* src)
{
  m_source = src;
}


void
IdealControlMessage::SetDestinationDevice (NetworkNode* dst)
{
  m_destination = dst;
}


NetworkNode*
IdealControlMessage::GetSourceDevice (void)
{
  return m_source;
}


NetworkNode*
IdealControlMessage::GetDestinationDevice (void)
{
  return m_destination;
}


void
IdealControlMessage::SetMessageType (IdealControlMessage::MessageType type)
{
  m_type = type;
}


IdealControlMessage::MessageType
IdealControlMessage::GetMessageType (void)
{
  return m_type;
}



// ----------------------------------------------------------------------------------------------------------


PdcchMapIdealControlMessage::PdcchMapIdealControlMessage (void)
{
  m_idealPdcchMessage =  new IdealPdcchMessage ();
  SetMessageType (IdealControlMessage::ALLOCATION_MAP);
}


PdcchMapIdealControlMessage::~PdcchMapIdealControlMessage (void)
{
  m_idealPdcchMessage->clear ();
  delete m_idealPdcchMessage;
}


void
PdcchMapIdealControlMessage::AddNewRecord (Direction direction,
                                           int subChannel, NetworkNode* ue, double mcs, int harqPid)
{
  IdealPdcchRecord record;
  record.m_direction = direction;
  record.m_idSubChannel = subChannel;
  record.m_mcsIndex = mcs;
  record.m_harqPid = harqPid;
  record.m_precodingMatrix = nullptr;
  record.m_ue = ue;
  m_idealPdcchMessage->push_back (record);
}

void
PdcchMapIdealControlMessage::AddNewRecord (Direction direction,
                                           int subChannel, NetworkNode* ue, double mcs, int harqPid, int rank)
{
  IdealPdcchRecord record;
  record.m_direction = direction;
  record.m_idSubChannel = subChannel;
  record.m_mcsIndex = mcs;
  record.m_harqPid = harqPid;
  record.m_rank = rank;
  record.m_precodingMatrix = nullptr;
  record.m_ue = ue;
  m_idealPdcchMessage->push_back (record);
}

void
PdcchMapIdealControlMessage::AddNewRecord (Direction direction,
                                           int subChannel, NetworkNode* ue, double mcs, int harqPid, int rank,
                                           vector<int> pmi)
{
  IdealPdcchRecord record;
  record.m_direction = direction;
  record.m_idSubChannel = subChannel;
  record.m_mcsIndex = mcs;
  record.m_harqPid = harqPid;
  record.m_rank = rank;
  record.m_pmi = pmi;
  record.m_precodingMatrix = nullptr;
  record.m_ue = ue;
  m_idealPdcchMessage->push_back (record);
}

void PdcchMapIdealControlMessage::AddNewRecord (Direction direction,
                                                int subChannel, NetworkNode* ue, double mcs, int harqPid, int rank,
                                                vector<int> pmi, vector<int> assignedLayers)
{
  IdealPdcchRecord record;
  record.m_direction = direction;
  record.m_idSubChannel = subChannel;
  record.m_mcsIndex = mcs;
  record.m_harqPid = harqPid;
  record.m_rank = rank;
  record.m_pmi = pmi;
  record.m_precodingMatrix = nullptr;
  record.m_assignedLayers = assignedLayers;
  record.m_ue = ue;
  m_idealPdcchMessage->push_back (record);
}


void PdcchMapIdealControlMessage::AddNewRecord (Direction direction,
                                                int subChannel, NetworkNode* ue, double mcs, int harqPid, int rank,
                                                shared_ptr<arma::cx_fmat> precodingMatrix, vector<int> assignedLayers)
{
  IdealPdcchRecord record;
  record.m_direction = direction;
  record.m_idSubChannel = subChannel;
  record.m_mcsIndex = mcs;
  record.m_harqPid = harqPid;
  record.m_rank = rank;

  // create fake PMI reports to prevent errors
  record.m_pmi.resize (2);
  record.m_pmi.at (0) = 0;
  record.m_pmi.at (1) = 0;

  record.m_precodingMatrix = precodingMatrix;
  record.m_assignedLayers = assignedLayers;
  record.m_fullCsiMatrix = nullptr;
  record.m_ue = ue;
  m_idealPdcchMessage->push_back (record);
}


void PdcchMapIdealControlMessage::AddNewRecord (Direction direction,
    int subChannel,
    NetworkNode* ue,
    double mcs,
    int harqPid,
    int rank,
    shared_ptr<arma::cx_fmat> precodingMatrix,
    vector<int> assignedLayers,
    shared_ptr<arma::cx_fmat> fullCsiMatrix)
{
  IdealPdcchRecord record;
  record.m_direction = direction;
  record.m_idSubChannel = subChannel;
  record.m_mcsIndex = mcs;
  record.m_harqPid = harqPid;
  record.m_rank = rank;

  // create fake PMI reports to prevent errors
  record.m_pmi.resize (2);
  record.m_pmi.at (0) = 0;
  record.m_pmi.at (1) = 0;

  record.m_precodingMatrix = precodingMatrix;
  record.m_assignedLayers = assignedLayers;
  record.m_fullCsiMatrix = fullCsiMatrix;
  record.m_ue = ue;
  m_idealPdcchMessage->push_back (record);
}




PdcchMapIdealControlMessage::IdealPdcchMessage*
PdcchMapIdealControlMessage::GetMessage (void)
{
  return m_idealPdcchMessage;
}

vector<NetworkNode*>
PdcchMapIdealControlMessage::GetTargetUEs (void)
{
  vector<NetworkNode*> targets;
  for (auto record: *m_idealPdcchMessage)
  {
    if(std::find(targets.begin (), targets.end (), record.m_ue) == targets.end ())
    {
      targets.push_back(record.m_ue);
    }
  }
  return targets;
}



// ----------------------------------------------------------------------------------------------------------


CqiIdealControlMessage::CqiIdealControlMessage (void)
{
  m_cqiFeedbacks = new CqiFeedbacks ();
  SetMessageType (IdealControlMessage::CQI_FEEDBACKS);
}


CqiIdealControlMessage::~CqiIdealControlMessage (void)
{
  m_cqiFeedbacks->clear ();
  delete m_cqiFeedbacks;
}


void
CqiIdealControlMessage::AddNewRecord (int subChannel, double cqi)
{
  CqiFeedback c;
  c.m_idSubChannel = subChannel;
  c.m_cqi = cqi;

  m_cqiFeedbacks->push_back (c);

}


CqiIdealControlMessage::CqiFeedbacks*
CqiIdealControlMessage::GetMessage (void)
{
  return m_cqiFeedbacks;
}



// ----------------------------------------------------------------------------------------------------------


ArqRlcIdealControlMessage::ArqRlcIdealControlMessage (void)
{
  SetMessageType (IdealControlMessage::ARQ_RLC_ACK);
}

void
ArqRlcIdealControlMessage::SetAck (int ack)
{
  m_ack = ack;
}

int
ArqRlcIdealControlMessage::GetAck (void)
{
  return m_ack;
}

void
ArqRlcIdealControlMessage::SetStartByte (int b)
{
  m_startByte = b;
}

void
ArqRlcIdealControlMessage::SetEndByte (int b)
{
  m_endByte = b;
}

int
ArqRlcIdealControlMessage::GetStartByte (void)
{
  return m_startByte;
}

int
ArqRlcIdealControlMessage::GetEndByte (void)
{
  return m_endByte;
}




// ----------------------------------------------------------------------------------------------------------


SchedulingRequestIdealControlMessage::SchedulingRequestIdealControlMessage (void)
{
  SetMessageType (IdealControlMessage::SCHEDULING_REQUEST);
}

int
SchedulingRequestIdealControlMessage::GetBufferStatusReport ()
{
  return m_bufferStatusReport;
}

void
SchedulingRequestIdealControlMessage::SetBufferStatusReport (int b)
{
  m_bufferStatusReport = b;
}


// ----------------------------------------------------------------------------------------------------------

RankIndicatorIdealControlMessage::RankIndicatorIdealControlMessage (void)
{
  SetMessageType(IdealControlMessage::RI_FEEDBACK);
}

int
RankIndicatorIdealControlMessage::GetRI (void)
{
  return m_rankIndicator;
}

void
RankIndicatorIdealControlMessage::SetRI (int ri)
{
  m_rankIndicator = ri;
}

// ----------------------------------------------------------------------------------------------------------


PmiIdealControlMessage::PmiIdealControlMessage (void)
{
  SetMessageType(IdealControlMessage::PMI_FEEDBACK);
}

vector< vector<int> >
PmiIdealControlMessage::GetPMI (void)
{
  return m_pmiFeedbacks;
}

void
PmiIdealControlMessage::SetPMI (vector< vector<int> > pmi)
{
  m_pmiFeedbacks = pmi;
}

// ----------------------------------------------------------------------------------------------------------

FullCsiIdealControlMessage::FullCsiIdealControlMessage (void)
{
  SetMessageType(IdealControlMessage::FULLCSI_FEEDBACK);
}

vector< shared_ptr<arma::cx_fmat> >
FullCsiIdealControlMessage::GetChannelMatrix(void)
{
  return m_channelMatrix;
}

void
FullCsiIdealControlMessage::SetChannelMatrix (vector< shared_ptr<arma::cx_fmat> > ch_mat)
{
  m_channelMatrix = ch_mat;
}

// ----------------------------------------------------------------------------------------------------------


HarqIdealControlMessage::HarqIdealControlMessage (void)
{
  SetMessageType (IdealControlMessage::HARQ_ACK);
}

void
HarqIdealControlMessage::SetAck (bool ack)
{
  m_ack = ack;
}

bool
HarqIdealControlMessage::GetAck (void)
{
  return m_ack;
}

void
HarqIdealControlMessage::SetPid (int pid)
{
  m_pid = pid;
}

int
HarqIdealControlMessage::GetPid (void)
{
  return m_pid;
}

// --------------------------------------------------------------------------------------------

#include "../../componentManagers/FrameManager.h"

RandomAccessPreambleIdealControlMessage::RandomAccessPreambleIdealControlMessage (void)
{
  SetMessageType (IdealControlMessage::RA_PREAMBLE);
  CreatePreamble();
  SetTimeTx();
}

RandomAccessPreambleIdealControlMessage::RandomAccessPreambleIdealControlMessage (int p)
{
  SetMessageType (IdealControlMessage::RA_PREAMBLE);
  SetTimeTx();
  std::uniform_int_distribution<> pre(1, p);
  extern std::mt19937 commonGen;
  m_preamble = pre(commonGen);
}


RandomAccessPreambleIdealControlMessage::~RandomAccessPreambleIdealControlMessage (void)
{}


void
RandomAccessPreambleIdealControlMessage::CreatePreamble ()
{
  m_preamble = rand() % 64;
}

int
RandomAccessPreambleIdealControlMessage::GetPreamble ()
{
  return m_preamble;
}

void
RandomAccessPreambleIdealControlMessage::SetMaxRAR (int max_rar)
{
  m_rar = rand() % max_rar;
}

int
RandomAccessPreambleIdealControlMessage::GetMultipleRAR()
{
  return m_rar;
}

void
RandomAccessPreambleIdealControlMessage::SetTimeTx()
{
  m_timeTx = FrameManager::Init()->GetTTICounter();
}

int
RandomAccessPreambleIdealControlMessage::GetTimeTx()
{
  return m_timeTx;
}



// ----------------------------------------------------------------------------------------------------

RandomAccessResponseIdealControlMessage::RandomAccessResponseIdealControlMessage(void)
{
  SetMessageType (IdealControlMessage::RA_RESPONSE);
  m_msg1Received = false;
}

RandomAccessResponseIdealControlMessage::~RandomAccessResponseIdealControlMessage(void)
{}

void
RandomAccessResponseIdealControlMessage::SetMsg2(bool r)
{
  m_msg1Received = r;
}

bool
RandomAccessResponseIdealControlMessage::GetMsg2()
{
  return m_msg1Received;
}

void
RandomAccessResponseIdealControlMessage::setMsg3Time(int time)
{
  m_msg3time = time;
}

int
RandomAccessResponseIdealControlMessage::getMsg3Time()
{
  return m_msg3time;
}

void
RandomAccessResponseIdealControlMessage::setMsg3RB(int rb)
{
  m_msg3RB = rb;
}

int
RandomAccessResponseIdealControlMessage::getMsg3RB()
{
  return m_msg3RB;
}

// ----------------------------------------------------------------------------------------------------
RandomAccessConnectionRequestIdealControlMessage::RandomAccessConnectionRequestIdealControlMessage(void)
{
  SetMessageType(IdealControlMessage::RA_CONNECTION_REQUEST);
  CreatUeID();
  GetUeID();
  void SetMotivationRequest(RandomAccessConnectionRequestIdealControlMessage::MotivationRequest motivation);
}

RandomAccessConnectionRequestIdealControlMessage::~RandomAccessConnectionRequestIdealControlMessage(void)
{}

void RandomAccessConnectionRequestIdealControlMessage::CreatUeID()
{
  m_UeID = rand();
}

int
RandomAccessConnectionRequestIdealControlMessage::GetUeID()
{
  return m_UeID;
}

void RandomAccessConnectionRequestIdealControlMessage::SetMotivationRequest(MotivationRequest motivation)
{
  m_motivation = SCHEDULING_REQUEST;
}

RandomAccessConnectionRequestIdealControlMessage::MotivationRequest RandomAccessConnectionRequestIdealControlMessage::GetMotivationRequest()
{
  return m_motivation;
}

//------------------------------------------------------------------------------------------------------
RandomAccessConnectionResolutionIdealControlMessage::RandomAccessConnectionResolutionIdealControlMessage(void)
{
  SetMessageType(IdealControlMessage::RA_CONNECTION_RESOLUTION);
}

RandomAccessConnectionResolutionIdealControlMessage::~RandomAccessConnectionResolutionIdealControlMessage(void)
{}

// ----------------------------------------------------------------------------------------------------------


NbIoTMapIdealControlMessage::NbIoTMapIdealControlMessage (void)
{
  m_idealPdcchMessage =  new IdealPdcchMessage ();
  SetMessageType (IdealControlMessage::NB_IOT_ALLOCATION_MAP);
}


NbIoTMapIdealControlMessage::~NbIoTMapIdealControlMessage (void)
{
  m_idealPdcchMessage->clear ();
  delete m_idealPdcchMessage;
}


void
NbIoTMapIdealControlMessage::AddNewRecord (int ru, NetworkNode* ue, double mcs, int subcarrier)
{
  IdealPdcchRecord record;
  record.m_ru = ru;
  record.m_mcsIndex = mcs;
  record.m_ue = ue;
  record.m_idSubChannel = subcarrier;
  m_idealPdcchMessage->push_back (record);
}


NbIoTMapIdealControlMessage::IdealPdcchMessage*
NbIoTMapIdealControlMessage::GetMessage (void)
{
  return m_idealPdcchMessage;
}
