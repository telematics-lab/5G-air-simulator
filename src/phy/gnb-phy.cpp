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


#include "gnb-phy.h"
#include "../device/NetworkNode.h"
#include "../channel/RadioChannel.h"
#include "../core/spectrum/bandwidth-manager.h"
#include "../protocolStack/packet/packet-burst.h"
#include "../core/spectrum/transmitted-signal.h"
#include "../core/idealMessages/ideal-control-messages.h"
#include "../device/GNodeB.h"
#include "../device/UserEquipment.h"
#include "interference.h"
#include "error-model.h"
#include "../channel/propagation-model/propagation-loss-model.h"
#include "../protocolStack/mac/AMCModule.h"
#include "../utility/eesm-effective-sinr.h"
#include "../utility/miesm-effective-sinr.h"
#include "../componentManagers/FrameManager.h"


#define UL_INTERFERENCE 4

GnbPhy::GnbPhy()
{
  SetDevice(nullptr);
  SetDlChannel(nullptr);
  SetUlChannel(nullptr);
  SetBandwidthManager(nullptr);
  SetTxSignal (nullptr);
  SetErrorModel (nullptr);
  SetInterference (nullptr);
  SetTxPower(43); //dBm
  SetTxAntennas(1);
  SetRxAntennas(1);
  GetAntennaParameters ()->SetType(Phy::AntennaParameters::ANTENNA_TYPE_OMNIDIRECTIONAL);
  GetAntennaParameters ()->SetBearing(0);
  GetAntennaParameters ()->SetEtilt(15);
}

GnbPhy::~GnbPhy()
{
  Destroy ();
}

void
GnbPhy::DoSetBandwidthManager (void)
{
  BandwidthManager* s = GetBandwidthManager ();
  vector<double> channels = s->GetDlSubChannels ();

  TransmittedSignal* txSignal = new TransmittedSignal ();

  vector< vector<double> > values;
  values.resize( GetTxAntennas () );

  double powerTx = pow (10., (GetTxPower () - 30) / 10); // in natural unit

  double txPower = 10 * log10 (powerTx / channels.size () ); //in dB

  for (int i = 0; i < GetTxAntennas (); i++)
    {
      for (auto channel : channels)
        {
          values.at(i).push_back(txPower);
        }
    }
  txSignal->SetValues (values);
  //txSignal->SetBandwidthManager (s->Copy());

  SetTxSignal (txSignal);
}

void
GnbPhy::StartTx (shared_ptr<PacketBurst> p)
{
  //cout << "Node " << GetDevice()->GetIDNetworkNode () << " starts phy tx" << endl;

  if (FrameManager::Init()->MbsfnEnabled()==true && FrameManager::Init()->isMbsfnSubframe()==true)
    {
      GetDlMcChannel ()->StartTx (p, GetTxSignal (), GetDevice ());
    }
  else
    {
      GetDlChannel ()->StartTx (p, GetTxSignal (), GetDevice ());
    }
}

void
GnbPhy::StartRx (shared_ptr<PacketBurst> p, TransmittedSignal* txSignal)
{
DEBUG_LOG_START_1(SIM_ENV_TEST_DEVICE_ON_CHANNEL)
  cout << "Node " << GetDevice()->GetIDNetworkNode () << " starts phy rx" << endl;
DEBUG_LOG_END

  //COMPUTE THE SINR
  vector<double> measuredSinr;
  vector<int> channelsForRx;
  vector<double> rxSignalValues = txSignal->GetValues().at(0);

  double interference = 0;
  double noise_interference = 10. * log10 (pow(10., GetThermalNoise()/10) + interference); // dB

  int chId = 0;
  for ( auto power : rxSignalValues ) // transmission power for the current sub channel [dB]
    {
      if (power != 0.)
        {
          channelsForRx.push_back (chId);
        }
      chId++;
      measuredSinr.push_back (power - noise_interference - UL_INTERFERENCE);
    }

  //CHECK FOR PHY ERROR
  bool phyError = false;
  /*
  if (GetErrorModel() != nullptr)
    {
    vector<int> cqi; //compute the CQI
    phyError = GetErrorModel ()->CheckForPhysicalError (channelsForRx, cqi, measuredSinr);
    if (_PHY_TRACING_)
      {
        if (phyError)
          {
            cout << "**** YES PHY ERROR (node " << GetDevice ()->GetIDNetworkNode () << ") ****" << endl;
          }
        else
          {
            cout << "**** NO PHY ERROR (node " << GetDevice ()->GetIDNetworkNode () << ") ****" << endl;
          }
      }
    }
    */

  if (!phyError && p->GetNPackets() > 0)
    {
      //FORWARD RECEIVED PACKETS TO THE DEVICE
      GetDevice()->ReceivePacketBurst(p);
    }

  delete txSignal;
}

