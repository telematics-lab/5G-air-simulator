/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2020 TELEMATICS LAB, Politecnico di Bari
 *
 * This file is part of 5G-air-simulator
 *
 * 5G-air-simulator is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation;
 * 5G-air-simulator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 5G-air-simulator; if not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Telematics Lab <telematics-dev@poliba.it>
 */

#include "dl-fls-packet-scheduler.h"
#include "../mac-entity.h"
#include "../../packet/Packet.h"
#include "../../packet/packet-burst.h"
#include "../../../device/NetworkNode.h"
#include "../../../device/MulticastDestination.h"
#include "../../../flows/radio-bearer.h"
#include "../../../protocolStack/rrc/rrc-entity.h"
#include "../../../flows/application/Application.h"
#include "../../../flows/application/TraceBased.h"
#include "../../../device/GNodeB.h"
#include "../../../protocolStack/mac/AMCModule.h"
#include "../../../phy/phy.h"
#include "../../../core/spectrum/bandwidth-manager.h"
#include "../../../flows/QoS/QoSForFLS.h"
#include "../../../flows/MacQueue.h"
#include "../../../utility/eesm-effective-sinr.h"
#include "../../../utility/miesm-effective-sinr.h"
#include "../../../componentManagers/FrameManager.h"


DL_FLS_PacketScheduler::DL_FLS_PacketScheduler()
{
  SetMacEntity (nullptr);
  CreateFlowsToSchedule ();
  m_runControlLaw = true;
  m_subFrameCounter = 0;
  m_startPrbAllocation = 0;
  m_freePrb = true;
  m_rtAllocation = false;
  m_lowerLevelSchedulerType = DL_FLS_PacketScheduler::PROPORTIONAL_FAIR;
}

DL_FLS_PacketScheduler::~DL_FLS_PacketScheduler()
{
  Destroy ();
}

void
DL_FLS_PacketScheduler::SetLowerLevelSchedulerType (LowerLevelSchedulerType type)
{
  m_lowerLevelSchedulerType = type;
}

DL_FLS_PacketScheduler::LowerLevelSchedulerType
DL_FLS_PacketScheduler::GetLowerLevelSchedulerType (void) const
{
  return m_lowerLevelSchedulerType;
}

void
DL_FLS_PacketScheduler::DoSchedule ()
{
DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
  cout << "Start DL packet (FLS) scheduler for node "
            << GetMacEntity ()->GetDevice ()->GetIDNetworkNode()<< endl;
DEBUG_LOG_END

  UpdateDataToTransmitAndAverageDataRate ();
  CheckForDLDropPackets ();
  m_startPrbAllocation = 0;
  m_freePrb = true;
  m_rtAllocation = false;

  m_subFrameCounter ++;
  if (m_runControlLaw)
    {
      RunControlLaw ();
    }
  if (m_subFrameCounter == 10)
    {
      m_runControlLaw = true;
      m_subFrameCounter = 0;
    }

  Select_RT_FlowsToSchedule ();

  if (GetFlowsToSchedule ()->size() != 0)
    {
      m_rtAllocation = true;
      m_freePrb = false;
      RBsAllocation ();
      StopSchedule ();
    }
  if (m_freePrb)
    {
      m_rtAllocation = false;
      Select_NRT_FlowsToSchedule ();
      if (GetFlowsToSchedule ()->size() != 0)
        {
          RBsAllocation ();
          StopSchedule ();
        }
      else
        {

DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
          cout << " FLS_DEBUG: RBs Have NOT BEEN allocated"<< endl;
DEBUG_LOG_END
        }
    }
  ClearFlowsToSchedule ();

}

