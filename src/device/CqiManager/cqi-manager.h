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

#ifndef CQIMANAGER_H_
#define CQIMANAGER_H_

#include <vector>
#include <memory>
#include <armadillo>
#include "../../load-parameters.h"

class NetworkNode;

class CqiManager
{
public:

  enum CQIReportingMode
  {
    PERIODIC,
    APERIODIC
  };

  CqiManager();
  virtual ~CqiManager() = default;

  void SetDevice (NetworkNode* d);
  NetworkNode* GetDevice (void);

  void SetCqiReportingMode (CQIReportingMode m);
  CQIReportingMode GetCqiReportingMode (void);

  void SetSendCqi (bool b);
  bool GetSendCqi (void);

  void SetReportingInterval (int i);
  int GetReportingInterval (void);

  void SetLastSent ();
  long int GetLastSent (void);

  void CreateCqiFeedbacks (vector<double> sinr);
  virtual vector<int> CalculateCqiFeedbacks (vector<double> sinr) = 0;
  void CreateRiFeedback (int rankIndicator);
  void CreatePmiFeedbacks (vector< vector< int > > pmi);
  void CreateFullCsiFeedbacks(vector< shared_ptr<arma::cx_fmat> > channelMatrix);

  bool NeedToSendFeedbacks (void);

private:
  CQIReportingMode m_reportingMode;

  bool m_sendCqi; //Used with aperiodic reporting. It is set to true by the gNB !

  int m_reportingInterval;
  long int m_lastSent;

  NetworkNode* m_device;

};

#endif /* CQIMANAGER_H_ */
