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
 *         Lukasz Rajewski <lukasz.rajewski@gmail.com> (optimized PRB allocation)
 *         Sergio Martiradonna <sergio.martiradonna@poliba.it>
 */

#include <algorithm>
#include <memory>
#include <armadillo>

#include "downlink-packet-scheduler.h"
#include "../mac-entity.h"
#include "../harq-manager.h"
#include "../../packet/Packet.h"
#include "../../packet/packet-burst.h"
#include "../../../device/NetworkNode.h"
#include "../../../device/MulticastDestination.h"
#include "../../../flows/radio-bearer.h"
#include "../../../protocolStack/rrc/rrc-entity.h"
#include "../../../flows/application/Application.h"
#include "../../../flows/application/ExternalSource.h"
#include "../../../flows/application/TraceBased.h"
#include "../../../flows/QoS/QoSParameters.h"
#include "../../../device/GNodeB.h"
#include "../../../device/UserEquipment.h"
#include "../../../protocolStack/mac/AMCModule.h"
#include "../../../phy/phy.h"
#include "../../../phy/gnb-phy.h"
#include "../../../phy/precoding-calculator.h"
#include "../../../phy/sinr-calculator.h"
#include "../../../core/spectrum/bandwidth-manager.h"
#include "../../../flows/MacQueue.h"
#include "../../../utility/eesm-effective-sinr.h"
#include "../../../utility/miesm-effective-sinr.h"
#include "../../../componentManagers/FrameManager.h"

DownlinkPacketScheduler::DownlinkPacketScheduler()
{
    m_maxUsersPerRB = 1;
}

DownlinkPacketScheduler::~DownlinkPacketScheduler()
{
    Destroy ();
}

void
DownlinkPacketScheduler::SetMaxUsersPerRB (int users)
{
    m_maxUsersPerRB = users;
}

int
DownlinkPacketScheduler::GetMaxUsersPerRB (void)
{
    return m_maxUsersPerRB;
}


