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

#ifndef MIMO_CODEBOOK_H_
#define MIMO_CODEBOOK_H_

#include <armadillo>

// Precoding matrices for transmit diversity with 4 tx antennas
const arma::vec::fixed<4> W_SFBC_FSTD[2] =
{
  {1,0,1,0},
  {0,1,0,1}
};

// Precoding matrices for 2 tx antennas and 1 layer
const arma::cx_mat::fixed<2,1> W_2tx_1layer_0 =
{
  complex<double>( 1, 0)/sqrt(2),
  complex<double>( 1, 0)/sqrt(2)
};

const arma::cx_mat::fixed<2,1> W_2tx_1layer_1 =
{
  complex<double>( 1, 0)/sqrt(2),
  complex<double>(-1, 0)/sqrt(2)
};

const arma::cx_mat::fixed<2,1> W_2tx_1layer_2 =
{
  complex<double>( 1, 0)/sqrt(2),
  complex<double>( 0, 1)/sqrt(2)
};

const arma::cx_mat::fixed<2,1> W_2tx_1layer_3 =
{
  complex<double>( 1, 0)/sqrt(2),
  complex<double>( 0,-1)/sqrt(2)
};


// Precoding matrices for 2 tx antennas and 2 layers
const arma::cx_mat::fixed<2,2> W_2tx_2layer_0 =
{
  // first column
  complex<double>( 1, 0)/sqrt(2),
  complex<double>( 0, 0)/sqrt(2),
  // second column
  complex<double>( 0, 0)/sqrt(2),
  complex<double>( 1, 0)/sqrt(2)
};

const arma::cx_mat::fixed<2,2> W_2tx_2layer_1 =
{
  // first column
  complex<double>( 1, 0)/sqrt(4),
  complex<double>( 1, 0)/sqrt(4),
  // second column
  complex<double>( 1, 0)/sqrt(4),
  complex<double>(-1, 0)/sqrt(4)
};

const arma::cx_mat::fixed<2,2> W_2tx_2layer_2 =
{
  // first column
  complex<double>( 1, 0)/sqrt(4),
  complex<double>( 0, 1)/sqrt(4),
  // second column
  complex<double>( 1, 0)/sqrt(4),
  complex<double>( 0,-1)/sqrt(4)
};

// Pointers to precoding matrices for 2 tx antennas
const arma::cx_mat * const mimo_codebook_2tx[4][2] =
{
  &W_2tx_1layer_0, &W_2tx_2layer_0,
  &W_2tx_1layer_1, &W_2tx_2layer_1,
  &W_2tx_1layer_2, &W_2tx_2layer_2,
  &W_2tx_1layer_3, nullptr
};

// Precoding matrices for 4 tx antennas and 1 layer
const arma::cx_mat::fixed<4,1> W_4tx_1layer_0 =
{
  complex<double>( 1, 0)/sqrt(4),
  complex<double>( 1, 0)/sqrt(4),
  complex<double>( 1, 0)/sqrt(4),
  complex<double>( 1, 0)/sqrt(4)
};

const arma::cx_mat::fixed<4,1> W_4tx_1layer_1 =
{
  complex<double>( 1, 0)/sqrt(4),
  complex<double>( 0, 1)/sqrt(4),
  complex<double>(-1, 0)/sqrt(4),
  complex<double>( 0,-1)/sqrt(4)
};

const arma::cx_mat::fixed<4,1> W_4tx_1layer_2 =
{
  complex<double>( 1, 0)/sqrt(4),
  complex<double>(-1, 0)/sqrt(4),
  complex<double>( 1, 0)/sqrt(4),
  complex<double>(-1, 0)/sqrt(4)
};

const arma::cx_mat::fixed<4,1> W_4tx_1layer_3 =
{
  complex<double>( 1, 0)/sqrt(4),
  complex<double>( 0,-1)/sqrt(4),
  complex<double>(-1, 0)/sqrt(4),
  complex<double>( 0, 1)/sqrt(4)
};

const arma::cx_mat::fixed<4,1> W_4tx_1layer_4 =
{
  complex<double>( 1, 0)/sqrt(4),
  complex<double>( 1, 1)/sqrt(8),
  complex<double>(-1, 0)/sqrt(4),
  complex<double>(-1, 1)/sqrt(8)
};

const arma::cx_mat::fixed<4,1> W_4tx_1layer_5 =
{
  complex<double>( 1, 0)/sqrt(4),
  complex<double>(-1, 1)/sqrt(8),
  complex<double>( 0,-1)/sqrt(4),
  complex<double>( 1, 1)/sqrt(8)
};

const arma::cx_mat::fixed<4,1> W_4tx_1layer_6 =
{
  complex<double>( 1, 0)/sqrt(4),
  complex<double>(-1,-1)/sqrt(8),
  complex<double>( 0, 1)/sqrt(4),
  complex<double>( 1, 1)/sqrt(8)
};

const arma::cx_mat::fixed<4,1> W_4tx_1layer_7 =
{
  complex<double>( 1, 0)/sqrt(4),
  complex<double>( 1,-1)/sqrt(8),
  complex<double>( 0,-1)/sqrt(4),
  complex<double>(-1,-1)/sqrt(8)
};

const arma::cx_mat::fixed<4,1> W_4tx_1layer_8 =
{
  complex<double>( 1, 0)/sqrt(4),
  complex<double>( 1, 0)/sqrt(4),
  complex<double>(-1, 0)/sqrt(4),
  complex<double>(-1, 0)/sqrt(4)
};

const arma::cx_mat::fixed<4,1> W_4tx_1layer_9 =
{
  complex<double>( 1, 0)/sqrt(4),
  complex<double>( 0, 1)/sqrt(4),
  complex<double>( 1, 0)/sqrt(4),
  complex<double>( 0, 1)/sqrt(4)
};

const arma::cx_mat::fixed<4,1> W_4tx_1layer_10 =
{
  complex<double>( 1, 0)/sqrt(4),
  complex<double>(-1, 0)/sqrt(4),
  complex<double>(-1, 0)/sqrt(4),
  complex<double>( 1, 0)/sqrt(4)
};

const arma::cx_mat::fixed<4,1> W_4tx_1layer_11 =
{
  complex<double>( 1, 0)/sqrt(4),
  complex<double>( 0,-1)/sqrt(4),
  complex<double>( 1, 0)/sqrt(4),
  complex<double>( 0,-1)/sqrt(4)
};

const arma::cx_mat::fixed<4,1> W_4tx_1layer_12 =
{
  complex<double>( 1, 0)/sqrt(4),
  complex<double>( 1, 0)/sqrt(4),
  complex<double>( 1, 0)/sqrt(4),
  complex<double>(-1, 0)/sqrt(4)
};

const arma::cx_mat::fixed<4,1> W_4tx_1layer_13 =
{
  complex<double>( 1, 0)/sqrt(4),
  complex<double>( 1, 0)/sqrt(4),
  complex<double>(-1, 0)/sqrt(4),
  complex<double>( 1, 0)/sqrt(4)
};

const arma::cx_mat::fixed<4,1> W_4tx_1layer_14 =
{
  complex<double>( 1, 0)/sqrt(4),
  complex<double>(-1, 0)/sqrt(4),
  complex<double>( 1, 0)/sqrt(4),
  complex<double>( 1, 0)/sqrt(4)
};

const arma::cx_mat::fixed<4,1> W_4tx_1layer_15 =
{
  complex<double>( 1, 0)/sqrt(4),
  complex<double>(-1, 0)/sqrt(4),
  complex<double>(-1, 0)/sqrt(4),
  complex<double>(-1, 0)/sqrt(4)
};


// Precoding matrices for 4 tx antennas and 2 layers
const arma::cx_mat::fixed<4,2> W_4tx_2layer_0 =
{
  // first column
  complex<double>( 1, 0)/sqrt(8),
  complex<double>( 1, 0)/sqrt(8),
  complex<double>( 1, 0)/sqrt(8),
  complex<double>( 1, 0)/sqrt(8),
  // second column
  complex<double>( 1, 0)/sqrt(8),
  complex<double>(-1, 0)/sqrt(8),
  complex<double>(-1, 0)/sqrt(8),
  complex<double>( 1, 0)/sqrt(8)
};

const arma::cx_mat::fixed<4,2> W_4tx_2layer_1 =
{
  // first column
  complex<double>( 1, 0)/sqrt(8),
  complex<double>( 0, 1)/sqrt(8),
  complex<double>(-1, 0)/sqrt(8),
  complex<double>( 0,-1)/sqrt(8),
  // second column
  complex<double>( 0,-1)/sqrt(8),
  complex<double>( 1, 0)/sqrt(8),
  complex<double>( 0,-1)/sqrt(8),
  complex<double>( 1, 0)/sqrt(8)
};

