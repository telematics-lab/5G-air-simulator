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


#ifndef CHANNEL_REALIZATION_HELPER_H_
#define CHANNEL_REALIZATION_HELPER_H_

#include "channel-realization.h"

static ChannelRealization* CreateChannelRealization (NetworkNode* src, NetworkNode* dst)
{
  if (src->GetNodeType()==NetworkNode::TYPE_GNODEB ||
      dst->GetNodeType()==NetworkNode::TYPE_GNODEB)
    {
      ChannelRealization* c =
        new ChannelRealization (src, dst, ChannelRealization::CHANNEL_MODEL_MACROCELL_URBAN_IMT);
      return c;
    }
  else if (src->GetNodeType()==NetworkNode::TYPE_HOME_BASE_STATION
           || dst->GetNodeType()==NetworkNode::TYPE_HOME_BASE_STATION)
    {

      //ChannelRealization* c =
      //          new ChannelRealization (src, dst, ChannelRealization::CHANNEL_MODEL_FEMTOCELL_URBAN);

      ChannelRealization* c =
        new ChannelRealization (src, dst, ChannelRealization::CHANNEL_MODEL_WINNER_DOWNLINK);
      return c;
    }
  else
    {
      ChannelRealization* c =
        new ChannelRealization (src, dst, ChannelRealization::CHANNEL_MODEL_MACROCELL_URBAN);
      return c;
    }

}

#endif /* CHANNEL_REALIZATION_HELPER_H_ */