void
DL_FLS_PacketScheduler::DoStopSchedule (void)
{
DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
  cout << "\t Do Stop Schedule (FLS) Creating Packet Burst" << endl;
DEBUG_LOG_END

  shared_ptr<PacketBurst> pb = make_shared<PacketBurst> ();

  //Create Packet Burst
  for (auto flow : *GetFlowsToSchedule())
    {
      int availableBytes = flow->GetAllocatedBits ()/8;

      if (availableBytes > 0)
        {
          flow->GetBearer ()->UpdateTransmittedBytes (availableBytes);
DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
          cout << "\t  --> add packets for flow "
                    << flow->GetBearer ()->GetApplication ()->GetApplicationID () << endl;
DEBUG_LOG_END

          //flow->GetBearer ()->GetMacQueue ()->PrintQueueInfo ();
          RlcEntity *rlc = flow->GetBearer ()->GetRlcEntity ();
          shared_ptr<PacketBurst> pb2 = rlc->TransmissionProcedure (availableBytes);


DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
          cout << "\t\t  nb of packets: " << pb2->GetNPackets () << endl;
DEBUG_LOG_END
          if (pb2->GetNPackets () > 0)
            {
              for (auto packet : pb2->GetPackets ())
                {
DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
                  cout << "\t\t  added packet of bytes " << packet->GetSize () << endl;
                  //(*it)->Print ();
DEBUG_LOG_END
                  pb->AddPacket (packet->Copy ());
                }
            }
        }
      else
        {}
    }
  //SEND PACKET BURST
DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
  if (pb->GetNPackets () == 0)
    cout << "\t Send only reference symbols" << endl;
DEBUG_LOG_END

  GetMacEntity ()->GetDevice ()->SendPacketBurst (pb);
}

double
DL_FLS_PacketScheduler::ComputeSchedulingMetric (RadioBearer *bearer, double spectralEfficiency, int subChannel)
{
  double metric;

  if (bearer->GetApplication ()->GetApplicationType () == Application::APPLICATION_TYPE_TRACE_BASED ||
      bearer->GetApplication ()->GetApplicationType () == Application::APPLICATION_TYPE_VOIP)
    {

      if (GetLowerLevelSchedulerType () == DL_FLS_PacketScheduler::MAXIMUM_THROUGHPUT)
        {
          metric = spectralEfficiency * 180000.;
        }

      if (GetLowerLevelSchedulerType () == DL_FLS_PacketScheduler::PROPORTIONAL_FAIR)
        {
          metric = (spectralEfficiency * 180000.)
                   /
                   bearer->GetAverageTransmissionRate();
        }
    }
  else
    {
      metric = (spectralEfficiency * 180000.)
               /
               bearer->GetAverageTransmissionRate();
    }

  return metric;
}


