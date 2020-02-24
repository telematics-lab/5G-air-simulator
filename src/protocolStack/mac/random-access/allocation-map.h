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


#ifndef ALLOCATION_MAP_H_
#define ALLOCATION_MAP_H_
#include <vector>
#include <map>
#include "../../../load-parameters.h"

class AllocationMap
{

public:
  AllocationMap();
  ~AllocationMap();
  void addElement(int tti, int rb);
  bool elementsExist(int tti);
  bool isAllocated(int tti, int rb);
  vector<int> getElements(int tti);
  void deleteElements(int tti);
  void deleteOldElements(int tti);

private:
  map< int, vector<int> > m_allocatedRBs; // key is TTI number, value is list of allocated RBs

};

#endif /* ALLOCATION_MAP_H_ */
