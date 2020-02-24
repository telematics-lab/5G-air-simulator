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

#ifndef ENB_NB_IOT_RANDOM_ACCESS_H_
#define ENB_NB_IOT_RANDOM_ACCESS_H_

#include <list>
#include <vector>
#include "gnb-random-access.h"
#include "../mac-entity.h"
#include "../../../device/NetworkNode.h"
#include "../../../device/UserEquipment.h"
#include "../../../core/idealMessages/ideal-control-messages.h"
#include "allocation-map.h"
#include <map>
#include <algorithm>


class GnbNbIoTRandomAccess : public GnbRandomAccess
{
    
public:
    int m_periodicity; // TTIs
    
    
    enum PreambleFormat
    {
        Preamble_FORMAT_0,
        Preamble_FORMAT_1
    };
    
    GnbNbIoTRandomAccess();
    GnbNbIoTRandomAccess(MacEntity* mac);
    virtual ~GnbNbIoTRandomAccess();
    
    void SetGnbNbIoTRandomAccess();
    void GetGnbNbIoTRandomAccess();
    
    void SetRachReservedSubChannels();
    virtual vector<int> GetRachReservedSubChannels();
    virtual vector<int> GetCcchReservedSubChannels();
    virtual vector<int> GetReservedSubChannels();
    
    void setNbRARs(int n);
    int getNbRARs(void);
    
    
    virtual void ReceiveMessage1(RandomAccessPreambleIdealControlMessage *msg);
    virtual void ReceiveMessage3(RandomAccessConnectionRequestIdealControlMessage *msg);
    void CheckCollisions();
    void SendResponses();
    virtual void SendMessage2(NetworkNode *dest, int msg3time, int msg3RB);
    virtual void SendMessage4(NetworkNode *dest);
    virtual bool isRachOpportunity();
    
    map<int, int> GetMaxPreambleTx();
    std::map<int, int> GetPreambleRep();
    std::map<int, int> GetRarWindow();
    std::map<int, int> GetSubcarriers();
    std::map<int, int> GetResPeriodicity();
    std::map<int, int> GetTimeOffset();
    std::map<int, int> GetRaoIndex();
    std::map<int, int> GetBackoff();
    
    void SetMaxPreambleTx(std::map<int, int>);
    void SetPreambleRep(std::map<int, int>);
    void SetRarWindow(std::map<int, int>);
    void SetSubcarriers(std::map<int, int>);
    void SetResPeriodicity(std::map<int, int>);
    void SetTimeOffset(std::map<int, int>);
    void SetRaoIndex(std::map<int, int>);
    void SetBackoff(std::map<int, int>);
    
    struct preambleMessage
    {
        NetworkNode* ue;
        int preamble;
    };
    
    
    struct rarElement
    {
        int msg1Time;
        int msg3Time;
        int msg3RB;
        NetworkNode* ue;
    };
    
    
    
    //typedef std::map<int, vector<preambleMessage>>  rachSession;
    
    
    
private:
    PreambleFormat m_PreambleFormat;
    int m_lastRachOpportunity; // TTIs
    int m_PreambleDuration; // TTIs
    int m_responseWindowDelay; // TTIs
    int m_responseWindowDuration; // TTIs
    std::map<int, std::map<int, vector<int>>> m_tonesReservedForRach;
    std::map<int, std::map<int, vector<preambleMessage>>> m_savedRachSessions;
    float m_maxCCCHUsage;
    AllocationMap m_RBsReservedForCCCH;
    queue <rarElement> m_rarQueue;
    int m_nbRARs;
    
    shared_ptr<Event> m_RachReservation;
    shared_ptr<Event> m_RarScheduling;
    
    std::map<int, int> m_maxPreambleTx;
    std::map<int, int> m_preambleRep;
    std::map<int, int> m_rarWindow;
    std::map<int, int> m_subcarriers;
    std::map<int, int> m_resPeriodicity;
    std::map<int, int> m_timeOffset;
    std::map<int, int> m_RAOindex;
    std::map<int, int> m_backoff;
    
};

#endif /* ENB_NB_IOT_RANDOM_ACCESS_H_ */
