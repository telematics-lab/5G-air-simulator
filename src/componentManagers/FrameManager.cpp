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



#include "FrameManager.h"
#include "../load-parameters.h"
#include "../device/GNodeB.h"
#include "../device/HeNodeB.h"

FrameManager* FrameManager::ptr=nullptr;

FrameManager::FrameManager()
{
  m_nbFrames = 0;
  m_nbSubframes = 0;
  m_TTICounter = 0;
  m_frameStructure = FrameManager::FRAME_STRUCTURE_FDD; //Default Value
  m_TDDFrameConfiguration = 1; //Default Value
  m_MBSFNPattern = 0; //No MBSFN by default
  Simulator::Init()->Schedule(0.0, &FrameManager::Start, this);
}

void
FrameManager::SetFrameStructure (FrameManager::FrameStructure frameStructure)
{
  m_frameStructure = frameStructure;
}

FrameManager::FrameStructure
FrameManager::GetFrameStructure (void) const
{
  return m_frameStructure;
}


void
FrameManager::SetTDDFrameConfiguration (int configuration)
{
  if (configuration < 0 && configuration > 6)
    {
      m_TDDFrameConfiguration = 0; //Default Value
    }
  else
    {
      m_TDDFrameConfiguration = configuration;
    }
}

int
FrameManager::GetTDDFrameConfiguration (void) const
{
  return m_TDDFrameConfiguration;
}

void
FrameManager::SetMbsfnPattern(int pattern)
{
  if (pattern < 0 || pattern >= (int)std::extent <decltype( MBSFNConfiguration )> ::value)
    {
      cout << "Invalid MBSFN pattern index: " << pattern << endl;
    }
  else
    {
      m_MBSFNPattern = pattern;
    }
}

int
FrameManager::GetMbsfnPattern(void) const
{
  return m_MBSFNPattern;
}

bool
FrameManager::MbsfnEnabled(void)
{
  if (m_MBSFNPattern!=0)
    {
      return true;
    }
  return false;
}

bool
FrameManager::isMbsfnSubframe(void)
{
  if (MBSFNConfiguration[ m_MBSFNPattern ][ GetNbSubframes()-1 ] == MbsfnFrameType::MBSFN_SUBFRAME)
    {
      return true;
    }
  return false;
}


int
FrameManager::GetSubFrameType (int nbSubFrame)
{
  return TDDConfiguration [GetTDDFrameConfiguration ()][nbSubFrame-1];
}


void
FrameManager::UpdateNbFrames (void)
{
  m_nbFrames++;
}

int
FrameManager::GetNbFrames (void) const
{
  return m_nbFrames;
}

void
FrameManager::UpdateNbSubframes (void)
{
  m_nbSubframes++;
}

void
FrameManager::ResetNbSubframes (void)
{
  m_nbSubframes = 0;
}

int
FrameManager::GetNbSubframes (void) const
{
  return m_nbSubframes;
}

void
FrameManager::UpdateTTIcounter (void)
{
  m_TTICounter++;
}

unsigned long
FrameManager::GetTTICounter () const
{
  return m_TTICounter;
}

void
FrameManager::Start (void)
{
DEBUG_LOG_START_1(SIM_ENV_FRAME_MANAGER_DEBUG)
  cout << " Simulation starts now! "<< endl;
DEBUG_LOG_END

  Simulator::Init()->Schedule(0.0, &FrameManager::StartFrame, this);
}

void
FrameManager::StartFrame (void)
{
  UpdateNbFrames ();

DEBUG_LOG_START_1(SIM_ENV_FRAME_MANAGER_DEBUG)
  cout << " +++++++++ Start Frame, time =  "
            << Simulator::Init()->Now() << " +++++++++" << endl;
DEBUG_LOG_END

  Simulator::Init()->Schedule(0.0,
                              &FrameManager::StartSubframe,
                              this);
}

void
FrameManager::StopFrame (void)
{
  Simulator::Init()->Schedule(0.0,
                              &FrameManager::StartFrame,
                              this);
}

void
FrameManager::StartSubframe (void)
{
DEBUG_LOG_START_2(SIM_ENV_FRAME_MANAGER_DEBUG,SIM_ENV_START_SUBFRAME_DEBUG)
  cout << " --------- Start SubFrame, time =  "
            << Simulator::Init()->Now() << " --------- " << endl;
DEBUG_LOG_END

  UpdateTTIcounter ();
  if (GetNbSubframes () == 10)
    {
      ResetNbSubframes ();
    }
  UpdateNbSubframes ();

  /*
   * At the beginning of each sub-frame the simulation
   * update user position. This function could be
   * implemented also every TTI.
   */
  //UpdateUserPosition(); --> moved to each UE class


DEBUG_LOG_START_1(SIM_ENV_PLOT_USER_POSITION)
  NetworkManager::Init()->PrintUserPosition();
DEBUG_LOG_END

  /*
   * According to the Frame Structure, the DW/UL link scheduler
   * will be called for each sub-frame.
   * (RBs allocation)
   */
  ResourceAllocation();
  Simulator::Init()->Schedule(0.001,
                              &FrameManager::StopSubframe,
                              this);
}

void
FrameManager::StopSubframe (void)
{
  if (GetNbSubframes () == 10)
    {
      Simulator::Init()->Schedule(0.0,
                                  &FrameManager::StopFrame,
                                  this);
    }
  else
    {
      Simulator::Init()->Schedule(0.0,
                                  &FrameManager::StartSubframe,
                                  this);
    }
}