void DownlinkPacketScheduler::SelectFlowsToSchedule ()
{
    
    DEBUG_LOG_START_2(SIM_ENV_SCHEDULER_DEBUG, SIM_ENV_SCHEDULER_DEBUG_ARQ)
    cout << "\t Select Flows to schedule" << endl;
    DEBUG_LOG_END
    
    RrcEntity *rrc = GetMacEntity ()->GetDevice ()->GetProtocolStack ()->GetRrcEntity ();
    for (auto bearer : *rrc->GetRadioBearerContainer ())
    {
        //SELECT FLOWS TO SCHEDULE
        if (bearer->HasPackets ()
            &&
            bearer->GetDestination ()->GetNodeState () == NetworkNode::STATE_ACTIVE
            &&
            (
             FrameManager::Init()->MbsfnEnabled()==false
             ||
             (
              FrameManager::Init()->MbsfnEnabled()==true
              &&
              FrameManager::Init()->isMbsfnSubframe()==true
              &&
              bearer->GetDestination()->GetNodeType()==NetworkNode::TYPE_MULTICAST_DESTINATION
              )
             ||
             (
              FrameManager::Init()->MbsfnEnabled()==true
              &&
              FrameManager::Init()->isMbsfnSubframe()==false
              &&
              bearer->GetDestination()->GetNodeType()!=NetworkNode::TYPE_MULTICAST_DESTINATION
              )
             )
            )
        {
            //compute data to transmit
            int dataToTransmit;
            if (bearer->GetApplication ()->GetApplicationType () == Application::APPLICATION_TYPE_INFINITE_BUFFER)
            {
                dataToTransmit = 100000000;
            }
            else
            {
                dataToTransmit = bearer->GetQueueSize ();
            }
            
            DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG_ARQ)
            cout << "\t --- UE " << bearer->GetSource()->GetIDNetworkNode() << " Queue " << dataToTransmit << endl;
            DEBUG_LOG_END
            
            //compute spectral efficiency
            GNodeB *gnb = GetMacEntity ()->GetDevice ();
            UserEquipment *ue = (UserEquipment*) bearer->GetDestination ();
            GNodeB::UserEquipmentRecord *ueRecord = ue->GetTargetNodeRecord ();
            if (ueRecord->CqiAvailable()==false)
            {
                continue;
            }
            AMCModule* amc = ueRecord->GetUE()->GetProtocolStack()->GetMacEntity()->GetAmcModule();
            vector<double> spectralEfficiency;
            vector<int> cqiFeedbacks = ueRecord->GetCQI ();
            int riFeedback = ueRecord->GetRI ();
            vector< vector<int> > pmiFeedbacks = ueRecord->GetPMI ();
            vector< shared_ptr<arma::cx_fmat> > fullCsiFeedbacks = ueRecord->GetChannelMatrix ();
            int txMode = ueRecord->GetDlTxMode();
            if (txMode == 11 && fullCsiFeedbacks.size() == 0)
            {
                continue;
            }
            int numberOfCqi = (int) cqiFeedbacks.size ();
            for (int i = 0; i < numberOfCqi; i++)
            {
                double sEff;
                if (txMode == 11&&false)
                {
                    vector<int> allLayers;
                    vector<double> widebandSinrs;
                    for (int n = 0; n < (int)fullCsiFeedbacks.at(i)->n_rows; n++)
                    {
                        allLayers.push_back(n);
                        widebandSinrs.push_back(ueRecord->GetCQI().at(0));
                    }
                    vector<int> ranks = {riFeedback};
                    vector< shared_ptr<arma::cx_fmat> > channelMatrix1 = {fullCsiFeedbacks.at(i)};
                    int nbOfRBs = (int) GetMacEntity ()->GetDevice ()->GetPhy ()->GetBandwidthManager ()->GetDlSubChannels ().size ();
                    shared_ptr<arma::cx_fmat> local_precoding = PrecodingCalculator::RegularizedZeroForcing(channelMatrix1,
                                                                                                            ranks,
                                                                                                            widebandSinrs,
                                                                                                            pow(10.,(GetMacEntity()->GetDevice()->GetPhy()->GetTxPower()-30)/10)/nbOfRBs
                                                                                                            );
                    double avgPower = arma::mean( (arma::fvec)arma::sum(arma::square(arma::abs(*fullCsiFeedbacks.at(i))),1) );
                    double noise_interference = 10*log10(avgPower) - 2 * amc->GetSinrFromCQI(cqiFeedbacks.at (i));
                    DEBUG_LOG_START_1(SIM_ENV_SINR_CALCULATOR_DEBUG)
                    cout << "SINR_TEST2 ue "
                    << ueRecord->GetUE()->GetIDNetworkNode()
                    << " sinr " << amc->GetSinrFromCQI(cqiFeedbacks.at (i))
                    << " t " << Simulator::Init()->Now()
                    << endl;
                    DEBUG_LOG_END
                    vector<double> sinrs_cqi = SinrCalculator::MimoReception(
                                                                             *fullCsiFeedbacks.at(i),
                                                                             *local_precoding,
                                                                             noise_interference,
                                                                             allLayers);
                    sEff = log2(1+pow(10.,sinrs_cqi.at(0)/10));
                    //delete local_precoding;
                }
                else
                {
                    sEff = amc->GetEfficiencyFromCQI (cqiFeedbacks.at (i));
                }
                spectralEfficiency.push_back (sEff);
            }
            if (txMode == 11)
            {
                double sum_se = 0;
                int count_se = 0;
                for (auto i : spectralEfficiency)
                {
                    sum_se += i;
                    count_se ++;
                }
                // cout << " SEFF ue " << ueRecord->GetUE()->GetIDNetworkNode()
                // << " eff " << sum_se/count_se
                // << " time " << Simulator::Init()->Now()
                // << endl;
                fill(spectralEfficiency.begin(),spectralEfficiency.end(),sum_se/count_se);
            }
            
            // user sub-selection for cover shifts
            if (std::getenv("USE_COVERSHIFT") != nullptr)
            {
                UserEquipment* ue = ueRecord->GetUE();
                
                CartesianCoordinates* src_position = gnb->GetMobilityModel ()->GetAbsolutePosition ();
                CartesianCoordinates* dst_position = ue->GetMobilityModel ()->GetAbsolutePosition ();
                double array_bearing = gnb->GetPhy ()->GetAntennaParameters ()->GetBearing()*M_PI/180.0;
                
                // UE horizontal angle compared to antenna boresight
                double dst_angle_h = dst_position->GetPolarAzimut(src_position) - array_bearing;
                if ( dst_angle_h > M_PI )
                {
                    dst_angle_h -= (2*M_PI);
                }
                
                double grid_angle_h_min = - M_PI / 3;
                double grid_angle_h_max = + M_PI / 3;
                
                double sector_width = grid_angle_h_max - grid_angle_h_min;
                
                GnbPhy* src = (GnbPhy*)(gnb->GetPhy());
                int index = FrameManager::Init()->GetCoverShiftIndex();
                
                // keep parameters synchronized to channel-realization.cpp
                if(src->GetAntennaParameters()->GetBearing()==0)
                {
                    if(index==0)
                    {
                        grid_angle_h_min = grid_angle_h_min + sector_width * 0.32;
                        grid_angle_h_max = grid_angle_h_max - sector_width * 0.32;
                    }
                    else if(index==1)
                    {
                        grid_angle_h_min = grid_angle_h_min + sector_width * 0.65;
                    }
                    else if(index==2)
                    {
                        grid_angle_h_max = grid_angle_h_max - sector_width * 0.65;
                    }
                }
                else if(src->GetAntennaParameters()->GetBearing()==120)
                {
                    if(index==0)
                    {
                        grid_angle_h_max = grid_angle_h_max - sector_width * 0.65;
                    }
                    else if(index==1)
                    {
                        grid_angle_h_min = grid_angle_h_min + sector_width * 0.32;
                        grid_angle_h_max = grid_angle_h_max - sector_width * 0.32;
                    }
                    else if(index==2)
                    {
                        grid_angle_h_min = grid_angle_h_min + sector_width * 0.65;
                    }
                }
                else if(src->GetAntennaParameters()->GetBearing()==240)
                {
                    if(index==0)
                    {
                        grid_angle_h_min = grid_angle_h_min + sector_width * 0.65;
                    }
                    else if(index==1)
                    {
                        grid_angle_h_max = grid_angle_h_max - sector_width * 0.65;
                    }
                    else if(index==2)
                    {
                        grid_angle_h_min = grid_angle_h_min + sector_width * 0.32;
                        grid_angle_h_max = grid_angle_h_max - sector_width * 0.32;
                    }
                }
                if (dst_angle_h<grid_angle_h_min || dst_angle_h>grid_angle_h_max)
                {
                    continue;
                }
            }
            
            //create flow to schedule record
            InsertFlowToSchedule(bearer, dataToTransmit, spectralEfficiency, cqiFeedbacks, riFeedback, pmiFeedbacks, fullCsiFeedbacks);
        }
        else
        {}
    }
}

void
DownlinkPacketScheduler::DoSchedule (void)
{
    DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
    cout << "Start DL packet scheduler for node "
    << GetMacEntity ()->GetDevice ()->GetIDNetworkNode()<< endl;
    DEBUG_LOG_END
    
    UpdateAverageTransmissionRate ();
    SelectFlowsToSchedule ();
    
    if (GetFlowsToSchedule ()->size() > 0)
    {
        RBsAllocation ();
    }
    
    StopSchedule ();
    ClearFlowsToSchedule ();
}

