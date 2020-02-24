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
 * Author: Sergio Martiradonna <sergio.martiradonna@poliba.it>
 */

#ifndef NB_AMCModule_H_
#define NB_AMCModule_H_

/*
 *  Adaptive Modulation And Coding Scheme for NB-IoT
 */

class nbAMCModule {
public:
  nbAMCModule();
  virtual ~nbAMCModule();

  int
  GetTBSizeFromMCS (int mcs, int nbRUs);

  int
  GetNbOfRUsFromSize (int mcs, int bits);

  int
  GetNbOfMCSfromTones (int tones);

  int
  GetRUindex (int nru);

  int
  GetMCSfromDistance (double distance, double radius, int tones);
};

#endif /* NB_AMCModule_H_ */

