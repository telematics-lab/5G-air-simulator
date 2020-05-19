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

#include "priority-uplink-packet-scheduler.h"
#include "../mac-entity.h"
#include "../gnb-mac-entity.h"
#include "../../packet/Packet.h"
#include "../../packet/packet-burst.h"
#include "../../../device/NetworkNode.h"
#include "../../../flows/radio-bearer.h"
#include "../../../protocolStack/rrc/rrc-entity.h"
#include "../../../flows/application/Application.h"
#include "../../../device/GNodeB.h"
#include "../../../protocolStack/mac/AMCModule.h"
#include "../../../phy/phy.h"
#include "../../../core/spectrum/bandwidth-manager.h"
#include "../../../core/idealMessages/ideal-control-messages.h"
#include "../../../flows/QoS/QoSParameters.h"
#include "../../../flows/MacQueue.h"

#include "../../../device/UserEquipment.h"
#include "../../../componentManagers/NetworkManager.h"
#include "../../../phy/gnb-phy.h"
#include "../../../flows/MacQueue.h"
#include "../../../utility/eesm-effective-sinr.h"
#include "../../../utility/miesm-effective-sinr.h"
#include "../../../phy/precoding-calculator.h"

#include <armadillo>

using namespace std;

PriorityUplinkPacketScheduler::PriorityUplinkPacketScheduler(GnbMacEntity* mac)
{
  SetMacEntity (mac);
  CreateUsersToSchedule ();
}

PriorityUplinkPacketScheduler::~PriorityUplinkPacketScheduler()
{
  Destroy ();
}

double myfunction(double x, double gamma)
{
    double g;
    if(x>=0 && x<=1)
        g = gamma*abs(log(x));
    else
        g = 0;
    return g;
}

double
PriorityUplinkPacketScheduler::ComputeSchedulingMetric (UserToSchedule* user, int subchannel)
{
    UsersToSchedule *users = GetUsersToSchedule ();
    double metric=1000000;
    if((m_index2) % (users->size()) != 0)
        metric = 100000;
    return metric;
}

void
PriorityUplinkPacketScheduler::SelectUsersToSchedule () {
    CreateUsersToSchedule ();
    
    GNodeB *node = GetMacEntity ()->GetDevice ();
    GnbPhy* nodePhy = (GnbPhy*) node->GetPhy();
    int txMode = node->GetMacEntity()->GetDefaultUlTxMode();
    DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
    cout << "NRUplinkPacketScheduler::SelectUsersToSchedule () "
    " users " << node->GetUserEquipmentRecords ()->size ()<< endl;
    
    cout << "LIST: "<< endl;
    DEBUG_LOG_END
    AMCModule* amc = GetMacEntity()->GetAmcModule();
    
    for (auto record : *node->GetUserEquipmentRecords()) {
        RrcEntity* rrc = record->m_UE->GetProtocolStack ()->GetRrcEntity ();
        
        if (record->GetSchedulingRequest () > 0) {
            UserToSchedule* user = new UserToSchedule ();
            vector< shared_ptr<arma::cx_fmat> > fullCsiFeedbacks = record->GetUplinkChannelMatricesIndicator();
            user->m_userToSchedule = record->GetUE ();
            record->GetUE ()->GetMacEntity()->SendSchedulingRequest();
            user->m_dataToTransmit = record->GetSchedulingRequest ();
            user->m_listOfAllocatedRBs.clear ();
            user->m_selectedMCS = 0;
            user->m_transmittedData = 0;
            user->m_channelCondition = amc->CreateCqiFeedbacks(record->GetUplinkChannelStatusIndicator());
            user->m_averageSchedulingGrant = record->GetSchedulingGrants ();
            user->m_fullCsiFeedbacks = fullCsiFeedbacks;
            user->m_prioritizedDataToTransmit = 0;
            user->m_listOfSelectedMCS.clear();
            user->m_assignedLayers.clear();
            DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
            cout << "\t UE "
            << user->m_userToSchedule->GetIDNetworkNode()
            << " - DATA "
            << user->m_dataToTransmit
            << endl;
            DEBUG_LOG_END
            
            GetUsersToSchedule ()->push_back (user);
        }
        
    }
    DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
    cout << "users to be scheduled = " << GetUsersToSchedule ()->size () << endl;
    DEBUG_LOG_END
    
}

