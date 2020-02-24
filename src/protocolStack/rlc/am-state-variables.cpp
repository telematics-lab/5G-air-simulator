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

#include "am-state-variables.h"
#include "iostream"

AmStateVariables::AmStateVariables()
{
  m_am_window_size = 512;
  m_vt_a = 0;
  m_vt_ms = m_vt_a + m_am_window_size;
  m_vt_s = 0;
  m_poll_sn = 0;

  m_pdu_without_poll_counter = 0;
  m_byte_without_poll_counter = 0;
  m_retx_counter = 0;

  m_vr_r = 0;
  m_vr_mr = m_vr_r + m_am_window_size;
  m_vr_x = m_am_window_size; //XXX ???
  m_vr_ms = 0;
  m_vr_h = 0;

  m_t_poll_retransmit = 10;
  m_t_reordering = 10;
  m_t_status_prohibit = 10;

  m_max_retx_threshold = 3;
  m_poll_PDU = 10;
  m_poll_Byte = 10;
}

void
AmStateVariables::Print (void)
{
  cout << "AmStateVariables"
            "\t TX STATE: VT(A)=" << m_vt_a << " VT(MS)=" << m_vt_ms << " VT(S)=" << m_vt_s <<
            "\t RX STATE: VR(R)=" << m_vr_r << " VR(MR)=" << m_vr_mr << " VR(H)=" << m_vr_h <<
            endl;
}

void
AmStateVariables::PrintTxVariables (void)
{
  cout << "TX AmStateVariables"
            "\t TX STATE: VT(A)=" << m_vt_a << " VT(MS)=" << m_vt_ms << " VT(S)=" << m_vt_s <<
            endl;
}

void
AmStateVariables::PrintRxVariables (void)
{
  cout << "RX AmStateVariables"
            "\t RX STATE: VR(R)=" << m_vr_r << " VR(MR)=" << m_vr_mr << " VR(H)=" << m_vr_h <<
            endl;
}

