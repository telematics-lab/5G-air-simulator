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


#ifndef PARAMETERS_H_
#define PARAMETERS_H_

#include <iostream>
#include <string>
#include <complex>
#include <memory>
#include <map>
#include <deque>
#include <queue>
#include <algorithm>
#include <list>
#include <numeric>
#include "utility/PrintVector.h"
#include "utility/Logging.h"

using std::cout;
using std::endl;
using std::vector;
using std::max;
using std::min;
using std::string;
using std::polar;
using std::shared_ptr;
using std::make_shared;
using std::pair;
using std::make_pair;
using std::map;
using std::find;
using std::deque;
using std::ifstream;
using std::sort;
using std::distance;
using std::fill;
using std::remove;
using std::complex;
using std::list;
using std::abs;
using std::queue;
using std::iota;

/* tracing */
static bool _APP_TRACING_ = true;
static bool _RLC_TRACING_ = false;
static bool _MAC_TRACING_ = false;
static bool _PHY_TRACING_ = true;

/* activate uplink */
static bool UPLINK = false;

/* tests */
static bool _TEST_BLER_ = false;

/* channel model type*/
static bool _channel_TU_ = false;
static bool _channel_AWGN_ = true;

/* harq activation */
static bool _harq_active_  = false;

/* channel realization type */
#define _channel_simple_
//#define _channel_advanced_

/* debugging */
#define PHYRX_DEBUG

#endif /* PARAMETERS_H_ */

