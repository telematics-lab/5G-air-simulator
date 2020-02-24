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

#ifndef PRECODING_CALCULATOR_H_
#define PRECODING_CALCULATOR_H_

#include <vector>
#include <memory>
#include <armadillo>

class PrecodingCalculator
{
public:

  static shared_ptr<arma::cx_fmat> RegularizedZeroForcing(
    vector< shared_ptr<arma::cx_fmat> > channelMatrices,
    vector<int> userRanks,
    vector<double> sinrs,
    double power
  );

  static vector< double > adjustSinrForMuMimo(
    vector<double> sinrs,
    vector< shared_ptr<arma::cx_fmat> > channelMatrices,
    vector< shared_ptr<arma::cx_fmat> > precodingMatrices,
    vector<int> assignedLayers);
};

#endif /* PRECODING_CALCULATOR_H */
