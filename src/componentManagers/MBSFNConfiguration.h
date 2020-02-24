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
 * Author: Alessandro Grassi <alessandro.grassi@poliba.it>
 */



#ifndef MBSFNCONFIGURATION_H_
#define MBSFNCONFIGURATION_H_

enum MbsfnFrameType
{
  NON_MBSFN_SUBFRAME, // code 0
  MBSFN_SUBFRAME      // code 1
};

static int MBSFNConfiguration[7][10] =
{
  {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0  // no MBSFN subframes
  },

  {
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0  // 1 MBSFN subframe
  },

  {
    0, 0, 0, 1, 0, 0, 0, 0, 1, 0  // 2 MBSFN subframes
  },

  {
    0, 0, 0, 1, 0, 0, 1, 0, 1, 0  // 3 MBSFN subframes
  },

  {
    0, 1, 1, 1, 0, 0, 1, 0, 0, 0  // 4 MBSFN subframes
  },

  {
    0, 1, 0, 1, 0, 0, 1, 1, 1, 0  // 5 MBSFN subframes
  },

  {
    0, 1, 1, 1, 0, 0, 1, 1, 1, 0  // 6 MBSFN subframes
  },
};

#endif /* MBSFNCONFIGURATION_H_ */
