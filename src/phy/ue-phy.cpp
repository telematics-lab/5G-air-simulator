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
 * Author: Sergio Martiradonna <sergio.martiradonna@poliba.it>
 */


#include <armadillo>
#include "ue-phy.h"
#include "../device/NetworkNode.h"
#include "../channel/RadioChannel.h"
#include "../core/spectrum/bandwidth-manager.h"
#include "../protocolStack/packet/packet-burst.h"
#include "../core/spectrum/transmitted-signal.h"
#include "../core/idealMessages/ideal-control-messages.h"
#include "../protocolStack/mac/AMCModule.h"
#include "../device/UserEquipment.h"
#include "../device/GNodeB.h"
#include "../device/HeNodeB.h"
#include "interference.h"
#include "error-model.h"
#include "../device/CqiManager/cqi-manager.h"
#include "../load-parameters.h"
#include "../core/eventScheduler/simulator.h"
#include "../protocolStack/mac/ue-mac-entity.h"
#include "../utility/eesm-effective-sinr.h"
#include "../utility/miesm-effective-sinr.h"
#include "gnb-phy.h"
#include "../utility/ComputePathLoss.h"
#include "../utility/mimo-codebook.h"
#include "sinr-calculator.h"
#include "precoding-calculator.h"
#include "../componentManagers/FrameManager.h"
#include "../protocolStack/mac/harq-manager.h"


UePhy::UePhy()
{
  m_channelsForRx.clear ();
  m_channelsForTx.clear ();
  m_mcsIndexForRx.clear ();
  m_mcsIndexForTx.clear ();
  m_pmiForRx.clear();
  m_rankForRx = 0;
  m_precodingMatricesForRx.clear();
  SetDevice (nullptr);
  SetDlChannel (nullptr);
  SetUlChannel (nullptr);
  SetBandwidthManager (nullptr);
  SetTxSignal (nullptr);
  SetErrorModel (nullptr);
  Interference *interference = new Interference ();
  SetInterference (interference);
  SetTxPower (23); //dBm
  SetTxAntennas(1);
  SetRxAntennas(1);
  GetAntennaParameters ()->SetType(Phy::AntennaParameters::ANTENNA_TYPE_OMNIDIRECTIONAL);
  GetAntennaParameters ()->SetBearing(0);

  if (UPLINK)
    {
      Simulator::Init()->Schedule(0.001, &UePhy::SetTxSignalForReferenceSymbols, this);
    }
}

UePhy::~UePhy()
{
  Destroy ();
}

void
UePhy::DoSetBandwidthManager (void)
{
  TransmittedSignal* txSignal = new TransmittedSignal ();

  vector< vector<double> > values;
  values.resize(1);
  for (auto channel : GetBandwidthManager ()->GetUlSubChannels ())
    {
      values.at(0).push_back(0);
    }

  if (m_channelsForTx.size () > 0)
    {
      double totPower = pow (10., (GetTxPower () - 30) / 10); // in natural unit
      double txPower = 10 * log10 (totPower / m_channelsForTx.size ()); //in dB
      for (auto channel : m_channelsForTx)
        {
          values.at(0).at (channel) = txPower;
        }
    }
  txSignal->SetValues (values);
  if (GetTxSignal() != NULL)
    {
      delete GetTxSignal();
    }
  SetTxSignal (txSignal);
}

void
UePhy::StartTx (shared_ptr<PacketBurst> p)
{
DEBUG_LOG_START_1(SIM_ENV_TEST_DEVICE_ON_CHANNEL)
  cout << "Node " << GetDevice()->GetIDNetworkNode () << " starts phy tx" << endl;
DEBUG_LOG_END

  GetUlChannel ()->StartTx (p, GetTxSignal (), GetDevice ());
}

