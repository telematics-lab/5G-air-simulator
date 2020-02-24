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

#include "cqi-manager.h"
#include "../../core/eventScheduler/simulator.h"
#include "../NetworkNode.h"
#include "../UserEquipment.h"
#include "../MulticastDestination.h"
#include "../../core/idealMessages/ideal-control-messages.h"
#include "../../core/spectrum/bandwidth-manager.h"

CqiManager::CqiManager()
{
  m_device  = nullptr;
  m_lastSent = 0;
  m_sendCqi = false;
}

void
CqiManager::SetDevice (NetworkNode* d)
{
  m_device = d;
}

NetworkNode*
CqiManager::GetDevice (void)
{
  return m_device;
}

void
CqiManager::SetCqiReportingMode (CqiManager::CQIReportingMode m)
{
  m_reportingMode = m;
  if (m_reportingMode == CqiManager::APERIODIC)
    {
      m_sendCqi = false;
    }
  else
    {
      m_sendCqi = true;
    }
}

CqiManager::CQIReportingMode
CqiManager::GetCqiReportingMode (void)
{
  return m_reportingMode;
}

void
CqiManager::SetSendCqi (bool b)
{
  m_sendCqi = b;
}

bool
CqiManager::GetSendCqi (void)
{
  return m_sendCqi;
}


void
CqiManager::SetReportingInterval (int i)
{
  m_reportingInterval = i;
}

int
CqiManager::GetReportingInterval (void)
{
  return m_reportingInterval;
}

void
CqiManager::SetLastSent ()
{
  m_lastSent = Simulator::Init ()->Now () * 1000; //TTI
}

long int
CqiManager::GetLastSent (void)
{
  return m_lastSent;
}

bool
CqiManager::NeedToSendFeedbacks (void)
{
//  cout <<"NeedToSendFeedbacks " << (Simulator::Init ()->Now ()*1000) << GetReportingInterval () << endl;

  if (GetSendCqi () && GetReportingInterval () == 1 )
    {
      return true;
    }

  else if (GetSendCqi () && ((int)(Simulator::Init ()->Now ()*1000) - GetLastSent ()) >= GetReportingInterval ())
    {
      return true;
    }
  else
    {
      return false;
    }
}

void
CqiManager::CreateCqiFeedbacks (vector<double> sinr)
{
DEBUG_LOG_START_1(SIM_ENV_TEST_CQI_FEEDBACKS)
  cout << "CqiManager -> CreateCqiFeedbacks " << endl;
DEBUG_LOG_END

  UserEquipment* thisNode = (UserEquipment*) GetDevice ();
  vector<GNodeB*> targetNodes;

  if (thisNode->GetNodeType()==NetworkNode::TYPE_UE)
    {
      targetNodes.push_back( thisNode->GetTargetNode () );
    }
  else if (thisNode->GetNodeType()==NetworkNode::TYPE_MULTICAST_DESTINATION)
    {
      targetNodes = ((MulticastDestination*)thisNode)->GetSources();
    }
  else
    {
      cout << "Error: unsupported device type for CQI feedback creation" << endl;
      exit(1);
    }

  vector<int> cqi = CalculateCqiFeedbacks (sinr);

DEBUG_LOG_START_1(SIM_ENV_TEST_CQI_FEEDBACKS)
  cout << "CQI values : " << cqi << endl;
DEBUG_LOG_END

  int nbSubChannels = cqi.size ();
  vector<double> dlSubChannels = thisNode->GetPhy ()->GetBandwidthManager ()->GetDlSubChannels ();

  for (auto dest:targetNodes)
    {
      CqiIdealControlMessage *msg = new CqiIdealControlMessage ();
      msg->SetSourceDevice (thisNode);
      msg->SetDestinationDevice (dest);
      for (int i = 0; i < nbSubChannels; i++)
        {
          msg->AddNewRecord (dlSubChannels.at (i), cqi.at (i));
        }
      thisNode->GetPhy ()->SendIdealControlMessage (msg);
    }

  SetLastSent ();
}

void
CqiManager::CreateRiFeedback (int rankIndicator)
{
  UserEquipment* thisNode = (UserEquipment*) GetDevice ();
  vector<GNodeB*> targetNodes;

  if (thisNode->GetNodeType()==NetworkNode::TYPE_UE)
    {
      targetNodes.push_back( thisNode->GetTargetNode () );
    }
  else if (thisNode->GetNodeType()==NetworkNode::TYPE_MULTICAST_DESTINATION)
    {
      targetNodes = ((MulticastDestination*)thisNode)->GetSources();
    }
  else
    {
      cout << "Error: unsupported device type for RI feedback creation" << endl;
      exit(1);
    }

  for (auto dest:targetNodes)
    {
      RankIndicatorIdealControlMessage *msg = new RankIndicatorIdealControlMessage ();

      msg->SetSourceDevice (thisNode);
      msg->SetDestinationDevice (dest);
      msg->SetRI (rankIndicator);

      thisNode->GetPhy ()->SendIdealControlMessage (msg);
    }
  SetLastSent ();
}

void
CqiManager::CreatePmiFeedbacks (vector< vector< int > > pmi)
{
  UserEquipment* thisNode = (UserEquipment*) GetDevice ();
  NetworkNode* targetNode = thisNode->GetTargetNode ();
  PmiIdealControlMessage *msg = new PmiIdealControlMessage();

  msg->SetSourceDevice (thisNode);
  msg->SetDestinationDevice (targetNode);
  msg->SetPMI(pmi);

  SetLastSent ();
  thisNode->GetPhy ()->SendIdealControlMessage (msg);
}

void
CqiManager::CreateFullCsiFeedbacks(vector< shared_ptr<arma::cx_fmat> > channelMatrix)
{
  UserEquipment* thisNode = (UserEquipment*) GetDevice ();
  NetworkNode* targetNode = thisNode->GetTargetNode ();
  FullCsiIdealControlMessage *msg = new FullCsiIdealControlMessage();

  msg->SetSourceDevice (thisNode);
  msg->SetDestinationDevice (targetNode);
  msg->SetChannelMatrix(channelMatrix);

  SetLastSent ();
  thisNode->GetPhy ()->SendIdealControlMessage (msg);
}
