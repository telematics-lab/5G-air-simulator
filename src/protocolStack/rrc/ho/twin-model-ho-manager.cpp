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

#include "twin-model-ho-manager.h"
#include "../../../device/NetworkNode.h"
#include "../../../device/UserEquipment.h"
#include "../../../device/GNodeB.h"
#include "../../../device/HeNodeB.h"
#include "../../../componentManagers/NetworkManager.h"
#include "../../../core/spectrum/bandwidth-manager.h"
#include "../../../phy/phy.h"
#include "../../../utility/ComputePathLoss.h"

TwinModelHoManager::TwinModelHoManager()
{
    m_target = nullptr;
    //SetHandoverMargin(0);
}


bool
TwinModelHoManager::CheckHandoverNeed (UserEquipment* ue)
{
    
    //double shadowing=0;
    /// calculate the power received  from its targetnode
    GNodeB *targetNode = ue->GetTargetNode ();
    int nbTargetTxAntennas = targetNode->GetPhy()->GetTxAntennas();
    
    double TXpower = 10 * log10 (
                                 pow (10., (targetNode->GetPhy()->GetTxPower() - 30)/10)
                                 /
                                 targetNode->GetPhy()->GetBandwidthManager()->GetDlSubChannels().size ()
                                 /
                                 nbTargetTxAntennas);
    double pathLoss;
    ChannelRealization* c_dl;
    c_dl = targetNode->GetPhy()->GetDlChannel()->GetPropagationLossModel()->GetChannelRealization(targetNode,ue);
    if (c_dl != nullptr)
    {
        
        pathLoss = c_dl->GetPathLoss()+c_dl->GetShadowing();
    }
    else
    {
        pathLoss = ComputePathLossForInterference(targetNode, ue, false);
    }
    
    double targetRXpower_0 = 0;
    double targetRXpower_1 = 0;
    double targetRXpower_2 = 0;
    for (int i = 0; i < nbTargetTxAntennas; i++)
    {
        targetRXpower_0 += pow(10., (TXpower - pathLoss + c_dl->GetBeamformingGain_cover(i,0))/10 );
        targetRXpower_1 += pow(10., (TXpower - pathLoss + c_dl->GetBeamformingGain_cover(i,1))/10 );
        targetRXpower_2 += pow(10., (TXpower - pathLoss + c_dl->GetBeamformingGain_cover(i,2))/10 );
    }
    
    
    double targetRXpower = max(targetRXpower_0,max(targetRXpower_1,targetRXpower_2));
    
    /*    DEBUG_LOG_START_1(SIM_ENV_HANDOVER_DEBUG)
     cout
     <<" RX POWER: "<<10*log10(targetRXpower)
     << " RX POWER_0: "<<targetRXpower_0<< " RX POWER_1: "<<targetRXpower_1<< " RX POWER_0: "<<targetRXpower_2
     <<" Time: "<<Simulator::Init()->Now()<<endl;
     DEBUG_LOG_END */
    
    targetRXpower = 10*log10(targetRXpower);  // power received from its own node
    
    /*    ofstream myfile;
     if(targetNode->GetIDNetworkNode()==1)
     {
     myfile.open("/home/dinesh/Desktop/5g-sim-dev-thales/powerplot/power1.txt",ios::out | ios::app );
     myfile<<targetRXpower<<endl;
     myfile.close();
     
     myfile.open("/home/dinesh/Desktop/5g-sim-dev-thales/powerplot/time1.txt",ios::out | ios::app );
     myfile<<Simulator::Init()->Now()<<endl;
     myfile.close();
     }
     if(targetNode->GetIDNetworkNode()==5)
     {
     myfile.open("/home/dinesh/Desktop/5g-sim-dev-thales/powerplot/power5.txt",ios::out | ios::app );
     myfile<<targetRXpower<<endl;
     myfile.close();
     myfile.open("/home/dinesh/Desktop/5g-sim-dev-thales/powerplot/time5.txt",ios::out | ios::app );
     myfile<<Simulator::Init()->Now()<<endl;
     myfile.close();
     }
     */
    //    targetRXpower = targetRXpower+ GetHandoverThresholdBefore();
    
    
    vector<double>powers;
    vector<double> enbIDs;
    for (auto probableNewTargetNode : *NetworkManager::Init ()->GetGNodeBContainer ())
    {
        if (probableNewTargetNode->GetIDNetworkNode () != targetNode->GetIDNetworkNode () )
        {
            int nbTxAntennas = probableNewTargetNode->GetPhy()->GetTxAntennas();
            
            TXpower = 10 * log10 (
                                  pow (10., (probableNewTargetNode->GetPhy()->GetTxPower() - 30)/10)
                                  /
                                  probableNewTargetNode->GetPhy()->GetBandwidthManager()->GetDlSubChannels().size ()
                                  /
                                  nbTxAntennas);
            c_dl = probableNewTargetNode->GetPhy()->GetDlChannel()->GetPropagationLossModel()->GetChannelRealization(probableNewTargetNode,ue);
            if (c_dl != nullptr)
            {
                pathLoss = c_dl->GetPathLoss()+ c_dl->GetShadowing();
                
            }
            else
            {
                pathLoss = ComputePathLossForInterference(probableNewTargetNode, ue, false);
            }
            
            double RXpower_0 = 0;
            double RXpower_1 = 0;
            double RXpower_2 = 0;
            for (int i = 0; i < nbTxAntennas; i++)
            {
                RXpower_0 += pow(10., (TXpower - pathLoss + c_dl->GetBeamformingGain_cover(i,0))/10 );
                RXpower_1 += pow(10., (TXpower - pathLoss + c_dl->GetBeamformingGain_cover(i,1))/10 );
                RXpower_2 += pow(10., (TXpower - pathLoss + c_dl->GetBeamformingGain_cover(i,2))/10 );
            }
            
            double RXpower = max(RXpower_0,max(RXpower_1,RXpower_2));  // power received from the other node
            RXpower = 10*log10(RXpower);
            powers.push_back(RXpower);
            enbIDs.push_back(probableNewTargetNode->GetIDNetworkNode());
            
            /*        if(targetNode->GetIDNetworkNode()==1&&probableNewTargetNode->GetIDNetworkNode()==5)
             {
             ofstream myfile;
             myfile.open("/home/dinesh/Desktop/5g-sim-dev-thales/powerplot/power1-5.txt",ios::out | ios::app );
             myfile<<RXpower<<endl;
             myfile.close();
             }
             
             if(targetNode->GetIDNetworkNode()==5&&probableNewTargetNode->GetIDNetworkNode()==1)
             {
             myfile.open("/home/dinesh/Desktop/5g-sim-dev-thales/powerplot/power5-1.txt",ios::out | ios::app );
             myfile<<RXpower<<endl;
             myfile.close();
             
             }
             
             if(targetNode->GetIDNetworkNode()==5&&probableNewTargetNode->GetIDNetworkNode()==1)
             {
             myfile.open("/home/dinesh/Desktop/5g-sim-dev-thales/powerplot/time2.txt",ios::out | ios::app );
             myfile<<Simulator::Init()->Now()<<endl;
             myfile.close();
             
             }  */
            
            if (RXpower > targetRXpower+GetHandoverThresholdBefore()) // if power from other node is greater than my node
            {
                if (NetworkManager::Init()->CheckHandoverPermissions(probableNewTargetNode,ue))
                {
                    targetRXpower = RXpower;  // my node power is updated
                    targetNode = probableNewTargetNode; // my target node is updated
                }
            }
        }
    }
    
    
    
    
    if (ue->GetTargetNode ()->GetIDNetworkNode () != targetNode->GetIDNetworkNode ())
    {
        m_target = targetNode;
        return true;
        
    }
    else
    {
        return false;
    }
    
}



