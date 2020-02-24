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

#include "bandwidth-manager.h"
#include <stdio.h>
#include <iostream>

#define UL_LOW_FREQUENCY_BAND_1 1920    //MHz
#define UL_HIGH_FREQUENCY_BAND_1 1980   //MHz
#define DL_LOW_FREQUENCY_BAND_1 2110    //MHz
#define DL_HIGH_FREQUENCY_BAND_1 2170   //MHz

// first element is the bandwidth in MHz, second one is the corresponding number of RBs
const static map<double, int> RBs_for_BW =
  {
    { 1.4, 6 },
    { 3, 15 },
    { 5, 25 },
    { 10, 50 },
    { 15, 75 },
    { 20, 100 } };

#define NB_IOT_ALLOWED_RBs_FOR_1_4_MHz 0
#define NB_IOT_ALLOWED_RBs_FOR_3_MHz 2
#define NB_IOT_ALLOWED_RBs_FOR_5_MHz 4
#define NB_IOT_ALLOWED_RBs_FOR_10_MHz 8
#define NB_IOT_ALLOWED_RBs_FOR_15_MHz 13
#define NB_IOT_ALLOWED_RBs_FOR_20_MHz 18

BandwidthManager::BandwidthManager (double ulBw, double dlBw, int ulOffset,
                                    int dlOffset, bool tddTrue)
{
  if (tddTrue == true)
    {
      m_ulBandwidth = ulBw + dlBw;
      m_dlBandwidth = dlBw + ulBw;
    }
  else
    {
      m_ulBandwidth = ulBw;
      m_dlBandwidth = dlBw;
    }
  m_ulOffsetBw = ulOffset;
  m_dlOffsetBw = dlOffset;

  NbIoTenabled = false;

  m_operativeSubBand = 1;

  m_dlSubChannels.clear ();
  m_ulSubChannels.clear ();

  for (int i = dlOffset; i < dlOffset + RBs_for_BW.at (dlBw); i++)
    {
      m_dlSubChannels.push_back (DL_LOW_FREQUENCY_BAND_1 + (i * 0.18));
      if (tddTrue == true)
        {
          m_ulSubChannels.push_back (DL_LOW_FREQUENCY_BAND_1 + (i * 0.18));
        }
    }

  for (int i = ulOffset; i < ulOffset + RBs_for_BW.at (ulBw); i++)
    {
      m_ulSubChannels.push_back (UL_LOW_FREQUENCY_BAND_1 + (i * 0.18));
      if (tddTrue == true)
        {
          m_dlSubChannels.push_back (UL_LOW_FREQUENCY_BAND_1 + (i * 0.18));
        }
    }
}

void
BandwidthManager::SetDlSubChannels (vector<double> s)
{
  if (NbIoTenabled)
    {
      m_NbIoTchannels = s;
    }
  else
    {
      m_dlSubChannels = s;
    }
}

vector<double>
BandwidthManager::GetDlSubChannels (void)
{
  if (NbIoTenabled)
    {
      return m_NbIoTchannels;
    }
  else
    {
      return m_dlSubChannels;
    }
}

void
BandwidthManager::SetUlSubChannels (vector<double> s)
{
  if (NbIoTenabled)
    {
      m_NbIoTchannels = s;
    }
  else
    {
      m_ulSubChannels = s;
    }
}

vector<double>
BandwidthManager::GetUlSubChannels (void)
{
  if (NbIoTenabled)
    {
      return m_NbIoTchannels;
    }
  else
    {
      return m_ulSubChannels;
    }
}

void
BandwidthManager::SetOperativeSubBand (int s)
{
  m_operativeSubBand = s;
}

int
BandwidthManager::GetOperativeSubBand (void)
{
  return m_operativeSubBand;
}

void
BandwidthManager::SetUlBandwidth (double b)
{
  m_ulBandwidth = b;
}

void
BandwidthManager::SetDlBandwidth (double b)
{
  m_dlBandwidth = b;
}

void
BandwidthManager::SetUlOffsetBw (int o)
{
  m_ulOffsetBw = o;
}

void
BandwidthManager::SetDlOffsetBw (int o)
{
  m_dlOffsetBw = o;
}

double
BandwidthManager::GetUlBandwidth (void)
{
  return m_ulBandwidth;
}

double
BandwidthManager::GetDlBandwidth (void)
{
  return m_dlBandwidth;
}

int
BandwidthManager::GetUlOffsetBw (void)
{
  return m_ulOffsetBw;
}

int
BandwidthManager::GetDlOffsetBw (void)
{
  return m_dlOffsetBw;
}

BandwidthManager*
BandwidthManager::Copy ()
{
  BandwidthManager *s = new BandwidthManager ();
  s->SetDlBandwidth (GetDlBandwidth ());
  s->SetUlBandwidth (GetUlBandwidth ());
  s->SetDlOffsetBw (GetDlOffsetBw ());
  s->SetUlOffsetBw (GetUlOffsetBw ());
  s->SetDlSubChannels (GetDlSubChannels ());
  s->SetUlSubChannels (GetUlSubChannels ());
  s->SetOperativeSubBand (GetOperativeSubBand ());

  return s;
}

