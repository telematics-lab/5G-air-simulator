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

#include "wideband-cqi-manager.h"
#include "../NetworkNode.h"
#include "../../protocolStack/mac/AMCModule.h"
#include "../../utility/miesm-effective-sinr.h"
#include "../../load-parameters.h"


vector<int>
WidebandCqiManager::CalculateCqiFeedbacks (vector<double> sinr)
{

  double effective_sinr = GetMiesmEffectiveSinr (sinr);
  for (int i = 0; i < (int)sinr.size (); i++)
    {
      sinr.at (i) = effective_sinr;
    }

DEBUG_LOG_START_1(SIM_ENV_AMC_MAPPING)
  cout << "\t sinr: ";
  for (int i = 0; i < (int)sinr.size (); i++)
    {
      cout << sinr.at (i) << " ";
    }
  cout << endl;
DEBUG_LOG_END

  AMCModule *amc = GetDevice ()->GetProtocolStack ()->GetMacEntity ()->GetAmcModule ();
  vector<int> cqi = amc->CreateCqiFeedbacks (sinr);
  return cqi;
}
