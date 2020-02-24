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

#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include "allocation-map.h"

AllocationMap::AllocationMap()
{
  m_allocatedRBs.clear();
}


AllocationMap::~AllocationMap()
{
}


void
AllocationMap::addElement(int tti, int rb)
{
  if(m_allocatedRBs.count(tti)==1)
    {
      m_allocatedRBs.at(tti).push_back(rb);
    }
  else
    {
      vector<int> elements = {rb};
      m_allocatedRBs.insert( pair< int, vector<int> >(tti,elements) );
    }
}


bool
AllocationMap::elementsExist(int tti)
{
  if (m_allocatedRBs.count(tti) == 1)
    {
      return true;
    }
  else
    {
      return false;
    }
}


bool
AllocationMap::isAllocated(int tti, int rb)
{
  if (elementsExist(tti) )
    {
      vector<int> elements = getElements(tti);
      if (find(elements.begin(),elements.end(),rb)!=elements.end())
        {
          return true;
        }
      else
        {
          return false;
        }
    }
  else
    {
      return false;
    }
}


vector<int>
AllocationMap::getElements(int tti)
{
  vector<int> result;
  if(m_allocatedRBs.count(tti)==1)
    {
      result = m_allocatedRBs.at(tti);
    }
  return result;
}


void
AllocationMap::deleteElements(int tti)
{
  m_allocatedRBs.erase(tti);
}


void
AllocationMap::deleteOldElements(int tti)
{
  if (m_allocatedRBs.empty()==false)
    {
      map< int, vector<int> >::iterator it = m_allocatedRBs.begin();
      while (it != m_allocatedRBs.end())
        {
          if ((*it).first<tti)
            {
              it = m_allocatedRBs.erase(it);
            }
          else
            {
              ++it;
            }
        }
    }
}