void
BandwidthManager::Print (void)
{
  cout << "BandwidthManager: " << this << endl;

  if (NbIoTenabled)
    {
      std::cout.setf(std::ios_base::fixed, std::ios_base::floatfield);
      std::cout.precision(5);
      cout << "\t Bandwidth " << min (m_dlBandwidth, m_ulBandwidth)
          << "\n\t NB-IoT Carriers " << m_nbOfNbIoTcarriers << "\n\t Tones  "
          << m_tones << "\n\t Spacing " << m_subcarrierSpacing << endl;

      cout << "\t channels: ";
      for (int i = 0; i < (int) m_NbIoTchannels.size (); i++)
        {
          cout << m_NbIoTchannels.at (i) << " ";
        }
      cout << endl;
      std::cout.precision(3);

    }
  else
    {
      cout << "\t operative sub band: " << m_operativeSubBand
          << "\n\t m_dlBandwidth " << m_dlBandwidth << "\n\t m_ulBandwidth "
          << m_ulBandwidth << "\n\t m_dlOffsetBw " << m_dlOffsetBw
          << "\n\t m_ulOffsetBw " << m_ulOffsetBw << endl;

      cout << "\t DL channels: ";
      for (int i = 0; i < (int) m_dlSubChannels.size (); i++)
        {
          cout << m_dlSubChannels.at (i) << " ";
        }
      cout << endl;

      cout << "\t UL channels: ";
      for (int i = 0; i < (int) m_ulSubChannels.size (); i++)
        {
          cout << m_ulSubChannels.at (i) << " ";
        }
      cout << endl;
    }
}

int
BandwidthManager::GetMaxNbIoTcarriers (void)
{
  int a;
  int minBw;
  if (m_dlBandwidth < m_ulBandwidth)
    {
      minBw = (int) m_dlBandwidth;
    }
  else
    {
      minBw = (int) m_ulBandwidth;
    }
  switch (minBw)
    {
    case 3:
      a = NB_IOT_ALLOWED_RBs_FOR_3_MHz;
      break;
    case 5:
      a = NB_IOT_ALLOWED_RBs_FOR_5_MHz;
      break;
    case 10:
      a = NB_IOT_ALLOWED_RBs_FOR_10_MHz;
      break;
    case 15:
      a = NB_IOT_ALLOWED_RBs_FOR_15_MHz;
      break;
    case 20:
      a = NB_IOT_ALLOWED_RBs_FOR_20_MHz;
      break;
    default:
      cout << "ERROR: Invalid NB-IoT Bandwidth" << endl;
      exit (0);
      break;
    }
  return a;
}

void
BandwidthManager::CreateNbIoTspectrum (int nbOfNbIoTcarriers, double spacing,
                                       int tones)
{
  NbIoTenabled = true;

  if (nbOfNbIoTcarriers > GetMaxNbIoTcarriers ())
    {
      m_nbOfNbIoTcarriers = GetMaxNbIoTcarriers ();
    }
  else
    {
      m_nbOfNbIoTcarriers = nbOfNbIoTcarriers;
    }

  m_subcarrierSpacing = spacing;
  if (spacing == 3.75)
    {
      m_tones = 1;
    }
  else
    {
      m_tones = tones;
    }
  float rubw = m_tones * spacing / 1000;

  vector<int> rbs = GetNbIoTrb ();

  for (int j = 0; j < m_nbOfNbIoTcarriers; j++)
    {
      int rb = rbs.at (j);
      for (int i = 0; i < (0.18 / rubw); i++)
        {
          m_NbIoTchannels.push_back (m_ulSubChannels.at (rb - 1) + (i * rubw));
        }
    }
}

vector<int>
BandwidthManager::GetNbIoTrb (void)
{
  vector<int> rb;
  int carr = GetMaxNbIoTcarriers ();
  switch (carr)
    {
    case 2:
      rb.push_back (1);
      rb.push_back (2);
      break;
    case 4:
      rb.push_back (2);
      rb.push_back (7);
      rb.push_back (17);
      rb.push_back (22);
      break;
    case 8:
      rb.push_back (4);
      rb.push_back (9);
      rb.push_back (14);
      rb.push_back (19);
      rb.push_back (30);
      rb.push_back (35);
      rb.push_back (40);
      rb.push_back (45);
      break;
    case 13:
      rb.push_back (2);
      rb.push_back (7);
      rb.push_back (17);
      rb.push_back (22);
      rb.push_back (27);
      rb.push_back (32);
      rb.push_back (42);
      rb.push_back (47);
      rb.push_back (52);
      rb.push_back (57);
      rb.push_back (62);
      rb.push_back (67);
      rb.push_back (72);
      rb.push_back (57);
      break;
    case 18:
      rb.push_back (4);
      rb.push_back (9);
      rb.push_back (14);
      rb.push_back (19);
      rb.push_back (24);
      rb.push_back (29);
      rb.push_back (34);
      rb.push_back (39);
      rb.push_back (44);
      rb.push_back (55);
      rb.push_back (60);
      rb.push_back (65);
      rb.push_back (70);
      rb.push_back (75);
      rb.push_back (80);
      rb.push_back (85);
      rb.push_back (90);
      rb.push_back (95);
      break;
    }
  return rb;
}

int
BandwidthManager::GetNbOfNbIoTcarriers () const
{
  if (NbIoTenabled)
    {
      return m_nbOfNbIoTcarriers;
    }
  else
    {
      cout << "ERROR: NB-IoT not enabled" << endl;
      exit (0);
    }
}

double
BandwidthManager::GetRUsBandwidth () const
{
  if (NbIoTenabled)
    {
      return m_tones * m_subcarrierSpacing / 1000;
    }
  else
    {
      cout << "ERROR: NB-IoT not enabled" << endl;
      exit (0);
    }
}

double
BandwidthManager::GetSubcarrierSpacing () const
{
  if (NbIoTenabled)
    {
      return m_subcarrierSpacing;
    }
  else
    {
      cout << "ERROR: NB-IoT not enabled" << endl;
      exit (0);
    }
}

int
BandwidthManager::GetTones () const
{
  if (NbIoTenabled)
    {
      return m_tones;
    }
  else
    {
      cout << "ERROR: NB-IoT not enabled" << endl;
      exit (0);
    }
}