void
UePhy::StartRx (shared_ptr<PacketBurst> p, ReceivedSignal* rxSignal)
{
DEBUG_LOG_START_1(SIM_ENV_TEST_DEVICE_ON_CHANNEL)
  cout << "Node " << GetDevice()->GetIDNetworkNode () << " starts phy rx" << endl;
DEBUG_LOG_END

  m_sinrForCQI.clear();

  //COMPUTE THE SINR
  vector< vector<double> > rxSignalValues;
  vector< vector<float> > rxSignalPhases;
  vector<double> sinrForBLER;

  rxSignalValues = rxSignal->GetValues();
  rxSignalPhases = rxSignal->GetPhases();
  bool isMbsfnSignal = rxSignal->GetIsMBSFNSignal();
  delete rxSignal;

  //compute noise + interference
  double interference;
  UserEquipment* ue = GetDevice();
  GNodeB* gnb = ue->GetTargetNode ();
  if (GetInterference () != nullptr)
    {
      interference = GetInterference ()->ComputeInterference (ue);
    }
  else
    {
      interference = 0;
    }

  double noise_interference = 10. * log10 (pow(10., GetThermalNoise()/10) + interference); // dB
  int txMode = ue->GetTargetNodeRecord()->GetDlTxMode();
  if (ue->GetMulticastDestination()!=nullptr)
    {
      if ( (FrameManager::Init()->MbsfnEnabled()==true && FrameManager::Init()->isMbsfnSubframe()==true)
           || FrameManager::Init()->MbsfnEnabled()==false )
        {
          txMode=1; //ue->GetTargetNodeRecord()->GetDlTxMode();
        }
    }
  int nbOfSubChannels = rxSignalValues.at (0).size ();
  int nbOfRxSubChannels = m_channelsForRx.size();
  int nbOfPaths = rxSignalValues.size ();
  int nbRxAntennas = GetRxAntennas();
  int nbTxAntennas = nbOfPaths / nbRxAntennas;
//if(isMbsfnSignal)cout << "MBSFN signal in ue-phy" << endl;
  if (
    (ue->GetCqiManager ()->NeedToSendFeedbacks () == false
      && nbOfRxSubChannels == 0
     && !(
       FrameManager::Init()->MbsfnEnabled()==true
       && FrameManager::Init()->isMbsfnSubframe()==true))
    || (FrameManager::Init()->MbsfnEnabled()==true
        && FrameManager::Init()->isMbsfnSubframe()==true
        && (isMbsfnSignal==false || ue->GetMulticastDestination() == nullptr))
  )
    {
      return;
    }

  if(FrameManager::Init()->MbsfnEnabled()==true && FrameManager::Init()->isMbsfnSubframe()==true)
    {
      m_measuredMBSFNSinr.clear();
    }

  double power; // power transmission for one sub channel [dB]
  switch(txMode)
    {
    case 1:
              if(FrameManager::Init()->isMbsfnSubframe()==true)
          {
            double mbsfnConstructiveInterference;
            if (GetInterference () != nullptr)
              {
                mbsfnConstructiveInterference = GetInterference ()->ComputeInterference (ue, true);
              }
            else
              {
                mbsfnConstructiveInterference = 0;
              }
            for (auto power : rxSignalValues.at (0))
              {
                m_sinrForCQI.push_back (10.*log10(pow(10,power/10.)+mbsfnConstructiveInterference) - noise_interference);
              }
            if(FrameManager::Init()->MbsfnEnabled()==true && FrameManager::Init()->isMbsfnSubframe()==true)
              {
                m_measuredMBSFNSinr = m_sinrForCQI;
              }

          }
        else if(nbRxAntennas==1)
        {
          for (auto power : rxSignalValues.at (0))
            {
              m_sinrForCQI.push_back (power - noise_interference);
            }
        }
      else
        {
          arma::vec powers = arma::vec(nbOfPaths); // power received from each path [W]
          arma::vec H0 = arma::vec(nbOfPaths); // linear gain of each path
          for (int i = 0; i < nbOfSubChannels; i++)
            {
              for( int j = 0; j < nbOfPaths; j++)
                {
                  powers(j) = pow(10,(long double)rxSignalValues.at (j).at (i)/10);
                }

              double avgPower = arma::mean(powers);
              H0 = sqrt(powers / avgPower);
              power = 10*log10( avgPower );

              double sum = 0;
              for( int j = 0; j < nbOfPaths; j++)
                {
                  sum += pow( H0(j), 2 );
                }
              m_sinrForCQI.push_back ( ( power + 10*log10(pow(sum,2) ) ) - ( noise_interference + 10*log10(sum) ) );
            }

DEBUG_LOG_START_1(CALIBRATION_STEP1)
          double step1a_power = gnb->GetPhy()->GetTxPower() - 30 - 10*log10(nbOfSubChannels);
          ChannelRealization* c_dl;
          if (FrameManager::Init()->MbsfnEnabled()==true && FrameManager::Init()->isMbsfnSubframe()==true)
            {
              c_dl = GetDlMcChannel ()->GetPropagationLossModel ()->GetChannelRealization(gnb,ue);
            }
          else
            {
              c_dl = GetDlChannel ()->GetPropagationLossModel ()->GetChannelRealization(gnb,ue);
            }
          double step1a_loss = c_dl->GetPathLoss() + c_dl->GetShadowing() + c_dl->GetPenetrationLoss();
          double step1a_sinr = step1a_power - step1a_loss - noise_interference;
          double step1c_sum = 0;
          for (auto sinr : m_sinrForCQI)
            {
              step1c_sum += pow(10,sinr/10);
            }
          double step1c_sinr = 10*log10(step1c_sum / nbOfSubChannels);

          cout << "CALIBRATION_1 PATHGAIN " << -step1a_loss
            << " SINR " << step1a_sinr
            << " SINR_FF " << step1c_sinr
            << endl;
DEBUG_LOG_END
        }
      sinrForBLER = m_sinrForCQI;
      break;

    case 2:
      for (int i = 0; i < nbOfSubChannels; i++)
        {
          arma::mat powers = arma::mat(nbRxAntennas,nbTxAntennas); // power received from each path [W]
          for( int j = 0; j < nbRxAntennas; j++)
            {
              for( int k = 0; k < nbTxAntennas; k++)
                {
                  powers(j,k)=pow(10,(long double)rxSignalValues.at (j*nbTxAntennas+k).at (i)/10);
                }
            }
          double avgPower = arma::mean( (arma::vec)arma::sum(arma::abs(powers),1) );
          double avgPathPower = arma::mean( arma::mean(arma::abs(powers)) );
          arma::mat H0 = arma::mat(nbRxAntennas,nbTxAntennas); // linear gain of each path
          H0 = sqrt(powers / avgPathPower);
          power = 10*log10( avgPower / 2 );
          if(nbTxAntennas==4)
            {
              H0 = H0 * arma::diagmat(W_SFBC_FSTD[(i/2)%2]);
            }
          double sum = arma::accu(H0 % H0);
          m_sinrForCQI.push_back ( ( power + 10*log10(pow(sum,2) ) ) - ( noise_interference + 10*log10(sum) ) );
        }
      sinrForBLER = m_sinrForCQI;
      break;

    case 3:
        {
          vector< vector<double> > measuredSinr;
          arma::cx_mat powers = arma::cx_mat(nbRxAntennas,nbTxAntennas); // power received from each path [W]

          int minLayers = 1;
          int maxLayers = min(nbRxAntennas,nbTxAntennas);

          if (ue->GetCqiManager ()->NeedToSendFeedbacks () == false)
            {
              minLayers = m_rankForRx;
              maxLayers = m_rankForRx;
            }

          // calculate SINR for each number of layers
          measuredSinr.resize(nbRxAntennas);
          for (int i = 0; i < nbOfSubChannels; i++)
            {
              for( int j = 0; j < nbRxAntennas; j++)
                {
                  for( int k = 0; k < nbTxAntennas; k++)
                    {
                      powers(j,k)=pow(10,(long double)rxSignalValues.at (j*nbTxAntennas+k).at (i)/10);
                    }
                }
              double avgPower = arma::mean( (arma::vec)arma::sum(arma::abs(powers),1) );
              double avgPathPower = arma::mean( arma::mean(arma::abs(powers)) );
              arma::cx_mat H0 = arma::cx_mat(nbRxAntennas,nbTxAntennas); // linear gain of each path
              for( int j = 0; j < nbRxAntennas; j++)
                {
                  for( int k = 0; k < nbTxAntennas; k++)
                    {
                      H0(j,k) =  polar( sqrt( abs( powers(j,k) ) / avgPathPower ), (double)rxSignalPhases[j*nbTxAntennas+k][i] );
                    }
                }
              for (int l=minLayers; l<=maxLayers; l++)
                {
                  if(l==1)
                    {
                      power = 10*log10( avgPower / 2 );
                      arma::cx_mat H0_TxD = H0;
                      if(nbTxAntennas==4)
                        {
                          H0_TxD = H0_TxD * arma::diagmat(W_SFBC_FSTD[(i/2)%2]);
                        }
                      double sum = arma::accu(abs(H0_TxD) % abs(H0_TxD));
                      measuredSinr.at(l-1).push_back ( ( power + 10*log10(pow(sum,2) ) ) - ( noise_interference + 10*log10(sum) ) );
                    }
                  else
                    {
                      arma::cx_mat precodedH0;
                      if (nbTxAntennas == 2 )
                        {
                          precodedH0 = H0 * (*mimo_codebook_2tx[0][l-1]) * (*CDD_D[l-1][i%l]) * (*CDD_U[l-1]);
                        }
                      if (nbTxAntennas == 4 )
                        {
                          precodedH0 = H0 * (*mimo_codebook_4tx[12+i%l][l-1]) * (*CDD_D[l-1][i%l]) * (*CDD_U[l-1]);
                        }
                      arma::cx_mat HHTN = avgPower * precodedH0 * arma::trans(precodedH0)
                                        + pow(10,noise_interference/10) * arma::eye<arma::mat>( nbRxAntennas, nbRxAntennas );
                      arma::cx_mat W = arma::inv( HHTN, true ) * sqrt(avgPower) * precodedH0;
                      arma::cx_mat correlation = arma::trans(W)*sqrt(avgPower)*precodedH0;
                      arma::cx_mat D = arma::diagmat(correlation);
                      arma::cx_mat Iself = correlation - D;
                      arma::vec SINRs = arma::abs( arma::diagvec(D*arma::trans(D)) )/ arma::abs( arma::diagvec(Iself*arma::trans(Iself) + pow(10,noise_interference/10)*arma::trans(W)*W) );
                      vector<double> sinrs = arma::conv_to< vector<double> >::from(10*log10(SINRs));
                      double effsinr = GetMiesmEffectiveSinr( sinrs );
                      measuredSinr.at(l-1).push_back ( effsinr );
                    }
                }
            }

          // compute RI feedback
          AMCModule *amc = GetDevice ()->GetProtocolStack ()->GetMacEntity ()->GetAmcModule ();
          int maxTBS = 0;
          m_rankForRiFeedback = 1;
          for (int l=minLayers; l<=maxLayers; l++)
            {
              vector<double> sinrForScheduledChannels;
              sinrForScheduledChannels.clear();
              for (auto channel : m_channelsForRx)
                {
                  sinrForScheduledChannels.push_back( measuredSinr.at(l-1).at(channel) );
                }
              if (sinrForScheduledChannels.size() == 0)
                {
                  sinrForScheduledChannels = measuredSinr.at(l-1);
                }
              int MCS = amc->GetMCSFromCQI(amc->GetCQIFromSinr( GetMiesmEffectiveSinr( sinrForScheduledChannels ) ) );
              int TBS = amc->GetTBSizeFromMCS(
                                MCS,
                                MCS,
                                sinrForScheduledChannels.size(),
                                l);
              if(TBS>maxTBS)
                {
                  maxTBS = TBS;
                  m_rankForRiFeedback = l;
                }
            }

        if(GetDevice()->GetMulticastDestination() != nullptr)
          {
            m_rankForRiFeedback = min(nbRxAntennas,nbTxAntennas);
          }
          // select SINR for CQI calculation
          m_sinrForCQI = measuredSinr.at(m_rankForRiFeedback-1);

          // select SINR for BLER calculation
          if(m_rankForRx < 1 || m_rankForRx > min(nbRxAntennas,nbTxAntennas))
            {
              m_rankForRx = min(nbRxAntennas,nbTxAntennas);
            }
          sinrForBLER = measuredSinr.at(m_rankForRx-1);
        }
      break;

    case 4:
    case 9:
        {
          vector< vector< vector<double> > > measuredSinr; //measured SINR for each number of layers, PMI and subchannel
          arma::cx_fmat powers = arma::cx_fmat(nbRxAntennas,nbTxAntennas); // power received from each path [W]

          int minLayers = 1;
          int maxLayers = min(nbRxAntennas,nbTxAntennas);


          if (nbTxAntennas == 8 && nbTxAntennas == 8)
            {
//              minLayers = 3;
            }

          if (ue->GetCqiManager ()->NeedToSendFeedbacks () == false)
            {
              minLayers = m_rankForRx;
              maxLayers = m_rankForRx;
            }

          measuredSinr.resize(maxLayers);

          arma::cx_fmat H0 = arma::cx_fmat(nbRxAntennas,nbTxAntennas); // linear gain of each path
          arma::cx_fmat receivedSignalLevels = arma::cx_fmat(nbRxAntennas,nbTxAntennas); // signal level of each path
          arma::cx_fmat precoding;
          arma::cx_fmat precodedH0;
          arma::cx_fmat HHTN;
          arma::cx_fmat W;
          arma::cx_fmat correlation;
          arma::cx_fmat D;
          arma::cx_fmat Iself;
          arma::vec SINRs;

          // calculate 1 sinr value for a group of sub-channels of size subBandSize
          int subBandSize;
          if (nbOfSubChannels<8)
            {
              subBandSize = 1;
            }
          else if (nbOfSubChannels>=8 && nbOfSubChannels<27)
            {
              subBandSize = 4;
            }
          else if (nbOfSubChannels>=27 && nbOfSubChannels<64)
            {
              subBandSize = 6;
            }
          else if (nbOfSubChannels>=64 && nbOfSubChannels<110)
            {
              subBandSize = 8;
            }

          // create valid PMI indexes if not receiving on all RBs
          if( (int)m_pmiForRx.size() < nbOfSubChannels )
            {
              int i = m_pmiForRx.size();
              m_pmiForRx.resize(nbOfSubChannels);
              for (; i < nbOfSubChannels; i++)
                {
                  if(nbTxAntennas == 8)
                    {
                      m_pmiForRx.at(i).resize(2);
                      m_pmiForRx.at(i).at(0) = 0;
                      m_pmiForRx.at(i).at(1) = 0;
                    }
                  else
                    {
                      m_pmiForRx.at(i).resize(1);
                      if (nbTxAntennas == 2 && m_rankForRx == 2)
                        {
                          m_pmiForRx.at(i).at(0) = 1;
                        }
                      else
                        {
                          m_pmiForRx.at(i).at(0) = 0;
                        }
                    }
                }
            }

          for (int i = 0; i < nbOfSubChannels; i++)
            {
              for( int j = 0; j < nbRxAntennas; j++)
                {
                  for( int k = 0; k < nbTxAntennas; k++)
                    {
                      powers(j,k)=pow(10,(long double)rxSignalValues.at (j*nbTxAntennas+k).at (i)/10);
                    }
                }

              double avgPower = arma::mean( (arma::fvec)arma::sum(arma::abs(powers),1) );
              double avgPathPower = arma::mean( arma::mean(arma::abs(powers)) );
              for( int j = 0; j < nbRxAntennas; j++)
                {
                  for( int k = 0; k < nbTxAntennas; k++)
                    {
                      H0(j,k) =  polar( (float)(sqrt( abs( powers(j,k) ) / avgPathPower )), rxSignalPhases[j*nbTxAntennas+k][i] );
                      receivedSignalLevels(j,k) =  polar( (float)sqrt( abs( powers(j,k) ) ), rxSignalPhases[j*nbTxAntennas+k][i] );
                    }
                }

              for (int l=minLayers; l<=maxLayers; l++)
                {
                  int min_pmi, max_pmi;
                  if (nbTxAntennas == 2 && l == 1)
                    {
                      min_pmi = 0;
                      max_pmi = 3;
                    }
                  else if (nbTxAntennas == 2 && l == 2)
                    {
                      min_pmi = 1;
                      max_pmi = 2;
                    }
                  else if (nbTxAntennas == 4)
                    {
                      min_pmi = 0;
                      max_pmi = 15;
                    }
                  else if (nbTxAntennas == 8)
                    {
                      min_pmi = 0;
                      // for 8 tx antennas, PMI is composed of two values, i1 and i2
                      // here, they are encoded as i1 * 16 + i2 for easier looping
                      max_pmi = 255;
                    }

                  measuredSinr.at(l-1).resize(max_pmi+1);

                  // restrict PMI choice if CQI feedback is not required
                  if (ue->GetCqiManager ()->NeedToSendFeedbacks () == false)
                    {

                    if(find(m_channelsForRx.begin (), m_channelsForRx.end (), i)!=m_channelsForRx.end ())
                      {
                        int index = distance(m_channelsForRx.begin (), find(m_channelsForRx.begin (), m_channelsForRx.end (), i));
                      if(nbTxAntennas == 8)
                        {
                            min_pmi = m_pmiForRx.at(index).at(0) * 16+m_pmiForRx.at(index).at(1);
                            max_pmi = m_pmiForRx.at(index).at(0) * 16+m_pmiForRx.at(index).at(1);
                        }
                      else
                        {
                            min_pmi = m_pmiForRx.at(index).at(0);
                            max_pmi = m_pmiForRx.at(index).at(0);
                          }
                      }
                    else
                      {
                        // not important, this sub-band is not accounted in BLER evaluation
                        min_pmi = 0;
                        max_pmi = 0;
                        }
                    }

                  for (int pmi=min_pmi; pmi<=max_pmi; pmi++)
                    {
                      if (nbTxAntennas == 2)
                        {
                          precoding = arma::conv_to<arma::cx_fmat>::from(*mimo_codebook_2tx[pmi][l-1]);
                        }
                      else if (nbTxAntennas == 4)
                        {
                          precoding = arma::conv_to<arma::cx_fmat>::from(*mimo_codebook_4tx[pmi][l-1]);
                        }
                      else if (nbTxAntennas == 8)
                        {
                          // limits on i1 values, depending on layers
                          if( (l > 2 && pmi/16 > 3) || (l == 8 && pmi/16 > 0) )
                            {
                              break;
                            }
                          precoding = arma::conv_to<arma::cx_fmat>::from(codebook_8tx(l, pmi/16, pmi%16));
                        }

if(m_rankForRx < 1 || m_rankForRx > min(nbRxAntennas,nbTxAntennas))
  {
    m_rankForRx = min(nbRxAntennas,nbTxAntennas);
  }
if (m_assignedLayers.size()==0)
  {
    for (int n = 0; n < m_rankForRx; n++)
      m_assignedLayers.push_back(n);
  }

                      vector<int> assignedLayers;
                      for (int l1=0; l1<(int)precoding.n_cols; l1++)
                        {
                          assignedLayers.push_back(l1);
                        }
                      vector<double> sinrs = SinrCalculator::MimoReception(receivedSignalLevels,
                                                                                precoding,
                                                                                noise_interference,
                                                                                assignedLayers);

                      double effsinr = GetMiesmEffectiveSinr( sinrs );
                      measuredSinr.at(l-1).at(pmi).resize(nbOfSubChannels);
                      measuredSinr.at(l-1).at(pmi).at(i) = effsinr;

                      if( subBandSize > 1 && nbOfSubChannels > 0 )
                        {
                          for(int n=i+1;n<min(i+subBandSize,nbOfSubChannels);n++)
                            {
                              measuredSinr.at(l-1).at(pmi).at(n) = effsinr;
                            }
                        }
                      if (nbTxAntennas == 8)
                        {
                          // limits on i2 values, depending on layers
                          if (l==4 && pmi%16 == 7)
                            {
                              pmi += 8;
                            }
                          else if (l > 4)
                            {
                              pmi += 15;
                            }
                        }
                    }
                }
              if( subBandSize > 1 && nbOfSubChannels > 0 )
                {
                  i += subBandSize-1;
                }
            }

          // Select the best PMI for each sub-channel and number of layers
          AMCModule *amc = GetDevice ()->GetProtocolStack ()->GetMacEntity ()->GetAmcModule ();
          vector< vector<int> > preferredPmis; // preferred PMI for each number of layers and for each subchannel
          preferredPmis.resize(nbRxAntennas);
          vector< vector<double> > SinrForPreferredPmis; //SINRs corresponding to preferredPmis
          SinrForPreferredPmis.resize(nbRxAntennas);
          for (int l=minLayers; l<=maxLayers; l++)
            {
              int min_pmi, max_pmi;
              if (nbTxAntennas == 2 && l == 1)
                {
                  min_pmi = 0;
                  max_pmi = 3;
                }
              else if (nbTxAntennas == 2 && l == 2)
                {
                  min_pmi = 1;
                  max_pmi = 2;
                }
              else if (nbTxAntennas == 4)
                {
                  min_pmi = 0;
                  max_pmi = 15;
                }
              else if (nbTxAntennas == 8)
                {
                  min_pmi = 0;
                  max_pmi = 255;
                }
              preferredPmis.at(l-1).resize(nbOfSubChannels);
              SinrForPreferredPmis.at(l-1).resize(nbOfSubChannels);

              for (int i = 0; i < nbOfSubChannels; i++)
                {
                  double maxSinr = -INFINITY;

                  // restrict PMI choice if CQI feedback is not required
                  if (ue->GetCqiManager ()->NeedToSendFeedbacks () == false)
                    {
                      if(i >= nbOfRxSubChannels)
                        {
                          break;
                        }
                      if(find(m_channelsForRx.begin (), m_channelsForRx.end (), i)!=m_channelsForRx.end ())
                        {
                          int index = distance(m_channelsForRx.begin (), find(m_channelsForRx.begin (), m_channelsForRx.end (), i));
                          if(nbTxAntennas == 8)
                            {
                              min_pmi = m_pmiForRx.at(index).at(0) * 16+m_pmiForRx.at(index).at(1);
                              max_pmi = m_pmiForRx.at(index).at(0) * 16+m_pmiForRx.at(index).at(1);
                            }
                          else
                            {
                              min_pmi = m_pmiForRx.at(index).at(0);
                              max_pmi = m_pmiForRx.at(index).at(0);
                            }
                        }
                      else
                        {
                          // not important, this sub-band is not accounted in BLER evaluation
                          min_pmi = 0;
                          max_pmi = 0;
                        }
                    }

                  for (int pmi=min_pmi; pmi<=max_pmi; pmi++)
                    {
                      if (nbTxAntennas == 8)
                        {
                          // limits on i1 values, depending on layers
                          if( (l > 2 && pmi/16 > 3) || (l == 8 && pmi/16 > 0) )
                            {
                              break;
                            }
                        }
                      double sinr = measuredSinr.at(l-1).at(pmi).at(i);
                      if(sinr>maxSinr)
                        {
                          maxSinr = sinr;
                          preferredPmis.at(l-1).at(i) = pmi;
                          SinrForPreferredPmis.at(l-1).at(i) = sinr;
                        }
                      if (nbTxAntennas == 8)
                        {
                          // limits on i2 values, depending on layers
                          if (l==4 && pmi%16 == 7)
                            {
                              pmi += 8;
                            }
                          else if (l > 4)
                            {
                              pmi += 15;
                            }
                        }
                    }
                }
            }
          vector<double> effSinrForPreferredPmis;
          effSinrForPreferredPmis.resize(nbRxAntennas);
          int maxTBS = 0;
          m_rankForRiFeedback = 1;
          m_pmiForFeedback.resize(nbOfSubChannels);
          for (int l=minLayers; l<=maxLayers; l++)
            {
              vector<double> sinrForScheduledChannels;
              sinrForScheduledChannels.clear();
              for (auto channel : m_channelsForRx)
                {
                  sinrForScheduledChannels.push_back( SinrForPreferredPmis.at(l-1).at(channel) );
                }
              if (sinrForScheduledChannels.size() == 0)
                {
                  sinrForScheduledChannels = SinrForPreferredPmis.at(l-1);
                }
              effSinrForPreferredPmis.at(l-1) = GetMiesmEffectiveSinr(sinrForScheduledChannels);
              int MCS = amc->GetMCSFromCQI(amc->GetCQIFromSinr(effSinrForPreferredPmis.at(l-1) ) );
              int TBS = amc->GetTBSizeFromMCS(
                                MCS,
                                MCS,
                                sinrForScheduledChannels.size(),
                                l);
              if( TBS > maxTBS )
                {
                  m_rankForRiFeedback = l;
                  maxTBS = TBS;
                  for (int i = 0; i < nbOfSubChannels; i++)
                    {
                      if(nbTxAntennas == 8)
                        {
                          m_pmiForFeedback.at(i).resize(2);
                          m_pmiForFeedback.at(i).at(0) = preferredPmis.at(l-1).at(i) / 16;
                          m_pmiForFeedback.at(i).at(1) = preferredPmis.at(l-1).at(i) % 16;
                        }
                      else
                        {
                          m_pmiForFeedback.at(i).resize(1);
                          m_pmiForFeedback.at(i).at(0) = preferredPmis.at(l-1).at(i);
                        }
                    }
                }
            }
          m_sinrForCQI = SinrForPreferredPmis.at(m_rankForRiFeedback-1);

          sinrForBLER.resize(nbOfSubChannels);
          if(m_rankForRx < 1 || m_rankForRx > min(nbRxAntennas,nbTxAntennas))
            {
              m_rankForRx = min(nbRxAntennas,nbTxAntennas);
            }
          if( m_channelsForRx.size() == 0 )
            {
              m_pmiForRx.resize(nbOfSubChannels);
              for (int i = 0; i < nbOfSubChannels; i++)
                {
                  if(nbTxAntennas == 8)
                    {
                      m_pmiForRx.at(i).resize(2);
                      m_pmiForRx.at(i).at(0) = 0;
                      m_pmiForRx.at(i).at(1) = 0;
                    }
                  else
                    {
                      m_pmiForRx.at(i).resize(1);
                      if (nbTxAntennas == 2 && m_rankForRx == 2)
                        {
                          m_pmiForRx.at(i).at(0) = 1;
                        }
                      else
                        {
                          m_pmiForRx.at(i).at(0) = 0;
                        }
                    }
                }
            }

          if (nbOfRxSubChannels > 0)
            {
              for (int i = 0; i < nbOfSubChannels; i++)
                {
                  if(nbTxAntennas == 8)
                    {
                      sinrForBLER.at(i) = measuredSinr.at(m_rankForRx-1).at( m_pmiForRx.at(i).at(0) * 16+m_pmiForRx.at(i).at(1) ).at(m_channelsForRx.at(i));
                    }
                  else
                    {
                      sinrForBLER.at(i) = measuredSinr.at(m_rankForRx-1).at(m_pmiForRx.at(i).at(0)).at(m_channelsForRx.at(i));
                    }
                }
            }
        }
      break;

      case 11: // Not a real LTE TM. Represents m-MIMO+GoB
        {
          vector<double> measuredSinr; // received sinr for each subchannel
          measuredSinr.resize(nbOfSubChannels);
          arma::cx_fmat powers = arma::cx_fmat(nbRxAntennas,nbTxAntennas);

          arma::cx_fmat receivedSignalLevels;  // received signal level on each path
          receivedSignalLevels.set_size(nbRxAntennas,nbTxAntennas);
          arma::cx_fmat precoding;
          arma::cx_fmat precodedH0;
          arma::fvec SINRs;

          m_fullCsiFeedback.clear();

          for (int i = 0; i < nbOfSubChannels; i++)
            {
              for( int j = 0; j < nbRxAntennas; j++)
                {
                  for( int k = 0; k < nbTxAntennas; k++)
                    {
                      powers(j,k)=pow(10,(long double)rxSignalValues.at (j*nbTxAntennas+k).at (i)/10);
                    }
                }

              double avgPower = arma::mean( (arma::fvec)arma::sum(arma::abs(powers),1) );
              double avgPathPower = arma::mean( arma::mean(arma::abs(powers)) );
              for( int j = 0; j < nbRxAntennas; j++)
                {
                  for( int k = 0; k < nbTxAntennas; k++)
                    {
                      receivedSignalLevels(j,k) =  polar( (float)sqrt( abs( powers(j,k) ) ), rxSignalPhases[j*nbTxAntennas+k][i] );
                    }
                }
              if(ue->GetCqiManager ()->NeedToSendFeedbacks () == true)
                {
                  shared_ptr<arma::cx_fmat> channelMatrixForFeedback = make_shared<arma::cx_fmat>(nbRxAntennas,nbTxAntennas);
                  *channelMatrixForFeedback = receivedSignalLevels;

                  double max_value = abs(channelMatrixForFeedback->max());
                  double threshold_db = INFINITY;
                  channelMatrixForFeedback->for_each( [max_value, threshold_db](complex<float>& val) { if(max_value/abs(val) > pow(10,threshold_db/20)) {val = 0;} } );
                  m_fullCsiFeedback.push_back(channelMatrixForFeedback);
                }

              if(m_rankForRx < 1 || m_rankForRx > min(nbRxAntennas,nbTxAntennas))
                {
                  m_rankForRx = min(nbRxAntennas,nbTxAntennas);
                }
              if(m_rankForRx>nbRxAntennas)
                {
                  cout << "Error: rank for RX transmission (" << m_rankForRx << ") is higher than supported (" << nbRxAntennas << ")" << endl;
                  exit(1);
                }
              if (find(m_channelsForRx.begin (), m_channelsForRx.end (), i)!=m_channelsForRx.end ())
                {
                  int index = distance(m_channelsForRx.begin (), find(m_channelsForRx.begin (), m_channelsForRx.end (), i));
                  if (m_precodingMatricesForRx.at(index) != nullptr)
                    {
                      precoding = *m_precodingMatricesForRx.at(index);
                    }
                  else
                    {
                      //TODO: how to measure channel quality when channel is not scheduled (i.e. when not using round robin)?
                      precoding = arma::eye<arma::cx_fmat>(nbTxAntennas,nbRxAntennas);
                      m_assignedLayers.clear();
                    }
                }
              else
                {
                  //TODO: how to measure channel quality when channel is not scheduled (i.e. when not using round robin)?
                  precoding = arma::eye<arma::cx_fmat>(nbTxAntennas,nbRxAntennas);
                      m_assignedLayers.clear();
                }

              if (m_assignedLayers.size()==0)
                {
//                  for (int n = 0; n < nbRxAntennas; n++)
                  for (int n = 0; n < 1; n++)
                    m_assignedLayers.push_back(n);
                }

              vector<double> sinrs;

              bool use_srta_pi = false;
              if (GetSrtaPi()==true)
                {
                  if ((int)m_channelMatricesForSrtaPi.size() > i)
                    {
                      if (m_channelMatricesForSrtaPi.at(i) != nullptr)
                        {
                          use_srta_pi = true;
                        }
                    }
                }
              int speed = GetDevice()->GetMobilityModel()->GetSpeed();
              WaveformType waveform = ue->GetTargetNode()->GetPhy()->GetWaveformType();
              double dopplerSIR = GetInterference()->ComputeDopplerInterference(speed,waveform);

              if (use_srta_pi==true)
                {
                  sinrs = SinrCalculator::MimoReception(
//                  sinrs = SinrCalculator::MimoReceptionMRC(
                                                *m_channelMatricesForSrtaPi.at(i),
                                                precoding,
                                                noise_interference,
                                                m_assignedLayers,
                                                dopplerSIR);
                }
              else
                {
                  sinrs = SinrCalculator::MimoReception(
//                  sinrs = SinrCalculator::MimoReceptionMRC(
                                                receivedSignalLevels,
                                                precoding,
                                                noise_interference,
                                                m_assignedLayers,
                                                dopplerSIR);
                }
              double effsinr = GetMiesmEffectiveSinr( sinrs );
              measuredSinr.at(i) = effsinr;

              // use simple formula for the CQI. the base station needs to adjust it to include MU-MIMO interference

              m_sinrForCQI.push_back((10*log10(avgPower)-noise_interference));
//              m_sinrForCQI.push_back((10*log10(avgPower)-noise_interference)/2);
//              m_sinrForCQI.push_back(effsinr);
            }

DEBUG_LOG_START_1(SIM_ENV_MCS_DEBUG)
          cout << "MCS_DEBUG UE " << GetDevice()->GetIDNetworkNode() << ", SINR for CQI " << m_sinrForCQI << endl;
DEBUG_LOG_END
          m_rankForRiFeedback = m_rankForRx;
//          m_rankForRiFeedback = 1;
          sinrForBLER = measuredSinr;
DEBUG_LOG_START_1(SIM_ENV_MCS_DEBUG)
          if(nbOfRxSubChannels>0)
            {
              cout << "MCS_DEBUG UE " << GetDevice()->GetIDNetworkNode() << ", SINR forBLER " << sinrForBLER << endl;
            }
DEBUG_LOG_END

          if( m_channelsForRx.size() > 0 )
            {
              // FIXME: overriding selected MCS, until proper gNB-side link adaptation is implemented
              AMCModule *amc = ue->GetMacEntity ()->GetAmcModule();

DEBUG_LOG_START_2(SIM_ENV_MCS_DEBUG,SIM_ENV_MCS_OVERRIDING_DEBUG)
//              cout << "MCS_DEBUG gNB mcs " << m_mcsIndexForRx.at(0)
//                   << " UE " << GetDevice()->GetIDNetworkNode()
//                   << " mcs " << amc->GetMCSFromSinrVector(sinrForBLER)
//                   << " harq " << ((UeMacEntity*)GetDevice ()->GetProtocolStack ()->GetMacEntity ())->GetHarqManager ()->ReceiveProcessExists (m_harqPidForRx)
//                   << endl;
DEBUG_LOG_END
//              int mcs = amc->GetMCSFromSinrVector(sinrForBLER);
//              fill(m_mcsIndexForRx.begin(),m_mcsIndexForRx.end(), mcs);
            }
        }
    break;
    default:
      cout << "Transmission mode " << txMode << " not supported" << endl;
      exit(1);
      break;
    }
  //CHECK FOR PHY ERROR
  bool phyError;

  HarqManager *harqManager = ue->GetMacEntity ()->GetHarqManager ();

  if (GetErrorModel() != nullptr && nbOfRxSubChannels > 0)
    {
      vector<double> sinrForBLER_2 = sinrForBLER;
      if (harqManager!=nullptr)
        {
          if (!(FrameManager::Init()->MbsfnEnabled()==true && FrameManager::Init()->isMbsfnSubframe()==true))
            {
              if (harqManager->ReceiveProcessExists (m_harqPidForRx))
                {
                  sinrForBLER_2 = harqManager->GetCombinedSinr(m_harqPidForRx,sinrForBLER);
                }
            }
        }


      vector<int> cqi_;
      for (int i = 0; i < (int)m_mcsIndexForRx.size (); i++)
        {
          AMCModule *amc = GetDevice ()->GetProtocolStack ()->GetMacEntity ()->GetAmcModule ();
          int cqi =  amc->GetCQIFromMCS (m_mcsIndexForRx.at (i));
          cqi_.push_back (cqi);
        }
      phyError = GetErrorModel ()->CheckForPhysicalError (m_channelsForRx, cqi_, sinrForBLER_2);


    if (harqManager != nullptr)
      {
          if (!(FrameManager::Init()->MbsfnEnabled()==true && FrameManager::Init()->isMbsfnSubframe()==true))
            {
        if (phyError)
          {
            if(harqManager->ReceiveProcessExists (m_harqPidForRx))
              {
                harqManager->UpdateRxProcess(m_harqPidForRx,sinrForBLER);
              }
            else
              {
                harqManager->CreateRxProcess(m_harqPidForRx,sinrForBLER);
              }
          }
        else
          {
            if(harqManager->ReceiveProcessExists (m_harqPidForRx))
              {
                harqManager->DeleteRxProcess(m_harqPidForRx);
              }
          }
      }
        }

      if (_PHY_TRACING_)
        {
          if (phyError)
            {
              cout << "**** YES PHY ERROR (node " << GetDevice ()->GetIDNetworkNode () << ") ****" << endl;
            }
          else
            {
              cout << "**** NO PHY ERROR (node " << GetDevice ()->GetIDNetworkNode () << ") ****" << endl;
            }
        }
    }
  else
    {
      phyError = false;
    }
  // Send HARQ ACK/NACK
  if(m_mcsIndexForRx.size()>0 && harqManager != nullptr && m_harqPidForRx != HarqManager::HARQ_NOT_USED)
    {
      DEBUG_LOG_START_1(SIM_ENV_HARQ_DEBUG)
         cout << "HARQ_DEBUG ---- UePhy::StartRx ----  UE " << GetDevice ()->GetIDNetworkNode () << " pid " << m_harqPidForRx << " error " << phyError << endl;
      DEBUG_LOG_END
      HarqIdealControlMessage* harqAck = new HarqIdealControlMessage();
      harqAck->SetSourceDevice (ue);
      harqAck->SetDestinationDevice (ue->GetTargetNode ());
      harqAck->SetPid (m_harqPidForRx);
      harqAck->SetAck (!phyError);
      SendIdealControlMessage (harqAck);
    }

if (_PHY_TRACING_) {
  double effective_sinr = GetMiesmEffectiveSinr (sinrForBLER);
  if (effective_sinr > 40) effective_sinr = 40;
  int MCS_ = 0;
  int TBS_ = 0;
  if(nbOfRxSubChannels>0)
    {
      MCS_ = m_mcsIndexForRx.at(0);
      TBS_ = GetDevice ()->GetProtocolStack ()->GetMacEntity ()->GetAmcModule ()->GetTBSizeFromMCS (m_mcsIndexForRx.at(0), m_mcsIndexForRx.at(0), nbOfRxSubChannels, m_rankForRx);
      cout << "PHY_RX SRC " << ue->GetTargetNode()->GetIDNetworkNode()
            << " DST " << ue->GetIDNetworkNode()
            << " X " << ue->GetMobilityModel ()->GetAbsolutePosition ()->GetCoordinateX ()
            << " Y " << ue->GetMobilityModel ()->GetAbsolutePosition ()->GetCoordinateY ()
            << " SINR " << effective_sinr
            << " RB " << nbOfRxSubChannels
            << " MCS " << MCS_
            << " SIZE " << TBS_
            << " ERR " << phyError
            << " T " << Simulator::Init()->Now();
            if(std::getenv("USE_COVERSHIFT") != nullptr)
            {
              cout << " CS " << FrameManager::Init()->GetCoverShiftIndex();
            }
           cout << endl;
    }
}

  if (!phyError && p->GetNPackets() > 0)
    {
      //FORWARD RECEIVED PACKETS TO THE DEVICE
      GetDevice()->ReceivePacketBurst(p);
    }

  //CQI report
  // for MBSFN, CQIs are collected by the multicast-destination-phy class,
  // so there is no need to send them from here
  if (FrameManager::Init()->isMbsfnSubframe()==false)
    {
      CreateCqiFeedbacks (m_sinrForCQI);
    }

  m_channelsForRx.clear ();
  m_channelsForTx.clear ();
  m_mcsIndexForRx.clear ();
  m_mcsIndexForTx.clear ();
  m_pmiForRx.clear();

}

