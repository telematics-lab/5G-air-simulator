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



#ifndef MULTICASTDESTINATION_H_
#define MULTICASTDESTINATION_H_

#include "UserEquipment.h"

class GNodeB;
class Gateway;
class CqiManager;


class MulticastDestination : public UserEquipment
{
public:
  MulticastDestination () = default;
  MulticastDestination (int idElement,
                        Cell *cell,
                        GNodeB* target);

  virtual ~MulticastDestination();

  void MakeActive (void);

  bool IsIndoor (void);

  //Debug
  void Print (void);

  void AddSource(GNodeB *n);
  void DeleteSource(GNodeB *n);
  vector<GNodeB*> GetSources();
  void SetSources(vector<GNodeB*> sources);

  void AddDestination(UserEquipment *n);
  void DeleteDestination(UserEquipment *n);
  vector<UserEquipment*> GetDestinations();
  void SetDestinations(vector<UserEquipment*> destinations);

  void SetMcs(int m_mcs);
  int GetMcs();

  NetworkNode::NodeState GetNodeState (void);

private:
  int m_mcs;
  vector<GNodeB*> m_sources;
  vector<UserEquipment*> m_destinations;

};

#endif /* MULTICASTDESTINATION_H_ */
