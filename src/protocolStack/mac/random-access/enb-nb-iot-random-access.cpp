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

#include "enb-nb-iot-random-access.h"
#include "../../../componentManagers/FrameManager.h"
#include "../../../core/eventScheduler/simulator.h"
#include "../../../device/GNodeB.h"
#include "ue-nb-iot-random-access.h"
#include "../gnb-mac-entity.h"
#include "../ue-mac-entity.h"
#include "../../../core/idealMessages/ideal-control-messages.h"
#include "../../../device/NetworkNode.h"
#include "../../../phy/phy.h"
#include "../../../core/spectrum/bandwidth-manager.h"
#include "../../../device/UserEquipment.h"
#include "../../../load-parameters.h"

GnbNbIoTRandomAccess::GnbNbIoTRandomAccess() {
    DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS_NB)
    cout << "GnbNbIoTRandomAccess::GnbNbIoTRandomAccess() " << endl;
    DEBUG_LOG_END
    m_PreambleFormat = Preamble_FORMAT_0; //#
    m_type = RA_TYPE_NB_IOT;
    m_lastRachOpportunity = 0; //#
    m_periodicity = 0;
    m_PreambleDuration = 6; // SFs
    m_macEntity = nullptr;
    m_responseWindowDelay = 3; // SFs
    m_responseWindowDuration = 48; // SFs
    m_nbRARs = 1; //#
    m_maxCCCHUsage = (float) (2 * m_nbRARs + 1) / ((m_nbRARs + 1) * 2);
    
    m_resPeriodicity.clear();
    m_RachReservation = Simulator::Init()->Schedule(0.00,
                                                    &GnbNbIoTRandomAccess::SetRachReservedSubChannels, this);
    m_RarScheduling = Simulator::Init()->Schedule(0.000,
                                                  &GnbNbIoTRandomAccess::CheckCollisions, this);
    for (int i = 0; i < 3; i++)
        m_RAOindex.insert(std::make_pair(i, 0));
}

GnbNbIoTRandomAccess::GnbNbIoTRandomAccess(MacEntity* mac) : GnbNbIoTRandomAccess() {
    m_macEntity = mac;
}

GnbNbIoTRandomAccess::~GnbNbIoTRandomAccess() {
    m_RachReservation->MarkDeleted();
    m_RarScheduling->MarkDeleted();
}

void GnbNbIoTRandomAccess::SetRachReservedSubChannels()
{
    if (m_resPeriodicity.empty())
    {
        m_RachReservation = Simulator::Init()->Schedule(0.001, &GnbNbIoTRandomAccess::SetRachReservedSubChannels, this);
    }
    else
    {
        int currentSF = FrameManager::Init()->GetTTICounter();
        
        DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS_NB)
        cout << "GnbNbIoTRandomAccess::SetRachReservedSubChannels() - SF "
        << currentSF << " T " << Simulator::Init()->Now() << endl;
        DEBUG_LOG_END
        
        vector<int> startTime;
        for (auto i : m_resPeriodicity)
            startTime.push_back(((m_RAOindex[i.first]) * i.second) + m_timeOffset[i.first]);
        int nextStartTime = *std::min_element(startTime.begin(), startTime.end());
        
        if (currentSF < nextStartTime -1)
        {
            DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS_NB)
            cout << "Next start time = " << nextStartTime
            << " - it will be scheduled in "
            << (((double) nextStartTime - (double) currentSF - 1) / 1000)
            << endl;
            DEBUG_LOG_END
            m_RachReservation = Simulator::Init()->Schedule(
                                                            (((double) nextStartTime - (double) currentSF - 1 ) / 1000),
                                                            &GnbNbIoTRandomAccess::SetRachReservedSubChannels, this);
        }
        else
        {
            for (auto i : m_resPeriodicity)
            {
                if (currentSF + 1 == m_RAOindex[i.first] *i.second +m_timeOffset[i.first])
                {
                    DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS_NB)
                    cout << "\t\t\tClass " << i.first << " RAO "
                    << m_RAOindex[i.first] << " Periodicity "
                    << i.second << " Start Time "
                    << m_timeOffset[i.first] << " Preamble Duration "
                    << ceil(m_preambleRep[i.first] * 5.6) << " RAR WINDOW "
                    << m_rarWindow[i.first] << endl;
                    DEBUG_LOG_END
                    DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS_NB)
                    cout << "SF " << currentSF << " NPRACH session of class "
                    << i.first << " planned at SF(s) ";
                    DEBUG_LOG_END
                    for (int sf=currentSF+1; sf < currentSF+(ceil(m_preambleRep[i.first]*5.6)); sf++)
                    {
                        DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS_NB)
                        cout << sf << " ";
                        DEBUG_LOG_END
                        
                        vector<int> tones;
                        for(int j=0; j<m_subcarriers[i.first]; j++) //#
                        {
                            tones.push_back(j);
                        }
                        m_tonesReservedForRach[i.first].insert(std::make_pair(sf, tones));
                        
                    }
                    DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS_NB)
                    cout << endl;
                    DEBUG_LOG_END
                    
                    std::map<int,vector<int>>::const_iterator it = m_tonesReservedForRach[i.first].find(currentSF+1);
                    if (it!=m_tonesReservedForRach[i.first].end() )
                        m_RAOindex[i.first]++;
                }
            }
            m_RachReservation = Simulator::Init()->Schedule( (((double) nextStartTime - (double) currentSF - 1 ) / 1000),
                                                            &GnbNbIoTRandomAccess::SetRachReservedSubChannels, this);
        }
    }
    
}

