/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Communications Research Laboratory, Ilmenau University of Technology
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
 * Author: Florian Schlembach <florian.schlembach@tu-ilmenau.de>
 * Author: Jianshu Zhang <jianshu.zhang@tu-ilmenau.de>
 * Author: Yao Cheng <y.cheng@tu-ilmenau.de>
 */

#ifndef MANHATTAN_H_
#define MANHATTAN_H_

#include "Mobility.h"

class Manhattan :public Mobility
{
public:
  Manhattan();
  virtual ~Manhattan();

  void
  UpdatePosition (double time);

  void
  ForceManhattanGrid (CartesianCoordinates *newpos);

};

#endif /* MANHATTAN_H_ */
