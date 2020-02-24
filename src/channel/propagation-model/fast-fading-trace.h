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


#ifndef FAST_FADING_TRACE_H_
#define FAST_FADING_TRACE_H_
#include "../../load-parameters.h"

#if defined _channel_simple_
    extern const float ff_m2135_UMi_1x1_3kmh[1][20000];
    extern const float ff_m2135_UMi_1x1_3kmh_phase[1][20000];
    extern const float ff_m2135_UMa_1x1_30kmh[1][20000];
    extern const float ff_m2135_UMa_1x1_30kmh_phase[1][20000];
    extern const float ff_m2135_RMa_1x1_120kmh[1][20000];
    extern const float ff_m2135_RMa_1x1_120kmh_phase[1][20000];

    const float (* const ff_m2135_1x1_simple_3kmh_UMi[1])[1][20000] =
      {
        &ff_m2135_UMi_1x1_3kmh
      };
    const float (* const ff_m2135_1x1_simple_3kmh_UMi_phase[1])[1][20000] =
      {
        &ff_m2135_UMi_1x1_3kmh_phase
      };
    const float (* const ff_m2135_1x1_simple_30kmh_UMa[1])[1][20000] =
      {
        &ff_m2135_UMa_1x1_30kmh
      };
    const float (* const ff_m2135_1x1_simple_30kmh_UMa_phase[1])[1][20000] =
      {
        &ff_m2135_UMa_1x1_30kmh_phase
      };
    const float (* const ff_m2135_1x1_simple_120kmh_RMa[1])[1][20000] =
      {
        &ff_m2135_RMa_1x1_120kmh
      };
    const float (* const ff_m2135_1x1_simple_120kmh_RMa_phase[1])[1][20000] =
      {
        &ff_m2135_RMa_1x1_120kmh_phase
      };