bool
TwinModelHoManager::CheckDetachTimeForTwin (UserEquipment* twin)
{
    double targetRXpower;
    double pathLoss;
    //double shadowing;
    ChannelRealization* c_dl;
    double targetRXpowerOld; // stores the power received by the UE from its targetnode
    double targetRXpowerNew; // stores the power received byt the UE from its Newtarget node
    
    
    double targetRXpower_0 = 0;
    double targetRXpower_1 = 0;
    double targetRXpower_2 = 0;
    
    
    /////  find the transmission power of the OLD ENodeB
    
    UserEquipment* ue = NetworkManager::Init()->GetTwinUserEquipmentByID(twin->GetIDNetworkNode());
    
    
    GNodeB *oldTargetNode = ue->GetTargetNode();
    int nbTargetTxAntennasOld = oldTargetNode->GetPhy()->GetTxAntennas();
    
    double oldTXpower = 10 * log10 (
                                    pow (10., (oldTargetNode->GetPhy()->GetTxPower() - 30)/10)
                                    /
                                    oldTargetNode->GetPhy()->GetBandwidthManager()->GetDlSubChannels().size ()
                                    /
                                    nbTargetTxAntennasOld);
    
    ///// find the trannsmission power of the NEW ENodeB
    
    GNodeB *newTargetNode = twin->GetTargetNode ();
    int nbTargetTxAntennasNew = newTargetNode->GetPhy()->GetTxAntennas();
    
    double newTXpower = 10 * log10 (
                                    pow (10., (newTargetNode->GetPhy()->GetTxPower() - 30)/10)
                                    /
                                    newTargetNode->GetPhy()->GetBandwidthManager()->GetDlSubChannels().size ()
                                    /
                                    nbTargetTxAntennasNew);
    
    
    
    
    
    //////////////**********************************************************
    
    // find the power received by UE------->>>>OLD ENodeB
    
    ////////////////************************************************************
    
    targetRXpower=0;
    pathLoss=0;
    targetRXpower_0=0;
    targetRXpower_1=0;
    targetRXpower_2=0;
    
    c_dl = oldTargetNode->GetPhy()->GetDlChannel()->GetPropagationLossModel()->GetChannelRealization(oldTargetNode,ue);
    if (c_dl != nullptr)
    {
        
        pathLoss = c_dl->GetPathLoss()+c_dl->GetShadowing();
        
    }
    else
    {
        pathLoss = ComputePathLossForInterference(oldTargetNode, ue, false);
    }
    
    for (int i = 0; i < nbTargetTxAntennasOld; i++)
    {
        targetRXpower_0 += pow(10., (oldTXpower - pathLoss + c_dl->GetBeamformingGain_cover(i,0))/10 );
        targetRXpower_1 += pow(10., (oldTXpower - pathLoss + c_dl->GetBeamformingGain_cover(i,1))/10 );
        targetRXpower_2 += pow(10., (oldTXpower - pathLoss + c_dl->GetBeamformingGain_cover(i,2))/10 );
    }
    
    
    targetRXpower = max(targetRXpower_0,max(targetRXpower_1,targetRXpower_2));
    targetRXpower = 10*log10(targetRXpower);
    
    
    targetRXpowerOld = targetRXpower;
    
    //////////////**********************************************************
    
    // find the power received by UE------->>>>NEW ENodeB
    
    ////////////////************************************************************
    
    targetRXpower=0;
    pathLoss=0;
    //shadowing=0;
    targetRXpower_0=0;
    targetRXpower_1=0;
    targetRXpower_2=0;
    
    
    c_dl = newTargetNode->GetPhy()->GetDlChannel()->GetPropagationLossModel()->GetChannelRealization(newTargetNode,ue);
    if (c_dl != nullptr)
    {
        pathLoss = c_dl->GetPathLoss()+c_dl->GetShadowing();
        
    }
    else
    {
        pathLoss = ComputePathLossForInterference(newTargetNode, ue, false);
    }
    
    for (int i = 0; i < nbTargetTxAntennasNew; i++)
    {
        targetRXpower_0 += pow(10., (newTXpower - pathLoss + c_dl->GetBeamformingGain_cover(i,0))/10 );
        targetRXpower_1 += pow(10., (newTXpower - pathLoss + c_dl->GetBeamformingGain_cover(i,1))/10 );
        targetRXpower_2 += pow(10., (newTXpower - pathLoss + c_dl->GetBeamformingGain_cover(i,2))/10 );
    }
    
    
    targetRXpower = max(targetRXpower_0,max(targetRXpower_1,targetRXpower_2));
    
    targetRXpower = 10*log10(targetRXpower);
    
    targetRXpowerNew = targetRXpower;
    
    /// compare powers
    
    if (targetRXpowerNew > targetRXpowerOld+GetHandoverThresholdAfter())
    {
        // stop twin and do handover procedures with the new ENB
        NetworkManager::Init()->StopTwin(twin);
        twin->SetTwinTransmittingFlag(false);
        ue->SetTwinTransmittingFlag(false);
        ue->GetPhy()->SetTxPower(23);
        return true;
        
    }
    
    else if(targetRXpowerOld>targetRXpowerNew+GetHandoverThresholdAfter())
    {
        // continue transmitting with the oldENB without handover and stop twin
        NetworkManager::Init()->StopTwin(twin);
        twin->SetTwinTransmittingFlag(false);// StopTwin
        ue->SetTwinTransmittingFlag(false);
        ue->GetPhy()->SetTxPower(23);
        return false;
    }
    else
    {    // continue transmitting with Twin
        return false;
    }
    
    
    
    
}



