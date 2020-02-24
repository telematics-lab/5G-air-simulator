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
 * Author: Sergio Martiradonna <sergio.martiradonna@poliba.it>
 */

#include "ue-nb-iot-random-access.h"
#include "enb-nb-iot-random-access.h"
#include "../../../componentManagers/FrameManager.h"
#include "../../../device/UserEquipment.h"
#include "../../../flows/radio-bearer.h"

#include "../../../flows/MacQueue.h"

UeNbIoTRandomAccess::UeNbIoTRandomAccess()
{
    
    DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS_NB)
    cout << "UeNbIoTRandomAccess() " << endl;
    DEBUG_LOG_END
    
    m_type = RA_TYPE_NB_IOT;
    m_macEntity = NULL;
    m_backoffIndicator = 0;
    m_CEClassStatic=0;
    m_CEClassDynamic=0;
    m_nbFailedAttemptsCE = 0;
    
}


UeNbIoTRandomAccess::UeNbIoTRandomAccess(MacEntity* mac) : UeNbIoTRandomAccess()
{
    m_macEntity = mac;
}


UeNbIoTRandomAccess::~UeNbIoTRandomAccess()
{}


void
UeNbIoTRandomAccess::StartRaProcedure()
{
    DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS_NB)
    cout << "UE " << m_macEntity ->GetDevice()->GetIDNetworkNode() << " StartRaProcedure() " << endl;
    DEBUG_LOG_END
    
    if ((m_macEntity->GetDevice()->GetNodeState()!= NetworkNode::STATE_ACTIVE))
    {
        if (m_RaProcedureActive == false)
        {
            m_RaProcedureActive = true;
            
            DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS)
            cout << "RANDOM_ACCESS START UE " << m_macEntity ->GetDevice()->GetIDNetworkNode()
            << " T " << Simulator::Init()->Now()
            << endl;
            DEBUG_LOG_END
            
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
UeNbIoTRandomAccess::ReStartRaProcedure()
{
    
    DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS_NB)
    cout << "ReStartRaProcedure() " << endl;
    DEBUG_LOG_END
    
    if (m_RaProcedureActive == true)
    {
        m_RaProcedureActive = false;
        m_nbFailedAttempts++;
        m_nbFailedAttemptsCE++;
        
        UserEquipment* ue = (UserEquipment*)m_macEntity->GetDevice();
        GnbNbIoTRandomAccess* gnbRam = (GnbNbIoTRandomAccess*) ue->GetTargetNode()->GetMacEntity()->GetRandomAccessManager();
        //		GNodeB* gnb = (GNodeB*)ue->GetTargetNode();
        //		GnbMacEntity* gnbMac = (GnbMacEntity*)gnb->GetProtocolStack()->GetMacEntity();
        //		GnbNbIoTRandomAccess* gnbRam = (GnbNbIoTRandomAccess*) gnbMac->GetRandomAccessManager();
        std::map<int, int> MaxPreambleAttempts=gnbRam->GetMaxPreambleTx();
        
        if (m_nbFailedAttempts < m_maxFailedAttempts)
        {
            if (m_nbFailedAttemptsCE >= MaxPreambleAttempts[m_CEClassDynamic] && m_CEClassDynamic+1<(int)MaxPreambleAttempts.size())
            {
                m_CEClassDynamic++;
                m_nbFailedAttemptsCE=0;
            }
            
            DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS_RESTART)
            cout << "RANDOM_ACCESS RESTART UE " << m_macEntity ->GetDevice()->GetIDNetworkNode()
            << " Att " << m_nbFailedAttempts << "/" << m_maxFailedAttempts
            << " UEclass " << m_CEClassStatic << "->"<< m_CEClassDynamic << " CEAtt " << m_nbFailedAttemptsCE << "/" << MaxPreambleAttempts[m_CEClassDynamic]
            << " T " << Simulator::Init()->Now() << endl;
            DEBUG_LOG_END
            Simulator::Init()->Schedule(0, &UeNbIoTRandomAccess::StartRaProcedure,this);
        }
        else
        {
            DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS_FAIL)
            cout << "RANDOM_ACCESS FAIL UE " << m_macEntity ->GetDevice()->GetIDNetworkNode()
            << " Att " << m_nbFailedAttempts << "/" << m_maxFailedAttempts
            << " UEclass " << m_CEClassStatic << "->"<< m_CEClassDynamic << " CEAtt " << m_nbFailedAttemptsCE << "/" << MaxPreambleAttempts[m_CEClassDynamic]
            << " T " << Simulator::Init()->Now() << endl;
            DEBUG_LOG_END
            m_nbFailedAttempts = 0;
            m_nbFailedAttemptsCE=0;
            m_CEClassDynamic=m_CEClassStatic;
        }
    }
}


