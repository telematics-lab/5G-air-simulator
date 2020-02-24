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



#ifndef FRAMEMANAGER_H_
#define FRAMEMANAGER_H_

#include <iostream>
#include "../load-parameters.h"
#include "NetworkManager.h"
#include "../core/eventScheduler/simulator.h"
#include "TDDConfiguration.h"
#include "MBSFNConfiguration.h"


/*
  * Frame Structure:
  *
  *  ** Frame structure type 1 (FS1): FDD
  *    ...
  *  ** Frame structure type 2 (FS2): TDD
  *    ...
  */


class FrameManager
{
public:
  enum FrameStructure
  {
    FRAME_STRUCTURE_FDD,
    FRAME_STRUCTURE_TDD
  };
private:
  FrameStructure m_frameStructure;

  int m_TDDFrameConfiguration;
  int m_MBSFNPattern;

  int m_nbFrames;
  int m_nbSubframes;
  unsigned long m_TTICounter;
  int m_TTILength;

  FrameManager();
  static FrameManager *ptr;

public:
  virtual ~FrameManager() = default;

  static FrameManager*
  Init (void)
  {
    if (ptr==nullptr)
      {
        ptr = new FrameManager;
      }
    return ptr;
  }

  void
  SetFrameStructure (FrameStructure frameStructure);
  FrameStructure
  GetFrameStructure (void) const;

  void
  SetTDDFrameConfiguration (int configuration);
  int
  GetTDDFrameConfiguration (void) const;

  void
  SetMbsfnPattern(int pattern);
  int
  GetMbsfnPattern(void) const;
  bool
  MbsfnEnabled(void);
  bool
  isMbsfnSubframe(void);

  int
  GetSubFrameType (int nbSubFrame);

  void
  UpdateNbFrames (void);
  int
  GetNbFrames (void) const;
  void
  UpdateNbSubframes (void);
  void
  ResetNbSubframes (void);
  int
  GetNbSubframes (void) const;
  void
  UpdateTTIcounter (void);
  unsigned long
  GetTTICounter (void) const;

  void
  Start (void);
  void
  StartFrame (void);
  void
  StopFrame (void);
  void
  StartSubframe (void);
  void
  StopSubframe (void);

  int GetCoverShiftIndex(void);

  NetworkManager* GetNetworkManager (void);

  void ResourceAllocation(void);
  void setTTILength (int tones, double spacing);
  int getTTILength (void) const;
};

#endif /* FRAMEMANAGER_H_ */
