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
 * Author: Francesco Capozzi <f.capozzi@poliba.it>
 */

#ifndef BUILDING_H_
#define BUILDING_H_

#include <vector>
#include "../load-parameters.h"

class CartesianCoordinates;
class Femtocell;
class UserEquipment;

class Building
{
public:

  enum BuildingType
  {
    TYPE_3GPP_5x5_grid,
    TYPE_3GPP_dual_stripe
  };

  Building () = default;
  Building(int idBuilding,
           int type,
           int nbFloors,
           double side_X,
           double side_Y,
           double center_X,
           double center_Y);

  virtual ~Building();

  void
  SetIdBuilding (int id);
  void
  SetBuildingType (BuildingType type);
  Building::BuildingType
  GetBuildingType ( void );
  void
  SetFloors (int floors);
  void
  SetSide (double* side);
  void
  SetCenterPosition (CartesianCoordinates* position);
  void
  SetFemtoCellsInBuilding (vector<Femtocell*>*);
  void
  AddFemtoCell (Femtocell* femtocell);

  int
  GetIdBuilding (void);
  vector<Femtocell*>*
  GetFemtoCellsInBuilding (void);
  CartesianCoordinates*
  GetCenterPosition (void);
  double*
  GetSide (void);

  int
  GetFemtoIdInBuilding (int cellID);
  bool
  IsInThisBuilding (UserEquipment* ue);
  Femtocell*
  GetClosestFemtoCellToUE(UserEquipment* ue);

  void
  ClearAll();

private:

  int m_idBuilding;
  BuildingType m_buildingType;
  int m_nbFloors;
  double* m_side;
  CartesianCoordinates* m_centerPosition;

  vector<Femtocell*> *m_femtoCellsInBuilding;
};


#endif /* BUILDING_H_ */