#elif defined _channel_advanced_
    // MIMO 8x8 realizations
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path00[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path01[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path02[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path03[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path04[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path05[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path06[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path07[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path08[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path09[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path10[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path11[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path12[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path13[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path14[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path15[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path16[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path17[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path18[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path19[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path20[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path21[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path22[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path23[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path24[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path25[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path26[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path27[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path28[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path29[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path30[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path31[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path32[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path33[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path34[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path35[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path36[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path37[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path38[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path39[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path40[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path41[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path42[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path43[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path44[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path45[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path46[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path47[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path48[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path49[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path50[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path51[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path52[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path53[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path54[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path55[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path56[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path57[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path58[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path59[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path60[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path61[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path62[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_path63[100][3000];

    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path00[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path01[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path02[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path03[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path04[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path05[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path06[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path07[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path08[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path09[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path10[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path11[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path12[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path13[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path14[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path15[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path16[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path17[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path18[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path19[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path20[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path21[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path22[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path23[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path24[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path25[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path26[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path27[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path28[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path29[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path30[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path31[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path32[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path33[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path34[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path35[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path36[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path37[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path38[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path39[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path40[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path41[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path42[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path43[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path44[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path45[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path46[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path47[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path48[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path49[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path50[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path51[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path52[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path53[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path54[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path55[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path56[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path57[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path58[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path59[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path60[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path61[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path62[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path63[100][3000];

    const float (* const ff_m2135_1x1_antennatypeC_3kmh_UMi[1])[100][3000] =
      {
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path00
      };
    const float (* const ff_m2135_1x1_antennatypeC_3kmh_UMi_phase[1])[100][3000] =
      {
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path00
      };

    const float (* const ff_m2135_2x1_antennatypeC_3kmh_UMi[2])[100][3000] =
      {
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path00,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path01
      };
    const float (* const ff_m2135_2x1_antennatypeC_3kmh_UMi_phase[2])[100][3000] =
      {
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path00,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path01
      };

    const float (* const ff_m2135_2x2_antennatypeC_3kmh_UMi[4])[100][3000] =
      {
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path00,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path01,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path08,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path09
      };
    const float (* const ff_m2135_2x2_antennatypeC_3kmh_UMi_phase[4])[100][3000] =
      {
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path00,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path01,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path08,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path09
      };

    const float (* const ff_m2135_4x2_antennatypeC_3kmh_UMi[8])[100][3000] =
      {
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path00,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path01,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path02,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path03,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path08,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path09,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path10,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path11
      };
    const float (* const ff_m2135_4x2_antennatypeC_3kmh_UMi_phase[8])[100][3000] =
      {
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path00,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path01,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path02,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path03,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path08,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path09,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path10,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path11
      };

    const float (* const ff_m2135_4x4_antennatypeC_3kmh_UMi[16])[100][3000] =
      {
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path00,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path01,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path02,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path03,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path08,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path09,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path10,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path11,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path16,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path17,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path18,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path19,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path24,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path25,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path26,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path27
      };
    const float (* const ff_m2135_4x4_antennatypeC_3kmh_UMi_phase[16])[100][3000] =
      {
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path00,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path01,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path02,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path03,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path08,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path09,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path10,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path11,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path16,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path17,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path18,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path19,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path24,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path25,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path26,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path27
      };

    const float (* const ff_m2135_8x8_antennatypeC_3kmh_UMi[64])[100][3000] =
      {
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path00,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path01,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path02,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path03,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path04,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path05,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path06,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path07,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path08,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path09,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path10,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path11,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path12,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path13,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path14,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path15,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path16,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path17,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path18,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path19,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path20,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path21,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path22,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path23,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path24,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path25,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path26,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path27,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path28,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path29,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path30,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path31,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path32,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path33,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path34,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path35,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path36,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path37,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path38,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path39,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path40,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path41,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path42,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path43,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path44,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path45,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path46,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path47,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path48,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path49,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path50,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path51,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path52,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path53,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path54,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path55,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path56,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path57,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path58,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path59,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path60,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path61,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path62,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_path63
      };

    const float (* const ff_m2135_8x8_antennatypeC_3kmh_UMi_phase[64])[100][3000] =
      {
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path00,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path01,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path02,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path03,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path04,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path05,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path06,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path07,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path08,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path09,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path10,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path11,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path12,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path13,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path14,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path15,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path16,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path17,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path18,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path19,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path20,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path21,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path22,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path23,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path24,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path25,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path26,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path27,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path28,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path29,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path30,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path31,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path32,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path33,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path34,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path35,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path36,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path37,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path38,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path39,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path40,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path41,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path42,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path43,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path44,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path45,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path46,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path47,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path48,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path49,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path50,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path51,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path52,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path53,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path54,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path55,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path56,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path57,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path58,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path59,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path60,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path61,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path62,
        &ff_m2135_8x8_antennatypeC_3kmh_UMi_phase_path63
      };

    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path00[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path01[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path02[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path03[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path04[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path05[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path06[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path07[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path08[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path09[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path10[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path11[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path12[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path13[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path14[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path15[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path16[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path17[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path18[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path19[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path20[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path21[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path22[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path23[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path24[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path25[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path26[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path27[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path28[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path29[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path30[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path31[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path32[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path33[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path34[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path35[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path36[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path37[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path38[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path39[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path40[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path41[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path42[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path43[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path44[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path45[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path46[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path47[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path48[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path49[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path50[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path51[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path52[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path53[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path54[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path55[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path56[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path57[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path58[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path59[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path60[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path61[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path62[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_path63[100][3000];

    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path00[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path01[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path02[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path03[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path04[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path05[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path06[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path07[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path08[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path09[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path10[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path11[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path12[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path13[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path14[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path15[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path16[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path17[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path18[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path19[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path20[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path21[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path22[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path23[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path24[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path25[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path26[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path27[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path28[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path29[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path30[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path31[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path32[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path33[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path34[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path35[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path36[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path37[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path38[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path39[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path40[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path41[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path42[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path43[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path44[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path45[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path46[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path47[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path48[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path49[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path50[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path51[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path52[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path53[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path54[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path55[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path56[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path57[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path58[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path59[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path60[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path61[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path62[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path63[100][3000];


    const float (* const ff_m2135_1x1_antennatypeC_30kmh_UMa[1])[100][3000] =
      {
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path00
      };
    const float (* const ff_m2135_1x1_antennatypeC_30kmh_UMa_phase[1])[100][3000] =
      {
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path00
      };

    const float (* const ff_m2135_2x1_antennatypeC_30kmh_UMa[2])[100][3000] =
      {
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path00,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path01
      };
    const float (* const ff_m2135_2x1_antennatypeC_30kmh_UMa_phase[2])[100][3000] =
      {
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path00,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path01
      };

    const float (* const ff_m2135_2x2_antennatypeC_30kmh_UMa[4])[100][3000] =
      {
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path00,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path01,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path08,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path09
      };
    const float (* const ff_m2135_2x2_antennatypeC_30kmh_UMa_phase[4])[100][3000] =
      {
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path00,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path01,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path08,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path09
      };

    const float (* const ff_m2135_4x2_antennatypeC_30kmh_UMa[8])[100][3000] =
      {
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path00,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path01,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path02,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path03,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path08,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path09,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path10,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path11
      };
    const float (* const ff_m2135_4x2_antennatypeC_30kmh_UMa_phase[8])[100][3000] =
      {
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path00,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path01,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path02,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path03,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path08,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path09,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path10,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path11
      };

    const float (* const ff_m2135_4x4_antennatypeC_30kmh_UMa[16])[100][3000] =
      {
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path00,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path01,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path02,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path03,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path08,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path09,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path10,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path11,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path16,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path17,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path18,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path19,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path24,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path25,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path26,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path27
      };
    const float (* const ff_m2135_4x4_antennatypeC_30kmh_UMa_phase[16])[100][3000] =
      {
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path00,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path01,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path02,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path03,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path08,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path09,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path10,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path11,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path16,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path17,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path18,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path19,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path24,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path25,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path26,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path27
      };

    const float (* const ff_m2135_8x8_antennatypeC_30kmh_UMa[64])[100][3000] =
      {
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path00,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path01,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path02,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path03,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path04,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path05,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path06,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path07,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path08,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path09,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path10,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path11,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path12,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path13,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path14,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path15,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path16,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path17,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path18,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path19,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path20,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path21,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path22,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path23,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path24,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path25,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path26,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path27,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path28,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path29,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path30,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path31,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path32,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path33,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path34,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path35,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path36,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path37,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path38,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path39,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path40,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path41,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path42,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path43,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path44,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path45,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path46,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path47,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path48,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path49,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path50,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path51,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path52,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path53,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path54,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path55,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path56,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path57,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path58,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path59,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path60,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path61,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path62,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_path63
      };
    const float (* const ff_m2135_8x8_antennatypeC_30kmh_UMa_phase[64])[100][3000] =
      {
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path00,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path01,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path02,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path03,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path04,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path05,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path06,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path07,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path08,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path09,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path10,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path11,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path12,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path13,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path14,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path15,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path16,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path17,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path18,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path19,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path20,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path21,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path22,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path23,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path24,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path25,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path26,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path27,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path28,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path29,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path30,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path31,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path32,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path33,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path34,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path35,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path36,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path37,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path38,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path39,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path40,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path41,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path42,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path43,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path44,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path45,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path46,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path47,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path48,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path49,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path50,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path51,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path52,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path53,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path54,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path55,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path56,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path57,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path58,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path59,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path60,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path61,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path62,
        &ff_m2135_8x8_antennatypeC_30kmh_UMa_phase_path63
      };


    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path00[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path01[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path02[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path03[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path04[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path05[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path06[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path07[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path08[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path09[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path10[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path11[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path12[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path13[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path14[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path15[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path16[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path17[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path18[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path19[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path20[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path21[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path22[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path23[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path24[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path25[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path26[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path27[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path28[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path29[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path30[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path31[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path32[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path33[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path34[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path35[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path36[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path37[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path38[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path39[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path40[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path41[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path42[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path43[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path44[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path45[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path46[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path47[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path48[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path49[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path50[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path51[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path52[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path53[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path54[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path55[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path56[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path57[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path58[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path59[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path60[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path61[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path62[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_path63[100][3000];

    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path00[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path01[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path02[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path03[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path04[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path05[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path06[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path07[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path08[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path09[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path10[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path11[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path12[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path13[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path14[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path15[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path16[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path17[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path18[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path19[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path20[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path21[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path22[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path23[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path24[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path25[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path26[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path27[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path28[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path29[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path30[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path31[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path32[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path33[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path34[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path35[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path36[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path37[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path38[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path39[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path40[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path41[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path42[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path43[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path44[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path45[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path46[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path47[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path48[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path49[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path50[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path51[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path52[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path53[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path54[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path55[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path56[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path57[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path58[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path59[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path60[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path61[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path62[100][3000];
    extern const float ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path63[100][3000];


    const float (* const ff_m2135_1x1_antennatypeC_120kmh_RMa[1])[100][3000] =
      {
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path00
      };
    const float (* const ff_m2135_1x1_antennatypeC_120kmh_RMa_phase[1])[100][3000] =
      {
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path00
      };

    const float (* const ff_m2135_2x1_antennatypeC_120kmh_RMa[2])[100][3000] =
      {
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path00,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path01
      };
    const float (* const ff_m2135_2x1_antennatypeC_120kmh_RMa_phase[2])[100][3000] =
      {
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path00,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path01
      };

    const float (* const ff_m2135_2x2_antennatypeC_120kmh_RMa[4])[100][3000] =
      {
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path00,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path01,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path08,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path09
      };
    const float (* const ff_m2135_2x2_antennatypeC_120kmh_RMa_phase[4])[100][3000] =
      {
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path00,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path01,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path08,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path09
      };

    const float (* const ff_m2135_4x2_antennatypeC_120kmh_RMa[8])[100][3000] =
      {
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path00,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path01,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path02,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path03,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path08,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path09,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path10,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path11
      };
    const float (* const ff_m2135_4x2_antennatypeC_120kmh_RMa_phase[8])[100][3000] =
      {
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path00,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path01,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path02,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path03,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path08,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path09,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path10,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path11
      };

    const float (* const ff_m2135_4x4_antennatypeC_120kmh_RMa[16])[100][3000] =
      {
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path00,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path01,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path02,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path03,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path08,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path09,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path10,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path11,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path16,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path17,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path18,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path19,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path24,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path25,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path26,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path27
      };
    const float (* const ff_m2135_4x4_antennatypeC_120kmh_RMa_phase[16])[100][3000] =
      {
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path00,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path01,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path02,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path03,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path08,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path09,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path10,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path11,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path16,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path17,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path18,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path19,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path24,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path25,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path26,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path27
      };

    const float (* const ff_m2135_8x8_antennatypeC_120kmh_RMa[64])[100][3000] =
      {
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path00,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path01,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path02,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path03,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path04,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path05,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path06,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path07,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path08,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path09,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path10,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path11,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path12,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path13,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path14,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path15,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path16,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path17,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path18,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path19,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path20,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path21,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path22,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path23,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path24,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path25,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path26,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path27,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path28,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path29,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path30,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path31,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path32,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path33,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path34,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path35,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path36,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path37,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path38,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path39,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path40,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path41,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path42,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path43,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path44,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path45,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path46,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path47,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path48,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path49,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path50,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path51,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path52,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path53,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path54,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path55,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path56,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path57,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path58,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path59,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path60,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path61,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path62,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_path63
      };
    const float (* const ff_m2135_8x8_antennatypeC_120kmh_RMa_phase[64])[100][3000] =
      {
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path00,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path01,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path02,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path03,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path04,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path05,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path06,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path07,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path08,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path09,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path10,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path11,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path12,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path13,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path14,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path15,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path16,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path17,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path18,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path19,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path20,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path21,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path22,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path23,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path24,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path25,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path26,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path27,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path28,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path29,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path30,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path31,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path32,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path33,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path34,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path35,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path36,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path37,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path38,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path39,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path40,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path41,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path42,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path43,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path44,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path45,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path46,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path47,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path48,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path49,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path50,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path51,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path52,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path53,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path54,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path55,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path56,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path57,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path58,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path59,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path60,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path61,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path62,
        &ff_m2135_8x8_antennatypeC_120kmh_RMa_phase_path63
      };


    // 8x2 realizations for m-MIMO with GoB

    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_path00[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_path01[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_path02[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_path03[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_path04[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_path05[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_path06[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_path07[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_path08[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_path09[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_path10[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_path11[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_path12[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_path13[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_path14[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_path15[100][3000];


    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_phase_path00[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_phase_path01[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_phase_path02[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_phase_path03[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_phase_path04[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_phase_path05[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_phase_path06[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_phase_path07[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_phase_path08[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_phase_path09[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_phase_path10[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_phase_path11[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_phase_path12[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_phase_path13[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_phase_path14[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_phase_path15[100][3000];


    const float (* const ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa[16])[100][3000] =
      {
        &ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_path00,
        &ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_path01,
        &ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_path02,
        &ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_path03,
        &ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_path04,
        &ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_path05,
        &ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_path06,
        &ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_path07,
        &ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_path08,
        &ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_path09,
        &ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_path10,
        &ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_path11,
        &ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_path12,
        &ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_path13,
        &ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_path14,
        &ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_path15
      };

    const float (* const ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_phase[16])[100][3000] =
      {
        &ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_phase_path00,
        &ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_phase_path01,
        &ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_phase_path02,
        &ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_phase_path03,
        &ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_phase_path04,
        &ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_phase_path05,
        &ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_phase_path06,
        &ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_phase_path07,
        &ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_phase_path08,
        &ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_phase_path09,
        &ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_phase_path10,
        &ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_phase_path11,
        &ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_phase_path12,
        &ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_phase_path13,
        &ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_phase_path14,
        &ff_3gpp3d_8x2_antennatypeC_GoB_3kmh_UMa_phase_path15
      };

    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_path00[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_path01[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_path02[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_path03[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_path04[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_path05[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_path06[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_path07[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_path08[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_path09[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_path10[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_path11[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_path12[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_path13[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_path14[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_path15[100][3000];


    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_phase_path00[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_phase_path01[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_phase_path02[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_phase_path03[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_phase_path04[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_phase_path05[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_phase_path06[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_phase_path07[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_phase_path08[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_phase_path09[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_phase_path10[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_phase_path11[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_phase_path12[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_phase_path13[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_phase_path14[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_phase_path15[100][3000];


    const float (* const ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa[16])[100][3000] =
      {
        &ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_path00,
        &ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_path01,
        &ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_path02,
        &ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_path03,
        &ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_path04,
        &ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_path05,
        &ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_path06,
        &ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_path07,
        &ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_path08,
        &ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_path09,
        &ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_path10,
        &ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_path11,
        &ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_path12,
        &ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_path13,
        &ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_path14,
        &ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_path15
      };
    const float (* const ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_phase[16])[100][3000] =
      {
        &ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_phase_path00,
        &ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_phase_path01,
        &ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_phase_path02,
        &ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_phase_path03,
        &ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_phase_path04,
        &ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_phase_path05,
        &ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_phase_path06,
        &ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_phase_path07,
        &ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_phase_path08,
        &ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_phase_path09,
        &ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_phase_path10,
        &ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_phase_path11,
        &ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_phase_path12,
        &ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_phase_path13,
        &ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_phase_path14,
        &ff_3gpp3d_8x2_antennatypeC_GoB_30kmh_UMa_phase_path15
      };

    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_path00[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_path01[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_path02[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_path03[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_path04[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_path05[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_path06[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_path07[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_path08[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_path09[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_path10[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_path11[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_path12[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_path13[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_path14[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_path15[100][3000];


    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_phase_path00[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_phase_path01[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_phase_path02[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_phase_path03[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_phase_path04[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_phase_path05[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_phase_path06[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_phase_path07[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_phase_path08[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_phase_path09[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_phase_path10[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_phase_path11[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_phase_path12[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_phase_path13[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_phase_path14[100][3000];
    extern const float ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_phase_path15[100][3000];


    const float (* const ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa[16])[100][3000] =
      {
        &ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_path00,
        &ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_path01,
        &ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_path02,
        &ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_path03,
        &ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_path04,
        &ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_path05,
        &ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_path06,
        &ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_path07,
        &ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_path08,
        &ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_path09,
        &ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_path10,
        &ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_path11,
        &ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_path12,
        &ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_path13,
        &ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_path14,
        &ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_path15
      };
    const float (* const ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_phase[16])[100][3000] =
      {
        &ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_phase_path00,
        &ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_phase_path01,
        &ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_phase_path02,
        &ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_phase_path03,
        &ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_phase_path04,
        &ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_phase_path05,
        &ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_phase_path06,
        &ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_phase_path07,
        &ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_phase_path08,
        &ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_phase_path09,
        &ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_phase_path10,
        &ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_phase_path11,
        &ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_phase_path12,
        &ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_phase_path13,
        &ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_phase_path14,
        &ff_3gpp3d_8x2_antennatypeC_GoB_120kmh_UMa_phase_path15
      };


    // 16x2 realizations for m-MIMO with GoB

    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path00[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path01[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path02[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path03[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path04[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path05[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path06[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path07[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path08[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path09[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path10[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path11[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path12[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path13[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path14[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path15[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path16[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path17[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path18[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path19[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path20[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path21[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path22[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path23[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path24[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path25[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path26[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path27[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path28[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path29[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path30[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path31[100][3000];


    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path00[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path01[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path02[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path03[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path04[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path05[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path06[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path07[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path08[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path09[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path10[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path11[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path12[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path13[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path14[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path15[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path16[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path17[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path18[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path19[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path20[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path21[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path22[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path23[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path24[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path25[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path26[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path27[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path28[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path29[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path30[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path31[100][3000];

    const float (* const ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa[32])[100][3000] =
      {
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path00,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path01,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path02,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path03,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path04,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path05,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path06,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path07,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path08,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path09,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path10,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path11,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path12,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path13,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path14,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path15,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path16,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path17,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path18,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path19,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path20,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path21,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path22,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path23,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path24,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path25,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path26,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path27,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path28,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path29,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path30,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_path31
      };

    const float (* const ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase[32])[100][3000] =
      {
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path00,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path01,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path02,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path03,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path04,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path05,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path06,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path07,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path08,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path09,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path10,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path11,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path12,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path13,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path14,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path15,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path16,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path17,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path18,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path19,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path20,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path21,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path22,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path23,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path24,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path25,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path26,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path27,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path28,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path29,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path30,
        &ff_3gpp3d_16x2_antennatypeC_GoB_3kmh_UMa_phase_path31
      };

    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path00[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path01[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path02[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path03[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path04[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path05[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path06[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path07[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path08[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path09[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path10[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path11[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path12[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path13[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path14[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path15[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path16[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path17[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path18[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path19[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path20[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path21[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path22[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path23[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path24[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path25[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path26[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path27[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path28[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path29[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path30[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path31[100][3000];


    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path00[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path01[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path02[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path03[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path04[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path05[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path06[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path07[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path08[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path09[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path10[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path11[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path12[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path13[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path14[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path15[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path16[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path17[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path18[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path19[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path20[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path21[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path22[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path23[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path24[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path25[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path26[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path27[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path28[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path29[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path30[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path31[100][3000];


    const float (* const ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa[32])[100][3000] =
      {
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path00,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path01,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path02,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path03,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path04,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path05,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path06,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path07,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path08,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path09,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path10,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path11,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path12,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path13,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path14,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path15,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path16,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path17,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path18,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path19,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path20,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path21,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path22,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path23,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path24,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path25,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path26,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path27,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path28,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path29,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path30,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_path31
      };

    const float (* const ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase[32])[100][3000] =
      {
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path00,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path01,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path02,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path03,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path04,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path05,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path06,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path07,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path08,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path09,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path10,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path11,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path12,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path13,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path14,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path15,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path16,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path17,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path18,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path19,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path20,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path21,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path22,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path23,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path24,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path25,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path26,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path27,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path28,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path29,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path30,
        &ff_3gpp3d_16x2_antennatypeC_GoB_30kmh_UMa_phase_path31
      };

    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path00[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path01[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path02[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path03[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path04[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path05[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path06[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path07[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path08[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path09[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path10[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path11[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path12[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path13[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path14[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path15[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path16[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path17[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path18[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path19[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path20[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path21[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path22[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path23[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path24[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path25[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path26[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path27[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path28[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path29[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path30[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path31[100][3000];


    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path00[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path01[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path02[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path03[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path04[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path05[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path06[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path07[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path08[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path09[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path10[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path11[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path12[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path13[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path14[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path15[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path16[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path17[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path18[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path19[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path20[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path21[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path22[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path23[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path24[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path25[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path26[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path27[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path28[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path29[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path30[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path31[100][3000];

    const float (* const ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa[32])[100][3000] =
      {
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path00,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path01,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path02,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path03,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path04,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path05,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path06,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path07,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path08,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path09,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path10,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path11,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path12,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path13,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path14,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path15,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path16,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path17,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path18,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path19,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path20,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path21,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path22,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path23,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path24,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path25,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path26,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path27,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path28,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path29,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path30,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_path31
      };

    const float (* const ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase[32])[100][3000] =
      {
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path00,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path01,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path02,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path03,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path04,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path05,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path06,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path07,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path08,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path09,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path10,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path11,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path12,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path13,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path14,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path15,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path16,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path17,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path18,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path19,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path20,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path21,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path22,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path23,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path24,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path25,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path26,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path27,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path28,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path29,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path30,
        &ff_3gpp3d_16x2_antennatypeC_GoB_120kmh_UMa_phase_path31
      };

    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path00[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path01[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path02[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path03[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path04[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path05[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path06[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path07[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path08[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path09[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path10[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path11[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path12[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path13[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path14[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path15[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path16[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path17[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path18[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path19[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path20[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path21[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path22[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path23[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path24[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path25[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path26[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path27[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path28[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path29[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path30[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path31[100][3000];


    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path00[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path01[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path02[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path03[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path04[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path05[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path06[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path07[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path08[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path09[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path10[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path11[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path12[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path13[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path14[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path15[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path16[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path17[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path18[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path19[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path20[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path21[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path22[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path23[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path24[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path25[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path26[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path27[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path28[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path29[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path30[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path31[100][3000];

    const float (* const ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa[32])[100][3000] =
      {
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path00,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path01,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path02,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path03,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path04,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path05,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path06,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path07,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path08,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path09,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path10,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path11,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path12,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path13,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path14,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path15,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path16,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path17,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path18,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path19,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path20,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path21,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path22,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path23,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path24,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path25,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path26,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path27,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path28,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path29,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path30,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_path31
      };

    const float (* const ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase[32])[100][3000] =
      {
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path00,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path01,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path02,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path03,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path04,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path05,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path06,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path07,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path08,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path09,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path10,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path11,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path12,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path13,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path14,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path15,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path16,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path17,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path18,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path19,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path20,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path21,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path22,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path23,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path24,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path25,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path26,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path27,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path28,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path29,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path30,
        &ff_3gpp3d_16x2_antennatypeC_GoB_250kmh_UMa_phase_path31
      };

    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path00[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path01[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path02[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path03[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path04[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path05[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path06[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path07[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path08[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path09[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path10[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path11[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path12[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path13[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path14[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path15[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path16[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path17[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path18[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path19[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path20[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path21[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path22[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path23[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path24[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path25[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path26[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path27[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path28[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path29[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path30[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path31[100][3000];


    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path00[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path01[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path02[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path03[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path04[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path05[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path06[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path07[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path08[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path09[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path10[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path11[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path12[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path13[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path14[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path15[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path16[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path17[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path18[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path19[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path20[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path21[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path22[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path23[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path24[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path25[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path26[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path27[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path28[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path29[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path30[100][3000];
    extern const float ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path31[100][3000];

    const float (* const ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa[32])[100][3000] =
      {
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path00,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path01,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path02,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path03,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path04,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path05,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path06,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path07,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path08,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path09,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path10,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path11,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path12,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path13,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path14,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path15,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path16,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path17,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path18,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path19,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path20,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path21,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path22,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path23,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path24,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path25,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path26,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path27,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path28,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path29,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path30,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_path31
      };

    const float (* const ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase[32])[100][3000] =
      {
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path00,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path01,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path02,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path03,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path04,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path05,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path06,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path07,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path08,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path09,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path10,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path11,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path12,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path13,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path14,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path15,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path16,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path17,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path18,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path19,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path20,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path21,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path22,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path23,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path24,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path25,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path26,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path27,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path28,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path29,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path30,
        &ff_3gpp3d_16x2_antennatypeC_GoB_500kmh_UMa_phase_path31
      };



    // 16x4 realizations for m-MIMO with GoB

    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path00[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path01[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path02[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path03[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path04[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path05[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path06[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path07[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path08[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path09[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path10[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path11[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path12[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path13[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path14[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path15[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path16[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path17[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path18[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path19[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path20[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path21[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path22[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path23[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path24[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path25[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path26[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path27[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path28[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path29[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path30[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path31[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path32[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path33[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path34[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path35[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path36[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path37[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path38[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path39[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path40[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path41[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path42[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path43[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path44[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path45[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path46[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path47[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path48[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path49[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path50[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path51[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path52[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path53[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path54[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path55[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path56[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path57[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path58[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path59[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path60[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path61[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path62[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path63[100][3000];

    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path00[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path01[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path02[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path03[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path04[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path05[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path06[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path07[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path08[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path09[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path10[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path11[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path12[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path13[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path14[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path15[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path16[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path17[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path18[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path19[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path20[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path21[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path22[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path23[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path24[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path25[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path26[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path27[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path28[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path29[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path30[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path31[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path32[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path33[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path34[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path35[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path36[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path37[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path38[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path39[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path40[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path41[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path42[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path43[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path44[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path45[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path46[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path47[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path48[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path49[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path50[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path51[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path52[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path53[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path54[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path55[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path56[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path57[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path58[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path59[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path60[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path61[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path62[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path63[100][3000];

    const float (* const ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa[64])[100][3000] =
      {
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path00,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path01,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path02,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path03,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path04,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path05,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path06,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path07,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path08,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path09,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path10,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path11,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path12,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path13,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path14,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path15,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path16,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path17,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path18,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path19,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path20,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path21,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path22,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path23,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path24,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path25,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path26,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path27,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path28,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path29,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path30,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path31,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path32,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path33,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path34,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path35,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path36,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path37,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path38,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path39,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path40,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path41,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path42,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path43,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path44,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path45,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path46,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path47,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path48,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path49,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path50,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path51,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path52,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path53,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path54,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path55,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path56,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path57,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path58,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path59,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path60,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path61,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path62,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_path63
      };

    const float (* const ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase[64])[100][3000] =
      {
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path00,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path01,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path02,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path03,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path04,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path05,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path06,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path07,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path08,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path09,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path10,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path11,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path12,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path13,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path14,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path15,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path16,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path17,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path18,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path19,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path20,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path21,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path22,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path23,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path24,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path25,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path26,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path27,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path28,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path29,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path30,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path31,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path32,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path33,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path34,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path35,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path36,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path37,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path38,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path39,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path40,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path41,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path42,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path43,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path44,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path45,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path46,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path47,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path48,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path49,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path50,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path51,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path52,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path53,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path54,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path55,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path56,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path57,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path58,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path59,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path60,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path61,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path62,
        &ff_3gpp3d_16x4_antennatypeC_GoB_3kmh_UMa_phase_path63
      };

    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path00[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path01[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path02[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path03[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path04[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path05[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path06[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path07[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path08[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path09[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path10[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path11[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path12[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path13[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path14[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path15[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path16[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path17[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path18[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path19[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path20[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path21[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path22[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path23[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path24[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path25[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path26[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path27[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path28[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path29[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path30[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path31[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path32[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path33[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path34[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path35[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path36[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path37[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path38[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path39[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path40[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path41[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path42[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path43[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path44[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path45[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path46[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path47[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path48[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path49[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path50[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path51[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path52[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path53[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path54[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path55[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path56[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path57[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path58[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path59[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path60[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path61[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path62[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path63[100][3000];

    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path00[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path01[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path02[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path03[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path04[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path05[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path06[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path07[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path08[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path09[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path10[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path11[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path12[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path13[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path14[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path15[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path16[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path17[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path18[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path19[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path20[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path21[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path22[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path23[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path24[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path25[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path26[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path27[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path28[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path29[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path30[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path31[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path32[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path33[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path34[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path35[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path36[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path37[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path38[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path39[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path40[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path41[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path42[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path43[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path44[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path45[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path46[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path47[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path48[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path49[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path50[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path51[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path52[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path53[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path54[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path55[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path56[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path57[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path58[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path59[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path60[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path61[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path62[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path63[100][3000];

    const float (* const ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa[64])[100][3000] =
      {
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path00,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path01,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path02,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path03,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path04,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path05,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path06,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path07,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path08,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path09,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path10,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path11,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path12,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path13,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path14,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path15,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path16,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path17,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path18,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path19,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path20,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path21,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path22,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path23,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path24,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path25,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path26,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path27,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path28,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path29,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path30,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path31,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path32,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path33,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path34,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path35,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path36,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path37,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path38,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path39,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path40,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path41,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path42,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path43,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path44,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path45,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path46,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path47,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path48,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path49,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path50,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path51,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path52,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path53,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path54,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path55,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path56,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path57,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path58,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path59,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path60,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path61,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path62,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_path63
      };

    const float (* const ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase[64])[100][3000] =
      {
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path00,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path01,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path02,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path03,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path04,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path05,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path06,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path07,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path08,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path09,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path10,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path11,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path12,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path13,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path14,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path15,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path16,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path17,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path18,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path19,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path20,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path21,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path22,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path23,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path24,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path25,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path26,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path27,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path28,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path29,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path30,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path31,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path32,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path33,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path34,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path35,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path36,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path37,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path38,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path39,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path40,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path41,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path42,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path43,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path44,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path45,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path46,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path47,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path48,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path49,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path50,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path51,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path52,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path53,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path54,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path55,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path56,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path57,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path58,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path59,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path60,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path61,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path62,
        &ff_3gpp3d_16x4_antennatypeC_GoB_30kmh_UMa_phase_path63
      };

    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path00[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path01[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path02[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path03[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path04[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path05[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path06[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path07[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path08[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path09[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path10[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path11[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path12[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path13[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path14[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path15[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path16[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path17[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path18[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path19[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path20[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path21[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path22[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path23[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path24[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path25[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path26[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path27[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path28[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path29[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path30[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path31[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path32[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path33[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path34[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path35[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path36[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path37[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path38[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path39[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path40[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path41[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path42[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path43[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path44[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path45[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path46[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path47[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path48[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path49[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path50[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path51[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path52[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path53[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path54[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path55[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path56[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path57[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path58[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path59[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path60[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path61[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path62[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path63[100][3000];

    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path00[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path01[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path02[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path03[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path04[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path05[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path06[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path07[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path08[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path09[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path10[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path11[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path12[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path13[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path14[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path15[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path16[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path17[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path18[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path19[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path20[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path21[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path22[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path23[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path24[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path25[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path26[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path27[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path28[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path29[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path30[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path31[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path32[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path33[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path34[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path35[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path36[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path37[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path38[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path39[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path40[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path41[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path42[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path43[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path44[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path45[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path46[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path47[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path48[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path49[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path50[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path51[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path52[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path53[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path54[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path55[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path56[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path57[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path58[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path59[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path60[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path61[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path62[100][3000];
    extern const float ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path63[100][3000];

    const float (* const ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa[64])[100][3000] =
      {
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path00,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path01,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path02,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path03,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path04,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path05,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path06,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path07,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path08,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path09,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path10,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path11,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path12,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path13,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path14,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path15,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path16,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path17,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path18,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path19,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path20,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path21,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path22,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path23,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path24,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path25,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path26,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path27,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path28,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path29,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path30,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path31,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path32,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path33,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path34,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path35,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path36,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path37,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path38,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path39,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path40,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path41,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path42,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path43,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path44,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path45,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path46,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path47,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path48,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path49,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path50,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path51,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path52,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path53,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path54,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path55,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path56,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path57,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path58,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path59,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path60,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path61,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path62,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_path63
      };

    const float (* const ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase[64])[100][3000] =
      {
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path00,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path01,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path02,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path03,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path04,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path05,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path06,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path07,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path08,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path09,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path10,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path11,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path12,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path13,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path14,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path15,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path16,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path17,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path18,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path19,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path20,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path21,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path22,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path23,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path24,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path25,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path26,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path27,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path28,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path29,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path30,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path31,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path32,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path33,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path34,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path35,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path36,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path37,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path38,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path39,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path40,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path41,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path42,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path43,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path44,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path45,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path46,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path47,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path48,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path49,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path50,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path51,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path52,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path53,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path54,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path55,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path56,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path57,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path58,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path59,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path60,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path61,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path62,
        &ff_3gpp3d_16x4_antennatypeC_GoB_120kmh_UMa_phase_path63
      };
#else
    cout << "Error: invalid combination of _channel_simple_ e _channel_advanced_" << endl;
#endif

#endif /* FAST_FADING_TRACE_H_ */

