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
 */


#ifndef EESM_EFFECTIVE_SINR_H_
#define EESM_EFFECTIVE_SINR_H_

#include <math.h>
#include <vector>

static double beta_value [20] =
{
  1.49,
  1.53,
  1.57,
  1.61,
  1.69,
  1.69,
  1.65,
  3.36,
  4.56,
  6.42,
  7.33,
  7.68,
  9.21,
  10.81,
  13.76,
  17.52,
  20.57,
  22.75,
  25.16,
  28.38
};

static double
GetEesmEffectiveSinr (vector <double> sinr)
{
  double eff_sinr;
  long double sum_I_sinr = 0;
  for (auto sinr_ : sinr)
    {
      //since sinr[] is expressed in dB we should convert it in natural unit!
      long double s = pow (10, (sinr_/10));
      sum_I_sinr += exp (-s);
    }

  eff_sinr = - log (sum_I_sinr / sinr.size ());

  eff_sinr = 10 * log10 (eff_sinr); //convert in dB

  return eff_sinr;
}

#endif /* EESM_EFFECTIVE_SINR_H_ */