void
UeNbIoTRandomAccess::SendMessage1()
{
    int currentTTI = FrameManager::Init()->GetTTICounter();
    
    UserEquipment* ue = (UserEquipment*)m_macEntity->GetDevice();
    GnbNbIoTRandomAccess* gnbRam = (GnbNbIoTRandomAccess*) ue->GetTargetNode()->GetMacEntity()->GetRandomAccessManager();
    //	GNodeB* gnb = (GNodeB*)ue->GetTargetNode();
    //	GnbMacEntity* gnbMac = (GnbMacEntity*)gnb->GetProtocolStack()->GetMacEntity();
    //	GnbNbIoTRandomAccess* gnbRam = (GnbNbIoTRandomAccess*) gnbMac->GetRandomAccessManager();
    
    std::map<int, int> subcarriers=gnbRam->GetSubcarriers();
    std::map<int, int> periodicity=gnbRam->GetResPeriodicity();
    std::map<int, int> timeOffset=gnbRam->GetTimeOffset();
    std::map<int, int> rao=gnbRam->GetRaoIndex();
    
    DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS_NB)
    cout << "T " << currentTTI << " UE " << m_macEntity ->GetDevice()->GetIDNetworkNode() << " Class " << m_CEClassDynamic << " is trying to send the Preamble " << endl;
    DEBUG_LOG_END
    
    
    if( isRachOpportunity() &&	(((currentTTI - timeOffset[m_CEClassDynamic]) % (periodicity[m_CEClassDynamic])) == 0))
    {
        
        RandomAccessPreambleIdealControlMessage* msg1 = new RandomAccessPreambleIdealControlMessage(subcarriers[m_CEClassDynamic]);
        msg1-> SetSourceDevice(ue);
        msg1->SetDestinationDevice (ue->GetTargetNode());
        
        msg1->SetMaxRAR(1);
        
        ue->GetPhy()->SendIdealControlMessage(msg1);
    }
    else
    {
        double delay;
        
        if (currentTTI - timeOffset[m_CEClassDynamic] < 0)
            delay = timeOffset[m_CEClassDynamic] - currentTTI;
        else
            delay = periodicity[m_CEClassDynamic] - ((currentTTI - timeOffset[m_CEClassDynamic])% periodicity[m_CEClassDynamic]);
        
        DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS_NB)
        cout << "T " << currentTTI << " UE " << m_macEntity ->GetDevice()->GetIDNetworkNode() << " Class " << m_CEClassDynamic << " will try to send the Preamble in " << delay << " ms" << endl;
        DEBUG_LOG_END
        Simulator::Init()->Schedule((delay)/1000,&UeNbIoTRandomAccess::SendMessage1,this);
    }
}


void
UeNbIoTRandomAccess::ReceiveMessage2(int msg3time)
{
    DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS_NB)
    cout << "ReceiveMessage2(" << msg3time<<") " << endl;
    DEBUG_LOG_END
    DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS)
    cout << "RANDOM_ACCESS RECEIVE_MSG2 UE " << m_macEntity ->GetDevice()->GetIDNetworkNode()
    << " T "<< Simulator::Init()->Now()
    << endl;
    DEBUG_LOG_END
    
    double delay = msg3time/1000.0;
    
    Simulator::Init()->Schedule(delay, &UeNbIoTRandomAccess::SendMessage3, this);
}


void
UeNbIoTRandomAccess::SendMessage3()
{
    DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS_NB)
    cout << "SendMessage3()" << endl;
    DEBUG_LOG_END
    UserEquipment* ue = (UserEquipment*)m_macEntity->GetDevice();
    DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS)
    cout << "RANDOM_ACCESS SEND_MSG3 UE " << ue->GetIDNetworkNode() << " T " << Simulator::Init()->Now() << endl;
    DEBUG_LOG_END
    
    m_msg3=new RandomAccessConnectionRequestIdealControlMessage();
    m_msg3->SetSourceDevice(ue);
    m_msg3->SetDestinationDevice (ue->GetTargetNode());
    ue->GetPhy()->SendIdealControlMessage(m_msg3);
}


void
UeNbIoTRandomAccess::ReceiveMessage4()
{
    DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS_NB)
    cout << "ReceiveMessage4()" << endl;
    DEBUG_LOG_END
    
    DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS)
    cout << "RANDOM_ACCESS RECEIVE_MSG4 UE " << m_macEntity ->GetDevice()->GetIDNetworkNode()
    << " T " << Simulator::Init()->Now()
    << endl;
    DEBUG_LOG_END
    
    RrcEntity *rrc = m_macEntity ->GetDevice ()->GetProtocolStack ()->GetRrcEntity ();
    RrcEntity::RadioBearersContainer* bearers = rrc->GetRadioBearerContainer ();
    std::vector<RadioBearer* >::iterator it =bearers->begin();
    int id = (*it)->GetMacQueue()->GetPacketQueue()->begin ()->GetPacket()->GetID();
    
    DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS_WIN)
    cout << "RACH_WIN UE " << m_macEntity ->GetDevice()->GetIDNetworkNode()
    << " T " << Simulator::Init()->Now()
    << " ID "<< id
    << endl;
    DEBUG_LOG_END
    
    UeMacEntity* mac = (UeMacEntity*)m_macEntity;
    
    mac -> GetDevice()->MakeActive();
    UeRandomAccess::SetRaProcedureActive(false);
    m_nbFailedAttempts = 0;
    m_nbFailedAttemptsCE=0;
    mac->SendSchedulingRequest();
    m_CEClassDynamic=m_CEClassStatic;
    
}
void UeNbIoTRandomAccess::SetCEClassStatic(int ceClassStatic)
{
    m_CEClassStatic=ceClassStatic;
}
int UeNbIoTRandomAccess::GetCEClassStatic(void)
{
    return m_CEClassStatic;
}
void UeNbIoTRandomAccess::SetCEClassDynamic(int ceClassDynamic)
{
    m_CEClassDynamic=ceClassDynamic;
}
int UeNbIoTRandomAccess::GetCEClassDynamic(void)
{
    return m_CEClassDynamic;
}

void UeNbIoTRandomAccess::SetNbFailedAttemptsCE(int nbFailedAttemptsCE)
{
    m_nbFailedAttemptsCE=nbFailedAttemptsCE;
}
int UeNbIoTRandomAccess::GetNbFailedAttemptsCE(void)
{
    return m_nbFailedAttemptsCE;
}
void UeNbIoTRandomAccess::SetMaxFailedAttempts(int maxFailedAttempts)
{
    m_maxFailedAttempts=maxFailedAttempts;
}
