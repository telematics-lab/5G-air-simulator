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
 * Author: Alessandro Grassi <alessandro.grassi@poliba.it>
 */

#ifndef LOGGING_H_
#define LOGGING_H_


#ifdef DEBUG
    #define DEBUG_LOG_START_1(X1)          if(getenv(#X1)!=nullptr) {
    #define DEBUG_LOG_START_2(X1,X2)       if(getenv(#X1)!=nullptr || getenv(#X2)!=nullptr) {
    #define DEBUG_LOG_START_3(X1,X2,X3)    if(getenv(#X1)!=nullptr || getenv(#X2)!=nullptr || getenv(#X3)!=nullptr) {
    #define DEBUG_LOG_START_4(X1,X2,X3,X4) if(getenv(#X1)!=nullptr || getenv(#X2)!=nullptr || getenv(#X3)!=nullptr || getenv(#X4)!=nullptr) {
#else
    #define DEBUG_LOG_START_1(X1)          if(false) {
    #define DEBUG_LOG_START_2(X1,X2)       if(false) {
    #define DEBUG_LOG_START_3(X1,X2,X3)    if(false) {
    #define DEBUG_LOG_START_4(X1,X2,X3,X4) if(false) {
#endif

#define DEBUG_LOG_END }


#endif /* LOGGING_H_ */