const arma::cx_mat::fixed<4,2> W_4tx_2layer_2 =
{
  // first column
  complex<double>( 1, 0)/sqrt(8),
  complex<double>(-1, 0)/sqrt(8),
  complex<double>( 1, 0)/sqrt(8),
  complex<double>(-1, 0)/sqrt(8),
  // second column
  complex<double>(-1, 0)/sqrt(8),
  complex<double>( 1, 0)/sqrt(8),
  complex<double>( 1, 0)/sqrt(8),
  complex<double>(-1, 0)/sqrt(8)
};

const arma::cx_mat::fixed<4,2> W_4tx_2layer_3 =
{
  // first column
  complex<double>( 1, 0)/sqrt(8),
  complex<double>( 0,-1)/sqrt(8),
  complex<double>(-1, 0)/sqrt(8),
  complex<double>( 0, 1)/sqrt(8),
  // second column
  complex<double>( 0, 1)/sqrt(8),
  complex<double>( 1, 0)/sqrt(8),
  complex<double>( 0, 1)/sqrt(8),
  complex<double>( 1, 0)/sqrt(8)
};

const arma::cx_mat::fixed<4,2> W_4tx_2layer_4 =
{
  // first column
  complex<double>( 1, 0)/sqrt(8),
  complex<double>( 1, 1)/sqrt(16),
  complex<double>( 0, 1)/sqrt(8),
  complex<double>(-1, 1)/sqrt(16),
  // second column
  complex<double>(-1,-1)/sqrt(16),
  complex<double>( 0, 1)/sqrt(8),
  complex<double>(-1, 1)/sqrt(16),
  complex<double>( 1, 0)/sqrt(8)
};

const arma::cx_mat::fixed<4,2> W_4tx_2layer_5 =
{
  // first column
  complex<double>( 1, 0)/sqrt(8),
  complex<double>(-1, 1)/sqrt(16),
  complex<double>( 0,-1)/sqrt(8),
  complex<double>( 1, 1)/sqrt(16),
  // second column
  complex<double>( 1,-1)/sqrt(16),
  complex<double>( 0,-1)/sqrt(8),
  complex<double>( 1, 1)/sqrt(16),
  complex<double>( 1, 0)/sqrt(8)
};

const arma::cx_mat::fixed<4,2> W_4tx_2layer_6 =
{
  // first column
  complex<double>( 1, 0)/sqrt(8),
  complex<double>(-1,-1)/sqrt(16),
  complex<double>( 0, 1)/sqrt(8),
  complex<double>( 1,-1)/sqrt(16),
  // second column
  complex<double>( 0,-1)/sqrt(8),
  complex<double>( 1,-1)/sqrt(16),
  complex<double>( 1, 0)/sqrt(8),
  complex<double>( 1, 1)/sqrt(16)
};

const arma::cx_mat::fixed<4,2> W_4tx_2layer_7 =
{
  // first column
  complex<double>( 1, 0)/sqrt(8),
  complex<double>( 1,-1)/sqrt(16),
  complex<double>( 0,-1)/sqrt(8),
  complex<double>(-1,-1)/sqrt(16),
  // second column
  complex<double>( 0, 1)/sqrt(8),
  complex<double>(-1,-1)/sqrt(16),
  complex<double>( 1, 0)/sqrt(8),
  complex<double>(-1, 1)/sqrt(16)
};

const arma::cx_mat::fixed<4,2> W_4tx_2layer_8 =
{
  // first column
  complex<double>( 1, 0)/sqrt(8),
  complex<double>( 1, 0)/sqrt(8),
  complex<double>(-1, 0)/sqrt(8),
  complex<double>(-1, 0)/sqrt(8),
  // second column
  complex<double>( 1, 0)/sqrt(8),
  complex<double>( 1, 0)/sqrt(8),
  complex<double>( 1, 0)/sqrt(8),
  complex<double>( 1, 0)/sqrt(8)
};

const arma::cx_mat::fixed<4,2> W_4tx_2layer_9 =
{
  // first column
  complex<double>( 1, 0)/sqrt(8),
  complex<double>( 0, 1)/sqrt(8),
  complex<double>( 1, 0)/sqrt(8),
  complex<double>( 0, 1)/sqrt(8),
  // second column
  complex<double>( 0,-1)/sqrt(8),
  complex<double>(-1, 0)/sqrt(8),
  complex<double>( 0, 1)/sqrt(8),
  complex<double>( 1, 0)/sqrt(8)
};

const arma::cx_mat::fixed<4,2> W_4tx_2layer_10 =
{
  // first column
  complex<double>( 1, 0)/sqrt(8),
  complex<double>(-1, 0)/sqrt(8),
  complex<double>(-1, 0)/sqrt(8),
  complex<double>( 1, 0)/sqrt(8),
  // second column
  complex<double>(-1, 0)/sqrt(8),
  complex<double>(-1, 0)/sqrt(8),
  complex<double>( 1, 0)/sqrt(8),
  complex<double>( 1, 0)/sqrt(8)
};

const arma::cx_mat::fixed<4,2> W_4tx_2layer_11 =
{
  // first column
  complex<double>( 1, 0)/sqrt(8),
  complex<double>( 0,-1)/sqrt(8),
  complex<double>( 1, 0)/sqrt(8),
  complex<double>( 0,-1)/sqrt(8),
  // second column
  complex<double>( 1, 0)/sqrt(8),
  complex<double>( 0, 1)/sqrt(8),
  complex<double>( 1, 0)/sqrt(8),
  complex<double>( 0, 1)/sqrt(8)
};

const arma::cx_mat::fixed<4,2> W_4tx_2layer_12 =
{
  // first column
  complex<double>( 1, 0)/sqrt(8),
  complex<double>( 1, 0)/sqrt(8),
  complex<double>( 1, 0)/sqrt(8),
  complex<double>(-1, 0)/sqrt(8),
  // second column
  complex<double>( 1, 0)/sqrt(8),
  complex<double>( 1, 0)/sqrt(8),
  complex<double>(-1, 0)/sqrt(8),
  complex<double>( 1, 0)/sqrt(8)
};

const arma::cx_mat::fixed<4,2> W_4tx_2layer_13 =
{
  // first column
  complex<double>( 1, 0)/sqrt(8),
  complex<double>( 1, 0)/sqrt(8),
  complex<double>(-1, 0)/sqrt(8),
  complex<double>( 1, 0)/sqrt(8),
  // second column
  complex<double>(-1, 0)/sqrt(8),
  complex<double>( 1, 0)/sqrt(8),
  complex<double>( 1, 0)/sqrt(8),
  complex<double>( 1, 0)/sqrt(8)
};

const arma::cx_mat::fixed<4,2> W_4tx_2layer_14 =
{
  // first column
  complex<double>( 1, 0)/sqrt(8),
  complex<double>(-1, 0)/sqrt(8),
  complex<double>( 1, 0)/sqrt(8),
  complex<double>( 1, 0)/sqrt(8),
  // second column
  complex<double>( 1, 0)/sqrt(8),
  complex<double>( 1, 0)/sqrt(8),
  complex<double>( 1, 0)/sqrt(8),
  complex<double>(-1, 0)/sqrt(8)
};

const arma::cx_mat::fixed<4,2> W_4tx_2layer_15 =
{
  // first column
  complex<double>( 1, 0)/sqrt(8),
  complex<double>(-1, 0)/sqrt(8),
  complex<double>(-1, 0)/sqrt(8),
  complex<double>(-1, 0)/sqrt(8),
  // second column
  complex<double>(-1, 0)/sqrt(8),
  complex<double>( 1, 0)/sqrt(8),
  complex<double>(-1, 0)/sqrt(8),
  complex<double>(-1, 0)/sqrt(8)
};


// Precoding matrices for 4 tx antennas and 3 layers
const arma::cx_mat::fixed<4,3> W_4tx_3layer_0 =
{
  // first column
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  // second column
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  complex<double>(-1, 0)/sqrt(12),
  complex<double>(-1, 0)/sqrt(12),
  // third column
  complex<double>( 1, 0)/sqrt(12),
  complex<double>(-1, 0)/sqrt(12),
  complex<double>(-1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12)
};

const arma::cx_mat::fixed<4,3> W_4tx_3layer_1 =
{
  // first column
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 0, 1)/sqrt(12),
  complex<double>(-1, 0)/sqrt(12),
  complex<double>( 0,-1)/sqrt(12),
  // second column
  complex<double>( 0,-1)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 0,-1)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  // third column
  complex<double>(-1, 0)/sqrt(12),
  complex<double>( 0, 1)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 0,-1)/sqrt(12)
};