void
UePhy::CreateCqiFeedbacks (vector<double> sinr)
{
  UserEquipment* ue = GetDevice ();
  if (ue->GetCqiManager ()->NeedToSendFeedbacks ())
    {
      if(sinr.size()>0)
        {
          ue->GetCqiManager ()->CreateCqiFeedbacks (sinr);
        }
      int txMode = ue->GetTargetNodeRecord()->GetDlTxMode();
      if(txMode == 3)
        {
          ue->GetCqiManager ()->CreateRiFeedback (m_rankForRiFeedback);
        }
      if(txMode == 4 || txMode == 9)
        {
          ue->GetCqiManager ()->CreateRiFeedback (m_rankForRiFeedback);
          ue->GetCqiManager ()->CreatePmiFeedbacks(m_pmiForFeedback);
        }
      if(txMode == 11)
        {
          ue->GetCqiManager ()->CreateRiFeedback (m_rankForRiFeedback);
          ue->GetCqiManager ()->CreateFullCsiFeedbacks(m_fullCsiFeedback);
          m_fullCsiFeedback.clear();
        }
    }
}


UserEquipment*
UePhy::GetDevice(void)
{
  Phy* phy = (Phy*)this;
  return (UserEquipment*)phy->GetDevice();
}

vector<double>&
UePhy::GetMeasuredMBSFNSinr (void)
{
  return m_measuredMBSFNSinr;
}

