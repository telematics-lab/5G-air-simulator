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
 * Author: Francesco Capozzi <f.capozzi@poliba.it>
 */

#include "Building.h"
#include "Femtocell.h"
#include "../core/cartesianCoodrdinates/CartesianCoordinates.h"
#include "../device/UserEquipment.h"

Building::Building (int idBuilding,
                    int type,
                    int nbFloors,
                    double side_X,
                    double side_Y,
                    double center_X,
                    double center_Y)
{
  m_idBuilding = idBuilding;

  if ( type == 0)
    m_buildingType = TYPE_3GPP_5x5_grid;
  else
    m_buildingType = TYPE_3GPP_dual_stripe;


  m_nbFloors = nbFloors;

  m_side = new double[2];
  m_side [0] = side_X;
  m_side [1] = side_Y;

  CartesianCoordinates *Position = new CartesianCoordinates(center_X,center_Y);
  m_centerPosition = Position;

  m_femtoCellsInBuilding = new vector<Femtocell*>;
}

Building::~Building()
{
  delete m_femtoCellsInBuilding;
  delete m_centerPosition;
}

void
Building::SetIdBuilding (int id)
{
  m_idBuilding = id;
}

void
Building::SetBuildingType (BuildingType type)
{
  m_buildingType = type;
}

Building::BuildingType
Building::GetBuildingType ( void )
{
  return m_buildingType;
}

void
Building::SetFloors (int floors)
{
  m_nbFloors = floors;
}

void
Building::SetSide (double* side)
{
  m_side = side;
}

void
Building::SetCenterPosition (CartesianCoordinates* position)
{
  m_centerPosition = position;
}

void
Building::SetFemtoCellsInBuilding (vector<Femtocell*>* vector)
{
  m_femtoCellsInBuilding = vector;
}

void
Building::AddFemtoCell (Femtocell* femtocell)
{
  m_femtoCellsInBuilding->push_back(femtocell);
}


vector<Femtocell*>*
Building::GetFemtoCellsInBuilding (void)
{
  return m_femtoCellsInBuilding;
}

int
Building::GetIdBuilding (void)
{
  return m_idBuilding;
}

CartesianCoordinates*
Building::GetCenterPosition (void)
{
  return m_centerPosition;
}

double*
Building::GetSide (void)
{
  return m_side;
}

int
Building::GetFemtoIdInBuilding ( int cellID )
{
  int firstID = m_femtoCellsInBuilding->at(0)->GetIdCell();

  if ( 0 <= (cellID - firstID) && (cellID - firstID) < (int)m_femtoCellsInBuilding->size())
    return (cellID - firstID)%m_femtoCellsInBuilding->size();

  return -1;
}

bool
Building::IsInThisBuilding (UserEquipment* ue)
{
  if ( ( abs( this->GetCenterPosition()->GetCoordinateX()
              - ue->GetMobilityModel()->GetAbsolutePosition()->GetCoordinateX() ) <= this->GetSide()[0]/2 ) &&
       ( abs( this->GetCenterPosition()->GetCoordinateY()
              - ue->GetMobilityModel()->GetAbsolutePosition()->GetCoordinateY() ) <= this->GetSide()[1]/2 ) )
    {
      return true;
    }
  return false;
}

Femtocell*
Building::GetClosestFemtoCellToUE(UserEquipment* ue)
{
  Femtocell *femtocell;
  double distance = 99999999;

  for (auto cell : *m_femtoCellsInBuilding)
    {
      if (cell->GetCellCenterPosition()->GetDistance(ue->GetMobilityModel()->GetAbsolutePosition()) < distance)
        {
          distance = cell->GetCellCenterPosition()->GetDistance(ue->GetMobilityModel()->GetAbsolutePosition());
          femtocell = cell;
        }
    }
  return femtocell;
}

void
Building::ClearAll()
{
  delete m_femtoCellsInBuilding;
  delete m_centerPosition;
}


