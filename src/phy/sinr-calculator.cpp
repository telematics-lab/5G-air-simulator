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

#include<iomanip>
#include "../utility/PrintVector.h"
#include "sinr-calculator.h"


vector<double>
SinrCalculator::MimoReception(arma::cx_fmat receivedSignalLevels, arma::cx_fmat precoding, double noise_interference, vector<int> assignedLayers, double dopplerSIR)
{
  int nbRxAntennas = receivedSignalLevels.n_rows;
  int nbLayers = precoding.n_cols;
  arma::cx_fmat directPrecoding = precoding.cols(arma::conv_to<arma::uvec >::from(assignedLayers));
  arma::cx_fmat Heff = receivedSignalLevels*directPrecoding;
//  arma::cx_fmat receiveFilter = arma::inv( arma::trans(Heff)*Heff + pow(10.,noise_interference)*arma::eye(Heff.n_cols,Heff.n_cols) ) * arma::trans(Heff);
  arma::cx_fmat receiveFilter = arma::trans(Heff) * arma::inv( Heff*arma::trans(Heff) + pow(10.,noise_interference/10)*arma::eye<arma::fmat>(nbRxAntennas,nbRxAntennas) );
  receiveFilter = arma::normalise(receiveFilter);

DEBUG_LOG_START_1(SIM_ENV_SINR_CALCULATOR_DEBUG)
  cout << "SINR_CALCULATOR receivedSignalLevels:" << endl << arma::abs(receivedSignalLevels )<< endl;
  cout << "SINR_CALCULATOR precoding:" << endl << precoding << endl;
DEBUG_LOG_END

//receiveFilter=arma::eye<arma::cx_fmat>(nbRxAntennas,nbRxAntennas);
  arma::cx_fmat precodedH0 = receiveFilter*receivedSignalLevels*precoding;

DEBUG_LOG_START_1(SIM_ENV_SINR_CALCULATOR_DEBUG)
  cout << "SINR_CALCULATOR rx_power: " << endl << arma::pow(arma::abs(receivedSignalLevels),2) << endl;
  cout << "SINR_CALCULATOR receiveFilter:" << endl << receiveFilter << endl;
  cout << "SINR_CALCULATOR precoded channel:" << endl << precodedH0 << endl;
  cout << "SINR_CALCULATOR interference: " << pow(10,noise_interference/10)  << endl << arma::trans(arma::diagvec( abs( pow(10,noise_interference/10)*arma::trans(receiveFilter)*receiveFilter ) ) );
DEBUG_LOG_END

  arma::cx_fmat signal_comp = arma::zeros<arma::cx_fmat>(assignedLayers.size(),nbLayers);
  arma::cx_fmat interference_comp = precodedH0;
  for(int i=0; i<(int)assignedLayers.size(); i++)
    {
      signal_comp(i,assignedLayers.at(i)) = precodedH0(i,assignedLayers.at(i));
      interference_comp(i,assignedLayers.at(i)) = 0;
DEBUG_LOG_START_1(SIM_ENV_SINR_CALCULATOR_DEBUG)
      cout << "SINR_CALCULATOR assigned layer " << assignedLayers.at(i) << endl;
DEBUG_LOG_END
    }
  arma::fvec signal = arma::diagvec( abs( signal_comp * arma::trans(signal_comp) ) );
  arma::fvec doppler = signal / pow(10,dopplerSIR/10);
  arma::fvec interference = arma::diagvec( abs( interference_comp * arma::trans(interference_comp) + pow(10,noise_interference/10)*receiveFilter*arma::trans(receiveFilter)) );
  arma::fvec sinr = signal / (interference+doppler);

DEBUG_LOG_START_1(SIM_ENV_SINR_CALCULATOR_DEBUG)
  cout << "SINR_CALCULATOR signal power:" <<  10*log10(arma::trans(signal)) + 30;
  cout << "SINR_CALCULATOR doppler interference:" <<  10*log10(arma::trans(doppler)) + 30;
  cout << "SINR_CALCULATOR intra-cell interference:" <<  10*log10(arma::trans(arma::diagvec( abs( interference_comp * arma::trans(interference_comp))) )) + 30;
  cout << "SINR_CALCULATOR noise+interference:" << 10*log10(arma::trans(arma::diagvec( abs( pow(10,noise_interference/10)*arma::trans(receiveFilter)*receiveFilter ) ) ) ) + 30;
  cout << "SINR_CALCULATOR sinr: " << arma::trans(10*log10(sinr)) << endl;
DEBUG_LOG_END

  return arma::conv_to< vector<double> >::from(10*log10(sinr));
}


