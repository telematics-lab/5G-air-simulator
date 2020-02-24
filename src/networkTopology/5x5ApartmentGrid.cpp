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

#include "5x5ApartmentGrid.h"
#include "../core/cartesianCoodrdinates/CartesianCoordinates.h"

ApartmentGrid::ApartmentGrid (int idBuilding,
                              int type,
                              int nbFloors,
                              double side_X,
                              double side_Y,
                              double center_X,
                              double center_Y)
{
  SetIdBuilding(idBuilding);

  SetBuildingType(TYPE_3GPP_5x5_grid);

  SetFloors(nbFloors);

  double* side = new double[2];
  side [0] = side_X;
  side [1] = side_Y;
  SetSide(side);

  CartesianCoordinates *Position = new CartesianCoordinates(center_X,center_Y);
  SetCenterPosition(Position);

  vector<Femtocell*>* femtoCellsInBuilding = new vector<Femtocell*>;
  SetFemtoCellsInBuilding(femtoCellsInBuilding);
}

ApartmentGrid::~ApartmentGrid()
{
  ClearAll();
}