void
DL_FLS_PacketScheduler::RunControlLaw ()
{
  m_runControlLaw = false;

DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
  cout << " FLS_DEBUG:Run Control Law"<< endl;
DEBUG_LOG_END
  RrcEntity *rrc = GetMacEntity ()->GetDevice ()->GetProtocolStack ()->GetRrcEntity ();

  for (auto bearer : *rrc->GetRadioBearerContainer ())
    {
      if (bearer->GetApplication ()->GetApplicationType() == Application::APPLICATION_TYPE_TRACE_BASED
          ||
          bearer->GetApplication ()->GetApplicationType() == Application::APPLICATION_TYPE_VOIP )
        {

DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
          bearer->GetMacQueue ()->PrintQueueInfo ();
DEBUG_LOG_END

          //Frame Level Scheduler Control Law!!!
          QoSForFLS* qos = (QoSForFLS*) bearer->GetQoSParameters ();

          int queueSize = bearer->GetQueueSize ();
          int* q = qos->GetQ ();
          int* u = qos->GetU ();
          double* c = qos->GetFilterCoefficients();
          int M = qos->GetNbOfCoefficients ();

          double dataToTransmit = ((double)(1-c[2]) * queueSize);
          for (int i = 0; i < M-1; i++)
            {
              dataToTransmit += (double)q[i]*c[i+2];
            }
          for (int i = 0; i < M-2; i++)
            {
              dataToTransmit -= (double)q[i]*c[i+3];
            }
          for (int i = 0; i < M-1; i++)
            {
              dataToTransmit -= (double)u[i]*c[i+2];
            }

          if (dataToTransmit < 0)
            {
              dataToTransmit = 0;
            }


          if (bearer->HasPackets())
            {
              int minData = 8 + bearer->GetHeadOfLinePacketSize ();
              int maxData = bearer->GetMacQueue ()->GetByte (dataToTransmit);
DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
              cout << "\t dataToTransmit " << dataToTransmit << " minData " << minData  << " maxData " <<  maxData  << endl;
DEBUG_LOG_END
              if (dataToTransmit < minData)
                {
DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
                  cout << "\t selected minData " << endl;
DEBUG_LOG_END
                  dataToTransmit = minData;
                }
              else
                {
                  if (dataToTransmit < maxData)
                    {
                      dataToTransmit=maxData;
DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
                      cout << "\t selected maxData " << endl;
DEBUG_LOG_END
                    }
                }
            }


DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
          cout << "\t selected flow " << bearer->GetApplication ()->GetApplicationID () << endl;
          qos->Print ();
          cout << "\t queue size  " <<  queueSize << "\n\t DATA TO TRANSMIT " << (ceil)(dataToTransmit) << endl;
DEBUG_LOG_END

          qos->UpdateQ (queueSize);
          qos->UpdateU ((ceil)(dataToTransmit));
          qos->SetDataToTransmit ((ceil)(dataToTransmit));
        }
    }
}


void
DL_FLS_PacketScheduler::Select_RT_FlowsToSchedule ()
{
  ClearFlowsToSchedule ();

DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
  cout << "\t Select RT Flows to schedule" << endl;
DEBUG_LOG_END

  RrcEntity *rrc = GetMacEntity ()->GetDevice ()->GetProtocolStack ()->GetRrcEntity ();
  for (auto bearer : *rrc->GetRadioBearerContainer ())
    {
      //SELECT FLOWS TO SCHEDULE
      QoSForFLS* qos = (QoSForFLS*) bearer->GetQoSParameters ();

      if (bearer->HasPackets ()
          &&
          bearer->GetDestination ()->GetNodeState () == NetworkNode::STATE_ACTIVE
          &&
          qos->GetDataToTransmit () > 0
          &&
          (bearer->GetApplication ()->GetApplicationType() == Application::APPLICATION_TYPE_TRACE_BASED
           ||
           bearer->GetApplication ()->GetApplicationType() == Application::APPLICATION_TYPE_VOIP )
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
          //data to transmit
          int dataToTransmit = qos->GetDataToTransmit ();

          //compute spectral efficiency
          UserEquipment *ue = (UserEquipment*) bearer->GetDestination();
          GNodeB::UserEquipmentRecord *ueRecord = ue->GetTargetNodeRecord();
          AMCModule* amc = ue->GetProtocolStack()->GetMacEntity()->GetAmcModule();
          vector<double> spectralEfficiency;
          vector<int> cqiFeedbacks = ueRecord->GetCQI ();
          int riFeedback = ueRecord->GetRI ();
          vector< vector<int> > pmiFeedbacks = ueRecord->GetPMI ();
          int numberOfCqi = cqiFeedbacks.size ();
          for (int i = 0; i < numberOfCqi; i++)
            {
              double sEff = amc->GetEfficiencyFromCQI (cqiFeedbacks.at (i));
              spectralEfficiency.push_back (sEff);
            }

          //create flow to schedule record
          InsertFlowToSchedule(bearer, dataToTransmit, spectralEfficiency, cqiFeedbacks, riFeedback, pmiFeedbacks);
        }
      else
        {}
    }
}