void GnbNbIoTRandomAccess::ReceiveMessage1(
                                           RandomAccessPreambleIdealControlMessage *msg) {
    
    int tTx = msg->GetTimeTx();
    
    preambleMessage ueAndPre;
    ueAndPre.ue = msg->GetSourceDevice();
    ueAndPre.preamble = msg->GetPreamble();
    UeMacEntity* ueMac = (UeMacEntity*) ueAndPre.ue->GetProtocolStack()->GetMacEntity();
    UeNbIoTRandomAccess* ueRam = (UeNbIoTRandomAccess*) ueMac->GetRandomAccessManager();
    int CEClass = ueRam->GetCEClassDynamic();
    
    DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS_NB)
    cout << "gNB ReceiveMessage1() at " << tTx << endl;
    DEBUG_LOG_END
    
    std::map<int, vector<preambleMessage>> rs;
    
    if (m_savedRachSessions.find(tTx) != m_savedRachSessions.end()) {
        rs = (*m_savedRachSessions.find(tTx)).second;
        m_savedRachSessions.erase(m_savedRachSessions.find(tTx));
    }
    rs[CEClass].push_back(ueAndPre);
    m_savedRachSessions.insert(std::make_pair(tTx, rs));
}

void GnbNbIoTRandomAccess::CheckCollisions() {
    
    int currentSF = FrameManager::Init()->GetTTICounter();
    
    DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS_NB)
    cout << "gNB CheckCollisions() at " << currentSF << endl;
    DEBUG_LOG_END
    
    std::map<int, std::map<int, vector<preambleMessage>>>::iterator it;
    it = m_savedRachSessions.begin();
    
    std::map<int, vector<int>> collidedId;
    collidedId.clear();
    if (it != m_savedRachSessions.end()) {
        int verifyTime = it->first;
        
        DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS)
        cout << "T " << Simulator::Init()->Now() << " SF " << currentSF
        << " - RANDOM_ACCESS RACH session found at SF " << verifyTime
        << endl;
        DEBUG_LOG_END
        
        std::map<int, vector<preambleMessage>> rachSession = it->second;
        
        UeMacEntity* ueMacK;
        UeMacEntity* ueMacJ;
        UeNbIoTRandomAccess* ueRamK;
        UeNbIoTRandomAccess* ueRamJ;
        int classK, classJ;
        
        for (auto h : rachSession) {
            for (auto k : h.second) {
                bool collision = false;
                for (auto j : h.second) {
                    ueMacK = (UeMacEntity*) k.ue->GetProtocolStack()->GetMacEntity();
                    ueRamK = (UeNbIoTRandomAccess*) ueMacK->GetRandomAccessManager();
                    classK = ueRamK->GetCEClassDynamic();
                    
                    ueMacJ = (UeMacEntity*) j.ue->GetProtocolStack()->GetMacEntity();
                    ueRamJ = (UeNbIoTRandomAccess*) ueMacJ->GetRandomAccessManager();
                    classJ = ueRamJ->GetCEClassDynamic();
                    
                    std::uniform_int_distribution<> bo (0, m_backoff[classK]);
                    extern std::mt19937 commonGen;
                    
                    if (k.preamble == j.preamble && k.ue != j.ue) {
                        collision = true;
                        if (find(collidedId[classK].begin(), collidedId[classK].end(),
                                 k.ue->GetIDNetworkNode()) == collidedId[classK].end()) {
                            collidedId[classK].push_back(k.ue->GetIDNetworkNode());
                            
                            DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS)
                            cout << "COLLISION UE " << k.ue->GetIDNetworkNode() << " CEC " << classK
                            << " UE " << j.ue->GetIDNetworkNode() << " CEC " << classJ <<" T "
                            << verifyTime << endl;
                            DEBUG_LOG_END
                            
                            int backoffTime;
                            int waitTime;
                            
                            backoffTime = bo(commonGen);
                            
                            if (m_preambleRep[classK]<64)
                            {
                                waitTime = (ceil(5.6*(m_preambleRep[classK]))) + m_rarWindow[classK]+ 4;
                                
                            }
                            else
                            {
                                waitTime = (ceil(5.6*(m_preambleRep[classK]))) + m_rarWindow[classK]+ 41;
                            }
                            Simulator::Init()->Schedule((double)((backoffTime+ waitTime-1)/1000.0), &UeNbIoTRandomAccess::ReStartRaProcedure, ueRamK);
                            
                            DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS_BACKOFF)
                            cout << "T " << Simulator::Init()->Now() << " UE " << k.ue->GetIDNetworkNode() << " CLASS " << classK << " WAITS FOR " <<
                            waitTime << " AND APPLIES A BACKOFF TIME OF " <<  backoffTime
                            << " [0, " << m_backoff[classK] << "]) " << endl;
                            DEBUG_LOG_END
                            
                        }
                    }
                }
                if (collision == false) {
                    rarElement e;
                    e.msg1Time = verifyTime;
                    e.msg3Time =(rand() % m_rarWindow[classK]) + ceil(5.6*(m_preambleRep[classK]));
                    e.msg3RB=k.preamble;
                    e.ue = k.ue;
                    m_rarQueue.push(e);
                }
            }
            if (h.second.size()>0)
            {
                m_savedRachSessions.erase(verifyTime);
                cout << "RACH INFO CLASS " << h.first << " SF " << verifyTime << " WIN "
                << h.second.size() - collidedId[h.first].size() << " COLLISIONS "
                << collidedId[h.first].size() << " TOT " << h.second.size() << endl;
            }
        }
    }
    
    if (!m_rarQueue.empty())
        SendResponses();
    
    vector<double> del;
    
    for (auto i : m_resPeriodicity)
        del.push_back((m_RAOindex[i.first])*i.second + m_timeOffset[i.first] - currentSF);
    double delay = *std::min_element(del.begin(), del.end());
    m_RarScheduling = Simulator::Init()->Schedule((delay+1.0)/1000,&GnbNbIoTRandomAccess::CheckCollisions, this);
    
}