const arma::cx_mat::fixed<4,3> W_4tx_3layer_2 =
{
  // first column
  complex<double>( 1, 0)/sqrt(12),
  complex<double>(-1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  complex<double>(-1, 0)/sqrt(12),
  // second column
  complex<double>(-1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  complex<double>(-1, 0)/sqrt(12),
  // third column
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12)
};

const arma::cx_mat::fixed<4,3> W_4tx_3layer_3 =
{
  // first column
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 0,-1)/sqrt(12),
  complex<double>(-1, 0)/sqrt(12),
  complex<double>( 0, 1)/sqrt(12),
  // second column
  complex<double>( 0, 1)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 0, 1)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  // third column
  complex<double>(-1, 0)/sqrt(12),
  complex<double>( 0,-1)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 0, 1)/sqrt(12)
};

const arma::cx_mat::fixed<4,3> W_4tx_3layer_4 =
{
  // first column
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 1, 1)/sqrt(24),
  complex<double>( 0, 1)/sqrt(12),
  complex<double>(-1, 1)/sqrt(24),
  // second column
  complex<double>( 1,-1)/sqrt(24),
  complex<double>( 1, 0)/sqrt(12),
  complex<double>(-1,-1)/sqrt(24),
  complex<double>( 0,-1)/sqrt(12),
  // third column
  complex<double>(-1,-1)/sqrt(24),
  complex<double>( 0, 1)/sqrt(12),
  complex<double>(-1, 1)/sqrt(24),
  complex<double>( 1, 0)/sqrt(12)
};

const arma::cx_mat::fixed<4,3> W_4tx_3layer_5 =
{
  // first column
  complex<double>( 1, 0)/sqrt(12),
  complex<double>(-1, 1)/sqrt(24),
  complex<double>( 0,-1)/sqrt(12),
  complex<double>( 1, 1)/sqrt(24),
  // second column
  complex<double>(-1,-1)/sqrt(24),
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 1,-1)/sqrt(24),
  complex<double>( 0, 1)/sqrt(12),
  // third column
  complex<double>( 1,-1)/sqrt(24),
  complex<double>( 0,-1)/sqrt(12),
  complex<double>( 1, 1)/sqrt(24),
  complex<double>( 1, 0)/sqrt(12)
};

const arma::cx_mat::fixed<4,3> W_4tx_3layer_6 =
{
  // first column
  complex<double>( 1, 0)/sqrt(12),
  complex<double>(-1,-1)/sqrt(24),
  complex<double>( 0, 1)/sqrt(12),
  complex<double>( 1,-1)/sqrt(24),
  // second column
  complex<double>( 0,-1)/sqrt(12),
  complex<double>( 1,-1)/sqrt(24),
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 1, 1)/sqrt(24),
  // third column
  complex<double>( 1, 1)/sqrt(24),
  complex<double>( 0, 1)/sqrt(12),
  complex<double>( 1,-1)/sqrt(24),
  complex<double>( 1, 0)/sqrt(12)
};

const arma::cx_mat::fixed<4,3> W_4tx_3layer_7 =
{
  // first column
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 1,-1)/sqrt(24),
  complex<double>( 0,-1)/sqrt(12),
  complex<double>(-1,-1)/sqrt(24),
  // second column
  complex<double>( 0, 1)/sqrt(12),
  complex<double>(-1,-1)/sqrt(24),
  complex<double>( 1, 0)/sqrt(12),
  complex<double>(-1, 1)/sqrt(24),
  // third column
  complex<double>(-1, 1)/sqrt(24),
  complex<double>( 0,-1)/sqrt(12),
  complex<double>(-1,-1)/sqrt(24),
  complex<double>( 1, 0)/sqrt(12)
};

const arma::cx_mat::fixed<4,3> W_4tx_3layer_8 =
{
  // first column
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  complex<double>(-1, 0)/sqrt(12),
  complex<double>(-1, 0)/sqrt(12),
  // second column
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  // third column
  complex<double>(-1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  complex<double>(-1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12)
};

const arma::cx_mat::fixed<4,3> W_4tx_3layer_9 =
{
  // first column
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 0, 1)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 0, 1)/sqrt(12),
  // second column
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 0,-1)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 0,-1)/sqrt(12),
  // third column
  complex<double>( 0,-1)/sqrt(12),
  complex<double>(-1, 0)/sqrt(12),
  complex<double>( 0, 1)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12)
};

const arma::cx_mat::fixed<4,3> W_4tx_3layer_10 =
{
  // first column
  complex<double>( 1, 0)/sqrt(12),
  complex<double>(-1, 0)/sqrt(12),
  complex<double>(-1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  // second column
  complex<double>(-1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  complex<double>(-1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  // third column
  complex<double>(-1, 0)/sqrt(12),
  complex<double>(-1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12)
};

const arma::cx_mat::fixed<4,3> W_4tx_3layer_11 =
{
  // first column
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 0,-1)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 0,-1)/sqrt(12),
  // second column
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 0, 1)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 0, 1)/sqrt(12),
  // third column
  complex<double>( 0, 1)/sqrt(12),
  complex<double>(-1, 0)/sqrt(12),
  complex<double>( 0,-1)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12)
};

const arma::cx_mat::fixed<4,3> W_4tx_3layer_12 =
{
  // first column
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  complex<double>(-1, 0)/sqrt(12),
  // second column
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  complex<double>(-1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  // third column
  complex<double>( 1, 0)/sqrt(12),
  complex<double>(-1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12)
};

const arma::cx_mat::fixed<4,3> W_4tx_3layer_13 =
{
  // first column
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  complex<double>(-1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  // second column
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  complex<double>(-1, 0)/sqrt(12),
  // third column
  complex<double>(-1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12)
};

const arma::cx_mat::fixed<4,3> W_4tx_3layer_14 =
{
  // first column
  complex<double>( 1, 0)/sqrt(12),
  complex<double>(-1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  // second column
  complex<double>(-1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  // third column
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  complex<double>(-1, 0)/sqrt(12)
};

const arma::cx_mat::fixed<4,3> W_4tx_3layer_15 =
{
  // first column
  complex<double>( 1, 0)/sqrt(12),
  complex<double>(-1, 0)/sqrt(12),
  complex<double>(-1, 0)/sqrt(12),
  complex<double>(-1, 0)/sqrt(12),
  // second column
  complex<double>(-1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  complex<double>(-1, 0)/sqrt(12),
  complex<double>(-1, 0)/sqrt(12),
  // third column
  complex<double>(-1, 0)/sqrt(12),
  complex<double>(-1, 0)/sqrt(12),
  complex<double>( 1, 0)/sqrt(12),
  complex<double>(-1, 0)/sqrt(12)
};


// Precoding matrices for 4 tx antennas and 4 layers
const arma::cx_mat::fixed<4,4> W_4tx_4layer_0 =
{
  // first column
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  // second column
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>(-1, 0)/sqrt(16),
  complex<double>(-1, 0)/sqrt(16),
  // third column
  complex<double>( 1, 0)/sqrt(16),
  complex<double>(-1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>(-1, 0)/sqrt(16),
  // fourth column
  complex<double>( 1, 0)/sqrt(16),
  complex<double>(-1, 0)/sqrt(16),
  complex<double>(-1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16)
};

const arma::cx_mat::fixed<4,4> W_4tx_4layer_1 =
{
  // first column
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 0, 1)/sqrt(16),
  complex<double>(-1, 0)/sqrt(16),
  complex<double>( 0,-1)/sqrt(16),
  // second column
  complex<double>( 0,-1)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 0,-1)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  // third column
  complex<double>(-1, 0)/sqrt(16),
  complex<double>( 0, 1)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 0,-1)/sqrt(16),
  // fourth column
  complex<double>( 0, 1)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 0, 1)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16)
};

const arma::cx_mat::fixed<4,4> W_4tx_4layer_2 =
{
  // first column
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  // second column
  complex<double>(-1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>(-1, 0)/sqrt(16),
  // third column
  complex<double>( 1, 0)/sqrt(16),
  complex<double>(-1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>(-1, 0)/sqrt(16),
  // fourth column
  complex<double>(-1, 0)/sqrt(16),
  complex<double>(-1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16)
};

const arma::cx_mat::fixed<4,4> W_4tx_4layer_3 =
{
  // first column
  complex<double>(-1, 0)/sqrt(16),
  complex<double>( 0,-1)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 0, 1)/sqrt(16),
  // second column
  complex<double>( 0, 1)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 0, 1)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  // third column
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 0,-1)/sqrt(16),
  complex<double>(-1, 0)/sqrt(16),
  complex<double>( 0, 1)/sqrt(16),
  // fourth column
  complex<double>( 0,-1)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 0,-1)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16)
};