int
UePhy::GetRankForRiFeedback()
{
  return m_rankForRiFeedback;
}

void
UePhy::SendIdealControlMessage (IdealControlMessage *msg)
{

DEBUG_LOG_START_1(SIM_ENV_RANDOM_ACCESS)
  if (msg->GetMessageType () == IdealControlMessage::RA_PREAMBLE)
    {
      cout << "RANDOM_ACCESS SEND_MSG1 UE " << msg->GetSourceDevice ()->GetIDNetworkNode () << " T " << Simulator::Init()->Now();
    }

DEBUG_LOG_END

  NetworkNode* dst = msg->GetDestinationDevice ();
  dst->GetPhy ()->ReceiveIdealControlMessage (msg);

  delete msg;
}

void
UePhy::ReceiveIdealControlMessage (IdealControlMessage *msg)
{
  if (msg->GetMessageType () == IdealControlMessage::ALLOCATION_MAP)
    {

      m_channelsForRx.clear ();
      m_channelsForTx.clear ();
      m_mcsIndexForRx.clear ();
      m_mcsIndexForTx.clear ();
      m_pmiForRx.clear();
      m_precodingMatricesForRx.clear();
      m_channelMatricesForSrtaPi.clear();

      PdcchMapIdealControlMessage *map = (PdcchMapIdealControlMessage*) msg;

      int node = GetDevice ()->GetIDNetworkNode ();

      for (auto allocation : *map->GetMessage ())
        {
          if (allocation.m_ue->GetIDNetworkNode () == node)
            {
              if (allocation.m_direction == PdcchMapIdealControlMessage::DOWNLINK)
                {
                  m_channelsForRx.push_back (allocation.m_idSubChannel);
                  m_mcsIndexForRx.push_back(allocation.m_mcsIndex);
                  m_rankForRx = allocation.m_rank;
                  m_pmiForRx.push_back( allocation.m_pmi );
                  m_assignedLayers = allocation.m_assignedLayers;
                  m_harqPidForRx = allocation.m_harqPid;
                  m_precodingMatricesForRx.push_back(allocation.m_precodingMatrix);
                  m_channelMatricesForSrtaPi.push_back(allocation.m_fullCsiMatrix);
                }
              else if (allocation.m_direction == PdcchMapIdealControlMessage::UPLINK)
                {
                  m_channelsForTx.push_back (allocation.m_idSubChannel);
                  m_mcsIndexForTx.push_back(allocation.m_mcsIndex);
                }
DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG_NB)
              cout << "RECEIVE ALLOCATION MAP MESSAGE UE " << node
                   << " MCS " << m_mcsIndexForTx.at(0)
                   << " SUBCARRIER " << allocation.m_idSubChannel
                   << endl;
DEBUG_LOG_END

            }
        }

      if (m_channelsForTx.size () > 0)
        {
          DoSetBandwidthManager ();
          GetDevice ()->GetMacEntity ()->ScheduleUplinkTransmission (m_channelsForTx.size (), m_mcsIndexForTx.at (0));
        }
    }
  else if (msg->GetMessageType () == IdealControlMessage::NB_IOT_ALLOCATION_MAP)
    {
      m_channelsForRx.clear ();
      m_channelsForTx.clear ();
      m_mcsIndexForRx.clear ();
      m_mcsIndexForTx.clear ();

      NbIoTMapIdealControlMessage *map = (NbIoTMapIdealControlMessage*) msg;

      int node = GetDevice ()->GetIDNetworkNode ();
      int nru = -1;
      for (auto allocation : *map->GetMessage ())
        {
          if (allocation.m_ue->GetIDNetworkNode () == node)
            {
               m_channelsForTx.push_back(allocation.m_idSubChannel);
               m_mcsIndexForTx.push_back(allocation.m_mcsIndex);
               nru = allocation.m_ru;
DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG_NB)
               cout << "RECEIVE ALLOCATION MAP MESSAGE UE " << node
                    << " MCS " << m_mcsIndexForTx.at(0)
                    << " RU " << nru
                    << " SUBCARRIER " << allocation.m_idSubChannel
                    << endl;
DEBUG_LOG_END
             }
        }

      if (m_channelsForTx.size () > 0)
        {
          DoSetBandwidthManager ();
          GetDevice ()->GetMacEntity ()->ScheduleNbUplinkTransmission (m_mcsIndexForTx.at(0), nru);
        }
    }
  else
    {
      GetDevice ()->GetMacEntity ()->ReceiveIdealControlMessage(msg);
    }
}