void
DownlinkPacketScheduler::DoStopSchedule (void)
{
    DEBUG_LOG_START_2(SIM_ENV_SCHEDULER_DEBUG, SIM_ENV_SCHEDULER_DEBUG_ARQ)
    cout << "\t Creating Packet Burst" << endl;
    DEBUG_LOG_END
    
    shared_ptr<PacketBurst> pb = make_shared<PacketBurst> ();
    
    //Create Packet Burst
    for (auto flow : *GetFlowsToSchedule ())
    {
        int availableBytes = flow->GetAllocatedBits ()/8;
        
        if (availableBytes > 0)
        {
            
            flow->GetBearer ()->UpdateTransmittedBytes (availableBytes);
            
            DEBUG_LOG_START_2(SIM_ENV_SCHEDULER_DEBUG, SIM_ENV_SCHEDULER_DEBUG_ARQ)
            cout << "\t  --> add packets for flow "
            << flow->GetBearer ()->GetApplication ()->GetApplicationID () << endl;
            DEBUG_LOG_END
            shared_ptr<PacketBurst> pb2;
            if (flow->IsHarqRetransmission () == false)
            {
                RlcEntity *rlc = flow->GetBearer ()->GetRlcEntity ();
                pb2 = rlc->TransmissionProcedure (availableBytes);
                flow->SetPacketBurst(pb2);
                if(flow->GetBearer()->GetDestination()->GetNodeType()==NetworkNode::TYPE_MULTICAST_DESTINATION)
                {
                    for (auto flowCopy : *(flow->GetMulticastClonedFlows()) )
                    {
                        flowCopy->SetPacketBurst(pb2->Copy());
                    }
                }
            }
            else
            {
                pb2 = flow->GetPacketBurst();
            }
            
            DEBUG_LOG_START_2(SIM_ENV_SCHEDULER_DEBUG, SIM_ENV_SCHEDULER_DEBUG_ARQ)
            cout << "\t\t  nb of packets: " << pb2->GetNPackets () << endl;
            DEBUG_LOG_END
            
            if (pb2->GetNPackets () > 0)
            {
                for (auto packet : pb2->GetPackets ())
                {
                    DEBUG_LOG_START_2(SIM_ENV_SCHEDULER_DEBUG, SIM_ENV_SCHEDULER_DEBUG_ARQ)
                    cout << "\t\t  added packet of bytes " << packet->GetSize () << endl;
                    //packet->Print ();
                    DEBUG_LOG_END
                    pb->AddPacket (packet->Copy ());
                }
            }
        }
    }
    
    //UpdateAverageTransmissionRate ();
    
    //SEND PACKET BURST
    
    DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
    if (pb->GetNPackets () == 0)
        cout << "\t Send only reference symbols" << endl;
    DEBUG_LOG_END
    
    GetMacEntity ()->GetDevice ()->SendPacketBurst (pb);
}



