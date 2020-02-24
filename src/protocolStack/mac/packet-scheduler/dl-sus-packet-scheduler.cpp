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

#include <armadillo>
#include "dl-sus-packet-scheduler.h"
#include "../../../flows/radio-bearer.h"
#include "../../../flows/application/Application.h"
#include "../mac-entity.h"
#include "../../../device/NetworkNode.h"

DL_SUS_PacketScheduler::DL_SUS_PacketScheduler()
{
  SetMacEntity (nullptr);
  CreateFlowsToSchedule ();
}

DL_SUS_PacketScheduler::~DL_SUS_PacketScheduler()
{
  Destroy ();
}

void
DL_SUS_PacketScheduler::DoSchedule (void)
{
  DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
  cout << "Start DL packet scheduler for node "
            << GetMacEntity ()->GetDevice ()->GetIDNetworkNode()<< endl;
  DEBUG_LOG_END

  UpdateAverageTransmissionRate ();
  SelectFlowsToSchedule ();
  SelectSemiOrthogonalFlows ();

  if (GetFlowsToSchedule ()->size() == 0)
    {}
  else
    {
      RBsAllocation ();
    }

  StopSchedule ();
  ClearFlowsToSchedule ();
}


void
DL_SUS_PacketScheduler::SelectSemiOrthogonalFlows (void)
{
  FlowsToSchedule* flows = GetFlowsToSchedule();
  FlowsToSchedule allFlows = *flows;
  FlowsToSchedule newFlows;
  vector<int> availableFlows, selectedFlows;
  vector<arma::cx_fmat> H, G, G_select;
  H.resize(allFlows.size());
  G.resize(allFlows.size());
  G_select.clear();
  int nbTxAntennas = GetMacEntity()->GetDevice()->GetPhy()->GetTxAntennas();
  double alpha = 0.3;


  // retrieve CSI of users
  for (int i=0; i<(int)allFlows.size(); i++ )
    {
      availableFlows.push_back(i);
      H.at(i) = *(allFlows.at(i)->GetFullCsiFeedbacks().at(0));
    }

  for (int i=0; i<(int)allFlows.size(); i++ )
    {
      // calculate orthogonal components
      for (int k : availableFlows)
        {
          arma::cx_fmat sum = arma::zeros<arma::cx_fmat>(nbTxAntennas,nbTxAntennas);
          for (int j=0; j<i-1; j++)
            {
              sum += arma::trans(G_select.at(j))*G_select.at(j)/pow(arma::norm(G_select.at(j)),2);
            }
          G.at(k) = H.at(k) * (arma::eye<arma::fmat>(nbTxAntennas,nbTxAntennas) - sum);
        }

      // select best user
      double max_comp = -INFINITY;
      int best_user = 0;

      for (int k : availableFlows)
        {
          double orth_comp = arma::accu( // norm?
                               arma::abs(
                                 arma::diagvec(
                                   G.at(k)*arma::trans(G.at(k))
                                 )
                               )
                             );
          if (orth_comp>max_comp)
            {
              best_user = k;
            }
        }
//cout << "SUS best user: " << best_user <<endl;
      selectedFlows.push_back(best_user);

      //correggere
      //H.at(i) = H.at(best_user);
      G_select.push_back(G.at(best_user));

      vector<int> availableFlows_next;

      for (int k : availableFlows)
        {
          if (k != best_user)
            {
              double similarity =
                arma::accu(
                  arma::abs(
                    arma::diagvec(
                      H.at(k)
                      * arma::trans(G_select.at(i))
                    )
                  )
                )
                / (
                  arma::accu(
                    arma::abs(
                      arma::sqrt(
                        H.at(k)
                        * arma::trans(H.at(k))
                      )
                    )
                  )
                  * arma::accu(
                    arma::abs(
                      arma::sqrt(
                        G_select.at(i)
                        * arma::trans(G_select.at(i))
                      )
                    )
                  )
                );
              cout << "SUS ue " << k << " vs " << i << " sim " << similarity << endl;
              if (similarity < alpha)
                {
                  availableFlows_next.push_back(k);
                }
            }
        }

      availableFlows = availableFlows_next;

      // exit from main loop if conditions are met
      // condizione sul throuhgput
      if (availableFlows.size()==0 || (int)selectedFlows.size()==GetMaxUsersPerRB())
        {
          break;
        }
    }

  for (int i : selectedFlows)
    {
      newFlows.push_back(allFlows.at(i));
    }
  *flows = newFlows;
  return;
}

double
DL_SUS_PacketScheduler::ComputeSchedulingMetric (RadioBearer *bearer, double spectralEfficiency, int subChannel)
{
  return 1;
}