NetworkManager*
FrameManager::GetNetworkManager (void)
{
  return NetworkManager::Init();
}


void
FrameManager::ResourceAllocation(void)
{
  vector<GNodeB*> *records = GetNetworkManager ()->GetGNodeBContainer ();
  for (auto record : *records)
    {

DEBUG_LOG_START_1(SIM_ENV_FRAME_MANAGER_DEBUG)
      cout << " FRAME_MANAGER_DEBUG: RBs allocation for gNB " <<
                record->GetIDNetworkNode() << endl;
DEBUG_LOG_END


      if (GetFrameStructure () == FrameManager::FRAME_STRUCTURE_FDD)
        {
          //record->ResourceBlocksAllocation ();
          Simulator::Init()->Schedule(0.0, &GNodeB::ResourceBlocksAllocation,record);
        }
      else
        {
          //see frame configuration in TDDConfiguration
          if (GetSubFrameType (GetNbSubframes ()) == 0)
            {
DEBUG_LOG_START_1(SIM_ENV_FRAME_MANAGER_DEBUG)
              cout << " FRAME_MANAGER_DEBUG: SubFrameType = SUBFRAME_FOR_DOWNLINK " << endl;
DEBUG_LOG_END
              //record->DownlinkResourceBlockAllocation();
              Simulator::Init()->Schedule(0.0, &GNodeB::DownlinkResourceBlockAllocation,record);
            }
          else if(GetSubFrameType (GetNbSubframes ()) == 1)
            {
DEBUG_LOG_START_1(SIM_ENV_FRAME_MANAGER_DEBUG)
              cout << " FRAME_MANAGER_DEBUG: SubFrameType = SUBFRAME_FOR_UPLINK " << endl;
DEBUG_LOG_END
              //record->UplinkResourceBlockAllocation();
              Simulator::Init()->Schedule(0.0, &GNodeB::UplinkResourceBlockAllocation,record);
            }
          else
            {
DEBUG_LOG_START_1(SIM_ENV_FRAME_MANAGER_DEBUG)
              cout << " FRAME_MANAGER_DEBUG: SubFrameType = SPECIAL_SUBFRAME" << endl;
DEBUG_LOG_END
            }
        }
    }

  vector<HeNodeB*> *records_2 = GetNetworkManager ()->GetHomeGNodeBContainer();
  for (auto record_2 : *records_2)
    {
DEBUG_LOG_START_1(SIM_ENV_FRAME_MANAGER_DEBUG)
      cout << " FRAME_MANAGER_DEBUG: RBs allocation for gNB " <<
                record_2->GetIDNetworkNode() << endl;
DEBUG_LOG_END


      if (GetFrameStructure () == FrameManager::FRAME_STRUCTURE_FDD)
        {
          //record_2->ResourceBlocksAllocation ();
          Simulator::Init()->Schedule(0.0, &GNodeB::ResourceBlocksAllocation,record_2);
        }
      else
        {
          //see frame configuration in TDDConfiguration
          if (GetSubFrameType (GetNbSubframes ()) == 0)
            {
DEBUG_LOG_START_1(SIM_ENV_FRAME_MANAGER_DEBUG)
              cout << " FRAME_MANAGER_DEBUG: SubFrameType = SUBFRAME_FOR_DOWNLINK " << endl;
DEBUG_LOG_END
              //record_2->DownlinkResourceBlockAllocation();
              Simulator::Init()->Schedule(0.0, &GNodeB::DownlinkResourceBlockAllocation,record_2);
            }
          else if(GetSubFrameType (GetNbSubframes ()) == 1)
            {
DEBUG_LOG_START_1(SIM_ENV_FRAME_MANAGER_DEBUG)
              cout << " FRAME_MANAGER_DEBUG: SubFrameType = SUBFRAME_FOR_UPLINK " << endl;
DEBUG_LOG_END
              //record_2->UplinkResourceBlockAllocation();
              Simulator::Init()->Schedule(0.0, &GNodeB::UplinkResourceBlockAllocation,record_2);
            }
          else
            {
DEBUG_LOG_START_1(SIM_ENV_FRAME_MANAGER_DEBUG)
              cout << " FRAME_MANAGER_DEBUG: SubFrameType = SPECIAL_SUBFRAME" << endl;
DEBUG_LOG_END
            }
        }
    }
}



void
FrameManager::setTTILength(int tones, double spacing)
{
   if (spacing == 3.75)
    {
      m_TTILength=32;
    }
  else
    {
      switch (tones)
        {
        case 1:
          m_TTILength=8;
          break;

        case 3:
          m_TTILength=4;
          break;

        case 6:
          m_TTILength=2;
          break;

        case 12:
          m_TTILength=1;
          break;

        default:
          cout << "error FrameManager::setTTILength :  unknown RU configuration" << endl;
          exit(1);
        }
    }
}

int
FrameManager::getTTILength() const
{
  return m_TTILength;
}


int
FrameManager::GetCoverShiftIndex(void)
{
  // Assuming 3 cover shift with periodic repetition in time domain
  int currentTTI = GetNbFrames() * 10 + GetNbSubframes();
  int coverShiftPeriod = 330;
  int index = (currentTTI/coverShiftPeriod) % 3;
  if (std::getenv("FORCE_COVER") != nullptr)
    {
      index = atoi(std::getenv("FORCE_COVER"));
    }
  return index;
}