void GnbNbIoTRandomAccess::SendResponses() {
    DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS_NB)
    cout << "GnbNbIoTRandomAccess::SendResponses() " << endl;
    DEBUG_LOG_END
    while (!m_rarQueue.empty()) {
        rarElement e = m_rarQueue.front();
        SendMessage2(e.ue, e.msg3Time, e.msg3RB);
        m_rarQueue.pop();
    }
}

void GnbNbIoTRandomAccess::SendMessage2(NetworkNode *dest, int msg3time,
                                        int msg3RB) {
    DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS_NB)
    cout << "GnbNbIoTRandomAccess::SendMessage2(" << dest->GetIDNetworkNode()
    << ") " << endl;
    DEBUG_LOG_END
    
    RandomAccessResponseIdealControlMessage* msg2 =
    new RandomAccessResponseIdealControlMessage();
    msg2->SetSourceDevice(m_macEntity->GetDevice());
    msg2->setMsg3Time(msg3time);
    msg2->setMsg3RB(msg3RB);
    msg2->SetDestinationDevice(dest);
    
    DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS)
    cout << "RANDOM_ACCESS SEND_MSG2 UE " << dest->GetIDNetworkNode() << " T "
    << Simulator::Init()->Now() << endl;
    DEBUG_LOG_END
    
    m_macEntity->GetDevice()->GetPhy()->SendIdealControlMessage(msg2);
}