void
DownlinkPacketScheduler::RBsAllocation ()
{
    DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
    cout << " ---- DownlinkPacketScheduler::RBsAllocation";
    DEBUG_LOG_END
    
    FlowsToSchedule* flows = GetFlowsToSchedule ();
    int nbOfRBs = (int) GetMacEntity ()->GetDevice ()->GetPhy ()->GetBandwidthManager ()->GetDlSubChannels ().size ();
    
    //create a matrix of flow metrics
    std::vector<std::vector<double>> metrics(nbOfRBs, std::vector<double>((int)flows->size ()));
    
    double l_dAllocatedRBCounter = 0;
    bool * l_bFlowScheduled = new bool[flows->size ()];
    for (int k = 0; k < (int)flows->size (); k++)
        l_bFlowScheduled[k] = false;
    int l_iScheduledFlows = 0;
    vector<double> * l_bFlowScheduledSINR = new vector<double>[flows->size ()];
    
    // vector to track how many users are allocated on each RB
    vector<int> allocationsPerRB(nbOfRBs);
    fill( allocationsPerRB.begin(), allocationsPerRB.end(), 0);
    vector<int> HarqScheduledUEs;
    
    int defaultTxMode = GetMacEntity()->GetDefaultDlTxMode();
    vector< shared_ptr<arma::cx_fmat> > precodingMatrices;
    if (defaultTxMode == 11) {
        precodingMatrices.resize(nbOfRBs);
        for (int rb = 0; rb < nbOfRBs; rb++) {
            precodingMatrices.at(rb) = nullptr;
        }
    }
    
    FlowsToSchedule flowsForRetransmission;
    
    for (int i = 0; i < nbOfRBs; i++) {
        for (int j = 0; j < (int)flows->size (); j++) {
            int harqPid;
            GNodeB::UserEquipmentRecord* ueRecord = flows->at (j)->GetUe()->GetTargetNodeRecord();
            HarqManager* harq = ueRecord->GetHarqManager ();
            if (_harq_active_ && harq != nullptr) {
                harqPid = harq->GetPidForNewTx ();
            }
            else {
                harqPid = HarqManager::HARQ_NOT_USED;
            }
            flows->at(j)->SetHarqPid (harqPid);
            if (harqPid == HarqManager::HARQ_NO_PID_AVAILABLE && flows->at(j)->IsHarqRetransmission() == false) {
                // no free HARQ process, flow can't be scheduled
                metrics[i][j] = -INFINITY;
            }
            else {
                metrics[i][j] = ComputeSchedulingMetric (flows->at (j)->GetBearer (),
                                                         flows->at (j)->GetSpectralEfficiency ().at (i),
                                                         i);
            }
        }
    }
    
    for (auto f : *flows) {
        vector<double> sinrs;
        AMCModule* amc = f->GetUe()->GetMacEntity()->GetAmcModule();
        for (int rb = 0; rb < nbOfRBs; rb++) {
            sinrs.push_back( amc->GetSinrFromCQI (f->GetCqiFeedbacks ().at (rb)) );
        }
        f->widebandSinr = GetMiesmEffectiveSinr(sinrs);
        //amc->GetSinrFromCQI (scheduledFlow->GetCqiFeedbacks ().at (rb))
    }
    
    if (_harq_active_) {
        // collect flows for possible HARQ retransmission
        GNodeB::UserEquipmentRecords* ueRecords = GetMacEntity ()->GetDevice()->GetUserEquipmentRecords();
        
        for (auto ue : *ueRecords) {
            HarqManager* harqManager = ue->GetHarqManager();
            if (harqManager != nullptr) {
                FlowsToSchedule harqFlows = harqManager->GetFlowsForRetransmission ();
                // TODO: group retransmissions to the same UE;
                for (auto flow : harqFlows) {
                    InsertFlowForRetransmission (flow);
                }
                flowsForRetransmission.insert(flowsForRetransmission.end(),harqFlows.begin(),harqFlows.end());
            }
        }
        
        
        DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG_ARQ)
        cout << " \t HARQ Flows " << flowsForRetransmission.size() << endl;
        DEBUG_LOG_END
        
        //create a matrix of flow metrics for HARQ flows
        double HARQ_metrics[nbOfRBs][flowsForRetransmission.size()];
        for (int i = 0; i < nbOfRBs; i++) {
            for (int j = 0; j < (int)flowsForRetransmission.size (); j++) {
                HARQ_metrics[i][j] = ComputeSchedulingMetric (flowsForRetransmission.at (j)->GetBearer (),
                                                              flowsForRetransmission.at (j)->GetSpectralEfficiency ().at (i),
                                                              i);
            }
        }
        
        
        DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
        cout << ", available RBs " << nbOfRBs << ", flows " << flows->size () << endl;
        for (int ii = 0; ii < (int)flows->size (); ii++) {
            cout << "\t metrics for flow "
            << flows->at (ii)->GetBearer ()->GetApplication ()->GetApplicationID () << ":";
            for (int jj = 0; jj < nbOfRBs; jj++) {
                cout << " " << metrics[jj][ii];
            }
            cout << endl;
        }
        DEBUG_LOG_END
    
        
        // schedule HARQ retransmissions first
        // Every HARQ retransmission requires the same number of RBs and the same MCS as the original transmission
        FlowsToSchedule flowsForRetransmission_copy = flowsForRetransmission;
        
        for (auto harqFlow : flowsForRetransmission) {
            vector<int> localSchedulingMetrics(nbOfRBs);
            int flowIndex = -1;
            for (int i=0; i<(int)flowsForRetransmission.size(); i++) {
                if (harqFlow == flowsForRetransmission.at(i)) {
                    flowIndex = i;
                }
            }
            for (int rb = 0; rb < nbOfRBs; rb++) {
                localSchedulingMetrics.at(rb) = HARQ_metrics[rb][flowIndex];
            }
            vector<int> schedulingMetricsIndices(nbOfRBs);
            iota(schedulingMetricsIndices.begin(), schedulingMetricsIndices.end(), 0);
            
            sort(schedulingMetricsIndices.begin(),
                 schedulingMetricsIndices.end(),
                 [&localSchedulingMetrics](int i1, int i2) {return localSchedulingMetrics[i1] < localSchedulingMetrics[i2];});
            
            int availableRBs = 0;
            for (int i = 0; i < nbOfRBs; i++) {
                if (allocationsPerRB.at(i) < GetMaxUsersPerRB()) {
                    availableRBs++;
                }
            }
            
            vector<int>* allocatedRBs = harqFlow->GetListOfAllocatedRBs();
            int numRequiredRBs = (int) allocatedRBs->size();
            allocatedRBs->clear();
            
            // set MIMO layers when using full CSI feedback
            vector< shared_ptr<arma::cx_fmat> > channelMatrices;
            vector<int> assignedLayers;
            if (harqFlow->GetFullCsiFeedbacks ().size()>0) {
                channelMatrices.push_back(harqFlow->GetFullCsiFeedbacks ().at (0));
                for( int j = 0; j < (int)harqFlow->GetFullCsiFeedbacks ().at (0)->n_rows; j++) {
                    assignedLayers.push_back (j);
                }
                harqFlow->SetAssignedLayers(assignedLayers);
            }
            
            if (availableRBs >= numRequiredRBs && numRequiredRBs > 0) {
                HarqScheduledUEs.push_back(flowsForRetransmission.at(flowIndex)->GetUe()->GetIDNetworkNode());
                int assignedRBs = 0;
                for (int i = 0; i < nbOfRBs; i++) {
                    if (allocationsPerRB.at( schedulingMetricsIndices.at(i) ) < GetMaxUsersPerRB()) {
                        allocatedRBs->push_back( schedulingMetricsIndices.at(i) );
                        allocationsPerRB.at( schedulingMetricsIndices.at(i) )++;
                        assignedRBs++;
                    }
                    if (assignedRBs >= numRequiredRBs) {
                        break;
                    }
                }
            }
            else {
                flowsForRetransmission_copy.erase( remove( flowsForRetransmission_copy.begin(), flowsForRetransmission_copy.end(), harqFlow ) );
            }
        }
        flowsForRetransmission = flowsForRetransmission_copy;
        
        
        DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG_ARQ)
        cout << " \t HARQ Flows " << flowsForRetransmission.size() << endl;
        DEBUG_LOG_END
    }
    
    //RBs allocation
    for (int rb = 0; rb < nbOfRBs; rb++)
    {
        if (l_iScheduledFlows == (int)flows->size ())
        {
            break;
        }
        
        bool RBIsAllocated = false;
        if ( allocationsPerRB.at(rb) > 0 )
        {
            RBIsAllocated = true;
        }
        vector< shared_ptr<FlowToSchedule> > scheduledFlows;
        vector<int> l_iScheduledFlowsIndices;
        
        // schedule non-retransmission flows
        double targetMetric;
        for (int i = allocationsPerRB.at(rb); i < m_maxUsersPerRB; i++)
        {
            targetMetric = 0;
            shared_ptr<FlowToSchedule> scheduledFlow;
            int l_iScheduledFlowIndex;
            for (int k = 0; k < (int)flows->size (); k++)
            {
                if (metrics[rb][k] > targetMetric
                    && metrics[rb][k] > -INFINITY
                    && !l_bFlowScheduled[k]
                    && flows->at(k)->IsHarqRetransmission()==false
                    && find(HarqScheduledUEs.begin(), HarqScheduledUEs.end(), flows->at (k)->GetUe()->GetIDNetworkNode()) == HarqScheduledUEs.end()
                    && find( scheduledFlows.begin(), scheduledFlows.end(), flows->at(k) ) == scheduledFlows.end()
                    && flows->at(k)->GetDataToTransmit() > 0)
                {
                    HarqManager* harqManager = flows->at(k)->GetUe()->GetTargetNodeRecord()->GetHarqManager();
                    if ( harqManager != nullptr) {
                        if (harqManager->GetPidForNewTx() >= 0) {
                            targetMetric = metrics[rb][k];
                            RBIsAllocated = true;
                            scheduledFlow = flows->at (k);
                            l_iScheduledFlowIndex = k;
                        }
                    }
                    else {
                        targetMetric = metrics[rb][k];
                        RBIsAllocated = true;
                        scheduledFlow = flows->at (k);
                        l_iScheduledFlowIndex = k;
                    }
                }
            }
            if (targetMetric > 0)
            {
                allocationsPerRB.at(rb)++;
                scheduledFlows.push_back (scheduledFlow);
                l_iScheduledFlowsIndices.push_back (l_iScheduledFlowIndex);
            }
        }
        DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
        cout << "Number of flows on RB " << rb << ": " << scheduledFlows.size() << endl;
        DEBUG_LOG_END
        
        // calculate precoding matrices if needed
        if (RBIsAllocated) {
            int txMode = GetMacEntity ()->GetDefaultDlTxMode ();
            
            if( txMode == 11 ) {
                
                if (precodingMatrices.at(rb) == nullptr) {
                    vector< shared_ptr<arma::cx_fmat> > channelMatrices;
                    vector<double> widebandSinrs;
                    vector<int> ranks;
                    int layerCounter = 0;
                    vector< shared_ptr<FlowToSchedule> > allScheduledFlows = scheduledFlows;
                    allScheduledFlows.insert(allScheduledFlows.end(),flowsForRetransmission.begin(),flowsForRetransmission.end());
                    
                    for (int k = 0; k < (int)allScheduledFlows.size(); k++) {
                        shared_ptr<FlowToSchedule> flow = allScheduledFlows.at(k);
                        // TODO: choose layers here or in specialized scheduler?
                        vector<int> assignedLayers;
                        
                        if (flow->GetFullCsiFeedbacks ().size()>0) {
                            channelMatrices.push_back(flow->GetFullCsiFeedbacks ().at (rb));
                            
                            for (int l=0; l<flow->GetRiFeedback(); l++) {
                                assignedLayers.push_back (layerCounter);
                                widebandSinrs.push_back(flow->widebandSinr);
                                layerCounter++;
                            }
                            
                            flow->SetAssignedLayers(assignedLayers);
                        }
                        
                        ranks.push_back(flow->GetRiFeedback());
                    }
                    
                    precodingMatrices.at(rb) = PrecodingCalculator::RegularizedZeroForcing(
                                                                                           channelMatrices,
                                                                                           ranks,
                                                                                           widebandSinrs,
                                                                                           pow(10.,(GetMacEntity()->GetDevice()->GetPhy()->GetTxPower()-30)/10)/(double)nbOfRBs
                                                                                           );
                }
            }
        }
        
        // detect which flows have fullfilled their requirements
        if (RBIsAllocated)
        {
            l_dAllocatedRBCounter++;
            
            for (int i = 0; i < (int)scheduledFlows.size(); i++) {
                shared_ptr<FlowToSchedule> scheduledFlow = scheduledFlows.at(i);
                AMCModule* amc = scheduledFlow->GetUe()->GetProtocolStack()->GetMacEntity()->GetAmcModule();
                scheduledFlow->GetListOfAllocatedRBs()->push_back (rb); // the rb RB has been allocated to that flow!
              
                
                DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
                cout << "\t *** RB " << rb << " assigned to the "
                "flow " << scheduledFlow->GetBearer ()->GetApplication ()->GetApplicationID ()
                << " (UE " << scheduledFlow->GetUe()->GetIDNetworkNode()
                << " rank " << scheduledFlow->GetRiFeedback() << ")"
                << endl;
                DEBUG_LOG_END
                
                
                double sinr = amc->GetSinrFromCQI (scheduledFlow->GetCqiFeedbacks ().at (rb));
                l_bFlowScheduledSINR[l_iScheduledFlowsIndices.at(i)].push_back(sinr);
                int mcs = amc->GetMCSFromSinrVector (l_bFlowScheduledSINR[l_iScheduledFlowsIndices.at(i)]);
                int txMode = scheduledFlow->GetUe()->GetTargetNodeRecord()->GetDlTxMode();
                int M, N, rank;
                M = scheduledFlow->GetBearer ()->GetSource ()->GetPhy()->GetTxAntennas();
                N = scheduledFlow->GetBearer ()->GetDestination ()->GetPhy()->GetRxAntennas();
                switch(txMode)
                {
                    case 1:
                    case 2:
                        rank = 1;
                        break;
                    case 3:
                    case 4:
                    case 9:
                        rank = scheduledFlow->GetRiFeedback();
                        if(rank < 1 || rank > min(M,N))
                        {
                            rank = min(M,N);
                        }
                        break;
                    case 11:
                        rank = scheduledFlow->GetRiFeedback();
                        if (rb == nbOfRBs-1)
                        {
                            vector<double> newSinr = PrecodingCalculator::adjustSinrForMuMimo(
                                                                                              l_bFlowScheduledSINR[l_iScheduledFlowsIndices.at(i)],
                                                                                              scheduledFlow->GetUeRecord()->GetChannelMatrix(),
                                                                                              precodingMatrices, scheduledFlow->GetAssignedLayers());
                            mcs = amc->GetMCSFromSinrVector(newSinr);
                            
                            DEBUG_LOG_START_1(SIM_ENV_MCS_DEBUG)
                            cout << "BS_SINR_2 UE " << scheduledFlows.at(i)->GetUe()->GetIDNetworkNode() << " SINR "<< newSinr << " MCS " << mcs << endl;
                            DEBUG_LOG_END
                        }
                        break;
                    default:
                        cout << "Error: invalid transmission mode " << txMode << endl;
                        exit(1);
                }
                int transportBlockSize = amc->GetTBSizeFromMCS (mcs, mcs, (int) scheduledFlow->GetListOfAllocatedRBs ()->size (),rank);
                if (transportBlockSize >= scheduledFlow->GetDataToTransmit() * 8)
                {
                    l_bFlowScheduled[l_iScheduledFlowsIndices.at(i)] = true;
                    l_iScheduledFlows++;
                }
            }
        }
    }
    
    delete [] l_bFlowScheduled;
    delete [] l_bFlowScheduledSINR;
    
    flows->insert(flows->end(),flowsForRetransmission.begin(),flowsForRetransmission.end());
    
    DEBUG_LOG_START_2(SIM_ENV_SCHEDULER_DEBUG, SIM_ENV_SCHEDULER_DEBUG_ARQ)
    cout << "SCHEDULING T=" << Simulator::Init()->Now() << " UEs ";
    for (auto flow : *flows)
    {
        if (flow->GetListOfAllocatedRBs ()->size () > 0)
        {
            cout << flow->GetUe()->GetIDNetworkNode() << " ";
        }
    }
    cout << endl;
    DEBUG_LOG_END
    
    //Finalize the allocation
    PdcchMapIdealControlMessage *pdcchMsg = new PdcchMapIdealControlMessage ();
    
    for (auto flow : *flows)
    {
        GNodeB::UserEquipmentRecord* ueRecord = flow->GetUe()->GetTargetNodeRecord();
        int txMode = ueRecord->GetDlTxMode();
        int M = flow->GetBearer ()->GetSource ()->GetPhy()->GetTxAntennas();
        int N = flow->GetBearer ()->GetDestination ()->GetPhy()->GetRxAntennas();
        int mcs, rank;
        AMCModule* amc = flow->GetUe()->GetProtocolStack()->GetMacEntity()->GetAmcModule();
        
        if (flow->GetListOfAllocatedRBs ()->size () > 0)
        {
            vector<int> pmi;
            
            if (flow->IsHarqRetransmission() == false)
            {
                //this flow has been scheduled
                vector<double> estimatedSinrValues;
                for (int rb = 0; rb < (int)flow->GetListOfAllocatedRBs ()->size (); rb++ )
                {
                    double sinr;
                    sinr = amc->GetSinrFromCQI (flow->GetCqiFeedbacks ().at (flow->GetListOfAllocatedRBs ()->at (rb)));
                    estimatedSinrValues.push_back (sinr);
                }
                
                //get the MCS for transmission
                if (FrameManager::Init()->isMbsfnSubframe()==true)
                {
                    MulticastDestination* mc = (MulticastDestination*)flow->GetBearer()->GetApplication()->GetDestination();
                    int mc_mcs = mc->GetMcs();
                    if(mc_mcs == -1)
                    {
                        mcs = amc->GetMCSFromSinrVector (estimatedSinrValues);
                    }
                    else if(mc_mcs>=0 && mc_mcs<=28)
                    {
                        mcs = mc_mcs;
                    }
                    else
                    {
                        cout << "Wrong MCS set for multicast stream: " << mc_mcs << endl;
                        exit(1);
                    }
                    //                  int burstSize = ((TraceBased*)(flow->GetBearer()->GetApplication()))->GetBurstSize();
                    //                  double maxDelay = flow->GetBearer()->GetApplication()->GetQoSParameters()->GetMaxDelay();
                    //                  int minTbs;
                    //                  switch(FrameManager::Init()->GetMbsfnPattern())
                    //                    {
                    //                      case 1:
                    //                        minTbs = round( burstSize / (maxDelay * 1000 * 1. / 10.) * 8 );
                    //                        break;
                    //                      case 2:
                    //                        minTbs = round( burstSize / (maxDelay * 1000 * 3. / 10.) * 8 );
                    //                        break;
                    //                      case 3:
                    //                        minTbs = round( burstSize / (maxDelay * 1000 * 6. / 10.) * 8 );
                    //                        break;
                    //                      default:
                    //                        minTbs = round( burstSize / (maxDelay * 1000 * 1. / 10.) * 8 );
                    //                        break;
                    //                    }
                    //
                    //                  AMCModule* amc = new AMCModule();
                    //                  for(mcs=0; mcs<27; mcs++)
                    //                    {
                    //                      if (amc->GetTBSizeFromMCS (mcs, nbOfRBs)>minTbs)
                    //                        {
                    //                          break;
                    //                        }
                    //                    }
                    //                  delete amc;
                }
                else if(txMode != 11)
                {
                    mcs = amc->GetMCSFromSinrVector (estimatedSinrValues);
                }
                else
                {
                    
                    DEBUG_LOG_START_1(SIM_ENV_MCS_DEBUG)
                    cout << "MCS_DEBUG estimated SINR values: UE " << flow->GetUe()->GetIDNetworkNode() << ", SINRs " << estimatedSinrValues << endl;
                    DEBUG_LOG_END
                    vector<double> newSinr = PrecodingCalculator::adjustSinrForMuMimo(
                                                                                      estimatedSinrValues,
                                                                                      flow->GetFullCsiFeedbacks(),
                                                                                      precodingMatrices, flow->GetAssignedLayers());
                    //                mcs = amc->GetMCSFromSinrVector(newSinr);
                    mcs = GetMacEntity()->GetAmcModule()->GetMCSFromSinrVector(newSinr);
                    
                    DEBUG_LOG_START_1(SIM_ENV_MCS_DEBUG)
                    cout << "MCS_DEBUG adjusted SINR values: UE " << flow->GetUe()->GetIDNetworkNode() << " SINRs " << newSinr << endl;
                    DEBUG_LOG_END
                    //                  mcs = amc->GetMCSFromSinrVector(estimatedSinrValues);
                }
                for (int rb = 0; rb < (int)flow->GetListOfAllocatedRBs ()->size (); rb++ ) {
                    flow->GetListOfSelectedMCS()->push_back(mcs);
                }
            }
            else {
                mcs = flow->GetListOfSelectedMCS()->at(0);
            }
                        
            rank = flow->GetRiFeedback();
            switch(txMode)
            {
                case 1:
                case 2:
                    rank = 1;
                    pmi.resize(0); // not used
                    break;
                    
                case 3:
                    if(rank < 1 || rank > min(M,N))
                    {
                        cout << "Error: invalid rank in DownlinkPacketScheduler (" << rank << ")" << endl;
                        rank = min(M,N);
                    }
                    pmi.resize(0); //not used
                    DEBUG_LOG_START_1(SIM_ENV_TEST_RI_FEEDBACK)
                    cout << "flow "
                    << flow->GetBearer ()->GetApplication ()->GetApplicationID ()
                    << ",\tRI feedback: "
                    << flow->GetRiFeedback()
                    << endl;
                    DEBUG_LOG_END
                    break;
                    
                case 4:
                case 9:
                case 11:
                    if(rank < 1 || rank > min(M,N))
                    {
                        cout << "Error: invalid rank in DownlinkPacketScheduler (" << rank << ")" << endl;
                        rank = min(M,N);
                    }
                    break;
            }
            
            if (flow->IsHarqRetransmission() == false)
            {
                //define the amount of bytes to transmit
                int transportBlockSize = amc->GetTBSizeFromMCS (mcs, mcs, (int) flow->GetListOfAllocatedRBs ()->size (), rank );
                flow->UpdateAllocatedBits (transportBlockSize);
                
                DEBUG_LOG_START_2(SIM_ENV_SCHEDULER_DEBUG, SIM_ENV_SCHEDULER_DEBUG_ARQ)
                cout << "\t\t --> flow " << flow->GetBearer ()->GetApplication ()->GetApplicationID ()
                << " has been scheduled: " <<
                "\n\t\t\t nb of RBs " << flow->GetListOfAllocatedRBs ()->size () <<
                //                      "\n\t\t\t effectiveSinr " << GetMiesmEffectiveSinr(estimatedSinrValues) <<
                "\n\t\t\t tbs " << transportBlockSize/8
                << endl;
                DEBUG_LOG_END
                DEBUG_LOG_START_1(SIM_ENV_DL_DEBUG)
                cout << "DL_DEBUG "
                << flow->GetBearer ()->GetApplication ()-> GetDestination ()-> GetMobilityModel ()->GetAbsolutePosition ()->GetCoordinateX () << " "
                << flow->GetBearer ()->GetApplication ()-> GetDestination ()-> GetMobilityModel ()->GetAbsolutePosition ()->GetCoordinateY () << " "
                //                << GetMiesmEffectiveSinr(estimatedSinrValues) << " "
                << mcs << " "
                << rank << " "
                << transportBlockSize
                << endl;
                DEBUG_LOG_END
            }
            
            //create PDCCH messages
            if(txMode != 11)
            {
                // TODO: extend if MU-MIMO is implemented for TM5 or TM9
                vector<int> assignedLayers;
                for(int i = 0; i < rank; i++)
                {
                    assignedLayers.push_back (i);
                }
                for (int rb = 0; rb < (int)flow->GetListOfAllocatedRBs ()->size (); rb++ )
                {
                    if (flow->GetPmiFeedbacks ().size() > 0)
                    {
                        pmi = flow->GetPmiFeedbacks ().at (flow->GetListOfAllocatedRBs ()->at (rb));
                    }
                    else
                    {
                        pmi.resize(0);
                    }
                    if (pmi.size() == 0)
                    {
                        pmi.resize(1);
                        if (M == 2 && rank == 2)
                        {
                            pmi.at(0) = 1;
                        }
                        else
                        {
                            pmi.at(0) = 0;
                        }
                        if(txMode == 9 && M == 8)
                        {
                            pmi.resize(2);
                            pmi.at(1) = 0;
                        }
                    }
                    if (flow->GetBearer ()->GetDestination ()->GetNodeType() == NetworkNode::TYPE_MULTICAST_DESTINATION)
                    {
                        // schedule this RB to each user of the multicast group
                        MulticastDestination* virtualDest = (MulticastDestination*)flow->GetBearer ()->GetDestination ();
                        for(auto dest : virtualDest->GetDestinations())
                        {
                            GNodeB::UserEquipmentRecord* userRecord = dest->GetTargetNodeRecord();
                            if (userRecord !=nullptr)
                            {
                                HarqManager* harq = userRecord->GetHarqManager ();
                                int harqPID;
                                if (harq != nullptr)
                                {
                                    harqPID = userRecord->GetHarqManager ()->GetPidForNewTx ();
                                }
                                else
                                {
                                    harqPID = HarqManager::HARQ_NOT_USED;
                                }
                                pdcchMsg->AddNewRecord (PdcchMapIdealControlMessage::DOWNLINK, flow->GetListOfAllocatedRBs ()->at (rb), dest, mcs, harqPID, rank, pmi);
                            }
                        }
                    }
                    else
                    {
                        pdcchMsg->AddNewRecord (PdcchMapIdealControlMessage::DOWNLINK,
                                                flow->GetListOfAllocatedRBs ()->at (rb),
                                                flow->GetBearer ()->GetDestination (),
                                                flow->GetListOfSelectedMCS ()->at (rb),
                                                flow->GetHarqPid(), rank, pmi, assignedLayers);
                    }
                }
            }
            else
            {
                for (int rb = 0; rb < (int)flow->GetListOfAllocatedRBs ()->size (); rb++ )
                {
                    shared_ptr<arma::cx_fmat> precodingMatrix = precodingMatrices.at(flow->GetListOfAllocatedRBs ()->at (rb));
                    vector<int> assignedLayers = flow->GetAssignedLayers();
                    
                    bool use_srta_pi = flow->GetUe()->GetPhy()->GetSrtaPi();
                    if (flow->GetBearer ()->GetDestination ()->GetNodeType() == NetworkNode::TYPE_MULTICAST_DESTINATION)
                    {
                        // schedule this RB to each user of the multicast group
                        MulticastDestination* virtualDest = (MulticastDestination*)flow->GetBearer ()->GetDestination ();
                        int nbOfMulticastUsers = 0;
                        for(auto dest : virtualDest->GetDestinations())
                        {
                            GNodeB::UserEquipmentRecord* userRecord = dest->GetTargetNodeRecord();
                            if (userRecord !=nullptr)
                            {
                                HarqManager* harq = userRecord->GetHarqManager ();
                                int harqPID;
                                if (harq != nullptr)
                                {
                                    harqPID = userRecord->GetHarqManager ()->GetPidForNewTx ();
                                }
                                else
                                {
                                    harqPID = HarqManager::HARQ_NOT_USED;
                                }
                                pdcchMsg->AddNewRecord (PdcchMapIdealControlMessage::DOWNLINK, flow->GetListOfAllocatedRBs ()->at (rb), dest, mcs, harqPID, rank, pmi);
                            }
                            
                            // XXX: temporary hack to detect users which made handover to another cell
                            if (flow->GetBearer()->GetSource() == dest->GetTargetNode())
                            {
                                nbOfMulticastUsers++;
                            }
                        }
                        //cout << "MULTICAST_INFO " << nbOfMulticastUsers << " active users" << endl;
                    }
                    else if (use_srta_pi==true)
                    {
                        pdcchMsg->AddNewRecord (PdcchMapIdealControlMessage::DOWNLINK,
                                                flow->GetListOfAllocatedRBs ()->at (rb),
                                                flow->GetBearer ()->GetDestination (),
                                                flow->GetListOfSelectedMCS ()->at (rb),
                                                flow->GetHarqPid(), rank,
                                                precodingMatrix,
                                                assignedLayers,
                                                flow->GetFullCsiFeedbacks().at(flow->GetListOfAllocatedRBs ()->at (rb)));
                    }
                    else
                    {
                        pdcchMsg->AddNewRecord (PdcchMapIdealControlMessage::DOWNLINK,
                                                flow->GetListOfAllocatedRBs ()->at (rb),
                                                flow->GetBearer ()->GetDestination (),
                                                flow->GetListOfSelectedMCS ()->at (rb),
                                                flow->GetHarqPid(), rank,
                                                precodingMatrix,
                                                assignedLayers);
                    }
                }
            }
            
            // notify operation to HARQ manager
            
            if(flow-> GetBearer ()->GetDestination ()->GetNodeType() == NetworkNode::TYPE_MULTICAST_DESTINATION)
            {
                MulticastDestination* virtualDest = (MulticastDestination*)flow->GetBearer ()->GetDestination ();
                for(auto dest : virtualDest->GetDestinations())
                {
                    GNodeB::UserEquipmentRecord* userRecord = dest->GetTargetNodeRecord();
                    if (userRecord != nullptr)
                    {
                        HarqManager* harqman = userRecord->GetHarqManager ();
                        int harqPID;
                        if (harqman != nullptr)
                        {
                            harqPID = userRecord->GetHarqManager ()->GetPidForNewTx ();
                            /*}
                             else
                             {
                             harqPID = HarqManager::HARQ_NOT_USED;
                             } */
                            //                          RadioBearer* rbearer;
                            //                          int data;
                            shared_ptr<FlowToSchedule> flowcp = make_shared<FlowToSchedule>(
                                                                                            flow->GetBearer(),
                                                                                            flow->GetDataToTransmit());
                            *flowcp = *flow;
                            flowcp ->SetHarqPid (harqPID);
                            harqman->NewMulticastTransmission (harqPID,flowcp);
                            flow->GetMulticastClonedFlows()->push_back(flowcp);
                        }
                    }
                }
            }
            else if (flow->IsHarqRetransmission() == true)
            {
                // increase retransmission counter
                ueRecord->GetHarqManager ()->Retransmission (flow->GetHarqPid());
            }
            else if (flow->GetHarqPid() != HarqManager::HARQ_NOT_USED)
            {
                // copy flow to HARQ manager
                ueRecord->GetHarqManager ()->NewTransmission (flow->GetHarqPid(), flow);
            }
        }
    }
    
    if (pdcchMsg->GetMessage()->size () > 0)
    {
        GetMacEntity ()->GetDevice ()->GetPhy ()->SendIdealControlMessage (pdcchMsg);
    }
}


void
DownlinkPacketScheduler::UpdateAverageTransmissionRate (void)
{
    RrcEntity *rrc = GetMacEntity ()->GetDevice ()->GetProtocolStack ()->GetRrcEntity ();
    for (auto bearer : *rrc->GetRadioBearerContainer ())
    {
        bearer->UpdateAverageTransmissionRate ();
    }
}
