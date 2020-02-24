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
 * Author2: Marco Saltarella <saltarella.marco@gmail.com>
 */


#ifndef FREQUENCY_REUSE_HELPER_H_
#define FREQUENCY_REUSE_HELPER_H_

#include <stdint.h>
#include "stdlib.h"
#include <math.h>
#include "../core/spectrum/bandwidth-manager.h"
#include "../componentManagers/FrameManager.h"

/*

Number of supported non-overlapping channels in each frequency band and bandwidth.

operative band		bandwidth 			Channel bandwidth (MHz)
										1.4 3 	5 	10 	15 	20

1 						60 				— 	— 	12 	6 	4 	3
2 						60 				42 	20 	12 	6	[4] [3]
3 						75 				53 	23 	15 	7 	[5] [3]
...

XXX: now is supported only the 1-th operative sub-band
*/

static vector <BandwidthManager*>
RunFrequencyReuseTechniques(int nodes, int cluster, double bandwidth)
{
  vector <BandwidthManager*> spectrum;

  int operativeSubBands;
  int cluster1[37] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  int cluster3[37] = {0,1,2,1,2,1,2,2,0,1,0,2,0,1,0,2,0,1,0,0,1,2,0,2,1,0,1,2,0,2,1,0,1,2,0,2,1};
  int cluster4[37] = {0,1,2,3,1,2,3,0,3,0,1,0,2,0,3,0,1,0,2,1,2,1,2,3,2,3,1,3,1,2,1,2,3,2,3,1,3};
  int cluster7[37] = {0,1,2,3,4,5,6,5,4,6,5,1,6,2,1,3,2,4,3,6,0,3,1,0,4,2,0,5,3,0,6,4,0,1,5,0,2};
  if (bandwidth == 1.4)
    {
      operativeSubBands = 6;
    }
  else if (bandwidth == 3)
    {
      operativeSubBands = 15;
    }
  else if (bandwidth == 5)
    {
      operativeSubBands = 25;
    }
  else if (bandwidth == 10)
    {
      operativeSubBands = 50;
    }
  else if (bandwidth == 15)
    {
      operativeSubBands = 75;
    }
  else if (bandwidth == 20)
    {
      operativeSubBands = 100;
    }
  else
    {
      cout << "ERROR: unsupported/invalid bandwidth: " << bandwidth << endl;
    }

  int* cluster_p;
  if (cluster == 1)
    {
      cluster_p = cluster1;
    }
  else if (cluster == 3)
    {
      cluster_p = cluster3;
    }
  else if (cluster == 4)
    {
      cluster_p = cluster4;
    }
  else if (cluster == 7)
    {
      cluster_p = cluster7;
    }
  else
    {
      cout << "ERROR: unsupported/invalid frequency reuse factor: " << cluster << endl;
    }

  for (int i = 0; i < nodes; i++)
    {
      int offset = cluster_p[i] * operativeSubBands;

      BandwidthManager *s = new BandwidthManager (bandwidth, bandwidth, offset, offset);
      spectrum.push_back (s);
    }

  return spectrum;
}



#endif /* FREQUENCY_REUSE_HELPER_H_ */
