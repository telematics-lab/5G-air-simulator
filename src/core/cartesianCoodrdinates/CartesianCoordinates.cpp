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



#include <stdint.h>
#include <iostream>
#include "CartesianCoordinates.h"

CartesianCoordinates::CartesianCoordinates()
{
  SetFloorHeight(3);
}

CartesianCoordinates::CartesianCoordinates(double x, double y) : CartesianCoordinates::CartesianCoordinates(x, y, 0.0)
{
}

CartesianCoordinates::CartesianCoordinates(double x, double y, double z) : CartesianCoordinates::CartesianCoordinates()
{
  SetCoordinates(x,y,z);
}

void
CartesianCoordinates::SetCoordinates (double x,
                                      double y)
{
  m_posX = x;
  m_posY = y;
}

void
CartesianCoordinates::SetCoordinates (double x,
                                      double y,
                                      double z)
{
  m_posX = x;
  m_posY = y;
  m_posZ = z;
}

void
CartesianCoordinates::SetCoordinatesFromPolar(CartesianCoordinates *center,
    double radius,
    double angle)
{
  m_posX = radius * sin (angle) + center->GetCoordinateX();
  m_posY = radius * cos (angle) + center->GetCoordinateY();
}

double
CartesianCoordinates::GetDistance(CartesianCoordinates *remoteObject)
{
  /*
   * Computes distance from this object and remoteObject
   */
  return
    sqrt (pow ((GetCoordinateX() - remoteObject->GetCoordinateX()),2) +
          pow ( (GetCoordinateY() - remoteObject->GetCoordinateY()),2));

}

double
CartesianCoordinates::GetDistance3D(CartesianCoordinates *remoteObject)
{
  return sqrt ( pow ( (GetCoordinateX() - remoteObject->GetCoordinateX()), 2) +
                pow ( (GetCoordinateY() - remoteObject->GetCoordinateY()), 2) +
                pow ( (GetCoordinateZ() - remoteObject->GetCoordinateZ()), 2) );
}

double
CartesianCoordinates::GetDistance(double remoteObjectPosX,
                                  double remoteObjectPosY)
{
  /*
   * Computes distance from this object and remoteObject
   */
  return
    sqrt (pow ((GetCoordinateX() - remoteObjectPosX),2) +
          pow ((GetCoordinateY() - remoteObjectPosY),2));
}

double
CartesianCoordinates::GetDistance3D(double remoteObjectPosX,
                                    double remoteObjectPosY,
                                    double remoteObjectPosZ)
{
  return
    sqrt ( pow ((GetCoordinateX() - remoteObjectPosX), 2) +
           pow ((GetCoordinateY() - remoteObjectPosY), 2) +
           pow ((GetCoordinateZ() - remoteObjectPosZ), 2) );
}


double
CartesianCoordinates::GetPolarAzimut(CartesianCoordinates *center)
{
  double X = GetCoordinateX() - center->GetCoordinateX();
  double Y = GetCoordinateY() - center->GetCoordinateY();
  double angle = atan2(Y, X);
  // azimut € [0,2*M_PI[
  if ( angle < 0 )
    {
      angle = angle + (2*M_PI);
    }
  return angle;
}

double
CartesianCoordinates::GetCoordinateX()
{
  return m_posX;
}

double
CartesianCoordinates::GetCoordinateY()
{
  return m_posY;
}

double
CartesianCoordinates::GetCoordinateZ()
{
  return m_posZ;
}

void
CartesianCoordinates::SetCoordinateX(double X)
{
  m_posX = (double) X;
}

void
CartesianCoordinates::SetCoordinateY(double Y)
{
  m_posY = (double) Y;
}

void
CartesianCoordinates::SetCoordinateZ(double Z)
{
  m_posZ = (double) Z;
}

double
CartesianCoordinates::GetFloorHeight(void)
{
  return m_floorHeight;
}

void
CartesianCoordinates::SetFloorHeight(double height)
{
  m_floorHeight = height;
}

int
CartesianCoordinates::GetFloor(void)
{
  int floor;
  if (m_posZ <= 1.5)
    {
      floor = 0;
    }
  else
    {
      floor = (m_posZ - 1.5) / m_floorHeight;
    }
  return floor;
}

void
CartesianCoordinates::SetFloor(int floor)
{
  m_posZ = floor * GetFloorHeight () + 1.5;
}


CartesianCoordinates*
CartesianCoordinates::operator+(CartesianCoordinates *A)
{
  CartesianCoordinates *Result =
    new CartesianCoordinates ((GetCoordinateX() + A->GetCoordinateX()),
                              (GetCoordinateY() + A->GetCoordinateY()));
  return Result;
}

CartesianCoordinates*
CartesianCoordinates::operator-(CartesianCoordinates *B)
{
  CartesianCoordinates *Result =
    new CartesianCoordinates ((GetCoordinateX() - B->GetCoordinateX()),
                              (GetCoordinateY() - B->GetCoordinateY()));
  return Result;
}

bool
CartesianCoordinates::operator == (CartesianCoordinates C)
{
  if (GetCoordinateX() == C.GetCoordinateX() &&
      GetCoordinateY() == C.GetCoordinateY() &&
      GetCoordinateZ() == C.GetCoordinateZ() )
    {
      return true;
    }
  else
    {
      return false;
    }
}
//Debug
void
CartesianCoordinates::Print()
{
  cout << "\t CenterCellPosition: "
            "x = " << GetCoordinateX() <<
            ", y = " << GetCoordinateY()
            << endl;
}