void
UePhy::SetTxSignalForReferenceSymbols (void)
{
  BandwidthManager* s = GetBandwidthManager ();
  vector<double> channels = s->GetUlSubChannels ();

  double powerTx = pow (10., (GetTxPower () - 30.) / 10.); // in natural unit
  double txPower = 10 * log10 (powerTx / channels.size ()); //in dB

  TransmittedSignal* txSignal = new TransmittedSignal ();
  vector< vector<double> > values;
  values.resize(1);
  for (auto channel : channels)
    {
      values.at(0).push_back(txPower);
    }
  txSignal->SetValues (values);
  m_txSignalForReferenceSymbols = txSignal;
  if(GetUlChannel ()!=nullptr)
    {
      SendReferenceSymbols();
    }
}

TransmittedSignal*
UePhy::GetTxSignalForReferenceSymbols (void)
{
  return m_txSignalForReferenceSymbols;
}

void
UePhy::SendReferenceSymbols (void)
{
  UserEquipment* ue = GetDevice ();
  GNodeB* target = ue->GetTargetNode ();
  GnbPhy* gnbPhy = (GnbPhy*) target->GetPhy ();
  ChannelRealization* cr = GetUlChannel()->GetPropagationLossModel()->GetChannelRealization(ue, target);
  if( target->GetDLScheduler() != nullptr && cr->hasFastFading() == true )
    {
      gnbPhy->ReceiveReferenceSymbols (ue, GetTxSignalForReferenceSymbols ());
      Simulator::Init()->Schedule(0.001, &UePhy::SendReferenceSymbols, this);
    }
}


