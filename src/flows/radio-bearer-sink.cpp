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
 * Author: Sergio Martiradonna <sergio.martiradonna@poliba.it>
 */

#include "radio-bearer-sink.h"
#include "radio-bearer.h"
#include "../device/NetworkNode.h"
#include "../device/IPClassifier/ClassifierParameters.h"
#include "application/application-sink.h"
#include "../protocolStack/rlc/rlc-entity.h"
#include "../protocolStack/rlc/tm-rlc-entity.h"
#include "../protocolStack/rlc/um-rlc-entity.h"
#include "../protocolStack/rlc/am-rlc-entity.h"
#include "../load-parameters.h"

RadioBearerSink::RadioBearerSink()
{
  SetClassifierParameters (nullptr);
  SetSource (nullptr);
  SetDestination (nullptr);

  //RlcEntity *rlc = new TmRlcEntity ();
  RlcEntity *rlc = new AmRlcEntity ();
  //RlcEntity *rlc = new UmRlcEntity ();

  rlc->SetRadioBearer (this);
  SetRlcEntity(rlc);
}

RadioBearerSink::RadioBearerSink(ApplicationSink* a)
{
  SetClassifierParameters (nullptr);
  SetSource (nullptr);
  SetDestination (nullptr);
  
  RlcEntity *rlc;
  switch (a->GetSourceApplication()->GetApplicationType()) {
    case Application::APPLICATION_TYPE_VOIP:
      rlc = new UmRlcEntity ();
      break;
    case Application::APPLICATION_TYPE_TRACE_BASED:
      rlc = new UmRlcEntity ();
      break;
    case Application::APPLICATION_TYPE_INFINITE_BUFFER:
      rlc = new UmRlcEntity ();
      break;
    case Application::APPLICATION_TYPE_CBR:
      rlc = new AmRlcEntity ();
      break;
    case Application::APPLICATION_TYPE_WEB:
      rlc = new AmRlcEntity ();
      break;
    case Application::APPLICATION_TYPE_FTP2:
      rlc = new AmRlcEntity ();
      break;
    case Application::APPLICATION_TYPE_EXTERNAL_SOURCE:
      rlc = new AmRlcEntity ();
      break;
    default:
      rlc = new UmRlcEntity ();
      break;
  }
  
  rlc->SetRadioBearer (this);
  SetRlcEntity(rlc);
  
  m_application = a;
}

RadioBearerSink::~RadioBearerSink()
{
  Destroy ();
}

void
RadioBearerSink::SetApplication (ApplicationSink* a)
{
  m_application = a;
}

ApplicationSink*
RadioBearerSink::GetApplication (void)
{
  return m_application;
}

void
RadioBearerSink::Receive (Packet* p)
{
  GetApplication ()->Receive (p);
}

