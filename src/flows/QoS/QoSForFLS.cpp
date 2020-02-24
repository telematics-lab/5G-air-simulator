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


#include "QoSForFLS.h"
#include <iostream>
#include <stdint.h>

QoSForFLS::QoSForFLS()
{
  m_nbOfCoefficients = 5;
  CreateQ ();
  CreateU ();
  CreateFilterCoefficients ();
  m_dataToTransmit = 0;
}

QoSForFLS::~QoSForFLS()
{
  delete[] m_q;
  delete[] m_u;
  delete[] m_filterCoefficients;
}

void
QoSForFLS::CreateQ (void)
{
  m_q = new int[m_nbOfCoefficients-1];
  for (int i = 0; i < m_nbOfCoefficients-1; i++)
    {
      m_q[i] = 0;
    }
}

void
QoSForFLS::CreateU (void)
{
  m_u = new int[m_nbOfCoefficients-1];
  for (int i = 0; i < m_nbOfCoefficients-1; i++)
    {
      m_u[i] = 0;
    }
}

void
QoSForFLS::CreateFilterCoefficients (void)
{
  m_filterCoefficients = new double[m_nbOfCoefficients+1];

  double delta = (double)(1.0/m_nbOfCoefficients);

  m_filterCoefficients[0] = 0;
  m_filterCoefficients[1] = 1;

  for (int i = 2; i < m_nbOfCoefficients+1; i++)
    {
      m_filterCoefficients[i] =
        m_filterCoefficients[i-1]/2;
    }

DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
//  cout << " CreateFilterCoefficients for Bearer " << GetBearerID () << endl;
//  for (int i = 0; i < m_nbOfCoefficients+1; i++)
//    {
//      cout << "\t c[" << i << "] = " << m_filterCoefficients[i] << endl;
//    }
DEBUG_LOG_END
}


void
QoSForFLS::SetNbOfCoefficients(int M)
{
  m_nbOfCoefficients = M;
  CreateQ ();
  CreateU ();
  CreateFilterCoefficients ();
}

int
QoSForFLS::GetNbOfCoefficients (void) const
{
  return m_nbOfCoefficients;
}

int*
QoSForFLS::GetQ (void)
{
  return m_q;
}

int*
QoSForFLS::GetU (void)
{
  return m_u;
}

double*
QoSForFLS::GetFilterCoefficients (void) const
{
  return m_filterCoefficients;
}

void
QoSForFLS::UpdateQ (int q_)
{
DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
//  cout << " BEFORE UpdateQ for Bearer " << GetBearerID () << endl;
//  for (int i = 0; i < m_nbOfCoefficients-1; i++)
//    {
//      cout << "\t q[" << i << "] = " << m_q[i] << endl;
//    }
DEBUG_LOG_END

  for (int i = m_nbOfCoefficients-2; i > 0; i--)
    {
      m_q[i] = m_q[i-1];
    }
  m_q[0] = q_;

DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
//  cout << " AFTER UpdateQ for Bearer " << GetBearerID () << endl;
//  for (int i = 0; i < m_nbOfCoefficients-1; i++)
//    {
//      cout << "\t q[" << i << "] = " << m_q[i] << endl;
//    }
DEBUG_LOG_END
}

void
QoSForFLS::UpdateU (int u_)
{
DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
//  cout << " BEFORE UpdateU for Bearer " << GetBearerID () << endl;
//  for (int i = 0; i < m_nbOfCoefficients-1; i++)
//    {
//      cout << "\t u[" << i << "] = " << m_u[i] << endl;
//    }
DEBUG_LOG_END

  for (int i = m_nbOfCoefficients-2; i > 0; i--)
    {
      m_u[i] = m_u[i-1];
    }
  m_u[0] = u_;

DEBUG_LOG_START_1(SIM_ENV_SCHEDULER_DEBUG)
//  cout << " AFTER UpdateU for Bearer " << GetBearerID () << endl;
//  for (int i = 0; i < m_nbOfCoefficients-1; i++)
//    {
//      cout << "\t u[" << i << "] = " << m_u[i] << endl;
//    }
DEBUG_LOG_END

}

void
QoSForFLS::SetDataToTransmit (int data)
{
  m_dataToTransmit = data;
}

int
QoSForFLS::GetDataToTransmit (void) const
{
  return m_dataToTransmit;
}

void
QoSForFLS::UpdateDataToTransmit (int data)
{
  m_dataToTransmit -= data;
  if (m_dataToTransmit < 0)
    {
      m_dataToTransmit = 0;
    }
}

//Debug
void
QoSForFLS::Print ()
{
  cout << "\t QoSForFLS parameters " //for FLOW " << GetBearerID () <<
            ":\n\t M = " << m_nbOfCoefficients << endl;

  for (int i = 0; i < m_nbOfCoefficients+1; i++)
    {
      cout << "\t c[" << i << "] = " << m_filterCoefficients[i] << endl;
    }
  for (int i = 0; i < m_nbOfCoefficients-1; i++)
    {
      cout << "\t\t q[" << i << "] = " << m_q[i] << endl;
    }
  for (int i = 0; i < m_nbOfCoefficients-1; i++)
    {
      cout << "\t\t\t u[" << i << "] = " << m_u[i] << endl;
    }
}
