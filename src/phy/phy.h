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


#ifndef PHY_H_
#define PHY_H_

#include <memory>
#include "../load-parameters.h"

class NetworkNode;
class RadioChannel;
class BandwidthManager;
class PacketBurst;
class TransmittedSignal;
class IdealControlMessage;
class Interference;
class ErrorModel;

class Phy
{
public:
  Phy();
  virtual ~Phy();

  void Destroy (void);

  virtual void StartTx (shared_ptr<PacketBurst> p) = 0;
  virtual void StartRx (shared_ptr<PacketBurst> p, TransmittedSignal* txSignal) = 0;

  void SetDevice (NetworkNode* d);
  NetworkNode* GetDevice (void);

  void SetDlChannel (RadioChannel* c);
  void SetDlMcChannel (RadioChannel* c);
  RadioChannel* GetDlChannel (void);
  RadioChannel* GetDlMcChannel (void);
  void SetUlChannel (RadioChannel* c);
  RadioChannel* GetUlChannel (void);

  void SetBandwidthManager (BandwidthManager* s);
  BandwidthManager* GetBandwidthManager (void);
  virtual void DoSetBandwidthManager (void) = 0;

  void SetCarrierFrequency (double frequency);
  double GetCarrierFrequency (void);

  void SetTxPower (double p);
  double GetTxPower (void);

  void SetTxSignal (TransmittedSignal* txSignal);
  TransmittedSignal* GetTxSignal (void);

  virtual void SendIdealControlMessage (IdealControlMessage *msg) = 0;
  virtual void ReceiveIdealControlMessage (IdealControlMessage *msg) = 0;

  void SetInterference (Interference* interference);
  void SetErrorModel (ErrorModel* e);

  void SetTxAntennas (int n);
  int GetTxAntennas (void);

  void SetRxAntennas (int n);
  int GetRxAntennas (void);

  void SetHeight (double height);
  double GetHeight (void);

  void SetNoiseFigure(double nf);
  double GetNoiseFigure(void);
  double GetThermalNoise(void);

  void SetAverageBuildingHeight(double height);
  double GetAverageBuildingHeight(void);

  Interference* GetInterference (void);
  ErrorModel* GetErrorModel (void);

  struct AntennaParameters
  {
    enum AntennaType
    {
      ANTENNA_TYPE_OMNIDIRECTIONAL,
      ANTENNA_TYPE_TRI_SECTOR
    };

    AntennaType m_type;
    // Orientation angle of the main lobe, in degrees.
    // 0° is East, increases clockwise.
    // Only meaningful with 3-sector antennas.
    double m_bearing;
    double m_etilt; // angle in degrees
    double m_gain; // in dB
    double m_horizontalBeamwidth3dB; // angle in degrees
    double m_verticalBeamwidth3dB; // angle in degrees
    double m_maxHorizontalAttenuation; // in dB
    double m_maxVerticalAttenuation; // in dB
    double m_feederLoss;

    void SetEtilt(int etilt);
    double GetEtilt(void);

    void SetType(AntennaType t);
    AntennaType GetType(void);

    void SetBearing(double bearing);
    double GetBearing(void);

    void SetGain(double gain);
    double GetGain(void);

    void SetHorizontalBeamwidth3db(double beamwidth);
    double GetHorizontalBeamwidth3db(void);

    void SetVerticalBeamwidth3db(double beamwidth);
    double GetVerticalBeamwidth3db(void);

    void SetMaxHorizontalAttenuation(double attenuation);
    double GetMaxHorizontalAttenuation(void);

    void SetMaxVerticalAttenuation(double attenuation);
    double GetMaxVerticalAttenuation(void);

    void SetFeederLoss(double loss);
    double GetFeederLoss(void);
  };

  AntennaParameters* GetAntennaParameters (void);

  enum WaveformType
  {
    WAVEFORM_TYPE_OFDM,
    WAVEFORM_TYPE_POFDM,
    WAVEFORM_TYPE_IDEAL_NO_DOPPLER
  };

  WaveformType GetWaveformType (void);
  void SetWaveformType (WaveformType w);

  bool GetSrtaPi (void);
  void SetSrtaPi (bool b);

private:
  NetworkNode* m_device;
  RadioChannel* m_dlChannel;
  RadioChannel* m_dlMcChannel;
  RadioChannel* m_ulChannel;

  BandwidthManager* m_bandwidthManager; //Description of the UL and DL available BandwidthManager
  double m_carrierFrequency; // MHz

  double m_txPower;
  TransmittedSignal* m_txSignal;
  double m_noiseFigure;
  double m_thermalNoise;
  double m_averageBuildingHeight;

  int m_txAntennas;
  int m_rxAntennas;
  AntennaParameters* m_antennaParameters;

  Interference *m_interference;
  ErrorModel *m_errorModel;
  WaveformType m_waveform;
  bool m_useSrtaPi;
};

#endif /* PHY_H_ */