vector<double>
SinrCalculator::MimoReceptionMRC(arma::cx_fmat receivedSignalLevels, arma::cx_fmat precoding, double noise_interference, vector<int> assignedLayers, double dopplerSIR)
{
  int nbRxAntennas = receivedSignalLevels.n_rows;
  int nbLayers = precoding.n_cols;
  int nbTxAntennas = receivedSignalLevels.n_cols;
  arma::cx_fmat directPrecoding = precoding.cols(arma::conv_to<arma::uvec >::from(assignedLayers));
  arma::cx_fmat Heff = receivedSignalLevels*directPrecoding;
//  arma::cx_fmat receiveFilter = arma::trans(Heff);
  arma::cx_fmat receiveFilter = arma::trans( receivedSignalLevels*arma::ones<arma::cx_fmat>( nbTxAntennas, 1 ) );
  receiveFilter = arma::normalise(receiveFilter);

DEBUG_LOG_START_1(SIM_ENV_SINR_CALCULATOR_DEBUG)
  cout << "SINR_CALCULATOR receivedSignalLevels:" << endl << arma::abs(receivedSignalLevels )<< endl;
  cout << "SINR_CALCULATOR precoding:" << endl << precoding << endl;
DEBUG_LOG_END

//receiveFilter=arma::eye<arma::cx_fmat>(nbRxAntennas,nbRxAntennas);
  arma::cx_fmat precodedH0 = receiveFilter*receivedSignalLevels*precoding;

DEBUG_LOG_START_1(SIM_ENV_SINR_CALCULATOR_DEBUG)
  cout << "SINR_CALCULATOR rx_power: " << endl << arma::pow(arma::abs(receivedSignalLevels),2) << endl;
  cout << "SINR_CALCULATOR receiveFilter:" << endl << receiveFilter << endl;
  cout << "SINR_CALCULATOR precoded channel:" << endl << precodedH0 << endl;
  cout << "SINR_CALCULATOR interference: " << pow(10,noise_interference/10)  << endl << arma::trans(arma::diagvec( abs( pow(10,noise_interference/10)*arma::trans(receiveFilter)*receiveFilter ) ) );
DEBUG_LOG_END

//  arma::cx_fmat signal_comp = arma::zeros<arma::cx_fmat>(nbRxAntennas,nbLayers); ???
  arma::cx_fmat signal_comp = arma::zeros<arma::cx_fmat>(assignedLayers.size(),nbLayers);
  arma::cx_fmat interference_comp = precodedH0;
  for(int i=0; i<(int)assignedLayers.size(); i++)
    {
      signal_comp(i,assignedLayers.at(i)) = precodedH0(i,assignedLayers.at(i));
      interference_comp(i,assignedLayers.at(i)) = 0;
DEBUG_LOG_START_1(SIM_ENV_SINR_CALCULATOR_DEBUG)
      cout << "SINR_CALCULATOR assigned layer " << assignedLayers.at(i) << endl;
DEBUG_LOG_END

    }
  arma::fvec signal = arma::diagvec( abs( signal_comp * arma::trans(signal_comp) ) );
  arma::fvec doppler = signal / pow(10,dopplerSIR/10);
  arma::fvec interference = arma::diagvec( abs( interference_comp * arma::trans(interference_comp) + pow(10,noise_interference/10)*receiveFilter*arma::trans(receiveFilter)) );
  arma::fvec sinr = signal / (interference+doppler);

DEBUG_LOG_START_1(SIM_ENV_SINR_CALCULATOR_DEBUG)
  cout << "GOB RX signal power:" <<  10*log10(arma::trans(signal)) + 30;
  cout << "GOB RX doppler interference:" <<  10*log10(arma::trans(doppler)) + 30;
  cout << "GOB RX intra-cell interference:" <<  10*log10(arma::trans(arma::diagvec( abs( interference_comp * arma::trans(interference_comp))) )) + 30;
  cout << "GOB RX noise+interference:" << 10*log10(arma::trans(arma::diagvec( abs( pow(10,noise_interference/10)*arma::trans(receiveFilter)*receiveFilter ) ) ) ) + 30;
  cout << "GOB RX sinr: " << arma::trans(10*log10(sinr)) << endl;
DEBUG_LOG_END

  return arma::conv_to< vector<double> >::from(10*log10(sinr));
}
