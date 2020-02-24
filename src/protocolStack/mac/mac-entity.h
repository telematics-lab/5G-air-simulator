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


#ifndef MAC_ENTITY_H
#define MAC_ENTITY_H

#include <list>
#include "../../load-parameters.h"
#include "../../core/idealMessages/ideal-control-messages.h"

class HarqManager;
class Packet;
class NetworkNode;
class AMCModule;
class nbAMCModule;


/*
 * This class provides a basic implementation of the MAC
 * layer
 */
class MacEntity
{
public:

  MacEntity (void) = default;
  virtual ~MacEntity (void);

  void Destroy (void);

  void SetDevice (NetworkNode* d);
  NetworkNode* GetDevice ();

  void SetAmcModule (AMCModule* amcModule);
  AMCModule* GetAmcModule (void) const;

  void SetNbAmcModule (nbAMCModule* nbAmcModule);
  nbAMCModule* GetNbAmcModule (void) const;

private:
  NetworkNode* m_device;
  AMCModule* m_amcModule;

  nbAMCModule* m_nbAmcModule;

};


#endif /* MAC_ENTITY_H */
