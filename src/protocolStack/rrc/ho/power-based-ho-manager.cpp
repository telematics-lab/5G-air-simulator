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
  targetRXpower = 10*log10(targetRXpower) + GetHandoverMargin();

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
