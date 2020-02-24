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
 * Author: Alessandro Grassi <alessandro.grassi@poliba.it>
 */


#include "propagation-loss-model.h"
#include "channel-realization.h"
#include "../../core/spectrum/transmitted-signal.h"
#include "../../load-parameters.h"
#include "../../device/NetworkNode.h"
#include <vector>
#include <iostream>
#include <math.h>


PropagationLossModel::~PropagationLossModel()
{
  m_channelRealizationMap.clear ();
}

void
PropagationLossModel::AddChannelRealization (ChannelRealization* chRealization)
{
  ChannelRealizationId_t idMap = make_pair (chRealization->GetSourceNode (),
                                 chRealization->GetDestinationNode ());
  m_channelRealizationMap.insert (
    pair <ChannelRealizationId_t, ChannelRealization* > (idMap, chRealization));
}

void
PropagationLossModel::DelChannelRealization (NetworkNode* src, NetworkNode* dst)
{
  ChannelRealizationId_t idMap = make_pair (src,dst);

  if (m_channelRealizationMap.find (idMap) != m_channelRealizationMap.end ())
    {
      m_channelRealizationMap.find (idMap)->second->Destroy ();
      m_channelRealizationMap.erase (idMap);
    }
}


ChannelRealization*
PropagationLossModel::GetChannelRealization (NetworkNode* src, NetworkNode* dst)
{
  ChannelRealizationId_t idMap = make_pair (src, dst);
  if (m_channelRealizationMap.count (idMap)>0)
    {
      return m_channelRealizationMap.find (idMap)->second;
    }
  else
    {
      return nullptr;
    }
}

PropagationLossModel::ChannelRealizationMap
PropagationLossModel::GetChannelRealizationMap (void)
{
  return m_channelRealizationMap;
}

ReceivedSignal*
PropagationLossModel::AddLossModel (NetworkNode* src,
                                    NetworkNode* dst,
                                    TransmittedSignal* txSignal)
{

DEBUG_LOG_START_1(SIM_ENV_TEST_PROPAGATION_LOSS_MODEL)
  cout << "\t  --> add loss between "
            << src->GetIDNetworkNode () << " and " << dst->GetIDNetworkNode () << endl;
DEBUG_LOG_END

  ReceivedSignal* rxSignal = txSignal->Copy ();

  /*
   * The loss propagation model is based on
   * a on a combination of four different models:
   * - the path loss
   * - the penetration loss
   * - the shadowind
   * - the multipath
   *
   * The rxPsd will be obtained considering, for each sub channel, the following
   * relations:
   * rxPsd (i) = txPsd (i) + m(i,t) - sh(i,t) - pnl(i,t) - pl (a,b);
   * where i is the i-th sub-channel and t is the current time (Simulator::Now()).
   */


  ChannelRealization* c = GetChannelRealization (src, dst);

  if(c!=nullptr)
    {
      vector< vector<double> > rxSignalValues;
      rxSignalValues = rxSignal->GetValues ();
      int nbOfTransmitAntennas;

      if(c->isMbsfnRealization())
        {
          nbOfTransmitAntennas = 1;
          rxSignal->SetIsMBSFNSignal(true);
//if(rxSignal->GetIsMBSFNSignal())cout << "MBSFN signal in propagation-loss-model" << endl;
          vector< vector<double> > temp(1, vector<double>(rxSignalValues[0].size(),0));
          //cout << temp.size() << "||" << temp[0].size();
          vector< vector<double> >::iterator row;
          vector<double>::iterator col;
          vector< vector<double> >::iterator i;
          vector<double>::iterator j;

          // sum the power from all the transmit antennas
          for (row = rxSignalValues.begin(); row != rxSignalValues.end(); row++)
            {
              i = temp.begin();
              j = i->begin();
              for (col = row->begin(); col != row->end(); col++)
                {
                  (*j) = 10*log10( pow(10.,(*j)/10) + pow(10.,(*col)/10));
                  j++;
                }
            }

          rxSignalValues = temp;
          if( nbOfTransmitAntennas != (int)temp.size () )
            {
              cout << "Error: TX signal dimension does not match the number of TX antennas" << endl;
              exit(1);
            }
        }
      else
        {
          nbOfTransmitAntennas = src->GetPhy ()->GetTxAntennas ();
          if( nbOfTransmitAntennas != (int)rxSignalValues.size () )
            {
              cout << "Error: TX signal dimension does not match the number of TX antennas" << endl;
              exit(1);
            }
        }

      int nbOfReceiveAntennas = dst->GetPhy ()->GetRxAntennas ();
      rxSignalValues.resize ( rxSignalValues.size () * nbOfReceiveAntennas );

      for (int i = nbOfTransmitAntennas * nbOfReceiveAntennas - 1; i > 0; i--)
        {
          /*
           * Warning: i/nbOfReceiveAntennas is an integer division, on purpose.
           * This loop copies each component of txSignalValues to "nbOfReceiveAntennas"
           * components of rxSignalValues.
           */
          rxSignalValues.at (i) = rxSignalValues.at (i/nbOfReceiveAntennas);
        }

      vector< vector<double> > loss = c->GetLoss ();

DEBUG_LOG_START_1(SIM_ENV_TEST_PROPAGATION_LOSS_MODEL)
      cout << "tx sub channels " << rxSignalValues.at (0).size () << " loss sub channels " << loss.at (0).size () << endl;
DEBUG_LOG_END

      int nbOfPaths = rxSignalValues.size ();
      int nbOfSubChannels = rxSignalValues.at (0).size ();

      for (int i = 0; i < nbOfPaths; i++)
        {
          for (int j = 0; j < nbOfSubChannels; j++)
            {
              double rxPower = rxSignalValues.at (i).at (j) + loss.at (i).at (j); // add propagation loss

DEBUG_LOG_START_1(SIM_ENV_TEST_PROPAGATION_LOSS_MODEL)
              cout << "\t\t path " << i << " sub channel = " << j
                        << " rxSignalValues = " << rxSignalValues.at (i).at (j)
                        << " loss = " << loss.at (i).at (j)
                        << " rxPower = " << rxPower
                        << endl;
DEBUG_LOG_END

              rxSignalValues.at (i).at (j) = rxPower; // in W/Hz
            }
        }
      rxSignal->SetValues (rxSignalValues);
      if(c->hasFastFading()==true)
        {
          rxSignal->SetPhases (c->GetPhases());
        }
    }
  else
    {
      cout << "ERROR: Channel Realization Not Found!" << endl;
      exit (1);
    }
  return rxSignal;
}
