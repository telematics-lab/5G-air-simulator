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
#include "ue-phy.h"
#include "sinr-calculator.h"

//TODO: CHECK GD i believe we should discuss how to dynamically implement this, by selecting if we want complex uplink interference or not
#define UL_INTERFERENCE 0

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
  values.clear();
  channels.clear();
}

void
GnbPhy::StartTx (shared_ptr<PacketBurst> p) {
    DEBUG_LOG_START_1(SIM_ENV_TEST_DEVICE_ON_CHANNEL)
        cout << "Node " << GetDevice()->GetIDNetworkNode () << " starts phy tx" << endl;
    DEBUG_LOG_END
    if (FrameManager::Init()->MbsfnEnabled()==true && FrameManager::Init()->isMbsfnSubframe()==true)
        GetDlMcChannel ()->StartTx (p, GetTxSignal (), GetDevice ());
    else
        GetDlChannel ()->StartTx (p, GetTxSignal (), GetDevice ());
}

//TODO: CHECK GD for the moment i commented the original method, let's discuss interference together when you complete the other TODOS (leave this one)
//void
//GnbPhy::StartRx (shared_ptr<PacketBurst> p, TransmittedSignal* txSignal)
//{
//DEBUG_LOG_START_1(SIM_ENV_TEST_DEVICE_ON_CHANNEL)
//  cout << "Node " << GetDevice()->GetIDNetworkNode () << " starts phy rx" << endl;
//DEBUG_LOG_END
//
//  //COMPUTE THE SINR
//  vector<double> measuredSinr;
//  vector<int> channelsForRx;
//  vector<double> rxSignalValues = txSignal->GetValues().at(0);
//
//  double interference = 0;
//  double noise_interference = 10. * log10 (pow(10., GetThermalNoise()/10) + interference); // dB
//
//  int chId = 0;
//  for ( auto power : rxSignalValues ) // transmission power for the current sub channel [dB]
//    {
//      if (power != 0.)
//        {
//          channelsForRx.push_back (chId);
//        }
//      chId++;
//      measuredSinr.push_back (power - noise_interference - UL_INTERFERENCE);
//    }
//
//  //CHECK FOR PHY ERROR
//  bool phyError = false;
//  /*
//  if (GetErrorModel() != nullptr)
//    {
//    vector<int> cqi; //compute the CQI
//    phyError = GetErrorModel ()->CheckForPhysicalError (channelsForRx, cqi, measuredSinr);
//    if (_PHY_TRACING_)
//      {
//        if (phyError)
//          {
//            cout << "**** YES PHY ERROR (node " << GetDevice ()->GetIDNetworkNode () << ") ****" << endl;
//          }
//        else
//          {
//            cout << "**** NO PHY ERROR (node " << GetDevice ()->GetIDNetworkNode () << ") ****" << endl;
//          }
//      }
//    }
//    */
//
//  if (!phyError && p->GetNPackets() > 0)
//    {
//      //FORWARD RECEIVED PACKETS TO THE DEVICE
//      GetDevice()->ReceivePacketBurst(p);
//    }
//
//  delete txSignal;
//}


