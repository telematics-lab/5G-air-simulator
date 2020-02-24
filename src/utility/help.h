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


#ifndef HELP_H_
#define HELP_H_
#include <iostream>
#include <stdio.h>

static void Help (void)
{

  cout <<
            "\n*** 5G-air-simulator is an open source framework to simulate the 5G air interface. ***\n"
            "\n\n"
            "run examples:"
            "\n"
            "\t ./5G-air-simulator SingleCell r nUE nVoip nVid nBE nCBR sched frStr speed maxD vidRate (seed)"
            "\n\t\t --> ./5G-air-simulator SingleCell 1 1 0 0 1 0 1 1 3 0.1 128"
            "\n\n"
            "\t ./5G-air-simulator SingleCellWithI nCell r nUE nVoip nVid nBE nCBR sched frStr speed maxD vidRate (seed)"
            "\n\t\t --> ./5G-air-simulator SingleCellWithI 7 1 1 0 0 1 0 1 1 3 0.1 128"
            "\n\n"
            "\t ./5G-air-simulator MultiCell nCell r nUE nVoip nVid nBE nCBR sched frStr speed maxD vidRate (seed)"
            "\n\t\t --> ./5G-air-simulator MultiCell 7 1 1 0 0 1 0 1 1 3 0.1 128"
            "\n\n"
            "\t ./5G-air-simulator SingleCellWithFemto r nBuilding buildingType activityRatio nUE nFemtoUE nVoip nVid nBE nCBR sched frStr speed accessPolicy maxD vidRate (seed)"
            "\n\t\t --> ./5G-air-simulator SingleCellWithFemto 1 1 0 1 0 1 0 0 1 0 1 1 3 0 0.1 128"
            "\n\n\n"
            "\t ./5G-air-simulator f5g-uc1 env isd density speed time tm nTx nMu nRx sched (seed)"
            "\n\t\t --> ./5G-air-simulator f5g-uc1 suburban 0.6 400 3 10 11 32 8 2 8 1"
            "\n\n"
            "\t ./5G-air-simulator f5g-uc6 env isd density pattern time mcs harq (seed)"
            "\n\t\t --> ./5G-air-simulator f5g-uc6 suburban 0.600 400 6 10 -1 0 1"
            "\n\n"
            "\t ./5G-air-simulator f5g-uc2 env isd nUe speed time nTx nM nRx sched srta wfIdx (seed)"
            "\n\t\t --> ./5G-air-simulator f5g-uc2 suburban 0.5 8 500 6 32 8 2 8 1 0 1"
            "\n\n"
            "\t ./5G-air-simulator MMC1 r  nUe  traf  frStr  maxD cbrT sync random_access_type  (seed)"
            "\n\t\t --> ./5G-air-simulator MMC1 290 1560 1 1 10 300 0 2 1"
            "\n\n"
            "\t ./5G-air-simulator  nbCell  sched_NB-IoT  time  r  nUe bw nC spa  tones  cbrT cbrS aMax nCl  [p0]  [pAtt]  [pRep]  [rWind]  [nP]  [period]  [o]  [boW]  (seed)"
            "\n\t\t --> ./5G-air-simulator nbCell 0 150 1000 1200 5 1 15 1 60 256 4 1 1 3 1 12 48 320 8 256 1"
            "\n\n\n"
            "\n\t legend:"
            "\n\t\t r is the cell radius (m)"
            "\n\t\t nUe is the number of users in the cell"
            "\n\t\t nVoip, nVid, nBE, nCBR are the number of VoIP, Video, Best-Effort, and CBR flows per UE, respectively"
            "\n\t\t speed is the speed of the users (km/h)"
            "\n\t\t maxD is the maximum tolerable delay of the transmissions (s)"
            "\n\t\t nCell is the number of cells"
            "\n\t\t nBuilding is the number of building containing Femto Cells"
            "\n\t\t nFemtoUe is the number of users in each Femto Cell"
            "\n\t\t isd is the Inter-Site Distance (km)"
            "\n\t\t density is the user density (users/km^2)"
            "\n\t\t time is the duration in seconds of each simulation run (s)"
            "\n\t\t nTx is the number of beams used at the base station for the first-stage precoding (or number of tx antennas)"
            "\n\t\t nMu is the number of users that can be scheduled simultaneously for each resource block and TTI"
            "\n\t\t nRx is the number of receiving antennas at the mobile terminals"
            "\n\t\t cbrT is the time interval between two successive transmission by the same user (s)"
            "\n"
            "\n\t\t bw is the total bandwidth used by the base station (MHz)"
            "\n\t\t nC is the number of NB-IoT carriers"
            "\n\t\t spa is the subcarrier spacing (kHz)"
            "\n\t\t tones is the number of tones"
            "\n\t\t cbrS is the size of the data sent by the users at each transmission (byte)"
            "\n\t\t aMax is the maximum number of retry attempts for the random access procedure"
            "\n\t\t nCl is the number of the coverage classes"
            "\n\t\t [p] is the probability that users belong to the coverage classes"
            "\n\t\t [pA] is the number of preamble transmission attempts"
            "\n\t\t [pRep] is the number of preamble repetition"
            "\n\t\t [rWind] is the duration of the RAR window (ms)"
            "\n\t\t [nP] is the number of different RACH preambles"
            "\n\t\t [period] is the periodicity of RACH resources (ms)"
            "\n\t\t [o] is the starting time of RACH resources (ms)"
            "\n\t\t [boW] is the duration of the RACH backoff window (ms)"
            "\n\t\t (seed) is an optional seed to initialize random quantities to different and reproducible values for each simulation run "
            "\n"
            "\n\t\t sched: 1-> PF, 2-> M-LWDF, 3-> EXP, 4-> FLS, 5 -> Optimize EXP Rule, 6 -> Optimized LOG Rule, 7-> MT, 8-> RR"
            "\n\t\t frStr: 1-> FDD, 2-> TDD"
            "\n\t\t vidRate: 128-> 128 kbps, 242-> 242 kbps, 440-> 440 kbps"
            "\n\t\t buildingType: 0-> 25 Femto Cells, 1-> 40 Femto Cells"
            "\n\t\t sched_NB-IoT: 0-> FIFO, 1-> RR"
            "\n\t\t env: urban, suburban, rural"
            "\n\t\t tm: 1-> TM1, 2-> TM2, 3-> TM3, 4-> TM4, 9 -> TM9, 11 -> Massive MIMO"
            "\n\t\t pattern: 0-> No MBSFN, 1-6-> Number of sub-frames to reserve for MBSFN "
            "\n\t\t mcs: -1-> MCS automatic selection, 0-28-> MCS to use for transmission"
            "\n\t\t harq: 0-> disable HARQ retransmission of broadcast packets, 1-> enable"
            "\n\t\t srta: 0-> no, 1-> yes"
            "\n\t\t wfIdx: 0-> OFDM, 1-> POFDM, 2-> Ideal without Doppler"
            "\n\t\t sync: 1-> Not Sync, 2-> Sync"
            "\n\t\t random_access_type: 1-> Baseline, 2-> Enhanced"
            "\n\t\t traf: 1-> CBR, 2-> FTP"
            << endl;

}

#endif /* HELP_H_ */
