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



#ifndef CARTESIANCOORDINATES_H_
#define CARTESIANCOORDINATES_H_

#include <math.h>
#include "../../load-parameters.h"


class CartesianCoordinates
{
private:
  double m_posX;
  double m_posY;
  double m_posZ;
  double m_floorHeight; //height of a floor in a building

public:
  CartesianCoordinates();
  CartesianCoordinates(double x,
                       double y);
  CartesianCoordinates(double x,
                       double y,
                       double z);
  virtual ~CartesianCoordinates() = default;

  void
  SetCoordinates(double x, double y);
  void
  SetCoordinates(double x, double y, double z);
  void
  SetCoordinatesFromPolar(CartesianCoordinates *center,
                          double radius,
                          double angle);
  double
  GetDistance(CartesianCoordinates *remoteObject);
  double
  GetDistance3D(CartesianCoordinates *remoteObject);
  double
  GetDistance(double remoteObjectPosX,
              double remoteObjectPosY);
  double
  GetDistance3D(double remoteObjectPosX,
                double remoteObjectPosY,
                double remoteObjectPosZ);
  double
  GetPolarAzimut(CartesianCoordinates *center);
  double
  GetCoordinateX();
  double
  GetCoordinateY();
  double
  GetCoordinateZ();
  void
  SetCoordinateX(double X);
  void
  SetCoordinateY(double Y);
  void
  SetCoordinateZ(double Z);

  double GetFloorHeight(void);
  void SetFloorHeight(double height);

  int GetFloor(void);
  void SetFloor(int floor);

  CartesianCoordinates*
  operator + (CartesianCoordinates *A);
  CartesianCoordinates*
  operator - (CartesianCoordinates *B);
  bool
  operator == (CartesianCoordinates C);

  //Debug
  void
  Print();

} ;



#endif /* CARTESIANCOORDINATES_H_ */