void
DL_FLS_PacketScheduler::Select_NRT_FlowsToSchedule ()
{
  ClearFlowsToSchedule ();

DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
  cout << "\t Select NRT Flows to schedule" << endl;
DEBUG_LOG_END

  RrcEntity *rrc = GetMacEntity ()->GetDevice ()->GetProtocolStack ()->GetRrcEntity ();
  for (auto bearer : *rrc->GetRadioBearerContainer ())
    {
      //SELECT FLOWS TO SCHEDULE
      if (bearer->HasPackets ()
          &&
          bearer->GetDestination ()->GetNodeState () == NetworkNode::STATE_ACTIVE
          &&
          (bearer->GetApplication ()->GetApplicationType() == Application::APPLICATION_TYPE_CBR
           ||
           bearer->GetApplication ()->GetApplicationType() == Application::APPLICATION_TYPE_INFINITE_BUFFER )
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

          if (bearer->GetApplication ()->GetApplicationType() == Application::APPLICATION_TYPE_INFINITE_BUFFER)
            {
              dataToTransmit = 100000;
            }
          else
            {
              dataToTransmit = bearer->GetQueueSize ();
            }

          //compute spectral efficiency
          UserEquipment *ue = (UserEquipment*) bearer->GetDestination();
          GNodeB::UserEquipmentRecord *ueRecord = ue->GetTargetNodeRecord();
          AMCModule* amc = ue->GetProtocolStack()->GetMacEntity()->GetAmcModule();
          vector<double> spectralEfficiency;
          vector<int> cqiFeedbacks = ueRecord->GetCQI ();
          int riFeedback = ueRecord->GetRI ();
          vector< vector<int> > pmiFeedbacks = ueRecord->GetPMI ();
          int numberOfCqi = cqiFeedbacks.size ();
          for (int i = 0; i < numberOfCqi; i++)
            {
              double sEff = amc->GetEfficiencyFromCQI (cqiFeedbacks.at (i));
              spectralEfficiency.push_back (sEff);
            }

          //create flow to schedule record
          InsertFlowToSchedule(bearer, dataToTransmit, spectralEfficiency, cqiFeedbacks, riFeedback, pmiFeedbacks);
        }
      else
        {}
    }
}