void
GnbPhy::SendIdealControlMessage (IdealControlMessage *msg)
{
  GNodeB *gnb = GetDevice ();
  switch( msg->GetMessageType() )
    {
    case IdealControlMessage::ALLOCATION_MAP:
        {
          PdcchMapIdealControlMessage *pdcchMsg =  (PdcchMapIdealControlMessage*)msg;
          for (auto ue : pdcchMsg->GetTargetUEs() )
            {
              ue->GetPhy ()->ReceiveIdealControlMessage (msg);
            }
        }
      break;

    case IdealControlMessage::NB_IOT_ALLOCATION_MAP:
        {
          NbIoTMapIdealControlMessage *pdcchMsg =  (NbIoTMapIdealControlMessage*)msg;
          for (auto record : *pdcchMsg->GetMessage())
            {
              record.m_ue->GetPhy ()->ReceiveIdealControlMessage (msg);
            }
        }
      break;

    case IdealControlMessage::RA_RESPONSE:
    case IdealControlMessage::RA_CONNECTION_RESOLUTION:
      msg->GetDestinationDevice()->GetPhy ()->ReceiveIdealControlMessage (msg);
      break;

    default:
      cout << "Error in GnbPhy::SendIdealControlMessage: unknown message type (" << msg->GetMessageType() << ")" << endl;
      exit(1);
    }

  delete msg;
}

void
GnbPhy::ReceiveIdealControlMessage (IdealControlMessage *msg)
{
  GNodeB* gnb = GetDevice();
  gnb->GetMacEntity()->ReceiveIdealControlMessage(msg);
}

void
GnbPhy::ReceiveReferenceSymbols (NetworkNode* n, TransmittedSignal* s)
{
  GNodeB::UserEquipmentRecord* user = ((UserEquipment*) n)->GetTargetNodeRecord();
  ReceivedSignal* rxSignal;
  if (GetUlChannel ()->GetPropagationLossModel () != nullptr)
    {
      rxSignal = GetUlChannel ()->GetPropagationLossModel ()->
                 AddLossModel (n, GetDevice (), s);
    }
  else
    {
      rxSignal = s->Copy ();
    }
  AMCModule* amc = user->GetUE()->GetProtocolStack ()->GetMacEntity ()->GetAmcModule ();
  vector<double> ulQuality;
  vector<double> rxSignalValues = rxSignal->GetValues ().at(0);
  delete rxSignal;
  double noise_interference = 10. * log10 (pow(10., GetThermalNoise()/10)); // dB
  for (auto power : rxSignalValues)
    {
      ulQuality.push_back (power - noise_interference - UL_INTERFERENCE);
    }


DEBUG_LOG_START_1(SIM_ENV_TEST_UL_SINR)
  double effectiveSinr = GetMiesmEffectiveSinr (ulQuality);
  if (effectiveSinr > 40) effectiveSinr = 40;
  int mcs = amc->GetMCSFromCQI (amc->GetCQIFromSinr(effectiveSinr));
  cout << "UL_SINR " << n->GetIDNetworkNode () << " "
            << n->GetMobilityModel ()->GetAbsolutePosition()->GetCoordinateX () << " "
            << n->GetMobilityModel ()->GetAbsolutePosition()->GetCoordinateY () << " "
            << effectiveSinr << " " << mcs << endl;
DEBUG_LOG_END


  user->SetUplinkChannelStatusIndicator (ulQuality);
}


GNodeB*
GnbPhy::GetDevice(void)
{
  Phy* phy = (Phy*)this;
  return (GNodeB*)phy->GetDevice();
}