void
GnbPhy::StartRx(shared_ptr<PacketBurst> p, TransmittedSignal* txSignal) {
    
    DEBUG_LOG_START_1(LTE_SIM_TEST_DEVICE_ON_CHANNEL)
    cout << "Node " << GetDevice()->GetIDNetworkNode () << " starts phy rx" << endl;
    DEBUG_LOG_END
    
    bool phyError = false;
    
    if (p->GetNPackets() > 0 && GetErrorModel() != nullptr)
    {
        AMCModule *amc = GetDevice ()->GetProtocolStack ()->GetMacEntity ()->GetAmcModule ();
        UePhy* uePhy = (UePhy*) GetDevice()->GetUserEquipmentRecord(p->GetPackets().front()->GetSourceID())->GetUE()->GetPhy();
        
        vector<int> channelsForRx = uePhy->GetChannelsForTx();
        if (channelsForRx.size() > 0) {
            //COMPUTE THE SINR
            vector< vector<double> > rxSignalValues;//for uplink mimo
            vector< vector<float> > rxSignalPhases;//for uplink mimo
            
            rxSignalValues = txSignal->GetValues();//for uplink mimo
            rxSignalPhases = txSignal->GetPhases();//for uplink mimo
            
            vector<int> rbsUePhy = uePhy->GetChannelsForTx();
            //compute the distance between user and enodeb
            CartesianCoordinates* enbpos = GetDevice()->GetMobilityModel()->GetAbsolutePosition();
            double dist = uePhy->GetDevice()->GetMobilityModel()->GetAbsolutePosition()->GetDistance(enbpos);
            uePhy->GetDevice()->SetDistanceFromServingCell(dist);
            
            int nbOfSubChannels = 100; //(int)rxSignalValues.at(0).size();//rxSignalValues.at (0).size ();//for uplink mimo
            int nbOfRxSubChannels = channelsForRx.size();//for uplink mimo
            int nbOfPaths = rxSignalValues.size ();//for uplink mimo
            int nbRxAntennas = GetRxAntennas();//for uplink mimo
            int nbTxAntennas = nbOfPaths / nbRxAntennas;//for uplink mimo
            map<UserEquipment*, double> interference;
            double Interference = 0;
            
            //MARK: Uplink interference
            //FIXME: No interference in uplink
            //TODO: Add interference in uplink
            if (GetInterference () != nullptr)
                interference = GetInterference ()->ComputeUplinkInterference(uePhy->GetDevice());
            else {
                //interference = 0;
            }
            double noise_interference; // = 10. * log10 (pow(10., GetThermalNoise()/10) + interference); // dB
            vector<double> measuredSinr;
            //vector<double> rxSignalValues = txSignal->GetValues().at(0);
            int txMode = GetDevice ()->GetMacEntity()->GetDefaultUlTxMode(); //for uplink mimo
            if(txMode == 11) {
                measuredSinr.resize(nbOfSubChannels);//for uplink mimo
                arma::cx_fmat powers = arma::cx_fmat(nbRxAntennas,nbTxAntennas);//for uplink mimo
                
                arma::cx_fmat receivedSignalLevels;  // received signal level on each path//for uplink mimo
                receivedSignalLevels.set_size(nbRxAntennas,nbTxAntennas);//for uplink mimo
                arma::cx_fmat precoding;//for uplink mimo
                arma::cx_fmat precodedH0;//for uplink mimo
                arma::fvec SINRs;//for uplink mimo
                
                for (int i = 0; i < nbOfSubChannels; i++){ //for uplink mimo
                    for( int j = 0; j < nbRxAntennas; j++)//for uplink mimo
                        for( int k = 0; k < nbTxAntennas; k++)//for uplink mimo
                            powers(j,k)=pow(10,(long double)rxSignalValues.at (j*nbTxAntennas+k).at (i)/10); //for uplink mimo
                    
                    double avgPower = arma::mean( (arma::fvec)arma::sum(arma::abs(powers),1) );//for uplink mimo
                    double avgPathPower = arma::mean( arma::mean(arma::abs(powers)) );//for uplink mimo
                    
                    for( int j = 0; j < nbRxAntennas; j++)//for uplink mimo
                        for( int k = 0; k < nbTxAntennas; k++)//for uplink mimo
                            receivedSignalLevels(j,k) =  polar( (float)sqrt( abs( powers(j,k) ) ), rxSignalPhases[j*nbTxAntennas+k][i] );//for uplink mimo

                    if (find(channelsForRx.begin (), channelsForRx.end (), i)!=channelsForRx.end ()) { //for uplink mimo
                        int index = distance(channelsForRx.begin (), find(channelsForRx.begin (), channelsForRx.end (), i));//for uplink mimo
                        if (uePhy->GetPrecodingMatricesForRx().at(index) != nullptr)//for uplink mimo
                            precoding = *uePhy->GetPrecodingMatricesForRx().at(index);//for uplink mimo
                        else//for uplink mimo
                            //TODO: how to measure channel quality when channel is not scheduled (i.e. when not using round robin)?
                            precoding = arma::eye<arma::cx_fmat>(nbTxAntennas,nbRxAntennas);//for uplink mimo
                    }
                    else
                        //TODO: how to measure channel quality when channel is not scheduled (i.e. when not using round robin)?
                        precoding = arma::eye<arma::cx_fmat>(nbTxAntennas,nbRxAntennas);//for uplink mimo
                    
                    vector<double> sinrs;//for uplink mimo
                    
                    int speed = uePhy->GetDevice()->GetMobilityModel()->GetSpeed();
                    WaveformType waveform = GetDevice()->GetPhy()->GetWaveformType();
                    double dopplerSIR = INFINITY;//GetInterference()->ComputeDopplerInterference(speed,waveform);
                    
                    std::map<UserEquipment*,double>::iterator it = interference.begin();
                    for(it=interference.begin();it!=interference.end();it++) {
                        UePhy* uephy = (UePhy*) it->first->GetPhy();
                        vector<int> rbs = uephy->GetChannelsForTx();
                        if(find(rbs.begin(),rbs.end(), i)!=rbs.end()&&find(rbsUePhy.begin(),rbsUePhy.end(), i)!=rbsUePhy.end())
                            Interference+=it->second;
                        else
                            //Interference+=0;
                        rbs.clear();
                    }
                    noise_interference = 10. * log10 (pow(10., GetThermalNoise()/10) + Interference);
                    sinrs = SinrCalculator::MimoReception(
                                                          //sinrs = SinrCalculator::MimoReceptionMRC(
                                                          receivedSignalLevels,
                                                          precoding,
                                                          noise_interference,
                                                          uePhy->GetAssignedLayers(),
                                                          dopplerSIR);
                    
                    double effsinr = GetMiesmEffectiveSinr( sinrs ); //for uplink mimo
                    measuredSinr.at(i) = effsinr; //for uplink mimo
                    //sinrs.clear();
                    Interference = 0;
                }
            }
            else //for uplink mimo
            {
                for ( auto power : rxSignalValues.at(0) ) {
                    for(int i = 0; i<nbOfSubChannels;i++) {
                        std::map<UserEquipment*,double>::iterator it = interference.begin();
                        for(it=interference.begin();it!=interference.end();it++) {
                            UePhy* uephy = (UePhy*) it->first->GetPhy();
                            vector<int> rbs = uephy->GetChannelsForTx();
                            if(find(rbs.begin(),rbs.end(), i)!=rbs.end()&&find(rbsUePhy.begin(),rbsUePhy.end(), i)!=rbsUePhy.end())
                                Interference+=it->second;
                            else {
                                //Interference+=0;
                            }
                        }
                        noise_interference = 10. * log10 (pow(10., GetThermalNoise()/10) + Interference);// transmission power for the current sub channel [dB]
                        measuredSinr.push_back (10*log10(power-noise_interference));//for uplink mimo
                    }
                }
            }
            
            
            vector<int> mcs = uePhy->GetMcsIndexForTx();//for uplink mimo
            vector<int> cqi;//for uplink mimo
            if(txMode == 11) {
                for (int i = 0; i < (int)mcs.size (); i++) { //for uplink mimo
                    AMCModule *amc = GetDevice ()->GetProtocolStack ()->GetMacEntity ()->GetAmcModule ();//for uplink mimo
                    int cqi_ =  amc->GetCQIFromMCS (uePhy->GetMcsIndexForTx().at (i));//for uplink mimo
                    cqi.push_back (cqi_);//for uplink mimo
                }
            }
            else
                for (auto m : mcs)
                    cqi.push_back (amc->GetCQIFromMCS (m));

            
            
            
            //CHECK FOR PHY ERROR
            phyError = GetErrorModel ()->CheckForPhysicalError (channelsForRx, cqi, measuredSinr);
            cqi.clear();
            if (_PHY_TRACING_) {
                vector<double> sinr_;
                
                for (auto c : channelsForRx)
                    sinr_.push_back (measuredSinr.at (c));

                double effective_sinr = GetMiesmEffectiveSinr (sinr_);
                
                if (effective_sinr > 40)
                    effective_sinr = 40;
                int MCS_ = 0;
                int TBS_ = 0;
                UserEquipment* ue = GetDevice()->GetUserEquipmentRecord(p->GetPackets().front()->GetSourceID())->GetUE();
                
                if (phyError)
                    cout << "**** YES PHY ERROR (node " << GetDevice ()->GetIDNetworkNode () << ") ****" << endl;
                else
                    cout << "**** NO PHY ERROR (node " << GetDevice ()->GetIDNetworkNode () << ") ****" << endl;
                
                MCS_ = mcs.at(0);
                if(txMode == 11)
                    TBS_ = amc->GetTBSizeFromMCS (MCS_, MCS_, (int) channelsForRx.size (), 1);
                else
                    TBS_ = amc->GetTBSizeFromMCS (MCS_, (int) channelsForRx.size ());
                cout << "PHY_RX SRC " << ue->GetIDNetworkNode()
                << " DST " << GetDevice()->GetIDNetworkNode()
                << " X " << ue->GetMobilityModel ()->GetAbsolutePosition ()->GetCoordinateX ()
                << " Y " << ue->GetMobilityModel ()->GetAbsolutePosition ()->GetCoordinateY ()
                << " SINR " << effective_sinr
                << " RB " << channelsForRx.size ()
                << " MCS " << MCS_
                << " SIZE " << TBS_
                << " ERR " << phyError
                << " T " << Simulator::Init()->Now()
                << " UL DIST " << dist
                << endl;
            }
            interference.clear();
            channelsForRx.clear();
            
            rxSignalPhases.clear();
            rxSignalValues.clear();
            rbsUePhy.clear();
            measuredSinr.clear();
            mcs.clear();
            rbsUePhy.clear();
        }
        uePhy->GetChannelsForTx().clear();
    }
    
    if (!phyError)
        //FORWARD RECEIVED PACKETS TO THE DEVICE
        GetDevice()->ReceivePacketBurst(p);
        
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

//TODO: CHECK GD for the moment i commented the original method, let's discuss interference together when you complete the other TODOS (leave this one)
//void
//GnbPhy::ReceiveReferenceSymbols (NetworkNode* n, TransmittedSignal* s)
//{
//  GNodeB::UserEquipmentRecord* user = ((UserEquipment*) n)->GetTargetNodeRecord();
//  ReceivedSignal* rxSignal;
//  if (GetUlChannel ()->GetPropagationLossModel () != nullptr)
//    {
//      rxSignal = GetUlChannel ()->GetPropagationLossModel ()->
//                 AddLossModel (n, GetDevice (), s);
//    }
//  else
//    {
//      rxSignal = s->Copy ();
//    }
//  AMCModule* amc = user->GetUE()->GetProtocolStack ()->GetMacEntity ()->GetAmcModule ();
//  vector<double> ulQuality;
//  vector<double> rxSignalValues = rxSignal->GetValues ().at(0);
//  delete rxSignal;
//  double noise_interference = 10. * log10 (pow(10., GetThermalNoise()/10)); // dB
//  for (auto power : rxSignalValues)
//    {
//      ulQuality.push_back (power - noise_interference - UL_INTERFERENCE);
//    }
//
//
//DEBUG_LOG_START_1(SIM_ENV_TEST_UL_SINR)
//  double effectiveSinr = GetMiesmEffectiveSinr (ulQuality);
//  if (effectiveSinr > 40) effectiveSinr = 40;
//  int mcs = amc->GetMCSFromCQI (amc->GetCQIFromSinr(effectiveSinr));
//  cout << "UL_SINR " << n->GetIDNetworkNode () << " "
//            << n->GetMobilityModel ()->GetAbsolutePosition()->GetCoordinateX () << " "
//            << n->GetMobilityModel ()->GetAbsolutePosition()->GetCoordinateY () << " "
//            << effectiveSinr << " " << mcs << endl;
//DEBUG_LOG_END
//
//
//  user->SetUplinkChannelStatusIndicator (ulQuality);
//}


void
GnbPhy::ReceiveReferenceSymbols (NetworkNode* n, TransmittedSignal* s)
{
    GNodeB::UserEquipmentRecord* user = ((UserEquipment*) n)->GetTargetNodeRecord();
    UePhy* uePhy = (UePhy*) user->m_UE->GetPhy();
    ReceivedSignal* rxSignal;

    if (GetUlChannel ()->GetPropagationLossModel () != nullptr)
    {
        rxSignal = GetUlChannel ()->GetPropagationLossModel ()->AddLossModel (n, GetDevice (), s);
    }
    else
    {
        rxSignal = s->Copy ();//for uplink mimo//vlmdol
    }

    vector<int> channelsForRx = uePhy->GetChannelsForTx() ;
    vector<double> ulQuality;
    vector< shared_ptr<arma::cx_fmat> > m_fullCsiForUL;
    //vector<double> rxSignalValues = rxSignal->GetValues ().at(0);

    vector< vector<double> > rxSignalValues;//for uplink mimo
    vector< vector<float> > rxSignalPhases;//for uplink mimo

    rxSignalValues = rxSignal->GetValues();//for uplink mimo
    rxSignalPhases = rxSignal->GetPhases();//for uplink mimo
    vector<int> rbsUePhy = uePhy->GetChannelsForTx();
    //compute the distance between user and enodeb

    int nbOfSubChannels =100; //(int)rxSignalValues.at(0).size();//rxSignalValues.at (0).size ();//for uplink mimo
    int nbOfRxSubChannels = channelsForRx.size();//for uplink mimo
    int nbOfPaths = rxSignalValues.size ();//for uplink mimo
    int nbRxAntennas = GetRxAntennas();//for uplink mimo
    int nbTxAntennas = 1;//nbOfPaths / nbRxAntennas;//for uplink mimo
    map<UserEquipment*, double> interference;
    double Interference = 0;
    //MARK: Uplink interference
    //FIXME: No interference in uplink
    //TODO: Add interference in uplink
    if (GetInterference () != nullptr)
    {
        interference = GetInterference ()->ComputeUplinkInterference(uePhy->GetDevice());
    }
    else
    {
        //interference = 0;
    }
    double noise_interference = 10. * log10 (pow(10., GetThermalNoise()/10)); // dB
    //vector<double> rxSignalValues = txSignal->GetValues().at(0);
    int txMode = GetDevice ()->GetMacEntity()->GetDefaultUlTxMode();//for uplink mimo
    if(txMode == 11)
    {
        ulQuality.resize(nbOfSubChannels);//for uplink mimo
        arma::cx_fmat powers = arma::cx_fmat(nbRxAntennas,nbTxAntennas);//for uplink mimo

        arma::cx_fmat receivedSignalLevels;  // received signal level on each path//for uplink mimo
        receivedSignalLevels.set_size(nbRxAntennas,nbTxAntennas);//for uplink mimo
        arma::cx_fmat precoding;//for uplink mimo
        arma::cx_fmat precodedH0;//for uplink mimo
        arma::fvec SINRs;//for uplink mimo

        for (int i = 0; i < nbOfSubChannels; i++)//for uplink mimo
        {
            for( int j = 0; j < nbRxAntennas; j++)//for uplink mimo
            {
                for( int k = 0; k < nbTxAntennas; k++)//for uplink mimo
                {
                    powers(j,k)=pow(10,(long double)rxSignalValues.at (j*nbTxAntennas+k).at (i)/10);//for uplink mimo
                }
            }

            double avgPower = arma::mean( (arma::fvec)arma::sum(arma::abs(powers),1) );//for uplink mimo
            double avgPathPower = arma::mean( arma::mean(arma::abs(powers)) );//for uplink mimo
            for( int j = 0; j < nbRxAntennas; j++)//for uplink mimo
            {
                for( int k = 0; k < nbTxAntennas; k++)//for uplink mimo
                {
                    receivedSignalLevels(j,k) =  polar( (float)sqrt( abs( powers(j,k) ) ), rxSignalPhases[j*nbTxAntennas+k][i] );//for uplink mimo
                }
            }




            if (find(channelsForRx.begin (), channelsForRx.end (), i)!=channelsForRx.end ())//for uplink mimo
            {
                int index = distance(channelsForRx.begin (), find(channelsForRx.begin (), channelsForRx.end (), i));//for uplink mimo
                if (uePhy->GetPrecodingMatricesForRx().at(index) != nullptr)//for uplink mimo
                {
                    precoding = *uePhy->GetPrecodingMatricesForRx().at(index);//for uplink mimo
                }
                else//for uplink mimo
                {
                    //TODO: how to measure channel quality when channel is not scheduled (i.e. when not using round robin)?
                    precoding = arma::eye<arma::cx_fmat>(nbTxAntennas,nbRxAntennas);//for uplink mimo
                }
            }
            else
            {
                //TODO: how to measure channel quality when channel is not scheduled (i.e. when not using round robin)?
                precoding = arma::eye<arma::cx_fmat>(nbTxAntennas,nbRxAntennas);//for uplink mimo
            }

            vector<double> sinrs;//for uplink mimo
            vector<int> assignedLayers;
            assignedLayers.push_back(0);
            int speed = uePhy->GetDevice()->GetMobilityModel()->GetSpeed();
            WaveformType waveform = GetDevice()->GetPhy()->GetWaveformType();
            double dopplerSIR = INFINITY;//GetInterference()->ComputeDopplerInterference(speed,waveform);

            std::map<UserEquipment*,double>::iterator it = interference.begin();
            for(it=interference.begin();it!=interference.end();it++)
            {
                UePhy* uephy = (UePhy*) it->first->GetPhy();
                vector<int> rbs = uephy->GetChannelsForTx();
                if(find(rbs.begin(),rbs.end(), i)!=rbs.end()&&find(rbsUePhy.begin(),rbsUePhy.end(), i)!=rbsUePhy.end())
                {
                    Interference+=it->second;
                }
                else
                {
                    Interference+=0;
                }
                rbs.clear();
            }
            noise_interference = 10. * log10 (pow(10., GetThermalNoise()/10) + Interference);
            sinrs = SinrCalculator::MimoReception(
                    //sinrs = SinrCalculator::MimoReceptionMRC(
                    receivedSignalLevels,
                    precoding,
                    noise_interference,
                    /*uePhy->GetAssignedLayers()*/assignedLayers,
                    dopplerSIR);

            double effsinr = GetMiesmEffectiveSinr( sinrs );//for uplink mimo
            ulQuality.at(i) = effsinr;//for uplink mimo
            Interference = 0;

            shared_ptr<arma::cx_fmat> channelMatrixForFeedback = make_shared<arma::cx_fmat>(nbRxAntennas,nbTxAntennas);
            *channelMatrixForFeedback = receivedSignalLevels;
            double max_value = abs(channelMatrixForFeedback->max());
            double threshold_db = INFINITY;
            channelMatrixForFeedback->for_each( [max_value, threshold_db](complex<float>& val) { if(max_value/abs(val) > pow(10,threshold_db/20)) {val = 0;} } );
            m_fullCsiForUL.push_back(channelMatrixForFeedback); ///channelmatrice for uplink transmission

        }

    }
    else//for uplink mimo
    {
        for ( auto power : rxSignalValues.at(0) ) { // transmission power for the current sub channel [dB]
            ulQuality.push_back (10*log10(power-noise_interference));//for uplink mimo
        }
    }//grgrgr

    user->SetUplinkChannelStatusIndicator (ulQuality);
    user->SetUplinkChannelMatricesIndicator(m_fullCsiForUL);

    channelsForRx.clear();
    ulQuality.clear();
    m_fullCsiForUL.clear();
    interference.clear();
    rxSignalValues.clear();
    rxSignalPhases.clear();
    rbsUePhy.clear();
    delete rxSignal;
}



GNodeB*
GnbPhy::GetDevice(void)
{
  Phy* phy = (Phy*)this;
  return (GNodeB*)phy->GetDevice();
}
