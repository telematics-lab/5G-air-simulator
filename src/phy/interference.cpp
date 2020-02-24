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
 * Author: Felice Paparusso <felice.paparusso@gmail.com>
 * Author: Vincenzo Fortunato <vincfort92@hotmail.it>
 */


#include <vector>
#include "interference.h"
#include "../componentManagers/NetworkManager.h"
#include "../device/UserEquipment.h"
#include "../device/GNodeB.h"
#include "../device/HeNodeB.h"
#include "../utility/ComputePathLoss.h"
#include "phy.h"
#include "../core/spectrum/bandwidth-manager.h"
#include "../utility/GaussianRandomVariable.h"
#include "../core/cartesianCoodrdinates/CartesianCoordinates.h"
#include "../componentManagers/FrameManager.h"

Interference::Interference()
{
  m_OFDMInterferenceValues =
  {
    {50,36},
    {150,27},
    {250,22},
    {350,19}
  };
  m_POFDMInterferenceValues =
  {
    {50,36},
    {150,27.5},
    {250,23},
    {350,20}
  };
}

double
Interference::ComputeInterference (UserEquipment *ue, bool MBSFNConstructiveInterference)
{
  GNodeB *node;

  double interference = 0;
  int idCellPrec;
  bool oldLosType;
  double oldShadowing = 0;

  int servingCellId = ue->GetTargetNode ()->GetCell()->GetIdCell();
  ChannelRealization* target_c_dl;
  if (FrameManager::Init()->MbsfnEnabled()==true && FrameManager::Init()->isMbsfnSubframe()==true)
    {
      target_c_dl = ue->GetTargetNode ()->GetPhy ()->GetDlMcChannel ()->GetPropagationLossModel ()->GetChannelRealization(ue ->GetTargetNode(), ue);
    }
  else
    {
      target_c_dl = ue->GetTargetNode ()->GetPhy ()->GetDlChannel ()->GetPropagationLossModel ()->GetChannelRealization(ue ->GetTargetNode(), ue);
    }

  //cout << "Compute interference for UE " << ue->GetIDNetworkNode () << " ,target node " << ue->GetTargetNode ()->GetIDNetworkNode ()<< endl;

  for (auto node : *NetworkManager::Init ()->GetGNodeBContainer ())
    {
      if (node->GetIDNetworkNode () != ue->GetTargetNode ()->GetIDNetworkNode () &&
          node->GetPhy ()->GetBandwidthManager ()->GetDlOffsetBw () ==
          ue->GetTargetNode ()->GetPhy ()->GetBandwidthManager ()->GetDlOffsetBw ())
        {
          int nbTxAntennas = node->GetPhy()->GetTxAntennas();
//cout << "selecting interference from cell " << node->GetIDNetworkNode() << endl;
          // XXX Assume the internal cluster of 19 cells is the MBSFN area, and the others create interference
          if (FrameManager::Init()->MbsfnEnabled()==true && FrameManager::Init()->isMbsfnSubframe()==true)
            {
              if (MBSFNConstructiveInterference==true)
                {
                  if (node->GetIDNetworkNode()>=57)
                    {
                      continue;
                    }
                }
              else
                {
                  if (node->GetIDNetworkNode()<57)
                    {
                      continue;
                    }
                }
            }
          double powerTXForSubBandwidth = 10 * log10 (
                                            pow (10., (node->GetPhy()->GetTxPower() - 30)/10)
                                            /
                                            node->GetPhy()->GetBandwidthManager ()->GetDlSubChannels().size ()
                                            /
                                            nbTxAntennas);

          double pathLoss = 0;
          double shadowing = 0;
          double penetrationLoss = 0;
          vector<double> beamFormingGain;

          int idCell = node->GetCell()->GetIdCell();

          ChannelRealization* c_dl;
          if (FrameManager::Init()->MbsfnEnabled()==true && FrameManager::Init()->isMbsfnSubframe()==true)
            {
              c_dl = node->GetPhy()->GetDlMcChannel()->GetPropagationLossModel ()->GetChannelRealization(node,ue);
            }
          else
            {
              c_dl = node->GetPhy()->GetDlChannel()->GetPropagationLossModel ()->GetChannelRealization(node,ue);
            }
          bool channelRealizationExists;
          if (c_dl != nullptr)
            {
              channelRealizationExists = true;
            }
          else
            {
              channelRealizationExists = false;
              if (FrameManager::Init()->MbsfnEnabled()==true && FrameManager::Init()->isMbsfnSubframe()==true)
                {
                  c_dl = ue->GetTargetNode ()->GetPhy ()->GetDlMcChannel ()->GetPropagationLossModel ()->GetChannelRealization(ue ->GetTargetNode(), ue);
                }
              else
                {
                  c_dl = ue->GetTargetNode ()->GetPhy ()->GetDlChannel ()->GetPropagationLossModel ()->GetChannelRealization(ue ->GetTargetNode(), ue);
                }
            }

          if(channelRealizationExists)
            {
              // use the same shadowing value for gNBs in the same site
              if(idCell == servingCellId)
                {
                  shadowing = target_c_dl->GetShadowing();
                  c_dl->SetShadowing(shadowing);
                }
              else if (idCell == idCellPrec)
                {
                  shadowing = oldShadowing;
                  c_dl->SetShadowing(shadowing);
                }
              else
                {
                  shadowing = c_dl->GetShadowing();
                  oldShadowing = shadowing;
                }

              pathLoss = c_dl->GetPathLoss();

              // penetration loss is the same from all BS, use value from serving gNB channel realization
              penetrationLoss = target_c_dl->GetPenetrationLoss();

              for (int i=0; i<nbTxAntennas; i++)
                {
                  beamFormingGain.push_back(c_dl->GetBeamformingGain(i));
                }

              idCellPrec = idCell;
            }
          else
            {
              // -- INIZIO --
              double distance = node->GetMobilityModel ()->GetAbsolutePosition ()->GetDistance(ue->GetMobilityModel ()->GetAbsolutePosition());
              double losProbability, randomProb;

              ChannelRealization* c_dl = ue->GetTargetNode ()->GetPhy ()->GetDlChannel ()->GetPropagationLossModel ()->GetChannelRealization(ue ->GetTargetNode(), ue);
              ChannelRealization::ChannelModel channelModel = c_dl->GetChannelModel();

              //ChannelRealization::ChannelModel channelModel = ue->GetTargetNode ()->GetPhy ()->GetDlChannel ()->
              //GetPropagationLossModel ()->GetChannelRealization (ue->GetTargetNode (), ue)->GetChannelModel();

              int idCell = node->GetCell()->GetIdCell();
              if (idCell == idCellPrec)
                {
                  shadowing = oldShadowing;
                  m_isLosType = oldLosType;
                  //cout << "Shadowing :" << shadowing << " " << oldShadowing << endl;
                  //cout << "LosType :" << m_isLosType << " " << oldLosType << endl;
                }
              else
                {

                  switch(channelModel)
                    {

                    // These channel models are not handled here, they are declared only to suppress a
                    // compiler warning. This section is deprecated, a proper ChannelRealization should be
                    // created for each pair of ue-gnb
                    case ChannelRealization::CHANNEL_MODEL_MACROCELL_URBAN:
                    case ChannelRealization::CHANNEL_MODEL_MACROCELL_SUB_URBAN:
                    case ChannelRealization::CHANNEL_MODEL_MACROCELL_RURAL:
                    case ChannelRealization::CHANNEL_MODEL_MICROCELL:
                    case ChannelRealization::CHANNEL_MODEL_FEMTOCELL_URBAN:
                    case ChannelRealization::CHANNEL_MODEL_3GPP_DOWNLINK:
                    case ChannelRealization::CHANNEL_MODEL_WINNER_DOWNLINK:
                    case ChannelRealization::CHANNEL_MODEL_MACROCELL_URBAN_IMT_3D:
                      cout <<
                                "Warning: channel model not supported in ComputeInterference(), assuming MACROCELL_URBAN_IMT"
                                << endl;

                    case ChannelRealization::CHANNEL_MODEL_MACROCELL_URBAN_IMT:
                      losProbability = (min(18/distance, 1.0) * (1 - exp(-distance/63))) + exp(-distance/63);
                      randomProb = ((double) rand() / RAND_MAX);

                      if (randomProb <= losProbability)
                        {
                          m_isLosType = true;
                          shadowing = GetGaussianRandomVariable(0,4);
                        }
                      else
                        {
                          m_isLosType = false;
                          shadowing = GetGaussianRandomVariable(0,6);
                        }

                      //cout << "Urban shadowing :" << shadowing << endl;
                      break;

                    case ChannelRealization::CHANNEL_MODEL_MACROCELL_RURAL_IMT:
                      losProbability = min(1.00, exp(-(distance-10)/200)); // R1 091320
                      randomProb = ((double) rand() / RAND_MAX);

                      if (randomProb <= losProbability)
                        {
                          m_isLosType = true;

                          double dbp = 2*M_PI * 35 * 1.5 * (800/300); // 2pi * hBS * hUT * (f/c)
                          if (distance >10 && distance < dbp)
                            {
                              shadowing = GetGaussianRandomVariable(0,4);
                            }
                          else if (distance > dbp && distance < 10000)
                            {
                              shadowing = GetGaussianRandomVariable(0,6);
                            }
                        }
                      else
                        {
                          m_isLosType = false;
                          shadowing = GetGaussianRandomVariable(0,8);
                        }

                      break;

                    case ChannelRealization::CHANNEL_MODEL_3GPP_CASE1:
                      shadowing = GetGaussianRandomVariable(0,8);

                    }
                  //cout << "Celle " << idCell << " " << idCellPrec << " Model: " << channelModel << endl;
                  idCellPrec = idCell;
                  oldShadowing = shadowing;
                  oldLosType = m_isLosType;
                }


              penetrationLoss = c_dl->GetPenetrationLoss();
              pathLoss = ComputePathLossForInterference (node, ue, /*m_isLosType*/false);
              // -- FINE --
            }

          double nodeInterference_db = powerTXForSubBandwidth - shadowing - penetrationLoss - pathLoss; // in dB

          double nodeInterference = 0;

          for (int i=0; i<nbTxAntennas; i++)
            {
              nodeInterference += pow(10, (nodeInterference_db + beamFormingGain.at(i))/10);
            }

          interference += nodeInterference;


          /*cout << "\t add interference from gNB " << node->GetIDNetworkNode ()
              << " " << powerTXForSubBandwidth << " "  << shadowing << " " << penetrationLoss << " " << ComputePathLossForInterference (node, ue, m_isLosType)
              << " " << nodeInterference_db << " " << nodeInterference
              << " --> tot: " << interference
              << endl;*/

        }
    }

  //cout << "Compute interference for UE " << ue->GetIDNetworkNode () << " ,target node " << ue->GetTargetNode ()->GetIDNetworkNode ()<< endl;

  for (auto henb : *NetworkManager::Init ()->GetHomeGNodeBContainer())
    {
      if (henb->GetIDNetworkNode () != ue->GetTargetNode ()->GetIDNetworkNode () &&
          henb->GetPhy ()->GetBandwidthManager ()->GetDlOffsetBw () ==
          ue->GetTargetNode ()->GetPhy ()->GetBandwidthManager ()->GetDlOffsetBw ())
        {
          double powerTXForSubBandwidth = 10 * log10 (
                                            pow (10., (henb->GetPhy()->GetTxPower() - 30)/10)
                                            /
                                            henb->GetPhy()->GetBandwidthManager ()->GetDlSubChannels().size ());


          //ATTENZIONE double nodeInterference_db = powerTXForSubBandwidth - 10 - ComputePathLossForInterference (henb, ue); // in dB
          double nodeInterference_db = powerTXForSubBandwidth - ComputePathLossForInterference (henb, ue, m_isLosType); // in dB
          double nodeInterference = pow(10, nodeInterference_db/10);

          interference += nodeInterference;

          /*
          cout << "\t add interference from gNB " << node->GetIDNetworkNode ()
          << " " << powerTXForSubBandwidth << " "  << ComputePathLossForInterference (node, ue)
          << " " << nodeInterference_db << " " << nodeInterference
          << " --> tot: " << interference
          << endl;
          */
        }
    }

  return interference;
}


