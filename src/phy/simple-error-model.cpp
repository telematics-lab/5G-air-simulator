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
 * Author: Francesco Capozzi <f.capozzi@poliba.it>
 */

#include "simple-error-model.h"
#include "BLERTrace/BLERvsSINR_15CQI_AWGN.h"
#include "BLERTrace/BLERvsSINR_15CQI_TU.h"
#include "../utility/RandomVariable.h"

bool
SimpleErrorModel::CheckForPhysicalError (vector<int> channels, vector<int> mcs, vector<double> sinr)
{
  /*
   * The device determines if a packet has been received correctly.
   * To this aim, for each sub-channel, used to transmit that packet,
   * the Block Error Rate (BLER) is estimated.
   *
   * The BLER is obtained considering both MCS used for the transmission
   * and SINR that the device has estimated for the considered sub-channel.
   * In particular, the BLER value is drawn using stored BLER-SINR curves
   * stored into trace files, generated using an ad hoc OFDMA tool written in matlab.
   * According to the proper BLER-SINR curve (depending on the used MCS),
   * the device establishes if the packet has been correctly received or not.
   * In the latter case, the packet is considered erroneous and ignored.
   */

  bool error = false;

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
  for (int i = 0; i < (int)sinr.size (); i++)
    {
      cout << sinr.at (i) << " ";
    }
  cout << "\n"<< endl;
DEBUG_LOG_END


  double randomNumber = (rand () %100 ) / 100.;

  for (int i = 0; i < (int)channels.size (); i++)
    {
      int mcs_ = mcs.at (i);
      double sinr_ = sinr.at (channels.at (i));


      double bler;
      if (_channel_TU_)
        {
          bler = GetBLER_TU (sinr_, mcs_);
        }
      else
        {
          bler = GetBLER_AWGN (sinr_, mcs_);
        }

DEBUG_LOG_START_1(SIM_ENV_BLER_DEBUG)
      cout <<"Get BLER for ch " << channels.at(i)<<
                " cqi " << mcs_ << " sinr "  << sinr_
                << " BLER = " << bler << endl;
DEBUG_LOG_END

      if (randomNumber < bler)
        {
DEBUG_LOG_START_1(SIM_ENV_BLER_DEBUG)
          cout << "ERROR RX ---- "
                    << "effective SINR:" << sinr_
                    << ", selected CQI: " << mcs_
                    << ", random " << randomNumber
                    << ", BLER: " << bler << endl;
DEBUG_LOG_END
          error = true;
          if (_TEST_BLER_) cout << "BLER PDF " << sinr_ << " 1" << endl;
        }
      else
        {
          if (_TEST_BLER_) cout << "BLER PDF " << sinr_ << " 0" << endl;
        }
    }

  return error;
}