void
DL_FLS_PacketScheduler::UpdateDataToTransmitAndAverageDataRate (void)
{
  RrcEntity *rrc = GetMacEntity ()->GetDevice ()->GetProtocolStack ()->GetRrcEntity ();
  for (auto bearer : *rrc->GetRadioBearerContainer ())
    {
      // UPDATE DATA TO TRASMIT FOR RT FLOWS
      if (bearer->GetApplication ()->GetApplicationType () == Application::APPLICATION_TYPE_TRACE_BASED
          ||
          bearer->GetApplication ()->GetApplicationType () == Application::APPLICATION_TYPE_VOIP)
        {
          QoSForFLS *qos = (QoSForFLS*) bearer->GetQoSParameters ();
          int dataToTransmit = qos->GetDataToTransmit ();
          int transmittedData = bearer->GetTransmittedBytes ();
          if (transmittedData >= dataToTransmit)
            {
              qos->SetDataToTransmit (0);
            }
          else
            {
              qos->SetDataToTransmit (dataToTransmit - transmittedData);
            }
        }

      // UPDATE AVERAGE TRANSMISSION RATE
      bearer->UpdateAverageTransmissionRate ();
    }
}
void
DL_FLS_PacketScheduler::RBsAllocation ()
{
DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
  cout << " ---- DownlinkPacketScheduler::RBsAllocation";
DEBUG_LOG_END


  FlowsToSchedule* flows = GetFlowsToSchedule ();
  int nbOfRBs = GetMacEntity ()->GetDevice ()->GetPhy ()->GetBandwidthManager ()->GetDlSubChannels ().size ();

  //create a matrix of flow metrics
  double metrics[nbOfRBs][flows->size ()];
  for (int i = 0; i < nbOfRBs; i++)
    {
      for (int j = 0; j < (int)flows->size (); j++)
        {
          metrics[i][j] = ComputeSchedulingMetric (flows->at (j)->GetBearer (),
                          flows->at (j)->GetSpectralEfficiency ().at (i),
                          i);
        }
    }

DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
  cout << ", available RBs " << nbOfRBs << ", flows " << flows->size () << endl;
  for (int ii = 0; ii < (int)flows->size (); ii++)
    {
      cout << "\t metrics for flow "
                << flows->at (ii)->GetBearer ()->GetApplication ()->GetApplicationID () << ":";
      for (int jj = 0; jj < nbOfRBs; jj++)
        {
          cout << " " << metrics[jj][ii];
        }
      cout << endl;
    }
DEBUG_LOG_END


  double l_dAllocatedRBCounter = 0;

  int l_iNumberOfUsers = ((GNodeB*)GetMacEntity()->GetDevice())->GetNbOfUserEquipmentRecords();

  bool * l_bFlowScheduled = new bool[flows->size ()];
  int l_iScheduledFlows = 0;
  vector<double> * l_bFlowScheduledSINR = new vector<double>[flows->size ()];
  for (int k = 0; k < (int)flows->size (); k++)
    l_bFlowScheduled[k] = false;

  //RBs allocation
  for (int s = m_startPrbAllocation; s < nbOfRBs; s++)
    {
      if (l_iScheduledFlows == (int)flows->size ())
        {
          if (m_rtAllocation)
            {
              m_freePrb = true;
              m_startPrbAllocation = s;
            }
          break;
        }

      double targetMetric = 0;
      bool RBIsAllocated = false;
      shared_ptr<FlowToSchedule> scheduledFlow;
      int l_iScheduledFlowIndex = 0;

      for (int k = 0; k < (int)flows->size (); k++)
        {
          if (metrics[s][k] > targetMetric && !l_bFlowScheduled[k])
            {
              targetMetric = metrics[s][k];
              RBIsAllocated = true;
              scheduledFlow = flows->at (k);
              l_iScheduledFlowIndex = k;
            }
        }

      if (RBIsAllocated)
        {
          l_dAllocatedRBCounter++;
          AMCModule* amc = scheduledFlow->GetUe()->GetProtocolStack()->GetMacEntity()->GetAmcModule();

          scheduledFlow->GetListOfAllocatedRBs()->push_back (s); // the s RB has been allocated to that flow!

DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
          cout << "\t *** RB " << s << " assigned to the "
                    " flow " << scheduledFlow->GetBearer ()->GetApplication ()->GetApplicationID ()
                    << endl;
DEBUG_LOG_END
          double sinr = amc->GetSinrFromCQI (scheduledFlow->GetCqiFeedbacks ().at (s));
          l_bFlowScheduledSINR[l_iScheduledFlowIndex].push_back(sinr);

          int mcs;
          if (FrameManager::Init()->isMbsfnSubframe()==true)
            {
              int burstSize = ((TraceBased*)(scheduledFlow->GetBearer()->GetApplication()))->GetBurstSize();
              double maxDelay = scheduledFlow->GetBearer()->GetApplication()->GetQoSParameters()->GetMaxDelay();
              int minTbs;
              switch(FrameManager::Init()->GetMbsfnPattern())
                {
                  case 1:
                    minTbs = round( burstSize / (maxDelay * 1000 * 1. / 10.) * 8 );
                    break;
                  case 2:
                    minTbs = round( burstSize / (maxDelay * 1000 * 3. / 10.) * 8 );
                    break;
                  case 3:
                    minTbs = round( burstSize / (maxDelay * 1000 * 6. / 10.) * 8 );
                    break;
                  default:
                    minTbs = round( burstSize / (maxDelay * 1000 * 1. / 10.) * 8 );
                    break;
                }

              for(mcs=0; mcs<27; mcs++)
                {
                  if (amc->GetTBSizeFromMCS (mcs, nbOfRBs)>minTbs)
                    {
                      break;
                    }
                }
            }
          else
            {
              mcs = amc->GetMCSFromSinrVector (l_bFlowScheduledSINR[l_iScheduledFlowIndex]);
            }

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
            default:
              cout << "Error: invalid transmission mode " << txMode <<endl;
              break;
            }
          int transportBlockSize = amc->GetTBSizeFromMCS (mcs, mcs, scheduledFlow->GetListOfAllocatedRBs ()->size (),rank);
          if (transportBlockSize >= scheduledFlow->GetDataToTransmit() * 8)
            {
              l_bFlowScheduled[l_iScheduledFlowIndex] = true;
              l_iScheduledFlows++;
            }

        }
    }

  delete [] l_bFlowScheduled;
  delete [] l_bFlowScheduledSINR;


  //Finalize the allocation
  PdcchMapIdealControlMessage *pdcchMsg = new PdcchMapIdealControlMessage ();

  for (auto flow : *flows)
    {
      if (flow->GetListOfAllocatedRBs ()->size () > 0)
        {
          //this flow has been scheduled
          AMCModule* amc = flow->GetUe()->GetProtocolStack()->GetMacEntity()->GetAmcModule();
          vector<double> estimatedSinrValues;
          for (int rb = 0; rb < (int)flow->GetListOfAllocatedRBs ()->size (); rb++ )

            {
              double sinr = amc->GetSinrFromCQI (
                              flow->GetCqiFeedbacks ().at (flow->GetListOfAllocatedRBs ()->at (rb)));

              estimatedSinrValues.push_back (sinr);
            }

          //get the MCS for transmission
          int mcs;
          if (FrameManager::Init()->isMbsfnSubframe()==true)
            {
              int burstSize = ((TraceBased*)(flow->GetBearer()->GetApplication()))->GetBurstSize();
              double maxDelay = flow->GetBearer()->GetApplication()->GetQoSParameters()->GetMaxDelay();
              int minTbs;
              switch(FrameManager::Init()->GetMbsfnPattern())
                {
                  case 1:
                    minTbs = round( (float) burstSize / (maxDelay * 1000 * 1. / 10.) * 8 );
                    break;
                  case 2:
                    minTbs = round( (float) burstSize / (maxDelay * 1000 * 3. / 10.) * 8 );
                    break;
                  case 3:
                    minTbs = round( (float) burstSize / (maxDelay * 1000 * 6. / 10.) * 8 );
                    break;
                  default:
                    minTbs = round( (float) burstSize / (maxDelay * 1000 * 1. / 10.) * 8 );
                    break;
                }
              for(mcs=0; mcs<27; mcs++)
                {
                  if (amc->GetTBSizeFromMCS (mcs, nbOfRBs)>minTbs)
                    {
                      break;
                    }
                }
            }
          else
            {
              mcs = amc->GetMCSFromSinrVector (estimatedSinrValues);
            }

          //define the amount of bytes to transmit
          //int transportBlockSize = amc->GetTBSizeFromMCS (mcs);
          int transportBlockSize;
          int txMode =  flow->GetUe()->GetTargetNodeRecord()->GetDlTxMode();
          int M, N, rank;
          vector <int> pmi;
          switch(txMode)
            {
            case 1:
            case 2:
              transportBlockSize = amc->GetTBSizeFromMCS (mcs, flow->GetListOfAllocatedRBs ()->size ());
              rank = 1;
              pmi.resize(0); // not used
              break;

            case 3:
              M = flow->GetBearer ()->GetSource ()->GetPhy()->GetTxAntennas();
              N = flow->GetBearer ()->GetDestination ()->GetPhy()->GetRxAntennas();
              rank = flow->GetRiFeedback();
              if(rank < 1 || rank > min(M,N))
                {
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
              transportBlockSize = amc->GetTBSizeFromMCS (mcs, mcs, flow->GetListOfAllocatedRBs ()->size (), rank );
              break;

            case 4:
            case 9:
              M = flow->GetBearer ()->GetSource ()->GetPhy()->GetTxAntennas();
              N = flow->GetBearer ()->GetDestination ()->GetPhy()->GetRxAntennas();
              rank = flow->GetRiFeedback ();
              if(rank < 1 || rank > min(M,N))
                {
                  rank = min(M,N);
                }
              transportBlockSize = amc->GetTBSizeFromMCS (mcs, mcs, flow->GetListOfAllocatedRBs ()->size (), rank );
              break;
            }
          double bitsToTransmit = transportBlockSize;
          flow->UpdateAllocatedBits (bitsToTransmit);

DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
          cout << "\t\t --> flow " << flow->GetBearer ()->GetApplication ()->GetApplicationID ()
                    << " has been scheduled: " <<
                    "\n\t\t\t nb of RBs " << flow->GetListOfAllocatedRBs ()->size () <<
                    "\n\t\t\t effectiveSinr " << GetMiesmEffectiveSinr(estimatedSinrValues) <<
                    "\n\t\t\t tbs " << transportBlockSize <<
                    "\n\t\t\t bitsToTransmit " << bitsToTransmit
                    << endl;
DEBUG_LOG_END
          int harqPid = 0;

DEBUG_LOG_START_1(SIM_ENV_DL_DEBUG)
          cout << "DL_DEBUG "
                    << flow->GetBearer ()->GetApplication ()-> GetDestination ()-> GetMobilityModel ()->GetAbsolutePosition ()->GetCoordinateX () << " "
                    << flow->GetBearer ()->GetApplication ()-> GetDestination ()-> GetMobilityModel ()->GetAbsolutePosition ()->GetCoordinateY () << " "
                    << GetMiesmEffectiveSinr(estimatedSinrValues) << " "
                    << mcs << " "
                    << transportBlockSize
                    << endl;
DEBUG_LOG_END
          //create PDCCH messages
          for (int rb = 0; rb < (int)flow->GetListOfAllocatedRBs ()->size (); rb++ )
            {
              if(txMode == 4 || txMode == 9)
                {
                  pmi = flow->GetPmiFeedbacks ().at(flow->GetListOfAllocatedRBs ()->at (rb));
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
                }
              if (flow->GetBearer ()->GetDestination ()->GetNodeType() == NetworkNode::TYPE_MULTICAST_DESTINATION)
                {
                  // schedule this RB to each user of the multicast group
                  MulticastDestination* virtualDest = (MulticastDestination*)flow->GetBearer ()->GetDestination ();
                  int nbOfMulticastUsers = 0;
                  for(auto dest : virtualDest->GetDestinations())
                    {
                      pdcchMsg->AddNewRecord (PdcchMapIdealControlMessage::DOWNLINK,
                                              flow->GetListOfAllocatedRBs ()->at (rb),
                                              dest,
                                              mcs, harqPid, rank, pmi);
                      // XXX: temporary hack to detect users which made handover to another cell
                      if (flow->GetBearer()->GetSource() == ((UserEquipment*)dest)->GetTargetNode())
                        {
                          nbOfMulticastUsers++;
                        }
                    }
                  cout << "MULTICAST_INFO " << nbOfMulticastUsers << " active users" << endl;
                }
              else
                {
              pdcchMsg->AddNewRecord (PdcchMapIdealControlMessage::DOWNLINK,
                                      flow->GetListOfAllocatedRBs ()->at (rb),
                                      flow->GetBearer ()->GetDestination (),
                                      mcs, harqPid, rank, pmi);
                }
            }
        }
    }

  if (pdcchMsg->GetMessage()->size () > 0)
    {
      GetMacEntity ()->GetDevice ()->GetPhy ()->SendIdealControlMessage (pdcchMsg);
    }
}
