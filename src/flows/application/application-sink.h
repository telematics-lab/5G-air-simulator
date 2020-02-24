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
 */


#ifndef APPLICATIONSINK_H_
#define APPLICATIONSINK_H_

#include <iostream>
#include <stdint.h>
#include "Application.h"

class ClassifierParameters;
class RadioBearer;
class RadioBearerSink;
class Packet;


class ApplicationSink
{
public:
  ApplicationSink();
  virtual ~ApplicationSink() = default;

  void SetClassifierParameters (ClassifierParameters* cp);
  ClassifierParameters* GetClassifierParameters (void);

  void SetRadioBearerSink (RadioBearerSink* r);
  RadioBearerSink* GetRadioBearerSink (void);

  void SetSourceApplication (Application* a);
  Application* GetSourceApplication (void);

  void Receive (Packet* p);

private:
  ClassifierParameters* m_classifierParameters;
  RadioBearerSink* m_radioBearer;

  Application* m_sourceApplication;
};


#endif /* APPLICATIONSINK_H_ */
