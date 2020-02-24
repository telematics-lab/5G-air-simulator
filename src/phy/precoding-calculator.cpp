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

#include "../load-parameters.h"
#include "precoding-calculator.h"
#include "sinr-calculator.h"
#include "../utility/miesm-effective-sinr.h"

shared_ptr<arma::cx_fmat>
PrecodingCalculator::RegularizedZeroForcing(
                        vector< shared_ptr<arma::cx_fmat> > channelMatrices,
                        vector<int> userRanks,
                        vector<double> sinrs,
                        double tx_power
                        )
{
  arma::cx_fmat channelMatrix;
  shared_ptr<arma::cx_fmat> precodingMatrix(make_shared<arma::cx_fmat>());
  vector<double> noise_interference_vec;
  vector<double> noise_interference_vec_2;
  vector<double> avg_power_vec;
  vector< vector<int> > assignedLayers;
  assignedLayers.resize(channelMatrices.size());
  int layer_index = 0;

  for (int i = 0; i < (int)channelMatrices.size(); i++)
    {
      channelMatrix = arma::join_cols (channelMatrix, arma::conv_to<arma::cx_fmat>::from(*(channelMatrices.at(i))));
      double avgPower = arma::mean(
                            (arma::fvec)arma::sum(
                                arma::pow(
                                    (arma::abs(*channelMatrices.at(i))),
                                2)
                            ,1)
                        );
      avg_power_vec.push_back(avgPower);
      for (int j=0; j<(int)(userRanks.at(i)); j++)
        {
          noise_interference_vec.push_back( pow( 10., (10*log10(avgPower) - 2 * sinrs.at(i) )/10 ) );
          assignedLayers.at(i).push_back(layer_index);
          layer_index++;
        }
      noise_interference_vec_2.push_back( pow( 10., (10*log10(avgPower) - 2 * sinrs.at(i) )/10 ) );
    }

  arma::fmat noise_interference_mat = arma::diagmat( arma::conv_to<arma::fvec>::from(noise_interference_vec) );

  int nbTxAntennas = channelMatrix.n_cols;
  int nbTotalRxAntennas = channelMatrix.n_rows;
  int nbOfUsers = channelMatrices.size();

  // XXX get this value from somewhere else
  double thermalNoise = 9 - 174 + 10*log10(180000) - 30;

  int nbStreams = 0;
  vector<arma::cx_fmat> receiverFilters(userRanks.size());
  for (int i=0; i<(int)userRanks.size(); i++)
    {
      nbStreams += userRanks.at(i);
      int nbUserRxAntennas = channelMatrices.at(i)->n_rows;
      if (userRanks.at(i) == nbUserRxAntennas)
        {
          receiverFilters.at(i) = arma::eye<arma::cx_fmat>( nbUserRxAntennas, nbUserRxAntennas );
        }
      else if (userRanks.at(i) == 1)
        {
          receiverFilters.at(i) = arma::trans( *channelMatrices.at(i)
                                              // assume MRC receive filter
//                                              *arma::ones<arma::cx_fmat>( nbTxAntennas, 1 ))
                                    //  assume MMSE receive filter
                                    * arma::inv( *channelMatrices.at(i)*arma::ones<arma::cx_fmat>( nbTxAntennas, 1 )*arma::ones<arma::cx_fmat>( 1, nbTxAntennas )*arma::trans(*channelMatrices.at(i))
                                                    + noise_interference_vec_2.at(i) ))
                                ;
          receiverFilters.at(i) = arma::normalise(receiverFilters.at(i));
        }
      else
        {
          cout << "Error: unsupported rank for precoding ("
            << userRanks.at(i) << " streams on "
            << nbUserRxAntennas << " receive antennas)"
            << endl;
          exit(1);
        }
    }
  arma::cx_fmat receiverMatrix = arma::zeros<arma::cx_fmat>( nbStreams, nbTotalRxAntennas );
  int x = 0;
  int y = 0;
  for (int i=0; i<(int)userRanks.size(); i++)
    {
      receiverMatrix.submat(
        y,
        x,
        y + userRanks.at(i) - 1,
        x + channelMatrices.at(i)->n_rows - 1
      ) = receiverFilters.at(i);
      x += channelMatrices.at(i)->n_rows;
      y += userRanks.at(i);
    }

  *precodingMatrix = arma::conv_to<arma::cx_fmat>::from(
                        arma::conv_to<arma::cx_mat>::from(arma::trans(channelMatrix)) *
                        arma::conv_to<arma::cx_mat>::from(arma::trans(receiverMatrix))*
                        arma::inv(
                          arma::conv_to<arma::cx_mat>::from(receiverMatrix) *
                          arma::conv_to<arma::cx_mat>::from(channelMatrix) *
                          arma::conv_to<arma::cx_mat>::from(arma::trans(channelMatrix)) *
                          arma::conv_to<arma::cx_mat>::from(arma::trans(receiverMatrix))
//                      +noise_interference_mat
                          +nbTotalRxAntennas*pow(10.,thermalNoise/10)
                              *arma::eye<arma::mat>( nbStreams, nbStreams )
                         )
                      );

  // user balancing
  *precodingMatrix = (*precodingMatrix) * ( arma::diagmat<arma::fmat>(1 / arma::sum(arma::abs(*precodingMatrix))) );


  // power normalization: the precoding matrix is scaled to use all the available power
  *precodingMatrix = arma::normalise(*precodingMatrix);
//  *precodingMatrix =
//    (*precodingMatrix) /
//    sqrt( arma::accu(arma::pow( arma::abs(*precodingMatrix), 2 )) );


  // estimate received SINR for each stream
  arma::cx_fmat eff_channel = receiverMatrix * arma::conv_to<arma::cx_fmat>::from(channelMatrix) * (*precodingMatrix);
  arma::cx_fmat signal_comp = arma::zeros<arma::cx_fmat>(eff_channel.n_rows,eff_channel.n_cols);
  arma::cx_fmat interference_comp = eff_channel;
  layer_index = 0;
  for(int i=0; i<(int)userRanks.size();i++)
    {
      for(int k=0; k<(int)userRanks.at(i); k++)
        {
          signal_comp(i,layer_index) = eff_channel(i,layer_index);
          interference_comp(i,layer_index) = 0;
          layer_index++;
        }
    }
  arma::fvec signal = arma::diagvec( abs( signal_comp * arma::trans(signal_comp) ) );
  arma::fvec interference = arma::diagvec( abs( interference_comp * arma::trans(interference_comp)
                                            + arma::diagmat(arma::conv_to<arma::fvec>::from(noise_interference_vec))*receiverMatrix*arma::trans(receiverMatrix)) );
  arma::fvec est_sinr = signal / interference;

DEBUG_LOG_START_1(SIM_ENV_RZF_DEBUG)
  cout << "RZF_PRECODING TX signal power:" <<  10*log10(arma::trans(signal)) + 30;
  cout << "RZF_PRECODING TX intra-cell interference:" <<  10*log10(arma::trans(arma::diagvec( abs( interference_comp * arma::trans(interference_comp))) )) + 30;
  cout << "RZF_PRECODING TX noise+interference:" << 10*log10(arma::trans(arma::diagvec( abs( arma::diagmat(arma::conv_to<arma::fvec>::from(noise_interference_vec))*receiverMatrix*arma::trans(receiverMatrix) ) ) ) ) + 30;
  cout << "RZF_PRECODING TX sinr: " << arma::trans(10*log10(est_sinr)) << endl;
  cout << "RZF_PRECODING CHANNEL GAINS ";
  for (auto channel:channelMatrices) {
    cout << arma::mean(arma::abs(*channel),1) << " ";
  }
  cout << endl;
DEBUG_LOG_END


  return precodingMatrix;
}


// TODO: extend to support different layer for each sub-channel
vector<double>
PrecodingCalculator::adjustSinrForMuMimo(
                        vector<double> sinrs, // sinr for each subchannel, derived from CQIs
                        vector< shared_ptr<arma::cx_fmat> > channelMatrices, // channel matrix for each subchannel
                        vector< shared_ptr<arma::cx_fmat> > precodingMatrices, // precoding matrix for each subchannel
                        vector<int> assignedLayers) // layers assigned to this user
{
  vector<double> adjustedSinrs;
  for (int rb = 0; rb < (int)sinrs.size(); rb++)
    {
      double avgPower = arma::mean(
                            (arma::fvec)arma::sum(
                                arma::pow(
                                    (arma::abs(*channelMatrices.at(rb))),
                                2)
                            ,1)
                        );

      double noise_interference = 10*log10(avgPower) - sinrs.at(rb);
      vector<double> rb_sinrs = SinrCalculator::MimoReception(*(channelMatrices.at(rb)), *precodingMatrices.at(rb), noise_interference, assignedLayers);

      double rb_sinr = GetMiesmEffectiveSinr( rb_sinrs );

      adjustedSinrs.push_back( rb_sinr );
    }

  return adjustedSinrs;
}
