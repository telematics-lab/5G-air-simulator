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
 * Author: Cosimo Damiano Di Pace <cd.dipace@gmail.com>
 * Author: Alessandro Grassi <alessandro.grassi@poliba.it>
 */

#include "ue-enhanced-random-access.h"
#include "gnb-enhanced-random-access.h"
#include "../../../componentManagers/FrameManager.h"
#include "../../../device/UserEquipment.h"


UeEnhancedRandomAccess::UeEnhancedRandomAccess()
{
  m_type = RA_TYPE_ENHANCED;
  m_macEntity = NULL;
  m_backoffIndicator = 5;
}


UeEnhancedRandomAccess::UeEnhancedRandomAccess(MacEntity* mac) : UeEnhancedRandomAccess()
{
  m_macEntity = mac;
}


UeEnhancedRandomAccess::~UeEnhancedRandomAccess()
{
}


void
UeEnhancedRandomAccess::StartRaProcedure()
{

  if ((m_macEntity->GetDevice()->GetNodeState()!= NetworkNode::STATE_ACTIVE))
    {
      if (m_RaProcedureActive == false)
        {
          m_RaProcedureActive = true;

DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS)
          cout << "RANDOM_ACCESS START UE " << m_macEntity ->GetDevice()->GetIDNetworkNode() << " T " << Simulator::Init()->Now() << endl;
DEBUG_LOG_END

          UeRandomAccess::SetRAProcedureStartTime(Simulator::Init()->Now());
          Simulator::Init()->Schedule(m_RAProcedureTimeout, &UeEnhancedRandomAccess::ReStartRaProcedure, this);

          SendMessage1();
        }
    }

  else
    {
      UserEquipment* ue = (UserEquipment*)m_macEntity->GetDevice();
      ue -> SetLastActivity();
    }

}


void
UeEnhancedRandomAccess::ReStartRaProcedure()
{
  if (m_RaProcedureActive == true)
    {
      m_RaProcedureActive = false;
      m_nbFailedAttempts++;
      if (m_nbFailedAttempts < m_maxFailedAttempts)
        {
DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS)
          cout << "RANDOM_ACCESS RESTART UE " << m_macEntity ->GetDevice()->GetIDNetworkNode() << " TRY " << m_nbFailedAttempts << " T " << Simulator::Init()->Now() << endl;
DEBUG_LOG_END
          int maxWaitingTime;
          if (m_backoffIndicator == 0)
            {
              maxWaitingTime = 0;
            }
          else if (m_backoffIndicator <= 12)
            {
              maxWaitingTime = pow(2,m_backoffIndicator+7);
            }
          else
            {
              cout << "Error: invalid backoff indicator " << m_backoffIndicator << endl;
              exit(1);
            }
          int waitingTime = rand() % maxWaitingTime;
          Simulator::Init()->Schedule(0.001*waitingTime,&UeEnhancedRandomAccess::StartRaProcedure,this);
        }
      else
        {
DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS)
          cout << "RANDOM_ACCESS FAIL UE " << m_macEntity ->GetDevice()->GetIDNetworkNode() << " T " << Simulator::Init()->Now() << endl;
DEBUG_LOG_END
          m_nbFailedAttempts = 0;
        }
    }
}


void
UeEnhancedRandomAccess::SendMessage1()
{
  if( isRachOpportunity() )
    {
      RandomAccessPreambleIdealControlMessage* msg1 = new RandomAccessPreambleIdealControlMessage();
      msg1-> SetSourceDevice(m_macEntity->GetDevice());

      UserEquipment* ue = (UserEquipment*)m_macEntity->GetDevice();
      msg1->SetDestinationDevice (ue->GetTargetNode());

      GnbMacEntity* gnbMac = ue->GetTargetNode()->GetMacEntity();
      GnbEnhancedRandomAccess* gnbRam = (GnbEnhancedRandomAccess*)gnbMac->GetRandomAccessManager();
      int max_rar = gnbRam->getNbRARs();
      msg1->SetMaxRAR(max_rar);

      m_macEntity->GetDevice()->GetPhy()->SendIdealControlMessage(msg1);
    }
  else
    {
      Simulator::Init()->Schedule(0.001,&UeEnhancedRandomAccess::SendMessage1,this);
    }
}


void
UeEnhancedRandomAccess::ReceiveMessage2(int msg3time)
{
DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS)
  cout << "RANDOM_ACCESS RECEIVE_MSG2 UE " << m_macEntity ->GetDevice()->GetIDNetworkNode() << " T " << Simulator::Init()->Now() << endl;
DEBUG_LOG_END

  double delay = msg3time/1000.0 - Simulator::Init()->Now();

  Simulator::Init()->Schedule(delay, &UeEnhancedRandomAccess::SendMessage3, this);
}


void
UeEnhancedRandomAccess::SendMessage3()
{
DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS)
  cout << "RANDOM_ACCESS SEND_MSG3 UE " << m_macEntity ->GetDevice()->GetIDNetworkNode() << " T " << Simulator::Init()->Now() << endl;
DEBUG_LOG_END

  m_msg3 = new RandomAccessConnectionRequestIdealControlMessage();
  m_msg3->SetSourceDevice(m_macEntity->GetDevice());


  UserEquipment* ue = (UserEquipment*)m_macEntity->GetDevice();
  m_msg3->SetDestinationDevice (ue->GetTargetNode());
  m_macEntity->GetDevice()->GetPhy()->SendIdealControlMessage(m_msg3);
}


void
UeEnhancedRandomAccess::ReceiveMessage4()
{
//DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS)
  cout << "RANDOM_ACCESS RECEIVE_MSG4 UE " << m_macEntity ->GetDevice()->GetIDNetworkNode() << " T " << Simulator::Init()->Now() << endl;
//DEBUG_LOG_END

  UeMacEntity* mac = (UeMacEntity*)m_macEntity;
  mac->GetDevice()->MakeActive();
  mac->SendSchedulingRequest();

  UeRandomAccess::SetRaProcedureActive(false);
  m_nbFailedAttempts = 0;
}