double Interference::ComputeDopplerInterference (int speed /* in km/h */, Phy::WaveformType type)
{
  vector< pair<double, double> >* interferenceTable;
  if (type==Phy::WAVEFORM_TYPE_OFDM)
    {
      interferenceTable = &m_OFDMInterferenceValues;
    }
  else if (type==Phy::WAVEFORM_TYPE_POFDM)
    {
      interferenceTable = &m_POFDMInterferenceValues;
    }
  else if (type==Phy::WAVEFORM_TYPE_IDEAL_NO_DOPPLER)
    {
      return INFINITY;
    }
  else
    {
      cout << "Error: invalid waveform type: " << type << endl;
      exit(1);
    }

  double SIR;
  int last = interferenceTable->size()-1;
  if (speed < interferenceTable->at(0).first)
    {
      // use linear extrapolation
      SIR = interferenceTable->at(0).second - (interferenceTable->at(0).first - speed) * (interferenceTable->at(1).second - interferenceTable->at(0).second) / (interferenceTable->at(1).first - interferenceTable->at(0).first);
    }
  else if (speed >= interferenceTable->at(interferenceTable->size()-1).first)
    {
      // use linear extrapolation
      SIR = interferenceTable->at(last).second + (speed - interferenceTable->at(last).first) * (interferenceTable->at(last).second - interferenceTable->at(last-1).second) / (interferenceTable->at(last).first - interferenceTable->at(last-1).first);
    }
  else
    {
      for (int i=0; i<last; i++)
        {
          if (speed >= interferenceTable->at(i).first && speed < interferenceTable->at(i+1).first)
            {
              // use linear interpolation
              SIR = interferenceTable->at(i).second + (speed - interferenceTable->at(i).first) * (interferenceTable->at(i+1).second - interferenceTable->at(i).second) / (interferenceTable->at(i+1).first - interferenceTable->at(i).first);
            }
        }
    }

  return SIR;
}