void GnbNbIoTRandomAccess::ReceiveMessage3(
                                           RandomAccessConnectionRequestIdealControlMessage* msg3) {
    DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS_NB)
    cout << "GnbNbIoTRandomAccess::ReceiveMessage3() " << endl;
    DEBUG_LOG_END
    NetworkNode *src = msg3->GetSourceDevice();
    DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS)
    cout << "RANDOM_ACCESS RECEIVE_MSG3 UE " << src->GetIDNetworkNode() << " T "
    << Simulator::Init()->Now() << endl;
    DEBUG_LOG_END
    
    Simulator::Init()->Schedule(0.001, &GnbNbIoTRandomAccess::SendMessage4, this,
                                src);
}

void GnbNbIoTRandomAccess::SendMessage4(NetworkNode *dest) {
    DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS_NB)
    cout << "GnbNbIoTRandomAccess::SendMessage4(dest) " << endl;
    DEBUG_LOG_END
    DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS)
    cout << "RANDOM_ACCESS SEND_MSG4 UE " << dest->GetIDNetworkNode() << " T "
    << Simulator::Init()->Now() << endl;
    DEBUG_LOG_END
    
    GNodeB* gnb = (GNodeB*) m_macEntity->GetDevice();
    RandomAccessConnectionResolutionIdealControlMessage *msg4;
    msg4 = new RandomAccessConnectionResolutionIdealControlMessage();
    msg4->SetSourceDevice(gnb);
    msg4->SetDestinationDevice(dest);
    
    m_macEntity->GetDevice()->GetPhy()->SendIdealControlMessage(msg4);
}

bool GnbNbIoTRandomAccess::isRachOpportunity() {
    int currentTTI = FrameManager::Init()->GetTTICounter();
    
    DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS_NB)
    cout << "GnbNbIoTRandomAccess::isRachOpportunity( " << currentTTI <<" ) " << endl;
    DEBUG_LOG_END
    vector<bool> found;
    for (auto i : m_tonesReservedForRach)
    {
        if (m_tonesReservedForRach[i.first].find(currentTTI) != m_tonesReservedForRach[i.first].end())
            found.push_back(true);
        else
            found.push_back(false);
    }
    return std::find(found.begin(), found.end(), true) != found.end();
}

vector<int>
GnbNbIoTRandomAccess::GetRachReservedSubChannels() {
    DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS_NB)
    cout << "GnbNbIoTRandomAccess::GetRachReservedSubChannels() " << endl;
    DEBUG_LOG_END
    int currentTTI = FrameManager::Init()->GetTTICounter();
    vector<int> tones;
    for (auto i : m_tonesReservedForRach)
        tones.insert(tones.end(),
                     m_tonesReservedForRach[i.first][currentTTI].begin(),
                     m_tonesReservedForRach[i.first][currentTTI].end());
    return tones;
}