const arma::cx_mat::fixed<4,4> W_4tx_4layer_4 =
{
  // first column
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 1, 1)/sqrt(32),
  complex<double>( 0, 1)/sqrt(16),
  complex<double>(-1, 1)/sqrt(32),
  // second column
  complex<double>( 1,-1)/sqrt(32),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>(-1,-1)/sqrt(32),
  complex<double>( 0,-1)/sqrt(16),
  // third column
  complex<double>( 0,-1)/sqrt(16),
  complex<double>(-1, 1)/sqrt(32),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>(-1,-1)/sqrt(32),
  // fourth column
  complex<double>(-1,-1)/sqrt(32),
  complex<double>( 0, 1)/sqrt(16),
  complex<double>(-1, 1)/sqrt(32),
  complex<double>( 1, 0)/sqrt(16)
};

const arma::cx_mat::fixed<4,4> W_4tx_4layer_5 =
{
  // first column
  complex<double>( 1, 0)/sqrt(16),
  complex<double>(-1, 1)/sqrt(32),
  complex<double>( 0,-1)/sqrt(16),
  complex<double>( 1, 1)/sqrt(32),
  // second column
  complex<double>(-1,-1)/sqrt(32),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 1,-1)/sqrt(32),
  complex<double>( 0, 1)/sqrt(16),
  // third column
  complex<double>( 0, 1)/sqrt(16),
  complex<double>( 1, 1)/sqrt(32),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 1,-1)/sqrt(32),
  // fourth column
  complex<double>( 1,-1)/sqrt(32),
  complex<double>( 0,-1)/sqrt(16),
  complex<double>( 1, 1)/sqrt(32),
  complex<double>( 1, 0)/sqrt(16)
};

const arma::cx_mat::fixed<4,4> W_4tx_4layer_6 =
{
  // first column
  complex<double>( 1, 0)/sqrt(16),
  complex<double>(-1,-1)/sqrt(32),
  complex<double>( 0, 1)/sqrt(16),
  complex<double>( 1,-1)/sqrt(32),
  // second column
  complex<double>( 0,-1)/sqrt(16),
  complex<double>( 1,-1)/sqrt(32),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 1, 1)/sqrt(32),
  // third column
  complex<double>(-1, 1)/sqrt(32),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 1, 1)/sqrt(32),
  complex<double>( 0,-1)/sqrt(16),
  // fourth column
  complex<double>( 1, 1)/sqrt(32),
  complex<double>( 0, 1)/sqrt(16),
  complex<double>( 1,-1)/sqrt(32),
  complex<double>( 1, 0)/sqrt(16)
};

const arma::cx_mat::fixed<4,4> W_4tx_4layer_7 =
{
  // first column
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 1,-1)/sqrt(32),
  complex<double>( 0,-1)/sqrt(16),
  complex<double>(-1,-1)/sqrt(32),
  // second column
  complex<double>( 0, 1)/sqrt(16),
  complex<double>(-1,-1)/sqrt(32),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>(-1, 1)/sqrt(32),
  // third column
  complex<double>( 1, 1)/sqrt(32),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>(-1, 1)/sqrt(32),
  complex<double>( 0, 1)/sqrt(16),
  // fourth column
  complex<double>(-1, 1)/sqrt(32),
  complex<double>( 0,-1)/sqrt(16),
  complex<double>(-1,-1)/sqrt(32),
  complex<double>( 1, 0)/sqrt(16)
};

const arma::cx_mat::fixed<4,4> W_4tx_4layer_8 =
{
  // first column
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>(-1, 0)/sqrt(16),
  complex<double>(-1, 0)/sqrt(16),
  // second column
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  // third column
  complex<double>(-1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>(-1, 0)/sqrt(16),
  // fourth column
  complex<double>(-1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>(-1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16)
};

const arma::cx_mat::fixed<4,4> W_4tx_4layer_9 =
{
  // first column
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 0, 1)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 0, 1)/sqrt(16),
  // second column
  complex<double>( 0,-1)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 0, 1)/sqrt(16),
  complex<double>(-1, 0)/sqrt(16),
  // third column
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 0,-1)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 0,-1)/sqrt(16),
  // fourth column
  complex<double>( 0,-1)/sqrt(16),
  complex<double>(-1, 0)/sqrt(16),
  complex<double>( 0, 1)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16)
};

const arma::cx_mat::fixed<4,4> W_4tx_4layer_10 =
{
  // first column
  complex<double>( 1, 0)/sqrt(16),
  complex<double>(-1, 0)/sqrt(16),
  complex<double>(-1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  // second column
  complex<double>(-1, 0)/sqrt(16),
  complex<double>(-1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  // third column
  complex<double>(-1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>(-1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  // fourth column
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16)
};

const arma::cx_mat::fixed<4,4> W_4tx_4layer_11 =
{
  // first column
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 0,-1)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 0,-1)/sqrt(16),
  // second column
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 0, 1)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 0, 1)/sqrt(16),
  // third column
  complex<double>( 0, 1)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 0,-1)/sqrt(16),
  complex<double>(-1, 0)/sqrt(16),
  // fourth column
  complex<double>( 0, 1)/sqrt(16),
  complex<double>(-1, 0)/sqrt(16),
  complex<double>( 0,-1)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16)
};

