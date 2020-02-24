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

#include "radio-bearer-instance.h"
#include "../device/NetworkNode.h"
#include "../device/IPClassifier/ClassifierParameters.h"
#include "application/Application.h"
#include "../protocolStack/packet/Packet.h"
#include "../load-parameters.h"
#include "../flows/MacQueue.h"
#include "../flows/QoS/QoSParameters.h"
#include "../flows/QoS/QoSForEXP.h"
#include "../flows/QoS/QoSForFLS.h"
#include "../flows/QoS/QoSForM_LWDF.h"
#include "../protocolStack/rlc/rlc-entity.h"
#include "../protocolStack/rlc/tm-rlc-entity.h"
#include "../protocolStack/rlc/um-rlc-entity.h"
#include "../protocolStack/rlc/am-rlc-entity.h"


RadioBearerInstance::RadioBearerInstance()
{
  m_src = nullptr;
  m_dst = nullptr;
  m_classifierParameters = nullptr;
  m_qosParameters = nullptr;
  m_rlc = nullptr;
}

RadioBearerInstance::~RadioBearerInstance()
{
  Destroy ();
}

void
RadioBearerInstance::Destroy (void)
{
  delete m_rlc;
}

void
RadioBearerInstance::SetClassifierParameters (ClassifierParameters* cp)
{
  m_classifierParameters = cp;
}

ClassifierParameters*
RadioBearerInstance::GetClassifierParameters (void)
{
  return m_classifierParameters;
}

void
RadioBearerInstance::SetSource (NetworkNode* src)
{
  m_src = src;
}

void
RadioBearerInstance::SetDestination (NetworkNode* dst)
{
  m_dst = dst;
}

NetworkNode*
RadioBearerInstance::GetSource (void)
{
  return m_src;
}

NetworkNode*
RadioBearerInstance::GetDestination (void)
{
  return m_dst;
}

void
RadioBearerInstance::SetQoSParameters (QoSParameters *parameters)
{
  m_qosParameters = parameters;
}

QoSParameters*
RadioBearerInstance::GetQoSParameters (void) const
{
  return m_qosParameters;
}

void
RadioBearerInstance::SetRlcEntity (RlcEntity *rlc)
{
  m_rlc = rlc;
}

RlcEntity*
RadioBearerInstance::GetRlcEntity (void)
{
  return m_rlc;
}

