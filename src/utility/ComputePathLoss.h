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


#ifndef COMPUTE_PATH_LOSS_H_
#define COMPUTE_PATH_LOSS_H_

#include "../device/NetworkNode.h"
#include <stdint.h>
#include "stdlib.h"
#include <math.h>
#include "IndoorScenarios.h"
#include "../channel/RadioChannel.h"
#include "../channel/propagation-model/propagation-loss-model.h"
#include "../channel/propagation-model/channel-realization.h"



static double
ComputePathLossForInterference (GNodeB* src, UserEquipment* dst, bool isLosType)
{
  /*
   * Path loss Models from sect. 5.2 in
   * 3GPP TSG RAN WG4 R4-092042
   */
  double externalWallAttenuation = 20;
  double pathLoss = 0.0;
  double f = src->GetPhy ()->GetCarrierFrequency ();

  double Henb = src->GetMobilityModel ()->GetAbsolutePosition ()->GetCoordinateZ ();
  double Hb = src->GetPhy ()->GetAverageBuildingHeight ();
  double losProbability;
  double randomProb;
  double dbp;

  //srand(time(nullptr));

  ChannelRealization::ChannelModel channelType = dst->GetTargetNode ()->GetPhy ()->GetDlChannel ()->
      GetPropagationLossModel ()->GetChannelRealization (dst->GetTargetNode (), dst)->GetChannelModel();

  if (src->GetNodeType() == NetworkNode::TYPE_GNODEB && dst->GetNodeType () == NetworkNode::TYPE_UE)
    {
      //Path Loss Model For Urban Environment
      double distance = src->GetMobilityModel ()->GetAbsolutePosition ()->GetDistance (
                          dst->GetMobilityModel ()->GetAbsolutePosition ());

      switch(channelType)
        {
        case ChannelRealization::CHANNEL_MODEL_MACROCELL_URBAN:
        default:
          pathLoss = 80 + 40*( 1 - 4 * 0.001 * (Henb-Hb) )*log10(distance * 0.001) - 18*log10(Henb-Hb) + 21*log10(f);
          if ( dst->IsIndoor() )
            {
              pathLoss = pathLoss + externalWallAttenuation;
            }
          break;

        case ChannelRealization::CHANNEL_MODEL_3GPP_CASE1:
        case ChannelRealization::CHANNEL_MODEL_MACROCELL_SUB_URBAN:
          pathLoss = 128.1 + (37.6 * log10 (distance * 0.001));
          break;

        case ChannelRealization::CHANNEL_MODEL_MACROCELL_RURAL:
          pathLoss = 69.55 + 26.16*log10(f) - 13.82*log10(Henb) + (44.9-6.55*log10(Henb))*log10(distance * 0.001) - 4.78*pow(log10(f),2) + 18.33*log10(f) - 40.94;
          break;

        case ChannelRealization::CHANNEL_MODEL_MICROCELL:
          pathLoss = 24 + (45 * log10 (distance));
          break;

        case ChannelRealization::CHANNEL_MODEL_MACROCELL_URBAN_IMT:
          //losProbability = min(18/distance, 1.0) * (1 - exp(-distance/63)) + exp(-distance/63);
          //randomProb = ((double) rand() / RAND_MAX);
          //dbp1 = 4*(Henb-1)*(1.5-1)*(f/300); // 4*(h'BS)*(h'UT)*(f/c)
          Hb = 20;
          if (isLosType) pathLoss = 22.0*log10(distance) + 34.02;
          else  pathLoss = 19.56+3.91*10*log10(distance);
          break;

        case ChannelRealization::CHANNEL_MODEL_MACROCELL_RURAL_IMT:
          /*if(distance <= 10) {
              losProbability = 1;
          } else {
              losProbability = exp(-((distance-10)/1000));
          }*/
          //losProbability = min(1.00, exp(-(distance-10)/200)); // R1 091320
          //randomProb = ((double) rand() / RAND_MAX);
          dbp = 2*M_PI * Henb * 1.5 * (f/300); // 2pi * hBS * hUT * (f/c)
          Hb = 5;
          if(isLosType)
            {
              // LoS
              if(distance > 10 && distance < dbp)
                {
                  pathLoss = 20*log10(40*M_PI*distance*(f*0.001/3)) + min(0.03*pow(Hb, 1.72), 10.00)*log10(distance) - min(0.044*pow(Hb, 1.72), 14.77)+0.002*log10(Hb)*distance;
                }
              else if(distance > dbp && distance < 10000)
                {
                  pathLoss = 20*log10(40*M_PI*dbp*(f*0.001/3)) + min(0.03*pow(Hb, 1.72), 10.00)*log10(dbp) - min(0.044*pow(Hb, 1.72), 14.77)+0.002*log10(Hb)*dbp + (40*log10(distance/dbp));
                }
            }
          else
            {
              // NLoS
              pathLoss = 161.04 - 7.1*log10(20) + 7.5*log10(Hb) - (24.37 - 3.7*pow(Hb/Henb, 2)) * log10(Henb) + (43.42 - 3.1*log10(Henb)) * (log10(distance) - 3) + 20*log10(f * 0.001) - (3.2 * pow(log10(11.75*1.5), 2) - 4.97);
            }
          break;
        }


      if( src->GetPhy ()->GetAntennaParameters ()->GetType() == Phy::AntennaParameters::ANTENNA_TYPE_TRI_SECTOR )
        {
          double src_bearing = src->GetPhy ()->GetAntennaParameters ()->GetBearing();
          double dst_bearing = dst->GetMobilityModel ()->GetAbsolutePosition ()->GetPolarAzimut (
                                 src->GetMobilityModel ()->GetAbsolutePosition () ) * 57.2957795;
          double relative_angle = src_bearing-dst_bearing;


          // adjust to [-180°, +180]
          while (relative_angle >  180 )
            {
              relative_angle -= 360;
            }
          while (relative_angle < -180 )
            {
              relative_angle += 360;
            }

          double antennaGain = src->GetPhy()->GetAntennaParameters ()->GetGain();
          double max_attenuation_h =  src->GetPhy ()->GetAntennaParameters ()->GetMaxHorizontalAttenuation ();
          double beamwidth_3dB_h = src->GetPhy ()->GetAntennaParameters ()->GetHorizontalBeamwidth3db ();
          double directional_loss_h = +min( 12*pow(relative_angle/beamwidth_3dB_h,2),max_attenuation_h);
          double relative_angle_v = atan(
                                      src->GetMobilityModel()->GetAbsolutePosition()->GetCoordinateZ() /
                                      dst->GetMobilityModel()->GetAbsolutePosition()->GetDistance(src->GetMobilityModel()->GetAbsolutePosition())
                                    ) * 57.2957795;
          double max_attenuation_v = src->GetPhy ()->GetAntennaParameters ()->GetMaxVerticalAttenuation ();
          double beamwidth_3dB_v = src->GetPhy ()->GetAntennaParameters ()->GetVerticalBeamwidth3db ();
          double etilt = src->GetPhy()->GetAntennaParameters ()->GetEtilt();
          double directional_loss_v = +min(12*pow((relative_angle_v-etilt)/beamwidth_3dB_v,2), max_attenuation_v);
          double directional_loss = min((directional_loss_h+directional_loss_v), max_attenuation_h);
          pathLoss += - antennaGain + directional_loss;

DEBUG_LOG_START_1(SIM_ENV_TRIPLE_SECTOR_DEBUG)
          cout << "ID UE: " << dst->GetIDNetworkNode()
                    << ",\tID gNB: " << src->GetIDNetworkNode()
                    << ",\tX: " << dst->GetMobilityModel ()->GetAbsolutePosition ()->GetCoordinateX()
                    << ",\tY: " << dst->GetMobilityModel ()->GetAbsolutePosition ()->GetCoordinateY()
                    << ",\tAngle " << relative_angle
                    << ",\tLoss: " << directional_loss
                    << endl;
DEBUG_LOG_END
        }

      return pathLoss;
    }

  if (src->GetNodeType() == NetworkNode::TYPE_HOME_BASE_STATION && dst->GetNodeType () == NetworkNode::TYPE_UE)
    {
      double minimumCouplingLoss = 45; //[dB] - see 3GPP TSG RAN WG4 #42bis (R4-070456)
      double floorPenetration = 0.0;

      //Path Loss Model For Urban Environment (Indoor Case)
      /*
      double distance = src->GetMobilityModel ()->GetAbsolutePosition ()->GetDistance (
        dst->GetMobilityModel ()->GetAbsolutePosition ());

      if( src->GetCell()->GetCellCenterPosition()->GetFloor() > 0
          && ((UserEquipment*) dst)->IsIndoor()
             && src->GetCell()->GetCellCenterPosition()->GetFloor() != dst->GetCell()->GetCellCenterPosition()->GetFloor())
      {
        int n = (int) abs( src->GetCell()->GetCellCenterPosition()->GetFloor() - dst->GetCell()->GetCellCenterPosition()->GetFloor() );
        floorPenetration = 18.3 * pow( n, ((n+2)/(n+1)-0.46));
      }

      pathLoss = max( minimumCouplingLoss, 127 + ( 30 * log10 (distance * 0.001) ) + floorPenetration);
      return pathLoss;
      */


      //Path Loss Model For WinnerII

      double distance = src->GetMobilityModel ()->GetAbsolutePosition ()->GetDistance (
                          dst->GetMobilityModel ()->GetAbsolutePosition ());
      int* nbWalls = GetWalls( (Femtocell*) (src->GetCell()), (UserEquipment*) dst);

      double A, B, C;
      double ExternalWallsAttenuation = 20.0;
      double InternalWallsAttenuation = 10.0;

      if (nbWalls[0] == 0 && nbWalls[1] == 0)
        {
          //LOS
          A = 18.7;
          B = 46.8;
          C = 20.0;
        }
      else
        {
          //NLOS
          A = 20.0;
          B = 46.4;
          C = 20.0;
        }

      pathLoss = A * log10( distance ) +
                 B +
                 C * log10(2. / 5.0) +
                 InternalWallsAttenuation * nbWalls[1] +
                 ExternalWallsAttenuation * nbWalls[0];

      delete [] nbWalls;




      return max(minimumCouplingLoss, pathLoss);

    }



  return pathLoss;

  //XXX: add other path loss models.
}



#endif /* COMPUTE_PATH_LOSS_H_ */