vector<int> GnbNbIoTRandomAccess::GetCcchReservedSubChannels() {
    DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS_NB)
    cout << "GnbNbIoTRandomAccess::GetCcchReservedSubChannels() " << endl;
    DEBUG_LOG_END
    int currentTTI = FrameManager::Init()->GetTTICounter();
    return m_RBsReservedForCCCH.getElements(currentTTI);
}

vector<int> GnbNbIoTRandomAccess::GetReservedSubChannels() {
    DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS_NB)
    cout << "GnbNbIoTRandomAccess::GetReservedSubChannels() " << endl;
    DEBUG_LOG_END
    int currentTTI = FrameManager::Init()->GetTTICounter();
    vector<int> RachRBs;
    for (auto i : m_tonesReservedForRach)
        RachRBs.insert(RachRBs.end(),
                       m_tonesReservedForRach[i.first][currentTTI].begin(),
                       m_tonesReservedForRach[i.first][currentTTI].end());
    vector<int> CcchRBs = m_RBsReservedForCCCH.getElements(currentTTI);
    vector<int> result;
    result.reserve(RachRBs.size() + CcchRBs.size());
    result.insert(result.end(), RachRBs.begin(), RachRBs.end());
    result.insert(result.end(), CcchRBs.begin(), CcchRBs.end());
    return result;
}

void GnbNbIoTRandomAccess::setNbRARs(int n) {
    DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS_NB)
    cout << "GnbNbIoTRandomAccess::setNbRARs(" << n << ") " << endl;
    DEBUG_LOG_END
    m_nbRARs = n;
}

int GnbNbIoTRandomAccess::getNbRARs(void) {
    DEBUG_LOG_START_1(SIM_ENV_TEST_RANDOM_ACCESS_NB)
    cout << "GnbNbIoTRandomAccess::getNbRARs() " << endl;
    DEBUG_LOG_END
    return m_nbRARs;
}

std::map<int, int> GnbNbIoTRandomAccess::GetMaxPreambleTx(void) {
    return m_maxPreambleTx;
}

std::map<int, int> GnbNbIoTRandomAccess::GetPreambleRep(void) {
    return m_preambleRep;
}

std::map<int, int> GnbNbIoTRandomAccess::GetRarWindow(void) {
    return m_rarWindow;
}

std::map<int, int> GnbNbIoTRandomAccess::GetSubcarriers(void) {
    return m_subcarriers;
}

std::map<int, int> GnbNbIoTRandomAccess::GetResPeriodicity(void) {
    return m_resPeriodicity;
}

std::map<int, int> GnbNbIoTRandomAccess::GetTimeOffset(void) {
    return m_timeOffset;
}

std::map<int, int> GnbNbIoTRandomAccess::GetRaoIndex(void) {
    return m_RAOindex;
}

std::map<int, int> GnbNbIoTRandomAccess::GetBackoff(void) {
    return m_backoff;
}

void GnbNbIoTRandomAccess::SetMaxPreambleTx(std::map<int, int> maxPreambleTx) {
    m_maxPreambleTx = maxPreambleTx;
}

void GnbNbIoTRandomAccess::SetPreambleRep(std::map<int, int> preambleRep) {
    m_preambleRep = preambleRep;
}

void GnbNbIoTRandomAccess::SetRarWindow(std::map<int, int> rarWindow) {
    m_rarWindow = rarWindow;
}

void GnbNbIoTRandomAccess::SetSubcarriers(std::map<int, int> subcarriers) {
    m_subcarriers = subcarriers;
}

void GnbNbIoTRandomAccess::SetResPeriodicity(std::map<int, int> periodicity) {
    m_resPeriodicity = periodicity;
}

void GnbNbIoTRandomAccess::SetTimeOffset(std::map<int, int> timeOffset) {
    m_timeOffset = timeOffset;
}

void GnbNbIoTRandomAccess::SetRaoIndex(std::map<int, int> RAOindex) {
    m_RAOindex = RAOindex;
}

void GnbNbIoTRandomAccess::SetBackoff(std::map<int, int> backoff) {
    m_backoff = backoff;
}
