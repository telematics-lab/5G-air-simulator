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


#include "phy.h"
#include "../device/NetworkNode.h"
#include "../channel/RadioChannel.h"
#include "../core/spectrum/bandwidth-manager.h"
#include "../protocolStack/packet/packet-burst.h"
#include "../core/spectrum/transmitted-signal.h"
#include "interference.h"
#include "error-model.h"

Phy::Phy()
{
  m_device = nullptr;
  m_dlChannel = nullptr;
  m_dlMcChannel = nullptr;
  m_ulChannel = nullptr;
  m_bandwidthManager = nullptr;
  m_carrierFrequency = 2000;
  m_txSignal = nullptr;
  m_antennaParameters = new AntennaParameters ();
  SetNoiseFigure(2.5);
  m_averageBuildingHeight = 10;

  m_antennaParameters->SetFeederLoss(0);

  // The following parameters are only meaningful with 3-sector antennas
  m_antennaParameters->SetGain (0);
  m_antennaParameters->SetHorizontalBeamwidth3db(70);
  m_antennaParameters->SetVerticalBeamwidth3db(10);
  m_antennaParameters->SetMaxHorizontalAttenuation(20);
  m_antennaParameters->SetMaxVerticalAttenuation(20);

  m_waveform = WAVEFORM_TYPE_OFDM;
  m_useSrtaPi = false;

}

Phy::~Phy()
{
  delete m_txSignal;
}

void
Phy::Destroy (void)
{
  delete m_txSignal;
  delete m_interference;
  delete m_errorModel;
}


void
Phy::SetDevice (NetworkNode* d)
{
  m_device = d;
}

NetworkNode*
Phy::GetDevice (void)
{
  return m_device;
}

void
Phy::SetDlChannel (RadioChannel* c)
{
  m_dlChannel = c;
}

void
Phy::SetDlMcChannel (RadioChannel* c)
{
  m_dlMcChannel = c;
}

RadioChannel*
Phy::GetDlChannel (void)
{
  return m_dlChannel;
}

RadioChannel*
Phy::GetDlMcChannel (void)
{
  return m_dlMcChannel;
}

void
Phy::SetUlChannel (RadioChannel* c)
{
  m_ulChannel = c;
}

RadioChannel*
Phy::GetUlChannel (void)
{
  return m_ulChannel;
}

void
Phy::SetBandwidthManager (BandwidthManager* s)
{
  m_bandwidthManager = s;
  if (s != nullptr)
    DoSetBandwidthManager ();
}

BandwidthManager*
Phy::GetBandwidthManager (void)
{
  return m_bandwidthManager;
}

void
Phy::SetCarrierFrequency (double frequency)
{
  m_carrierFrequency = frequency;
}

double
Phy::GetCarrierFrequency (void)
{
  return m_carrierFrequency;
}

void
Phy::SetTxPower (double p)
{
  m_txPower = p;
}

double
Phy::GetTxPower (void)
{
  return m_txPower;
}

void
Phy::SetTxSignal (TransmittedSignal* txSignal)
{
  m_txSignal = txSignal;
}

TransmittedSignal*
Phy::GetTxSignal (void)
{
  return m_txSignal;
}


void
Phy::SetInterference (Interference* i)
{
  m_interference = i;
}

void
Phy::SetErrorModel (ErrorModel* e)
{
  m_errorModel = e;
}

void
Phy::SetTxAntennas (int n)
{
  m_txAntennas = n;
}

int
Phy::GetTxAntennas (void)
{
  return m_txAntennas;
}

void
Phy::SetRxAntennas (int n)
{
  m_rxAntennas = n;
}

int
Phy::GetRxAntennas (void)
{
  return m_rxAntennas;
}

void
Phy::SetHeight (double height)
{
  GetDevice ()->GetMobilityModel ()->GetAbsolutePosition ()->SetCoordinateZ (height);
}

double
Phy::GetHeight (void)
{
  return GetDevice ()->GetMobilityModel ()->GetAbsolutePosition ()->GetCoordinateZ ();
}

Interference*
Phy::GetInterference (void)
{
  return m_interference;
}


ErrorModel*
Phy::GetErrorModel (void)
{
  return m_errorModel;
}

Phy::AntennaParameters* Phy::GetAntennaParameters (void)
{
  return m_antennaParameters;
}

void
Phy::AntennaParameters::SetEtilt(int etilt)
{
  m_etilt = etilt;
}

double
Phy::AntennaParameters::GetEtilt(void)
{
  return m_etilt;
}

void
Phy::AntennaParameters::SetType(AntennaType t)
{
  m_type = t;
}

Phy::AntennaParameters::AntennaType
Phy::AntennaParameters::GetType(void)
{
  return m_type;
}

void
Phy::AntennaParameters::SetBearing(double bearing)
{
  m_bearing = bearing;
}

double
Phy::AntennaParameters::GetBearing(void)
{
  return m_bearing;
}

void
Phy::AntennaParameters::SetGain(double gain)
{
  m_gain = gain;
}

double
Phy::AntennaParameters::GetGain(void)
{
  return m_gain;
}

void
Phy::AntennaParameters::SetHorizontalBeamwidth3db(double beamwidth)
{
  m_horizontalBeamwidth3dB = beamwidth;
}

double
Phy::AntennaParameters::GetHorizontalBeamwidth3db(void)
{
  return m_horizontalBeamwidth3dB;
}

void
Phy::AntennaParameters::SetVerticalBeamwidth3db(double beamwidth)
{
  m_verticalBeamwidth3dB = beamwidth;
}

double
Phy::AntennaParameters::GetVerticalBeamwidth3db(void)
{
  return m_verticalBeamwidth3dB;
}

void
Phy::AntennaParameters::SetMaxHorizontalAttenuation(double attenuation)
{
  m_maxHorizontalAttenuation = attenuation;
}

double
Phy::AntennaParameters::GetMaxHorizontalAttenuation(void)
{
  return m_maxHorizontalAttenuation;
}

void
Phy::AntennaParameters::SetMaxVerticalAttenuation(double attenuation)
{
  m_maxVerticalAttenuation = attenuation;
}

double
Phy::AntennaParameters::GetMaxVerticalAttenuation(void)
{
  return m_maxVerticalAttenuation;
}


void
Phy::AntennaParameters::SetFeederLoss(double loss)
{
  m_feederLoss = loss;
}

double
Phy::AntennaParameters::GetFeederLoss(void)
{
  return m_feederLoss;
}

void
Phy::SetNoiseFigure(double nf)
{
  m_noiseFigure = nf;
  /*
   * Noise is computed as follows:
   *  - n0 = -174 dBm
   *  - sub channel bandwidth = 180 kHz
   *
   *  noise_db = noise figure + n0 + 10log10 (180000) - 30
   */
  m_thermalNoise = m_noiseFigure - 174 + 10*log10(180000) - 30;
}

double
Phy::GetNoiseFigure(void)
{
  return m_noiseFigure;
}

double
Phy::GetThermalNoise(void)
{
  return m_thermalNoise;
}

void
Phy::SetAverageBuildingHeight(double height)
{
  m_averageBuildingHeight = height;
}

double
Phy::GetAverageBuildingHeight(void)
{
  return m_averageBuildingHeight;
}


Phy::WaveformType
Phy::GetWaveformType (void)
{
  return m_waveform;
}

void
Phy::SetWaveformType (Phy::WaveformType w)
{
  m_waveform = w;
}

bool
Phy::GetSrtaPi (void)
{
  return m_useSrtaPi;
}

void
Phy::SetSrtaPi (bool b)
{
  m_useSrtaPi = b;
}

