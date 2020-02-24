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

#include "wideband-cqi-eesm-error-model.h"
#include "BLERTrace/BLERvsSINR_15CQI_AWGN.h"
#include "BLERTrace/BLERvsSINR_15CQI_TU.h"
#include "../utility/RandomVariable.h"
#include "../utility/eesm-effective-sinr.h"
#include "../utility/miesm-effective-sinr.h"
#include "../load-parameters.h"

bool
WidebandCqiEesmErrorModel::CheckForPhysicalError (vector<int> channels, vector<int> mcs, vector<double> sinr)
{

  bool error = false;

  //compute the sinr vector associated to assigned sub channels
  vector<double> new_sinr;
  for (int i = 0; i < (int)channels.size (); i++)
    {
      new_sinr.push_back (sinr.at (channels.at (i)));
    }

DEBUG_LOG_START_1(SIM_ENV_BLER_DEBUG)
  cout << "\n--> CheckForPhysicalError \n\t\t Channels: ";
  for (int i = 0; i < (int)channels.size (); i++)
    {
      cout << channels.at (i) << " ";
    }
  cout << "\n\t\t MCS: ";
  for (int i = 0; i < (int)mcs.size (); i++)
    {
      cout << mcs.at (i) << " ";
    }
  cout << "\n\t\t SINR: ";
  for (int i = 0; i < (int)new_sinr.size (); i++)
    {
      cout << new_sinr.at (i) << " ";
    }
  cout << "\n"<< endl;
DEBUG_LOG_END


  double effective_sinr = GetMiesmEffectiveSinr (new_sinr);
  double randomNumber = (rand () %100 ) / 100.;
  int mcs_ = mcs.at (0);
  double bler;

  if (_channel_AWGN_)
    {
      bler = GetBLER_AWGN (effective_sinr, mcs_);
    }
  else if (_channel_TU_)
    {
      bler = GetBLER_TU (effective_sinr, mcs_);
    }
  else
    {
      bler = GetBLER_AWGN (effective_sinr, mcs_);
    }

DEBUG_LOG_START_1(SIM_ENV_BLER_DEBUG)
  cout <<"CheckForPhysicalError: , effective SINR:" << effective_sinr
            << ", selected CQI: " << mcs_
            << ", random " << randomNumber
            << ", BLER: " << bler << endl;
DEBUG_LOG_END

  if (randomNumber < bler)
    {
      error = true;
      if (_TEST_BLER_) cout << "BLER PDF " << effective_sinr << " 1" << endl;
    }
  else
    {
      if (_TEST_BLER_) cout << "BLER PDF " << effective_sinr << " 0" << endl;
    }

  return error;
}
