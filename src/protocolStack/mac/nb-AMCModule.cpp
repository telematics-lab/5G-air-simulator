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
 * Author: Sergio Martiradonna <sergio.martiradonna@poliba.it>
 */

#include "nb-AMCModule.h"
#include <iostream>


int IRUtoRU[8] = {1, 2, 3, 4, 5, 6, 8, 10};

// 3GPP TS 36.213: Transport block size table (dimension 14×8)
int nbTransportBlockSizeTable [8][14] = {
  /* NRU 1*/ {  16,   24,   32,     40,     56,     72,     88,     104,    120,    136,    144,    176,    208,     224},
  /* NRU 2*/ {  32,   56,   72,    104,    120,    144,    176,     224,    256,    296,    328,    376,    440,     488},
  /* NRU 3*/ {  56,   88,  144,    176,    208,    224,    256,     328,    392,    456,    504,    584,    680,     744},
  /* NRU 4*/ {  88,  144,  176,    208,    256,    328,    392,     472,    536,    616,    680,    776,   1000,    1032},
  /* NRU 5*/ { 120,  176,  208,    256,    328,    424,    504,     584,    680,    776,    872,   1000,   1128,    1256},
  /* NRU 6*/ { 152,  208,  256,    328,    408,    504,    600,     712,    808,    936,   1000,   1192,   1352,    1544},
  /* NRU 8*/ { 208,  256,  328,    440,    552,    680,    808,    1000,   1096,   1256,   1384,   1608,   1800,    2024},
  /* NRU 10*/{ 256,  344,  424,    568,    680,    872,   1000,    1224,   1384,   1544,   1736,   2024,   2280,    2536},
  };

nbAMCModule::nbAMCModule()
{
}

nbAMCModule::~nbAMCModule()
{
}


int
nbAMCModule::GetTBSizeFromMCS (int mcs, int nbRUs)
{
//  std::cout << "nbAMCModule::GetTBSizeFromMCS (" << mcs << ", "<< nbRUs << ")" <<  std::endl;
  return (nbTransportBlockSizeTable[GetRUindex (nbRUs)][mcs]);
}

int
nbAMCModule::GetNbOfRUsFromSize (int mcs, int bits)
{
  int iRU;
  if (bits > nbTransportBlockSizeTable[7][mcs])
    {
      iRU = 7;
    }
  else
    {
      for (int i = 0; i < 8; i++)
        {
          if (bits <= nbTransportBlockSizeTable[i][mcs])
            {
              iRU = i;
              break;
            }
        }
    }
  return IRUtoRU[iRU];
}


int
nbAMCModule::GetNbOfMCSfromTones (int tones)
{
  if (tones==1)
    {
      return 11;
    }
  else
    {
      return 14;
    }
}

int
nbAMCModule::GetMCSfromDistance (double distance, double radius, int tones)
{
  int zone;

  int nbOfZones = GetNbOfMCSfromTones (tones);
  double zoneWidth = (double) radius / nbOfZones;
  double zones[nbOfZones+1];
  for (int i=0; i <= nbOfZones; i++)
    {
      zones[i]= i*zoneWidth;
    }
  if (distance >= zones[nbOfZones])
    {
      zone= 0;
    }
  for (int i= 0; i < nbOfZones; i++)
    {
      if (distance >= zones[i] && distance <= zones[i+1])
        {
          zone = nbOfZones-1-i;
        }
    }
  return zone;
}

int
nbAMCModule::GetRUindex (int nru)
{
  switch (nru)
    {
      case 1:
        return 0;
      case 2:
        return 1;
      case 3:
        return 2;
      case 4:
        return 3;
      case 5:
        return 4;
      case 6:
        return 5;
      case 8:
        return 6;
      case 10:
        return 7;
      default:
            std::cout << "ERROR: Invalid RU Index" << std::endl;
            exit (0);
        break;
    }
}