void
TwinModelHoManager::CalculatePower(UserEquipment* ue)
{
    
    GNodeB* targetNode = ue->GetTargetNode();
    double pathLoss =0;
    ChannelRealization* c_dl;
    
    vector<double>powers;
    vector<double> enbIDs;
    for (auto probableNewTargetNode : *NetworkManager::Init ()->GetGNodeBContainer ())
    {
        int nbTxAntennas = probableNewTargetNode->GetPhy()->GetTxAntennas();
        
        double TXpower = 10 * log10 (
                                     pow (10., (probableNewTargetNode->GetPhy()->GetTxPower() - 30)/10)
                                     /
                                     probableNewTargetNode->GetPhy()->GetBandwidthManager()->GetDlSubChannels().size ()
                                     /
                                     nbTxAntennas);
        c_dl = probableNewTargetNode->GetPhy()->GetDlChannel()->GetPropagationLossModel()->GetChannelRealization(probableNewTargetNode,ue);
        if (c_dl != nullptr)
        {
            pathLoss = c_dl->GetShadowing();
            DEBUG_LOG_START_1(SIM_ENV_HANDOVER_DEBUG)
            cout<<" SRC: "<<probableNewTargetNode->GetIDNetworkNode()<<" DST: "<<ue->GetIDNetworkNode();
            cout<<" SHADOWING: "<< pathLoss;
            DEBUG_LOG_END
            
            pathLoss = c_dl->GetPathLoss()+ pathLoss;
            
            DEBUG_LOG_START_1(SIM_ENV_HANDOVER_DEBUG)
            cout<<" PATHLOSS: "<<c_dl->GetPathLoss();
            cout<<" TOTAL LOSS: "<<pathLoss;
            DEBUG_LOG_END
            
        }
        else
        {
            pathLoss = ComputePathLossForInterference(probableNewTargetNode, ue, false);
        }
        
        double RXpower_0 = 0;
        double RXpower_1 = 0;
        double RXpower_2 = 0;
        for (int i = 0; i < nbTxAntennas; i++)
        {
            RXpower_0 += pow(10., (TXpower - pathLoss + c_dl->GetBeamformingGain_cover(i,0))/10 );
            RXpower_1 += pow(10., (TXpower - pathLoss + c_dl->GetBeamformingGain_cover(i,1))/10 );
            RXpower_2 += pow(10., (TXpower - pathLoss + c_dl->GetBeamformingGain_cover(i,2))/10 );
        }
        
        double RXpower = max(RXpower_0,max(RXpower_1,RXpower_2));  // this is the power received from the other node
        DEBUG_LOG_START_1(SIM_ENV_HANDOVER_DEBUG)
        cout
        <<" RX POWER: "<<10*log10(RXpower)
        << " RX POWER_0: "<<RXpower_0<< " RX POWER_1: "<<RXpower_1<< " RX POWER_0: "<<RXpower_2
        <<" Time: "<<Simulator::Init()->Now()<<endl;
        DEBUG_LOG_END
        
        RXpower = 10*log10(RXpower);
        
//   TODO: CHECK GD what is this code?!?!?!?!?!??!?!?!!?!? never ever have explicit path (e.g., /home/dinesh/Desktop/5g-sim-dev-thales/powerplot/twin/timetwin.txt") in the code.
//        I commented everything, verify if it is neede, and in that case rewrite it
//        if(ue->IsTwin())
//        {
//            if(probableNewTargetNode->GetIDNetworkNode()==1)
//            {
//                ofstream myfile;
//                myfile.open("/home/dinesh/Desktop/5g-sim-dev-thales/powerplot/twin/twin1.txt",ios::out | ios::app );
//                myfile<<RXpower<<endl;
//                myfile.close();
//
//                myfile.open("/home/dinesh/Desktop/5g-sim-dev-thales/powerplot/twin/timetwin.txt",ios::out | ios::app );
//                myfile<<Simulator::Init()->Now()<<endl;
//                myfile.close();
//
//            }
//
//            if(probableNewTargetNode->GetIDNetworkNode()==5)
//            {
//                ofstream myfile;
//                myfile.open("/home/dinesh/Desktop/5g-sim-dev-thales/powerplot/twin/twin5.txt",ios::out | ios::app );
//                myfile<<RXpower<<endl;
//                myfile.close();
//            }
//        }
//        if(!ue->IsTwin())
//        {
//            if(probableNewTargetNode->GetIDNetworkNode()==1)
//            {
//                ofstream myfile;
//                myfile.open("/home/dinesh/Desktop/5g-sim-dev-thales/powerplot/ue/ue1.txt",ios::out | ios::app );
//                myfile<<RXpower<<endl;
//                myfile.close();
//
//
//                myfile.open("/home/dinesh/Desktop/5g-sim-dev-thales/powerplot/ue/timeue.txt",ios::out | ios::app );
//                myfile<<Simulator::Init()->Now()<<endl;
//                myfile.close();
//
//
//            }
//
//            if(probableNewTargetNode->GetIDNetworkNode()==5)
//            {
//                ofstream myfile;
//                myfile.open("/home/dinesh/Desktop/5g-sim-dev-thales/powerplot/ue/ue5.txt",ios::out | ios::app );
//                myfile<<RXpower<<endl;
//                myfile.close();
//            }
            
            /*if(probableNewTargetNode->GetIDNetworkNode()==1)//&&Simulator::Init()->Now()>1.586)
             {
             ofstream myfile;
             myfile.open("/home/dinesh/Desktop/5g-sim-dev-thales/powerplot/shadowing/forUeAfterHo1.txt",ios::out | ios::app );
             myfile<<c_dl->GetShadowing()<<endl;
             myfile.close();
             myfile.open("/home/dinesh/Desktop/5g-sim-dev-thales/powerplot/shadowing/forUeAfterHo1time.txt",ios::out | ios::app );
             myfile<<Simulator::Init()->Now()<<endl;
             myfile.close();
             
             }
             if(probableNewTargetNode->GetIDNetworkNode()==2)//&&Simulator::Init()->Now()>1.586)
             {
             ofstream myfile;
             myfile.open("/home/dinesh/Desktop/5g-sim-dev-thales/powerplot/shadowing/forUeAfterHo2.txt",ios::out | ios::app );
             myfile<<c_dl->GetShadowing()<<endl;
             myfile.close();
             }
             if(probableNewTargetNode->GetIDNetworkNode()==3)//&&Simulator::Init()->Now()>1.586)
             {
             ofstream myfile;
             myfile.open("/home/dinesh/Desktop/5g-sim-dev-thales/powerplot/shadowing/forUeAfterHo3.txt",ios::out | ios::app );
             myfile<<c_dl->GetShadowing()<<endl;
             myfile.close();
             }
             if(probableNewTargetNode->GetIDNetworkNode()==4)//&&Simulator::Init()->Now()>1.586)
             {
             ofstream myfile;
             myfile.open("/home/dinesh/Desktop/5g-sim-dev-thales/powerplot/shadowing/forUeAfterHo4.txt",ios::out | ios::app );
             myfile<<c_dl->GetShadowing()<<endl;
             myfile.close();
             }
             if(probableNewTargetNode->GetIDNetworkNode()==5)//&&Simulator::Init()->Now()>1.586)
             {
             ofstream myfile;
             myfile.open("/home/dinesh/Desktop/5g-sim-dev-thales/powerplot/shadowing/forUeAfterHo5.txt",ios::out | ios::app );
             myfile<<c_dl->GetShadowing()<<endl;
             myfile.close();
             }
             if(probableNewTargetNode->GetIDNetworkNode()==0)//&&Simulator::Init()->Now()>1.586)
             {
             ofstream myfile;
             myfile.open("/home/dinesh/Desktop/5g-sim-dev-thales/powerplot/shadowing/forUeAfterHo0.txt",ios::out | ios::app );
             myfile<<c_dl->GetShadowing()<<endl;
             myfile.close();
             }*/
            
            
            
//        }
    }
    
}


void
TwinModelHoManager::SetHandoverThresholdBefore(double thresholdBefore) {
    m_handoverThresholdBefore = thresholdBefore;
}

double
TwinModelHoManager::GetHandoverThresholdBefore(void) {
    return m_handoverThresholdBefore;
}

void
TwinModelHoManager::SetHandoverThresholdAfter(double thresholdAfter) {
    m_handoverThresholdAfter = thresholdAfter;
}

double
TwinModelHoManager::GetHandoverThresholdAfter(void) {
    return m_handoverThresholdAfter;
}
