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

#ifndef HENODEB_H_
#define HENODEB_H_

#include "GNodeB.h"


class HeNodeB : public GNodeB
{
public:

  HeNodeB (int idElement,
           Femtocell *cell);

  virtual ~HeNodeB();

  void
  SetFemtoCell (Femtocell *cell);
  Femtocell*
  GetFemtoCell (void);
  HenbMacEntity* GetMacEntity(void) const;

private:

  Femtocell* m_femtocell;
};


#endif /* HENODEB_H_ */
