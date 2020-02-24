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

#ifndef STREET_H_
#define STREET_H_

#include <vector>
#include "../load-parameters.h"


class Building;
class CartesianCoordinates;


class Street
{
public:

  Street ( int streetID,
           CartesianCoordinates* center,
           double theta,
           int nbBuilding,
           int building_type,
           double street_width,
           double building_distance,
           double apartmentSide);

  virtual ~Street();

  void
  AddBuilding (Building* building);

  vector<CartesianCoordinates*>*
  GetBuildingDistributionInStreet ( void );

private:

  int m_streetID;

  CartesianCoordinates* m_centerPosition;
  double m_thetaStreet; // angle theta for street inclination in the scenario

  int m_nbBuildings; // number of Buildings on one side of the street
  double m_streetWidth;
  double m_buildingDistance;
  int m_buildingTypeInStreet;
  double m_apartmentSide;

  vector<Building*>* m_buildingsInStreet;



};

#endif /* STREET_H_ */
