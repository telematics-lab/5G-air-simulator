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
 * Author: Francesco Capozzi <f.capozzi@poliba.it>
 * Author: Alessandro Grassi <alessandro.grassi@poliba.it>
 */


#include "power-based-ho-manager.h"
#include "../../../device/NetworkNode.h"
#include "../../../device/UserEquipment.h"
#include "../../../device/GNodeB.h"
#include "../../../device/HeNodeB.h"
#include "../../../componentManagers/NetworkManager.h"
#include "../../../core/spectrum/bandwidth-manager.h"
#include "../../../phy/phy.h"
#include "../../../utility/ComputePathLoss.h"

//TODO: CHECK GD I am not sure that modification are needed here, everything should go inside twin-model-ho-manager class, shouldn't it?

PowerBasedHoManager::PowerBasedHoManager()
{
  m_target = nullptr;
  SetHandoverMargin(0);
}

bool
PowerBasedHoManager::CheckHandoverNeed (UserEquipment* ue)
{
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
      pathLoss = c_dl->GetPathLoss() + c_dl->GetShadowing();
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

  targetRXpower = 10*log10(targetRXpower); //TODO: CHECK GD WHY?!

DEBUG_LOG_START_1(SIM_ENV_HANDOVER_DEBUG)
  cout << "HO power-based, UE " << ue->GetIDNetworkNode() << ", target RX power " << targetRXpower << endl;
DEBUG_LOG_END

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
              pathLoss = c_dl->GetPathLoss() + c_dl->GetShadowing();
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

          double RXpower = max(RXpower_0,max(RXpower_1,RXpower_2));
          RXpower = 10*log10(RXpower);


DEBUG_LOG_START_1(SIM_ENV_HANDOVER_DEBUG)
          cout << "HO power-based, UE " << ue->GetIDNetworkNode()
                    << ", RX power " << RXpower
                    << " from gNB " << probableNewTargetNode->GetIDNetworkNode() << endl;
DEBUG_LOG_END
          if (RXpower > targetRXpower + GetHandoverThresholdBefore()) //TODO: CHECK GD WHY?!
            {
              if (NetworkManager::Init()->CheckHandoverPermissions(probableNewTargetNode,ue))
                {
                  targetRXpower = RXpower;
                  targetNode = probableNewTargetNode;
                }
            }
        }
    }

  for (auto probableNewTargetNode : *NetworkManager::Init ()->GetHomeGNodeBContainer())
    {
      if (probableNewTargetNode->GetIDNetworkNode () != targetNode->GetIDNetworkNode () )
        {
          int nbTxAntennas = probableNewTargetNode->GetPhy()->GetTxAntennas();

          TXpower =   10 * log10 (
                        pow (10., (probableNewTargetNode->GetPhy()->GetTxPower() - 30)/10)
                        /
                        probableNewTargetNode->GetPhy()->GetBandwidthManager()->GetDlSubChannels().size ()
                        /
                        nbTxAntennas);
          c_dl = probableNewTargetNode->GetPhy()->GetDlChannel()->GetPropagationLossModel()->GetChannelRealization(probableNewTargetNode,ue);
          if (c_dl != nullptr)
            {
              pathLoss = c_dl->GetPathLoss() + c_dl->GetShadowing();
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

          double RXpower = max(RXpower_0,max(RXpower_1,RXpower_2));
          RXpower = 10*log10(RXpower);


DEBUG_LOG_START_1(SIM_ENV_HANDOVER_DEBUG)
          cout << "HO power-based, UE " << ue->GetIDNetworkNode()
                    << ", RX power " << RXpower
                    << " from gNB " << probableNewTargetNode->GetIDNetworkNode() << endl;
DEBUG_LOG_END

          if (RXpower > targetRXpower)
            {
              if (NetworkManager::Init()->CheckHandoverPermissions(probableNewTargetNode,ue))
                {
                  targetRXpower = RXpower;
                  targetNode = probableNewTargetNode;
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
PowerBasedHoManager::CheckDetachTimeForTwin(UserEquipment* twin)
{
    return true;
}


void
PowerBasedHoManager::CalculatePower(UserEquipment* ue)
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
        
        
        //    powers.push_back(RXpower);
        //enbIDs.push_back(probableNewTargetNode->GetIDNetworkNode());
        //   TODO: CHECK GD what is this code?!?!?!?!?!??!?!?!!?!? never ever have explicit path (e.g., /home/dinesh/Desktop/5g-sim-dev-thales/powerplot/twin/timetwin.txt") in the code.
        
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
//
//            if(probableNewTargetNode->GetIDNetworkNode()==1)//&&Simulator::Init()->Now()>1.586)
//            {
//                ofstream myfile;
//                myfile.open("/home/dinesh/Desktop/5g-sim-dev-thales/powerplot/shadowing/forUeAfterHo1.txt",ios::out | ios::app );
//                myfile<<c_dl->GetShadowing()<<endl;
//                myfile.close();
//                myfile.open("/home/dinesh/Desktop/5g-sim-dev-thales/powerplot/shadowing/forUeAfterHo1time.txt",ios::out | ios::app );
//                myfile<<Simulator::Init()->Now()<<endl;
//                myfile.close();
//
//            }
//            if(probableNewTargetNode->GetIDNetworkNode()==2)//&&Simulator::Init()->Now()>1.586)
//            {
//                ofstream myfile;
//                myfile.open("/home/dinesh/Desktop/5g-sim-dev-thales/powerplot/shadowing/forUeAfterHo2.txt",ios::out | ios::app );
//                myfile<<c_dl->GetShadowing()<<endl;
//                myfile.close();
//            }
//            if(probableNewTargetNode->GetIDNetworkNode()==3)//&&Simulator::Init()->Now()>1.586)
//            {
//                ofstream myfile;
//                myfile.open("/home/dinesh/Desktop/5g-sim-dev-thales/powerplot/shadowing/forUeAfterHo3.txt",ios::out | ios::app );
//                myfile<<c_dl->GetShadowing()<<endl;
//                myfile.close();
//            }
//            if(probableNewTargetNode->GetIDNetworkNode()==4)//&&Simulator::Init()->Now()>1.586)
//            {
//                ofstream myfile;
//                myfile.open("/home/dinesh/Desktop/5g-sim-dev-thales/powerplot/shadowing/forUeAfterHo4.txt",ios::out | ios::app );
//                myfile<<c_dl->GetShadowing()<<endl;
//                myfile.close();
//            }
//            if(probableNewTargetNode->GetIDNetworkNode()==5)//&&Simulator::Init()->Now()>1.586)
//            {
//                ofstream myfile;
//                myfile.open("/home/dinesh/Desktop/5g-sim-dev-thales/powerplot/shadowing/forUeAfterHo5.txt",ios::out | ios::app );
//                myfile<<c_dl->GetShadowing()<<endl;
//                myfile.close();
//            }
//            if(probableNewTargetNode->GetIDNetworkNode()==0)//&&Simulator::Init()->Now()>1.586)
//            {
//                ofstream myfile;
//                myfile.open("/home/dinesh/Desktop/5g-sim-dev-thales/powerplot/shadowing/forUeAfterHo0.txt",ios::out | ios::app );
//                myfile<<c_dl->GetShadowing()<<endl;
//                myfile.close();
//            }
//
//
//
//        }
    }
    
    
    
}