void
PriorityUplinkPacketScheduler::DoSchedule (void) {
    DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
    cout << "Start UPLINK packet scheduler for node "
    << GetMacEntity ()->GetDevice ()->GetIDNetworkNode()<< endl;
    DEBUG_LOG_END
    
    int ulTxMode = GetMacEntity()->GetDefaultUlTxMode();
    SelectUsersToSchedule ();
    
    if (GetUsersToSchedule ()->size () > 0)
        RBsAllocation();
    
    DeleteUsersToSchedule ();
}

void
PriorityUplinkPacketScheduler::RBsAllocation ()
{
    DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
    cout << " ---- UL RBs Allocation";
    DEBUG_LOG_END
    
    GnbRandomAccess* randomAccessManager = GetMacEntity ()->GetRandomAccessManager ();
    vector<int> reservedRBs = randomAccessManager->GetReservedSubChannels ();
    
    UsersToSchedule* users = GetUsersToSchedule ();
    int nbOfRBs = (int) GetMacEntity ()->GetDevice ()->GetPhy ()->GetBandwidthManager ()->GetUlSubChannels ().size ();
    vector<int> prioritizedUsersIndices;
    double margin; //for scheduling with priority
    
    int ulSchedType = users->at(0)->m_userToSchedule->GetTargetNode()->m_ulSchedType;
    
    if(ulSchedType != 5)
        margin = 1;
    
    //create a matrix of flow metrics
    vector< vector<double> > metrics;
    metrics.resize(users->size (), vector<double> (nbOfRBs));
    
    
    for (int i = 0; i < (int)users->size (); i++) {
        RrcEntity *rrc = users->at(i)->m_userToSchedule->GetProtocolStack ()->GetRrcEntity (); //for priority scheduling
        
        switch( ulSchedType ) {
            case 5:
                for(auto bearer : *rrc->GetRadioBearerContainer()) {
                    if(!(bearer->GetMacQueue()->IsEmpty()) &&
                       bearer->GetApplication()->GetApplicationType() == Application::APPLICATION_TYPE_CBR)
                        users->at(i)->m_prioritizedDataToTransmit += bearer->GetQueueSize ();
                }
                
                for (int j = 0; j < nbOfRBs; j++) {
                    int channelCondition = users->at(i)->m_channelCondition.at (j);
                    AMCModule* amc = users->at(i)->m_userToSchedule->GetProtocolStack()->GetMacEntity()->GetAmcModule();
                    metrics.at(i).at(j) = amc->GetEfficiencyFromCQI(channelCondition) * 180000.;
                }
                break;
                
            default:
                for (int j = 0; j < nbOfRBs; j++)
                    metrics.at(i).at(j) = ComputeSchedulingMetric (users->at (i), j);
                break;
        }
    }
    
    for(int i = 0; i < (int)users->size (); i++)
        if(users->at(i)->m_prioritizedDataToTransmit>0)
            prioritizedUsersIndices.push_back(i);
    
    double l_dAllocatedRBCounter = 0;
    int availableRBs = 0;
    bool* l_bUserScheduled = new bool[users->size()];
    int l_iScheduledUsers = 0;
    
    for(int k=0;k<(int)users->size(); k++)
        l_bUserScheduled[k]=false;
    
    int ulTxMode = GetMacEntity()->GetDefaultUlTxMode();
    DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
    cout << ", available RBs " << nbOfRBs << ", users " << users->size () << endl;
    for (int ii = 0; ii < (int)users->size (); ii++) {
        cout << "\t metrics for user "
        << users->at (ii)->m_userToSchedule->GetIDNetworkNode ();
        for (int jj = 0; jj < nbOfRBs; jj++) {
            cout << " " << metrics[ii][jj];
        }
        cout << endl;
    }
    DEBUG_LOG_END
    
    //initializing precoding matrix if uplink mimo is used
    vector< shared_ptr<arma::cx_fmat> > precodingMatrices;
    //vector< shared_ptr<arma::cx_fmat> > precodingMatrices_2;
    vector< shared_ptr<arma::cx_fmat> > channelMatrices_2;
    if (ulTxMode == 11){
        precodingMatrices.resize(nbOfRBs);
        for (int rb = 0; rb < nbOfRBs; rb++)
            precodingMatrices.at(rb) = nullptr;
    }
    vector<int> allocationsPerRB(nbOfRBs);
    fill(allocationsPerRB.begin(), allocationsPerRB.end(), 0);
    
    //RBs allocation
    int nbOfPrioritizedUsers;
    int rb = 0;
    vector<int> rbsIndexes;
    vector<bool>* isScheduled = new vector<bool>(users->size ());
    vector<double> * l_bUserScheduledSINR = new vector<double>[users->size ()];
    fill(isScheduled->begin(), isScheduled->end(), false);
    
    //collecting cqi of each user in a matrix
    int nbOfUsers = (int) users->size();
    vector<vector<int>> cqis;
    if(ulSchedType == 5){
        
        for(int i=0;i<(int)prioritizedUsersIndices.size(); i++) {
            cqis.push_back(users->at(prioritizedUsersIndices.at(i))->m_channelCondition);
        }
        nbOfPrioritizedUsers = (int) prioritizedUsersIndices.size();
        //find indexes of the best channels
        if(cqis.size()>0){
            int index = 0;
            int k=0;
            int userIndex;
            if(nbOfPrioritizedUsers>0)
                userIndex = prioritizedUsersIndices.at(0);
            int max, p;
            while(index < 100) {
                max = cqis.at(k).at(0);
                p = 0;
                for (int j=0; j<100; j++) {
                    if(cqis.at(k).at(j)>max) {
                        max=cqis.at(k).at(j);
                        p=j;
                    }
                }
                rbsIndexes.push_back(p);
                metrics.at(userIndex).at(p) = 100000000;
                for(int j=0;j<(int)prioritizedUsersIndices.size();j++) {
                    cqis.at(j).at(p) = -1;
                }
                if(k<(int)prioritizedUsersIndices.size()-1)
                    k++;
                else
                    k=0;
                userIndex = prioritizedUsersIndices.at(k);
                index++;
            }
        }}
    
    for(int rb1=0;rb1<nbOfRBs;rb1++) {
        if(ulSchedType == 5 && rbsIndexes.size()>0)
            rb = rbsIndexes.at(rb1);
        else
            rb=rb1;
        
        if(l_iScheduledUsers == (int) users->size())
            break;
        
        bool RBIsAllocated = false;
        if(allocationsPerRB.at(rb)>0)
            RBIsAllocated = true;
        
        vector<UserToSchedule*> scheduledUsers;
        vector<int> l_iScheduledUsersIndices;
        double targetMetric = 0;
        int l_iScheduledUserIndex=-1;
        for(int i=allocationsPerRB.at(rb); i<1;i++) {
            UserToSchedule* scheduledUser;
            
            //int scheduledUserIndex;
            for (int k = 0; k < (int)users->size (); k++) {
                //double averageMetric = (double) accumulate( next(metrics.at(k).begin(), rb), metrics.at(k).end(), 0.0) / availableRBs;
                if (metrics.at(k).at(rb) > targetMetric
                    && metrics.at(k).at(rb) > -INFINITY
                    && !l_bUserScheduled[k]
                    && find( scheduledUsers.begin(), scheduledUsers.end(), users->at(k) ) == scheduledUsers.end()
                    && users->at(k)->m_dataToTransmit > 0) {
                    
                    targetMetric = metrics.at(k).at(rb);
                    RBIsAllocated = true;
                    scheduledUser = users->at(k);
                    l_iScheduledUserIndex=k;
                }
            }
            if(targetMetric > 0) {
                allocationsPerRB.at(rb)++;
                scheduledUsers.push_back(scheduledUser);
                l_iScheduledUsersIndices.push_back(l_iScheduledUserIndex);
            }
        }
        vector<int> assignedLayers;
        
        if (RBIsAllocated) {
            l_dAllocatedRBCounter++;
            if(ulTxMode == 11) {
                //create channel matrices
                if (precodingMatrices.at(rb) == nullptr) {
                    vector< shared_ptr<arma::cx_fmat> > channelMatrices;
                    vector<double> widebandSinrs;
                    vector<int> ranks_new;
                    int layerCounter = 0;
                    for(int k = 0; k < (int) scheduledUsers.size(); k++) {
                        
                        UserToSchedule* user = scheduledUsers.at(k);
                        
                        if(user->m_fullCsiFeedbacks.size()>0){
                            channelMatrices.push_back(user->m_fullCsiFeedbacks.at(rb));
                            for (int l=0; l<1/*(int)user->m_userToSchedule->GetTargetNodeRecord()->GetRI()*/; l++) {
                                
                                assignedLayers.push_back (layerCounter);
                                widebandSinrs.push_back(user->m_widebSinr);
                                layerCounter++;
                            }
                            user->m_assignedLayers = assignedLayers;
                            layerCounter = 0;
                        }
                        ranks_new.push_back(1);//for  uplink mimo
                    }
                    
                    //precodingMatrices_2=precodingMatrices;
                    precodingMatrices.at(rb) = PrecodingCalculator::RegularizedZeroForcing(
                                                                                           channelMatrices,
                                                                                           ranks_new,
                                                                                           widebandSinrs,
                                                                                           pow(10.,(GetMacEntity()->GetDevice()->GetPhy()->GetTxPower()-30)/10)/(double)nbOfRBs
                                                                                           );
                    channelMatrices.clear();
                    widebandSinrs.clear();
                    ranks_new.clear();
                }
                int index=0;
                for (int i = 0; i < (int) scheduledUsers.size(); i++)
                {
                    if(scheduledUsers.at(i)->m_listOfSelectedMCS.size() > 0)
                        scheduledUsers.at(i)->m_listOfSelectedMCS.clear();
                    scheduledUsers.at(i)->m_listOfAllocatedRBs.push_back(rb);
                    int mcs;
                    AMCModule* amc = scheduledUsers.at(i)->m_userToSchedule->GetProtocolStack()->GetMacEntity()->GetAmcModule();
                    channelMatrices_2.push_back(scheduledUsers.at(i)->m_fullCsiFeedbacks.at(rb));
                    //this flow has been scheduled
                    double sinr;
                    sinr = amc->GetSinrFromCQI (scheduledUsers.at(i)->m_channelCondition.at (rb));
                    l_bUserScheduledSINR[l_iScheduledUsersIndices.at(i)].push_back(sinr);
                    mcs = amc->GetMCSFromSinrVector(l_bUserScheduledSINR[l_iScheduledUsersIndices.at(i)], margin);
                    /*if(rb1 == nbOfRBs - 1)
                     {
                     vector<double> newSinr = PrecodingCalculator::adjustSinrForMuMimo(
                     l_bUserScheduledSINR[l_iScheduledUsersIndices.at(i)],
                     channelMatrices_2,
                     precodingMatrices, scheduledUsers.at(i)->m_assignedLayers);
                     //channelMatrices_2.clear();
                     mcs = amc->GetMCSFromSinrVector(newSinr, margin);*/
                    //}
                    int tbs = (amc->GetTBSizeFromMCS (mcs, mcs, scheduledUsers.at(i)->m_listOfAllocatedRBs.size(), 1)) / 8;
                    int dataToTransmit = scheduledUsers.at(i)->m_dataToTransmit;
                    int prioritizedDataToTransmit = scheduledUsers.at(i)->m_prioritizedDataToTransmit;
                    
                    switch(ulSchedType)
                    {
                        case 5:
                            if(prioritizedDataToTransmit > 0)
                            {
                                if(tbs >= prioritizedDataToTransmit
                                   && tbs >= dataToTransmit)
                                {
                                    l_bUserScheduled[l_iScheduledUsersIndices.at(i)] = true;
                                    l_iScheduledUsers++;
                                    
                                    int j=rb1+nbOfPrioritizedUsers, leng = (int)rbsIndexes.size(), first;
                                    
                                    if(j<leng){
                                        first = rbsIndexes.at(j);
                                        do
                                        {
                                            rbsIndexes.push_back(rbsIndexes.at(j));
                                            rbsIndexes.at(j)=rbsIndexes.at(j+1);
                                            rbsIndexes.erase(rbsIndexes.begin()+j+1);
                                            
                                            if(nbOfPrioritizedUsers>1)
                                                j=j+nbOfPrioritizedUsers-1;
                                            else
                                                j++;
                                        }while(j<leng && rbsIndexes.at(j) != first);
                                    }
                                    
                                    nbOfPrioritizedUsers--;
                                }
                                else if(tbs >= prioritizedDataToTransmit
                                        && tbs<dataToTransmit)
                                {
                                    
                                    for(int m = rb1 + nbOfPrioritizedUsers; m<nbOfRBs; m=m+nbOfPrioritizedUsers)
                                    {
                                        int channelCondition = scheduledUsers.at(i)->m_channelCondition.at (m);
                                        AMCModule* amc = scheduledUsers.at(i)->m_userToSchedule->GetProtocolStack()->GetMacEntity()->GetAmcModule();
                                        metrics.at(i).at(rbsIndexes.at(m)) = amc->GetEfficiencyFromCQI(channelCondition) * 180000.;
                                    }
                                    
                                    int j=rb1+nbOfPrioritizedUsers, leng = (int)rbsIndexes.size(), first;
                                    
                                    if(j<leng){
                                        first = rbsIndexes.at(j);
                                        do
                                        {
                                            rbsIndexes.push_back(rbsIndexes.at(j));
                                            rbsIndexes.at(j)=rbsIndexes.at(j+1);
                                            rbsIndexes.erase(rbsIndexes.begin()+j+1);
                                            
                                            if(nbOfPrioritizedUsers>1)
                                                j=j+nbOfPrioritizedUsers-1;
                                            else
                                                j++;
                                        }while(j<leng && rbsIndexes.at(j) != first);
                                    }
                                    
                                }
                                
                            }
                            
                            else
                            {
                                if(tbs >= dataToTransmit)
                                {
                                    l_bUserScheduled[l_iScheduledUsersIndices.at(i)] = true;
                                    l_iScheduledUsers++;
                                }
                            }
                            
                            break;
                        default:
                            if(tbs >= dataToTransmit)
                            {
                                l_bUserScheduled[l_iScheduledUsersIndices.at(i)] = true;
                                l_iScheduledUsers++;
                            }
                            break;
                    }
                    
                }
            }
        }
        
    }
    delete[] l_bUserScheduled;
    delete [] l_bUserScheduledSINR;
    channelMatrices_2.clear();
    
    //Finalize the allocation
    PdcchMapIdealControlMessage *pdcchMsg = new PdcchMapIdealControlMessage ();
    
    for (auto user : *users)
    {
        if(user->m_listOfSelectedMCS.size() > 0)
            user->m_listOfSelectedMCS.clear();
        int mcs;
        AMCModule* amc = user->m_userToSchedule->GetProtocolStack()->GetMacEntity()->GetAmcModule();
        
        if(ulTxMode == 11)
        {
            for(int k=0;k<nbOfRBs;k++)
            {
                channelMatrices_2.push_back(user->m_fullCsiFeedbacks.at(k));
            }
        }
        
        if (user->m_listOfAllocatedRBs.size()>0)
        {
            vector<int> pmi;
            //this flow has been scheduled
            vector<double> estimatedSinrValues;
            for (auto rb : user->m_listOfAllocatedRBs)
            {
                double sinr;
                sinr = amc->GetSinrFromCQI (user->m_channelCondition.at (rb));
                estimatedSinrValues.push_back (sinr);
            }
            
            if(ulTxMode!=11)
            {
                mcs = amc->GetMCSFromSinrVector(estimatedSinrValues);
            }
            else
            {
                mcs = amc->GetMCSFromSinrVector(estimatedSinrValues, margin);
                /*vector<double> newSinr = PrecodingCalculator::adjustSinrForMuMimo(
                 estimatedSinrValues,
                 channelMatrices_2,
                 precodingMatrices, user->m_assignedLayers);
                 
                 mcs = amc->GetMCSFromSinrVector(newSinr, margin);
                 newSinr.clear();*/
                //mcs = GetMacEntity()->GetAmcModule()->GetMCSFromSinrVector(newSinr);
            }
            pmi.resize(0);
            AMCModule* amc = user->m_userToSchedule->GetProtocolStack()->GetMacEntity()->GetAmcModule();
            int tbs = (amc->GetTBSizeFromMCS (mcs, mcs, (int)user->m_listOfAllocatedRBs.size(), 1)) / 8;
            user->m_transmittedData = tbs;
            int dataToTransmit = user->m_dataToTransmit;
            for (int rb = 0; rb < (int)user->m_listOfAllocatedRBs.size(); rb++ )
            {
                user->m_listOfSelectedMCS.push_back(mcs);
            }
            
            for (int rb = 0; rb < (int)user->m_listOfAllocatedRBs.size (); rb++ )
            {
                shared_ptr<arma::cx_fmat> precodingMatrix;
                if(ulTxMode == 11)
                    precodingMatrix = precodingMatrices.at(user->m_listOfAllocatedRBs.at (rb));
                vector<int> assignedLayers = user->m_assignedLayers;
                
                if(ulTxMode != 11)
                {
                    pdcchMsg->AddNewRecord (PdcchMapIdealControlMessage::UPLINK,
                                            user->m_listOfAllocatedRBs.at (rb),
                                            (NetworkNode*)user->m_userToSchedule,
                                            user->m_listOfSelectedMCS.at (rb),
                                            0, 1,
                                            pmi,
                                            assignedLayers);
                }
                else
                    pdcchMsg->AddNewRecord (PdcchMapIdealControlMessage::UPLINK,
                                            user->m_listOfAllocatedRBs.at (rb),
                                            (NetworkNode*)user->m_userToSchedule,
                                            user->m_listOfSelectedMCS.at (rb),
                                            0, 1,
                                            precodingMatrix,
                                            assignedLayers);
                assignedLayers.clear();
                pmi.clear();
                estimatedSinrValues.clear();
            }
            
            GNodeB::UserEquipmentRecord* record = user->m_userToSchedule->GetTargetNodeRecord();
            record->m_schedulingRequest -= user->m_transmittedData;
            if (record->m_schedulingRequest < 0 )
                record->m_schedulingRequest = 0;
        }
        
    }
    
    if (pdcchMsg->GetMessage()->size () > 0)
        GetMacEntity ()->GetDevice ()->GetPhy ()->SendIdealControlMessage (pdcchMsg);
    
    //users->clear();
    delete isScheduled;
    reservedRBs.clear();
    precodingMatrices.clear();
    channelMatrices_2.clear();
}