const arma::cx_mat::fixed<4,4> W_4tx_4layer_12 =
{
  // first column
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>(-1, 0)/sqrt(16),
  // second column
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>(-1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  // third column
  complex<double>( 1, 0)/sqrt(16),
  complex<double>(-1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  // fourth column
  complex<double>(-1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16)
};

const arma::cx_mat::fixed<4,4> W_4tx_4layer_13 =
{
  // first column
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>(-1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  // second column
  complex<double>(-1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  // third column
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>(-1, 0)/sqrt(16),
  // fourth column
  complex<double>( 1, 0)/sqrt(16),
  complex<double>(-1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16)
};

const arma::cx_mat::fixed<4,4> W_4tx_4layer_14 =
{
  // first column
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>(-1, 0)/sqrt(16),
  // second column
  complex<double>(-1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  // third column
  complex<double>( 1, 0)/sqrt(16),
  complex<double>(-1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  // fourth column
  complex<double>( 1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>(-1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16)
};

const arma::cx_mat::fixed<4,4> W_4tx_4layer_15 =
{
  // first column
  complex<double>( 1, 0)/sqrt(16),
  complex<double>(-1, 0)/sqrt(16),
  complex<double>(-1, 0)/sqrt(16),
  complex<double>(-1, 0)/sqrt(16),
  // second column
  complex<double>(-1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>(-1, 0)/sqrt(16),
  complex<double>(-1, 0)/sqrt(16),
  // third column
  complex<double>(-1, 0)/sqrt(16),
  complex<double>(-1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16),
  complex<double>(-1, 0)/sqrt(16),
  // fourth column
  complex<double>(-1, 0)/sqrt(16),
  complex<double>(-1, 0)/sqrt(16),
  complex<double>(-1, 0)/sqrt(16),
  complex<double>( 1, 0)/sqrt(16)
};


// Pointers to precoding matrices for 4 tx antennas
const arma::cx_mat * const mimo_codebook_4tx[16][4] =
{
  &W_4tx_1layer_0,  &W_4tx_2layer_0,  &W_4tx_3layer_0,  &W_4tx_4layer_0,
  &W_4tx_1layer_1,  &W_4tx_2layer_1,  &W_4tx_3layer_1,  &W_4tx_4layer_1,
  &W_4tx_1layer_2,  &W_4tx_2layer_2,  &W_4tx_3layer_2,  &W_4tx_4layer_2,
  &W_4tx_1layer_3,  &W_4tx_2layer_3,  &W_4tx_3layer_3,  &W_4tx_4layer_3,
  &W_4tx_1layer_4,  &W_4tx_2layer_4,  &W_4tx_3layer_4,  &W_4tx_4layer_4,
  &W_4tx_1layer_5,  &W_4tx_2layer_5,  &W_4tx_3layer_5,  &W_4tx_4layer_5,
  &W_4tx_1layer_6,  &W_4tx_2layer_6,  &W_4tx_3layer_6,  &W_4tx_4layer_6,
  &W_4tx_1layer_7,  &W_4tx_2layer_7,  &W_4tx_3layer_7,  &W_4tx_4layer_7,
  &W_4tx_1layer_8,  &W_4tx_2layer_8,  &W_4tx_3layer_8,  &W_4tx_4layer_8,
  &W_4tx_1layer_9,  &W_4tx_2layer_9,  &W_4tx_3layer_9,  &W_4tx_4layer_9,
  &W_4tx_1layer_10, &W_4tx_2layer_10, &W_4tx_3layer_10, &W_4tx_4layer_10,
  &W_4tx_1layer_11, &W_4tx_2layer_11, &W_4tx_3layer_11, &W_4tx_4layer_11,
  &W_4tx_1layer_12, &W_4tx_2layer_12, &W_4tx_3layer_12, &W_4tx_4layer_12,
  &W_4tx_1layer_13, &W_4tx_2layer_13, &W_4tx_3layer_13, &W_4tx_4layer_13,
  &W_4tx_1layer_14, &W_4tx_2layer_14, &W_4tx_3layer_14, &W_4tx_4layer_14,
  &W_4tx_1layer_15, &W_4tx_2layer_15, &W_4tx_3layer_15, &W_4tx_4layer_15,
};

// Matrices for CDD processing

const arma::cx_mat::fixed<1,1> CDD_U_1layer =
{
  1
};

const arma::cx_mat::fixed<2,2> CDD_U_2layer =
{
  polar(1/sqrt(2),0.), polar(1/sqrt(2),0.),
  polar(1/sqrt(2),0.), polar(1/sqrt(2),2*M_PI/2)
};

const arma::cx_mat::fixed<3,3> CDD_U_3layer =
{
  polar(1/sqrt(3),0.), polar(1/sqrt(3),0.),     polar(1/sqrt(3),0.),
  polar(1/sqrt(3),0.), polar(1/sqrt(3),2*M_PI/3), polar(1/sqrt(3),4*M_PI/3),
  polar(1/sqrt(3),0.), polar(1/sqrt(3),4*M_PI/3), polar(1/sqrt(3),8*M_PI/3)
};

const arma::cx_mat::fixed<4,4> CDD_U_4layer =
{
  polar(1/sqrt(4),0.), polar(1/sqrt(4),0.),     polar(1/sqrt(4), 0.),     polar(1/sqrt(4), 0.),
  polar(1/sqrt(4),0.), polar(1/sqrt(4),2*M_PI/4), polar(1/sqrt(4), 4*M_PI/4), polar(1/sqrt(4), 6*M_PI/4),
  polar(1/sqrt(4),0.), polar(1/sqrt(4),4*M_PI/4), polar(1/sqrt(4), 8*M_PI/4), polar(1/sqrt(4),12*M_PI/4),
  polar(1/sqrt(4),0.), polar(1/sqrt(4),6*M_PI/4), polar(1/sqrt(4),12*M_PI/4), polar(1/sqrt(4),18*M_PI/4)
};

const arma::cx_mat * const CDD_U[4] =
{
  &CDD_U_1layer,
  &CDD_U_2layer,
  &CDD_U_3layer,
  &CDD_U_4layer
};

const arma::cx_mat::fixed<1,1> CDD_D_1layer_0 =
{
  1
};

const arma::cx_mat::fixed<2,2> CDD_D_2layer_0 =
{
  1,   0,
  0,   polar(1.0, 2*M_PI*0/2)
};

const arma::cx_mat::fixed<2,2> CDD_D_2layer_1 =
{
  1,   0,
  0,   polar(1.0, 2*M_PI*1/2)
};

const arma::cx_mat::fixed<3,3> CDD_D_3layer_0 =
{
  1,   0,                    0,
  0,   polar(1.0, 2*M_PI*0/3), 0,
  0,   0,                    polar(1.0, 4*M_PI*0/3)
};

const arma::cx_mat::fixed<3,3> CDD_D_3layer_1 =
{
  1,   0,                    0,
  0,   polar(1.0, 2*M_PI*1/3), 0,
  0,   0,                    polar(1.0, 4*M_PI*1/3)
};

const arma::cx_mat::fixed<3,3> CDD_D_3layer_2 =
{
  1,   0,                    0,
  0,   polar(1.0, 2*M_PI*2/3), 0,
  0,   0,                    polar(1.0, 4*M_PI*2/3)
};


const arma::cx_mat::fixed<4,4> CDD_D_4layer_0 =
{
  1,   0,                    0,                    0,
  0,   polar(1.0, 2*M_PI*0/4), 0,                    0,
  0,   0,                    polar(1.0, 4*M_PI*0/4), 0,
  0,   0,                    0,                    polar(1.0, 6*M_PI*0/4)
};

const arma::cx_mat::fixed<4,4> CDD_D_4layer_1 =
{
  1,   0,                    0,                    0,
  0,   polar(1.0, 2*M_PI*1/4), 0,                    0,
  0,   0,                    polar(1.0, 4*M_PI*1/4), 0,
  0,   0,                    0,                    polar(1.0, 6*M_PI*1/4)
};

const arma::cx_mat::fixed<4,4> CDD_D_4layer_2 =
{
  1,   0,                    0,                    0,
  0,   polar(1.0, 2*M_PI*2/4), 0,                    0,
  0,   0,                    polar(1.0, 4*M_PI*2/4), 0,
  0,   0,                    0,                    polar(1.0, 6*M_PI*2/4)
};

const arma::cx_mat::fixed<4,4> CDD_D_4layer_3 =
{
  1,   0,                    0,                    0,
  0,   polar(1.0, 2*M_PI*3/4), 0,                    0,
  0,   0,                    polar(1.0, 4*M_PI*3/4), 0,
  0,   0,                    0,                    polar(1.0, 6*M_PI*3/4)
};


const arma::cx_mat * const CDD_D[4][4] =
{
  { &CDD_D_1layer_0, nullptr,         nullptr,         nullptr,        },
  { &CDD_D_2layer_0, &CDD_D_2layer_1, nullptr,         nullptr,        },
  { &CDD_D_3layer_0, &CDD_D_3layer_1, &CDD_D_3layer_2, nullptr,        },
  { &CDD_D_4layer_0, &CDD_D_4layer_1, &CDD_D_4layer_2, &CDD_D_4layer_3 }
};


// Precoding matrices for 8 tx antennas
arma::cx_mat codebook_8tx(int l, int i1, int i2)
{
  switch(l)
    {
      case 1:
      {
        if (i1<0 || i1>15 || i2<0 || i2>15)
          {
            cout << "Invalid parameters for 8-tx codebook: l=" << l << ", i1=" << i1 << ", i2=" << i2 << endl;
            exit(1);
          }
        int m = 2 * i1 + (i2 / 4) % 4;
        int n = i2 % 4;
        arma::cx_mat::fixed<8,1> W =
        {
          polar( 1/sqrt(8), 0*M_PI*m/32 ),
          polar( 1/sqrt(8), 2*M_PI*m/32 ),
          polar( 1/sqrt(8), 4*M_PI*m/32 ),
          polar( 1/sqrt(8), 6*M_PI*m/32 ),
          polar( 1/sqrt(8), 0*M_PI*m/32 ) * polar( 1.0, M_PI*n/2 ),
          polar( 1/sqrt(8), 2*M_PI*m/32 ) * polar( 1.0, M_PI*n/2 ),
          polar( 1/sqrt(8), 4*M_PI*m/32 ) * polar( 1.0, M_PI*n/2 ),
          polar( 1/sqrt(8), 6*M_PI*m/32 ) * polar( 1.0, M_PI*n/2 )
        };
        return W;
      }
      break;

      case 2:
      {
        if (i1<0 || i1>15 || i2<0 || i2>15)
          {
            cout << "Invalid parameters for 8-tx codebook: l=" << l << ", i1=" << i1 << ", i2=" << i2 << endl;
            exit(1);
          }
        int m = 2 * i1;
        int m1 = 2 * i1;
        int n = i2 % 2;
        switch(i2)
          {
            case 0:
            case 1:
              break;
            case 2:
            case 3:
              m  += 1;
              m1 += 1;
              break;
            case 4:
            case 5:
              m  += 2;
              m1 += 2;
              break;
            case 6:
            case 7:
              m  += 3;
              m1 += 3;
              break;
            case 8:
            case 9:
              m1 += 1;
              break;
            case 10:
            case 11:
              m  += 1;
              m1 += 2;
              break;
            case 12:
            case 13:
              m1 += 3;
              break;
            case 14:
            case 15:
              m  += 1;
              m1 += 3;
              break;
          }

        arma::cx_mat::fixed<8,2> W =
        {
          // first column
          polar( 1/sqrt(16), 0*M_PI*m/32 ),
          polar( 1/sqrt(16), 2*M_PI*m/32 ),
          polar( 1/sqrt(16), 4*M_PI*m/32 ),
          polar( 1/sqrt(16), 6*M_PI*m/32 ),
          polar( 1/sqrt(16), 0*M_PI*m/32 ) * polar( 1.0, M_PI*n/2 ),
          polar( 1/sqrt(16), 2*M_PI*m/32 ) * polar( 1.0, M_PI*n/2 ),
          polar( 1/sqrt(16), 4*M_PI*m/32 ) * polar( 1.0, M_PI*n/2 ),
          polar( 1/sqrt(16), 6*M_PI*m/32 ) * polar( 1.0, M_PI*n/2 ),
          // second column
          polar( 1/sqrt(16), 0*M_PI*m1/32 ),
          polar( 1/sqrt(16), 2*M_PI*m1/32 ),
          polar( 1/sqrt(16), 4*M_PI*m1/32 ),
          polar( 1/sqrt(16), 6*M_PI*m1/32 ),
          polar( 1/sqrt(16), 0*M_PI*m1/32 ) * polar( 1.0, M_PI*n/2 ) * (-1.0),
          polar( 1/sqrt(16), 2*M_PI*m1/32 ) * polar( 1.0, M_PI*n/2 ) * (-1.0),
          polar( 1/sqrt(16), 4*M_PI*m1/32 ) * polar( 1.0, M_PI*n/2 ) * (-1.0),
          polar( 1/sqrt(16), 6*M_PI*m1/32 ) * polar( 1.0, M_PI*n/2 ) * (-1.0)
        };
        return W;
      }
      break;

      case 3:
      {
        if (i1<0 || i1>3 || i2<0 || i2>15)
          {
            cout << "Invalid parameters for 8-tx codebook: l=" << l << ", i1=" << i1 << ", i2=" << i2 << endl;
            exit(1);
          }
        int m =  8 * i1 + 2 * (i2 % 4) + 8 * (i2 % 2);
        int m1 = 8 * i1 + 2 * (i2 % 4) + 8 * (((i2+1)%4)/3);
        int m2 = 8 * i1 + 2 * (i2 % 4) + 8 * ((((i2+1)%4)+((i2+2)%4))/3);
        int inversion = (((i2/2)%2)*2 - 1);

        arma::cx_mat::fixed<8,3> W =
        {
          // first column
          polar( 1/sqrt(24), 0*M_PI*m/32 ),
          polar( 1/sqrt(24), 2*M_PI*m/32 ),
          polar( 1/sqrt(24), 4*M_PI*m/32 ),
          polar( 1/sqrt(24), 6*M_PI*m/32 ),
          polar( 1/sqrt(24), 0*M_PI*m/32 ),
          polar( 1/sqrt(24), 2*M_PI*m/32 ),
          polar( 1/sqrt(24), 4*M_PI*m/32 ),
          polar( 1/sqrt(24), 6*M_PI*m/32 ),
          // second column
          polar( 1/sqrt(24), 0*M_PI*m1/32 ),
          polar( 1/sqrt(24), 2*M_PI*m1/32 ),
          polar( 1/sqrt(24), 4*M_PI*m1/32 ),
          polar( 1/sqrt(24), 6*M_PI*m1/32 ),
          polar( 1/sqrt(24), 0*M_PI*m1/32 ) * (double)inversion,
          polar( 1/sqrt(24), 2*M_PI*m1/32 ) * (double)inversion,
          polar( 1/sqrt(24), 4*M_PI*m1/32 ) * (double)inversion,
          polar( 1/sqrt(24), 6*M_PI*m1/32 ) * (double)inversion,
          // third column
          polar( 1/sqrt(24), 0*M_PI*m2/32 ),
          polar( 1/sqrt(24), 2*M_PI*m2/32 ),
          polar( 1/sqrt(24), 4*M_PI*m2/32 ),
          polar( 1/sqrt(24), 6*M_PI*m2/32 ),
          polar( 1/sqrt(24), 0*M_PI*m2/32 ) * (-1.0),
          polar( 1/sqrt(24), 2*M_PI*m2/32 ) * (-1.0),
          polar( 1/sqrt(24), 4*M_PI*m2/32 ) * (-1.0),
          polar( 1/sqrt(24), 6*M_PI*m2/32 ) * (-1.0)
        };
        return W;
      }
      break;

      case 4:
      {
        if (i1<0 || i1>3 || i2<0 || i2>7)
          {
            cout << "Invalid parameters for 8-tx codebook: l=" << l << ", i1=" << i1 << ", i2=" << i2 << endl;
            exit(1);
          }
        int m =  8 * i1 + 2 * (i2 / 2);
        int m1 = 8 * i1 + 8 + 2 * (i2 / 2);
        int n = i2 % 2;

        arma::cx_mat::fixed<8,4> W =
        {
          // first column
          polar( 1/sqrt(32), 0*M_PI*m/32 ),
          polar( 1/sqrt(32), 2*M_PI*m/32 ),
          polar( 1/sqrt(32), 4*M_PI*m/32 ),
          polar( 1/sqrt(32), 6*M_PI*m/32 ),
          polar( 1/sqrt(32), 0*M_PI*m/32 ) * polar( 1.0, M_PI*n/2 ),
          polar( 1/sqrt(32), 2*M_PI*m/32 ) * polar( 1.0, M_PI*n/2 ),
          polar( 1/sqrt(32), 4*M_PI*m/32 ) * polar( 1.0, M_PI*n/2 ),
          polar( 1/sqrt(32), 6*M_PI*m/32 ) * polar( 1.0, M_PI*n/2 ),
          // second column
          polar( 1/sqrt(32), 0*M_PI*m1/32 ),
          polar( 1/sqrt(32), 2*M_PI*m1/32 ),
          polar( 1/sqrt(32), 4*M_PI*m1/32 ),
          polar( 1/sqrt(32), 6*M_PI*m1/32 ),
          polar( 1/sqrt(32), 0*M_PI*m1/32 ) * polar( 1.0, M_PI*n/2 ),
          polar( 1/sqrt(32), 2*M_PI*m1/32 ) * polar( 1.0, M_PI*n/2 ),
          polar( 1/sqrt(32), 4*M_PI*m1/32 ) * polar( 1.0, M_PI*n/2 ),
          polar( 1/sqrt(32), 6*M_PI*m1/32 ) * polar( 1.0, M_PI*n/2 ),
          // third column
          polar( 1/sqrt(32), 0*M_PI*m/32 ),
          polar( 1/sqrt(32), 2*M_PI*m/32 ),
          polar( 1/sqrt(32), 4*M_PI*m/32 ),
          polar( 1/sqrt(32), 6*M_PI*m/32 ),
          polar( 1/sqrt(32), 0*M_PI*m/32 ) * polar( 1.0, M_PI*n/2 ) * (-1.0),
          polar( 1/sqrt(32), 2*M_PI*m/32 ) * polar( 1.0, M_PI*n/2 ) * (-1.0),
          polar( 1/sqrt(32), 4*M_PI*m/32 ) * polar( 1.0, M_PI*n/2 ) * (-1.0),
          polar( 1/sqrt(32), 6*M_PI*m/32 ) * polar( 1.0, M_PI*n/2 ) * (-1.0),
          // fourth column
          polar( 1/sqrt(32), 0*M_PI*m1/32 ),
          polar( 1/sqrt(32), 2*M_PI*m1/32 ),
          polar( 1/sqrt(32), 4*M_PI*m1/32 ),
          polar( 1/sqrt(32), 6*M_PI*m1/32 ),
          polar( 1/sqrt(32), 0*M_PI*m1/32 ) * polar( 1.0, M_PI*n/2 ) * (-1.0),
          polar( 1/sqrt(32), 2*M_PI*m1/32 ) * polar( 1.0, M_PI*n/2 ) * (-1.0),
          polar( 1/sqrt(32), 4*M_PI*m1/32 ) * polar( 1.0, M_PI*n/2 ) * (-1.0),
          polar( 1/sqrt(32), 6*M_PI*m1/32 ) * polar( 1.0, M_PI*n/2 ) * (-1.0)
        };
        return W;
      }
      break;

      case 5:
      {
        if (i1<0 || i1>3 || i2!=0)
          {
            cout << "Invalid parameters for 8-tx codebook: l=" << l << ", i1=" << i1 << ", i2=" << i2 << endl;
            exit(1);
          }
        int m  = 2 * i1;
        int m1 = 2 * i1 + 8;
        int m2 = 2 * i1 + 16;

        arma::cx_mat::fixed<8,5> W =
        {
          // first column
          polar( 1/sqrt(40), 0*M_PI*m/32 ),
          polar( 1/sqrt(40), 2*M_PI*m/32 ),
          polar( 1/sqrt(40), 4*M_PI*m/32 ),
          polar( 1/sqrt(40), 6*M_PI*m/32 ),
          polar( 1/sqrt(40), 0*M_PI*m/32 ),
          polar( 1/sqrt(40), 2*M_PI*m/32 ),
          polar( 1/sqrt(40), 4*M_PI*m/32 ),
          polar( 1/sqrt(40), 6*M_PI*m/32 ),
          // second column
          polar( 1/sqrt(40), 0*M_PI*m/32 ),
          polar( 1/sqrt(40), 2*M_PI*m/32 ),
          polar( 1/sqrt(40), 4*M_PI*m/32 ),
          polar( 1/sqrt(40), 6*M_PI*m/32 ),
          polar( 1/sqrt(40), 0*M_PI*m/32 ) * (-1.0),
          polar( 1/sqrt(40), 2*M_PI*m/32 ) * (-1.0),
          polar( 1/sqrt(40), 4*M_PI*m/32 ) * (-1.0),
          polar( 1/sqrt(40), 6*M_PI*m/32 ) * (-1.0),
          // third column
          polar( 1/sqrt(40), 0*M_PI*m1/32 ),
          polar( 1/sqrt(40), 2*M_PI*m1/32 ),
          polar( 1/sqrt(40), 4*M_PI*m1/32 ),
          polar( 1/sqrt(40), 6*M_PI*m1/32 ),
          polar( 1/sqrt(40), 0*M_PI*m1/32 ),
          polar( 1/sqrt(40), 2*M_PI*m1/32 ),
          polar( 1/sqrt(40), 4*M_PI*m1/32 ),
          polar( 1/sqrt(40), 6*M_PI*m1/32 ),
          // fourth column
          polar( 1/sqrt(40), 0*M_PI*m1/32 ),
          polar( 1/sqrt(40), 2*M_PI*m1/32 ),
          polar( 1/sqrt(40), 4*M_PI*m1/32 ),
          polar( 1/sqrt(40), 6*M_PI*m1/32 ),
          polar( 1/sqrt(40), 0*M_PI*m1/32 ) * (-1.0),
          polar( 1/sqrt(40), 2*M_PI*m1/32 ) * (-1.0),
          polar( 1/sqrt(40), 4*M_PI*m1/32 ) * (-1.0),
          polar( 1/sqrt(40), 6*M_PI*m1/32 ) * (-1.0),
          // fifth column
          polar( 1/sqrt(40), 0*M_PI*m2/32 ),
          polar( 1/sqrt(40), 2*M_PI*m2/32 ),
          polar( 1/sqrt(40), 4*M_PI*m2/32 ),
          polar( 1/sqrt(40), 6*M_PI*m2/32 ),
          polar( 1/sqrt(40), 0*M_PI*m2/32 ),
          polar( 1/sqrt(40), 2*M_PI*m2/32 ),
          polar( 1/sqrt(40), 4*M_PI*m2/32 ),
          polar( 1/sqrt(40), 6*M_PI*m2/32 )
        };
        return W;
      }
      break;

      case 6:
      {
        if (i1<0 || i1>3 || i2!=0)
          {
            cout << "Invalid parameters for 8-tx codebook: l=" << l << ", i1=" << i1 << ", i2=" << i2 << endl;
            exit(1);
          }
        int m  = 2 * i1;
        int m1 = 2 * i1 + 8;
        int m2 = 2 * i1 + 16;

        arma::cx_mat::fixed<8,6> W =
        {
          // first column
          polar( 1/sqrt(48), 0*M_PI*m/32 ),
          polar( 1/sqrt(48), 2*M_PI*m/32 ),
          polar( 1/sqrt(48), 4*M_PI*m/32 ),
          polar( 1/sqrt(48), 6*M_PI*m/32 ),
          polar( 1/sqrt(48), 0*M_PI*m/32 ),
          polar( 1/sqrt(48), 2*M_PI*m/32 ),
          polar( 1/sqrt(48), 4*M_PI*m/32 ),
          polar( 1/sqrt(48), 6*M_PI*m/32 ),
          // second column
          polar( 1/sqrt(48), 0*M_PI*m/32 ),
          polar( 1/sqrt(48), 2*M_PI*m/32 ),
          polar( 1/sqrt(48), 4*M_PI*m/32 ),
          polar( 1/sqrt(48), 6*M_PI*m/32 ),
          polar( 1/sqrt(48), 0*M_PI*m/32 ) * (-1.0),
          polar( 1/sqrt(48), 2*M_PI*m/32 ) * (-1.0),
          polar( 1/sqrt(48), 4*M_PI*m/32 ) * (-1.0),
          polar( 1/sqrt(48), 6*M_PI*m/32 ) * (-1.0),
          // third column
          polar( 1/sqrt(48), 0*M_PI*m1/32 ),
          polar( 1/sqrt(48), 2*M_PI*m1/32 ),
          polar( 1/sqrt(48), 4*M_PI*m1/32 ),
          polar( 1/sqrt(48), 6*M_PI*m1/32 ),
          polar( 1/sqrt(48), 0*M_PI*m1/32 ),
          polar( 1/sqrt(48), 2*M_PI*m1/32 ),
          polar( 1/sqrt(48), 4*M_PI*m1/32 ),
          polar( 1/sqrt(48), 6*M_PI*m1/32 ),
          // fourth column
          polar( 1/sqrt(48), 0*M_PI*m1/32 ),
          polar( 1/sqrt(48), 2*M_PI*m1/32 ),
          polar( 1/sqrt(48), 4*M_PI*m1/32 ),
          polar( 1/sqrt(48), 6*M_PI*m1/32 ),
          polar( 1/sqrt(48), 0*M_PI*m1/32 ) * (-1.0),
          polar( 1/sqrt(48), 2*M_PI*m1/32 ) * (-1.0),
          polar( 1/sqrt(48), 4*M_PI*m1/32 ) * (-1.0),
          polar( 1/sqrt(48), 6*M_PI*m1/32 ) * (-1.0),
          // fifth column
          polar( 1/sqrt(48), 0*M_PI*m2/32 ),
          polar( 1/sqrt(48), 2*M_PI*m2/32 ),
          polar( 1/sqrt(48), 4*M_PI*m2/32 ),
          polar( 1/sqrt(48), 6*M_PI*m2/32 ),
          polar( 1/sqrt(48), 0*M_PI*m2/32 ),
          polar( 1/sqrt(48), 2*M_PI*m2/32 ),
          polar( 1/sqrt(48), 4*M_PI*m2/32 ),
          polar( 1/sqrt(48), 6*M_PI*m2/32 ),
          // sixth column
          polar( 1/sqrt(48), 0*M_PI*m2/32 ),
          polar( 1/sqrt(48), 2*M_PI*m2/32 ),
          polar( 1/sqrt(48), 4*M_PI*m2/32 ),
          polar( 1/sqrt(48), 6*M_PI*m2/32 ),
          polar( 1/sqrt(48), 0*M_PI*m2/32 ) * (-1.0),
          polar( 1/sqrt(48), 2*M_PI*m2/32 ) * (-1.0),
          polar( 1/sqrt(48), 4*M_PI*m2/32 ) * (-1.0),
          polar( 1/sqrt(48), 6*M_PI*m2/32 ) * (-1.0)
        };
        return W;
      }
      break;

      case 7:
      {
        if (i1<0 || i1>3 || i2!=0)
          {
            cout << "Invalid parameters for 8-tx codebook: l=" << l << ", i1=" << i1 << ", i2=" << i2 << endl;
            exit(1);
          }
        int m  = 2 * i1;
        int m1 = 2 * i1 + 8;
        int m2 = 2 * i1 + 16;
        int m3 = 2 * i1 + 24;

        arma::cx_mat::fixed<8,7> W =
        {
          // first column
          polar( 1/sqrt(56), 0*M_PI*m/32 ),
          polar( 1/sqrt(56), 2*M_PI*m/32 ),
          polar( 1/sqrt(56), 4*M_PI*m/32 ),
          polar( 1/sqrt(56), 6*M_PI*m/32 ),
          polar( 1/sqrt(56), 0*M_PI*m/32 ),
          polar( 1/sqrt(56), 2*M_PI*m/32 ),
          polar( 1/sqrt(56), 4*M_PI*m/32 ),
          polar( 1/sqrt(56), 6*M_PI*m/32 ),
          // second column
          polar( 1/sqrt(56), 0*M_PI*m/32 ),
          polar( 1/sqrt(56), 2*M_PI*m/32 ),
          polar( 1/sqrt(56), 4*M_PI*m/32 ),
          polar( 1/sqrt(56), 6*M_PI*m/32 ),
          polar( 1/sqrt(56), 0*M_PI*m/32 ) * (-1.0),
          polar( 1/sqrt(56), 2*M_PI*m/32 ) * (-1.0),
          polar( 1/sqrt(56), 4*M_PI*m/32 ) * (-1.0),
          polar( 1/sqrt(56), 6*M_PI*m/32 ) * (-1.0),
          // third column
          polar( 1/sqrt(56), 0*M_PI*m1/32 ),
          polar( 1/sqrt(56), 2*M_PI*m1/32 ),
          polar( 1/sqrt(56), 4*M_PI*m1/32 ),
          polar( 1/sqrt(56), 6*M_PI*m1/32 ),
          polar( 1/sqrt(56), 0*M_PI*m1/32 ),
          polar( 1/sqrt(56), 2*M_PI*m1/32 ),
          polar( 1/sqrt(56), 4*M_PI*m1/32 ),
          polar( 1/sqrt(56), 6*M_PI*m1/32 ),
          // fourth column
          polar( 1/sqrt(56), 0*M_PI*m1/32 ),
          polar( 1/sqrt(56), 2*M_PI*m1/32 ),
          polar( 1/sqrt(56), 4*M_PI*m1/32 ),
          polar( 1/sqrt(56), 6*M_PI*m1/32 ),
          polar( 1/sqrt(56), 0*M_PI*m1/32 ) * (-1.0),
          polar( 1/sqrt(56), 2*M_PI*m1/32 ) * (-1.0),
          polar( 1/sqrt(56), 4*M_PI*m1/32 ) * (-1.0),
          polar( 1/sqrt(56), 6*M_PI*m1/32 ) * (-1.0),
          // fifth column
          polar( 1/sqrt(56), 0*M_PI*m2/32 ),
          polar( 1/sqrt(56), 2*M_PI*m2/32 ),
          polar( 1/sqrt(56), 4*M_PI*m2/32 ),
          polar( 1/sqrt(56), 6*M_PI*m2/32 ),
          polar( 1/sqrt(56), 0*M_PI*m2/32 ),
          polar( 1/sqrt(56), 2*M_PI*m2/32 ),
          polar( 1/sqrt(56), 4*M_PI*m2/32 ),
          polar( 1/sqrt(56), 6*M_PI*m2/32 ),
          // sixth column
          polar( 1/sqrt(56), 0*M_PI*m2/32 ),
          polar( 1/sqrt(56), 2*M_PI*m2/32 ),
          polar( 1/sqrt(56), 4*M_PI*m2/32 ),
          polar( 1/sqrt(56), 6*M_PI*m2/32 ),
          polar( 1/sqrt(56), 0*M_PI*m2/32 ) * (-1.0),
          polar( 1/sqrt(56), 2*M_PI*m2/32 ) * (-1.0),
          polar( 1/sqrt(56), 4*M_PI*m2/32 ) * (-1.0),
          polar( 1/sqrt(56), 6*M_PI*m2/32 ) * (-1.0),
          // seventh column
          polar( 1/sqrt(56), 0*M_PI*m3/32 ),
          polar( 1/sqrt(56), 2*M_PI*m3/32 ),
          polar( 1/sqrt(56), 4*M_PI*m3/32 ),
          polar( 1/sqrt(56), 6*M_PI*m3/32 ),
          polar( 1/sqrt(56), 0*M_PI*m3/32 ),
          polar( 1/sqrt(56), 2*M_PI*m3/32 ),
          polar( 1/sqrt(56), 4*M_PI*m3/32 ),
          polar( 1/sqrt(56), 6*M_PI*m3/32 )
        };
        return W;
      }
      break;

      case 8:
      {
        if (i1!=0 || i2!=0)
          {
            cout << "Invalid parameters for 8-tx codebook: l=" << l << ", i1=" << i1 << ", i2=" << i2 << endl;
            exit(1);
          }
        int m  = 2 * i1;
        int m1 = 2 * i1 + 8;
        int m2 = 2 * i1 + 16;
        int m3 = 2 * i1 + 24;

        arma::cx_mat::fixed<8,8> W =
        {
          // first column
          polar( 1/sqrt(64), 0*M_PI*m/32 ),
          polar( 1/sqrt(64), 2*M_PI*m/32 ),
          polar( 1/sqrt(64), 4*M_PI*m/32 ),
          polar( 1/sqrt(64), 6*M_PI*m/32 ),
          polar( 1/sqrt(64), 0*M_PI*m/32 ),
          polar( 1/sqrt(64), 2*M_PI*m/32 ),
          polar( 1/sqrt(64), 4*M_PI*m/32 ),
          polar( 1/sqrt(64), 6*M_PI*m/32 ),
          // second column
          polar( 1/sqrt(64), 0*M_PI*m/32 ),
          polar( 1/sqrt(64), 2*M_PI*m/32 ),
          polar( 1/sqrt(64), 4*M_PI*m/32 ),
          polar( 1/sqrt(64), 6*M_PI*m/32 ),
          polar( 1/sqrt(64), 0*M_PI*m/32 ) * (-1.0),
          polar( 1/sqrt(64), 2*M_PI*m/32 ) * (-1.0),
          polar( 1/sqrt(64), 4*M_PI*m/32 ) * (-1.0),
          polar( 1/sqrt(64), 6*M_PI*m/32 ) * (-1.0),
          // third column
          polar( 1/sqrt(64), 0*M_PI*m1/32 ),
          polar( 1/sqrt(64), 2*M_PI*m1/32 ),
          polar( 1/sqrt(64), 4*M_PI*m1/32 ),
          polar( 1/sqrt(64), 6*M_PI*m1/32 ),
          polar( 1/sqrt(64), 0*M_PI*m1/32 ),
          polar( 1/sqrt(64), 2*M_PI*m1/32 ),
          polar( 1/sqrt(64), 4*M_PI*m1/32 ),
          polar( 1/sqrt(64), 6*M_PI*m1/32 ),
          // fourth column
          polar( 1/sqrt(64), 0*M_PI*m1/32 ),
          polar( 1/sqrt(64), 2*M_PI*m1/32 ),
          polar( 1/sqrt(64), 4*M_PI*m1/32 ),
          polar( 1/sqrt(64), 6*M_PI*m1/32 ),
          polar( 1/sqrt(64), 0*M_PI*m1/32 ) * (-1.0),
          polar( 1/sqrt(64), 2*M_PI*m1/32 ) * (-1.0),
          polar( 1/sqrt(64), 4*M_PI*m1/32 ) * (-1.0),
          polar( 1/sqrt(64), 6*M_PI*m1/32 ) * (-1.0),
          // fifth column
          polar( 1/sqrt(64), 0*M_PI*m2/32 ),
          polar( 1/sqrt(64), 2*M_PI*m2/32 ),
          polar( 1/sqrt(64), 4*M_PI*m2/32 ),
          polar( 1/sqrt(64), 6*M_PI*m2/32 ),
          polar( 1/sqrt(64), 0*M_PI*m2/32 ),
          polar( 1/sqrt(64), 2*M_PI*m2/32 ),
          polar( 1/sqrt(64), 4*M_PI*m2/32 ),
          polar( 1/sqrt(64), 6*M_PI*m2/32 ),
          // sixth column
          polar( 1/sqrt(64), 0*M_PI*m2/32 ),
          polar( 1/sqrt(64), 2*M_PI*m2/32 ),
          polar( 1/sqrt(64), 4*M_PI*m2/32 ),
          polar( 1/sqrt(64), 6*M_PI*m2/32 ),
          polar( 1/sqrt(64), 0*M_PI*m2/32 ) * (-1.0),
          polar( 1/sqrt(64), 2*M_PI*m2/32 ) * (-1.0),
          polar( 1/sqrt(64), 4*M_PI*m2/32 ) * (-1.0),
          polar( 1/sqrt(64), 6*M_PI*m2/32 ) * (-1.0),
          // seventh column
          polar( 1/sqrt(64), 0*M_PI*m3/32 ),
          polar( 1/sqrt(64), 2*M_PI*m3/32 ),
          polar( 1/sqrt(64), 4*M_PI*m3/32 ),
          polar( 1/sqrt(64), 6*M_PI*m3/32 ),
          polar( 1/sqrt(64), 0*M_PI*m3/32 ),
          polar( 1/sqrt(64), 2*M_PI*m3/32 ),
          polar( 1/sqrt(64), 4*M_PI*m3/32 ),
          polar( 1/sqrt(64), 6*M_PI*m3/32 ),
          // eighth column
          polar( 1/sqrt(64), 0*M_PI*m3/32 ),
          polar( 1/sqrt(64), 2*M_PI*m3/32 ),
          polar( 1/sqrt(64), 4*M_PI*m3/32 ),
          polar( 1/sqrt(64), 6*M_PI*m3/32 ),
          polar( 1/sqrt(64), 0*M_PI*m3/32 ) * (-1.0),
          polar( 1/sqrt(64), 2*M_PI*m3/32 ) * (-1.0),
          polar( 1/sqrt(64), 4*M_PI*m3/32 ) * (-1.0),
          polar( 1/sqrt(64), 6*M_PI*m3/32 ) * (-1.0)
        };
        return W;
      }
      break;

      default:
      {
        // This should never happen if the input is valid.
        // This return statement avoids a compiler warning.
        arma::cx_mat::fixed<1,1> unit = {1};
        return unit;
        cout << "Error: incorrect parameters for 8Tx precoding matrix" << endl;
      }
      break;

    }

  // This should never happen if the input is valid.
  // This return statement avoids a compiler warning.
  cout << "Error: incorrect parameters for 8Tx precoding matrix" << endl;
  arma::cx_mat::fixed<1,1> unit = {1};
  return unit;
}



#endif // MIMO_CODEBOOK_H_
