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
 * Author: Alessandro Grassi <alessandro.grassi@poliba.it>
 */

#include <iostream>
#include <algorithm>
#include "multicast-destination-phy.h"
#include "../device/GNodeB.h"
#include "../device/MulticastDestination.h"
#include "../core/spectrum/bandwidth-manager.h"
#include "../protocolStack/mac/AMCModule.h"
#include "../device/CqiManager/cqi-manager.h"
#include "../componentManagers/FrameManager.h"

MulticastDestinationPhy::MulticastDestinationPhy()
{
  Simulator::Init()->Schedule(0.001,&MulticastDestinationPhy::CreateCqiFeedbacks,this);
}

void
MulticastDestinationPhy::CreateCqiFeedbacks ()
{
  // CQI for the multicast group
  vector<int> cqi;
  vector<double> sinr;

  // CQI of each user in the group (first vector is for the RBs, second for the UEs)
  vector< vector<int> > ue_cqi;
  int nbRBs = GetBandwidthManager()->GetDlSubChannels().size();

  cqi.resize(nbRBs);
  sinr.resize(nbRBs);
  ue_cqi.resize(nbRBs);
  for (int i=0; i<nbRBs; i++)
    {
      ue_cqi.at(i).clear();
    }

  int nbRxAntennas = GetRxAntennas();
  int nbTxAntennas = GetDevice()->GetTargetNode()->GetPhy()->GetTxAntennas();
  m_rankForRiFeedback = min(nbRxAntennas,nbTxAntennas);

  for( auto ue : GetDevice ()->GetDestinations() )
    {
      UePhy* uePhy = (UePhy*)ue->GetPhy();
      GNodeB* gnb = ue->GetTargetNode();
      CqiManager* cqiManager = ue->GetCqiManager();
      vector<int> temp_cqi = cqiManager->CalculateCqiFeedbacks(uePhy->GetMeasuredMBSFNSinr());
//      if(gnb==GetDevice()->GetTargetNode() && (int)temp_cqi.size() > 0)
      vector<GNodeB*> sources = GetDevice()->GetSources ();
      if ( find(
             sources.begin(),
             sources.end (),
             gnb
           ) != sources.end () )
        {
          if ((int)temp_cqi.size() > 0)
            {
              for (int i=0; i<nbRBs; i++)
                {
                  ue_cqi.at(i).push_back(temp_cqi.at(i));
                }
              m_rankForRiFeedback = min(m_rankForRiFeedback,uePhy->GetRankForRiFeedback());
            }
        }
    }

  if(ue_cqi.at(0).size()>0)
    {
      AMCModule* amc = GetDevice()->GetProtocolStack()->GetMacEntity()->GetAmcModule();
      for (int i=0; i<nbRBs; i++)
        {
          sort(ue_cqi.at(i).begin(), ue_cqi.at(i).end());

          int cqi_index;
          //      if (FrameManager::Init()->MbsfnEnabled()==true)
          //        {
          //          // for MBSFN, take the lowest CQI
          //          cqi_index = 0;
          //        }
          //      else
          {
            // for SC-PTM, take the 95% lowest CQI
            cqi_index = floor(ue_cqi.at(i).size() * 0.1);
          }


          sinr.at(i) = amc->GetSinrFromCQI( ue_cqi.at(i).at(cqi_index));
        }
      GetDevice()->GetCqiManager()->CreateCqiFeedbacks(sinr);

      int txMode = GetDevice ()->GetTargetNodeRecord()->GetDlTxMode();
      if(txMode == 3)
        {
          GetDevice()->GetCqiManager ()->CreateRiFeedback (m_rankForRiFeedback);
        }
    }
  Simulator::Init()->Schedule(0.001,&MulticastDestinationPhy::CreateCqiFeedbacks,this);
}


MulticastDestination*
MulticastDestinationPhy::GetDevice(void)
{
  Phy* phy = (Phy*)this;
  return (MulticastDestination*)phy->GetDevice();
}
