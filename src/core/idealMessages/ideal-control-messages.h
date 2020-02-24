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


#ifndef IDEAL_CONTROL_MESSAGES_H
#define IDEAL_CONTROL_MESSAGES_H

#include <list>
#include "../../load-parameters.h"

class NetworkNode;


/*
 * The IdealControlMessage provides a basic implementations for
 * control messages (such as PDCCH allocation map, CQI feedbacks)
 * that are exchanged among gNodeB and UEs.
 */
class IdealControlMessage
{
public:
  enum MessageType
  {
    CQI_FEEDBACKS, ALLOCATION_MAP,
    ARQ_RLC_ACK,
    HARQ_ACK,
    SCHEDULING_REQUEST,
    RI_FEEDBACK,
    PMI_FEEDBACK,
    FULLCSI_FEEDBACK,
    RA_PREAMBLE, RA_RESPONSE,
    RA_CONNECTION_REQUEST, RA_CONNECTION_RESOLUTION,
    NB_IOT_ALLOCATION_MAP
  };

  IdealControlMessage (void);
  virtual ~IdealControlMessage (void) = default;

  void SetSourceDevice (NetworkNode* src);
  void SetDestinationDevice (NetworkNode* dst);

  NetworkNode* GetSourceDevice (void);
  NetworkNode* GetDestinationDevice (void);

  void SetMessageType (MessageType type);
  MessageType GetMessageType (void);

private:
  NetworkNode* m_source;
  NetworkNode* m_destination;
  MessageType m_type;
};

#endif /* IDEAL_CONTROL_MESSAGES_H */

// ----------------------------------------------------------------------------------------------------------


#ifndef PDCCH_MAP_IDEAL_CONTROL_MESSAGES_H
#define PDCCH_MAP_IDEAL_CONTROL_MESSAGES_H

#include <list>
#include <vector>
#include <memory>
#include <armadillo>

class NetworkNode;

/*
 * The PdcchMapIdealControlMessage defines an ideal allocation map
 * for both UL and DL sends by the gNodeB to all UE,
 * using an ideal PDCCH control channel.
 * IdealPdcchMessage is composed by a list of IdealPdcchRecord
 * where is indicated the UE that can use a particular sub channel
 * with a proper MCS scheme.
 *
 * This records are the same for both UL and DL, and are created by the
 * packet scheduler at the beginning of each sub frame.
 *
 * When the IdealPdcchMessage is sent under an ideal control channel,
 * all UE stores into a proper variables the informations about
 * the resource mapping.
 */

#include <armadillo>

class PdcchMapIdealControlMessage : public IdealControlMessage
{
public:

  PdcchMapIdealControlMessage (void);
  virtual ~PdcchMapIdealControlMessage (void);

  enum Direction
  {
    DOWNLINK, UPLINK
  };

  struct IdealPdcchRecord
  {
    Direction m_direction;
    int m_idSubChannel;
    NetworkNode* m_ue;
    double m_mcsIndex;
    int m_rank;
    vector<int> m_pmi;
    shared_ptr<arma::cx_fmat> m_precodingMatrix;
    int m_harqPid;
    vector<int> m_assignedLayers;
    shared_ptr<arma::cx_fmat> m_fullCsiMatrix;
  };

  typedef list<struct IdealPdcchRecord>  IdealPdcchMessage;

  void AddNewRecord (Direction direction,
                     int subChannel, NetworkNode* ue, double mcs, int harqPid);

  void AddNewRecord (Direction direction,
                     int subChannel, NetworkNode* ue, double mcs, int harqPid, int rank);

  void AddNewRecord (Direction direction,
                     int subChannel, NetworkNode* ue, double mcs, int harqPid, int rank,
                     vector<int> pmi);
  void AddNewRecord (Direction direction,
                     int subChannel, NetworkNode* ue, double mcs, int harqPid, int rank,
                     vector<int> pmi, vector<int> relevantLayers);
  void AddNewRecord (Direction direction,
                     int subChannel, NetworkNode* ue, double mcs, int harqPid, int rank,
                     shared_ptr<arma::cx_fmat> precodingMatrix, vector<int> assignedLayers);
  void AddNewRecord (Direction direction,
                     int subChannel, NetworkNode* ue, double mcs, int harqPid, int rank,
                     shared_ptr<arma::cx_fmat> precodingMatrix, vector<int> assignedLayers,
                     shared_ptr<arma::cx_fmat> fullCsiMatrix);
  IdealPdcchMessage* GetMessage (void);
  
  vector<NetworkNode*> GetTargetUEs (void);

private:
  IdealPdcchMessage *m_idealPdcchMessage;
};

#endif /* PDCCH_MAP_IDEAL_CONTROL_MESSAGES_H */



// ----------------------------------------------------------------------------------------------------------



#ifndef CQI_IDEAL_CONTROL_MESSAGES_H
#define CQI_IDEAL_CONTROL_MESSAGES_H

#include <list>


class NetworkNode;

/*
 * The CqiIdealControlMessage defines an ideal list of feedback about
 * the channel quality sent by the UE to the gNodeB.
 */
class CqiIdealControlMessage : public IdealControlMessage
{
public:

  CqiIdealControlMessage (void);
  virtual ~CqiIdealControlMessage (void);


  struct CqiFeedback
  {
    int m_idSubChannel;
    double m_cqi;
  };

  typedef list<struct CqiFeedback>  CqiFeedbacks;

  void AddNewRecord (int subChannel, double cqi);
  CqiFeedbacks* GetMessage (void);


private:
  CqiFeedbacks *m_cqiFeedbacks;
};


#endif /* CQI_IDEAL_CONTROL_MESSAGES_H */


// ----------------------------------------------------------------------------------------------------------



#ifndef ARQ_IDEAL_CONTROL_MESSAGES_H
#define ARQ_IDEAL_CONTROL_MESSAGES_H

#include <list>

class NetworkNode;

/*
 *
 */
class ArqRlcIdealControlMessage : public IdealControlMessage
{
public:

  ArqRlcIdealControlMessage (void);
  virtual ~ArqRlcIdealControlMessage (void) = default;

  void SetAck (int ack);
  int GetAck (void);

  void SetStartByte (int b);
  void SetEndByte (int b);
  int GetStartByte (void);
  int GetEndByte (void);

private:
  int m_ack;
  int m_startByte;
  int m_endByte;
};

#endif /* ARQ_IDEAL_CONTROL_MESSAGES_H */



// ----------------------------------------------------------------------------------------------------------



#ifndef SCHEDULING_REQUEST_IDEAL_CONTROL_MESSAGES_H
#define SCHEDULING_REQUEST_IDEAL_CONTROL_MESSAGES_H

#include <list>

class NetworkNode;

/*
 *
 */
class SchedulingRequestIdealControlMessage : public IdealControlMessage
{
public:

  SchedulingRequestIdealControlMessage (void);
  virtual ~SchedulingRequestIdealControlMessage (void) = default;

  int GetBufferStatusReport (void);
  void SetBufferStatusReport (int b);

private:
  int m_bufferStatusReport;
};

#endif /* SCHEDULING_REQUEST_IDEAL_CONTROL_MESSAGES_H */


#ifndef RANK_INDICATOR_IDEAL_CONTROL_MESSAGES_H
#define RANK_INDICATOR_IDEAL_CONTROL_MESSAGES_H

class NetworkNode;

/*
 *
 */
class RankIndicatorIdealControlMessage : public IdealControlMessage
{
public:

  RankIndicatorIdealControlMessage (void);
  virtual ~RankIndicatorIdealControlMessage (void) = default;

  int GetRI (void);
  void SetRI (int ri);

private:
  int m_rankIndicator;
};

#endif /* RANK_INDICATOR_IDEAL_CONTROL_MESSAGES_H */

#ifndef PMI_IDEAL_CONTROL_MESSAGES_H
#define PMI_IDEAL_CONTROL_MESSAGES_H

class NetworkNode;

/*
 *
 */
class PmiIdealControlMessage : public IdealControlMessage
{
public:

  PmiIdealControlMessage (void);
  virtual ~PmiIdealControlMessage (void) = default;

  vector< vector<int> > GetPMI (void);
  void SetPMI (vector< vector<int> > pmi);

private:
  vector< vector<int> > m_pmiFeedbacks;
};

#endif /* PMI_IDEAL_CONTROL_MESSAGES_H */


#ifndef FULLCSI_IDEAL_CONTROL_MESSAGES_H
#define FULLCSI_IDEAL_CONTROL_MESSAGES_H

class NetworkNode;

/*
 *
 */
#include <armadillo>
class FullCsiIdealControlMessage : public IdealControlMessage
{
public:
  FullCsiIdealControlMessage (void);
  virtual ~FullCsiIdealControlMessage (void) = default;

  vector< shared_ptr<arma::cx_fmat> > GetChannelMatrix(void);
  void SetChannelMatrix (vector< shared_ptr<arma::cx_fmat> > ch_mat);

private:
  vector< shared_ptr<arma::cx_fmat> > m_channelMatrix;
};

#endif /* FULLCSI_IDEAL_CONTROL_MESSAGES_H */


// ----------------------------------------------------------------------------------------------------------



#ifndef HARQ_IDEAL_CONTROL_MESSAGES_H
#define HARQ_IDEAL_CONTROL_MESSAGES_H

/*
 *
 */
class HarqIdealControlMessage : public IdealControlMessage
{
public:
  HarqIdealControlMessage (void);
  virtual ~HarqIdealControlMessage (void) = default;

  void SetAck (bool ack);
  bool GetAck (void);

  void SetPid (int pid);
  int GetPid (void);

private:
  bool m_ack;
  int m_pid;
};

#endif /* HARQ_IDEAL_CONTROL_MESSAGES_H */


// ----------------------------------------------------------------------------



#ifndef RANDOM_ACCESS_PREAMBLE_IDEAL_CONTROL_MESSAGES_H
#define RANDOM_ACCESS_PREAMBLE_IDEAL_CONTROL_MESSAGES_H

class NetworkNode;

class RandomAccessPreambleIdealControlMessage : public IdealControlMessage
{
public:
  RandomAccessPreambleIdealControlMessage (void);
  RandomAccessPreambleIdealControlMessage (int p);
  virtual ~RandomAccessPreambleIdealControlMessage (void);

  void CreatePreamble();
  int GetPreamble();
  void SetMaxRAR(int max_rar);
  int GetMultipleRAR();
  void SetTimeTx();
  int GetTimeTx();


private:
  int m_preamble;
  int m_rar;
  int m_timeTx;
};


#endif /* RANDOM_ACCESS_PREAMBLE_IDEAL_CONTROL_MESSAGES_H */
// ----------------------------------------------------------------------------



#ifndef RANDOM_ACCESS_RESPONSE_IDEAL_CONTROL_MESSAGES_H
#define RANDOM_ACCESS_RESPONSE_IDEAL_CONTROL_MESSAGES_H

class NetworkNode;

class RandomAccessResponseIdealControlMessage : public IdealControlMessage
{
public:
  RandomAccessResponseIdealControlMessage (void);
  virtual ~RandomAccessResponseIdealControlMessage (void);
  void SetMsg2(bool r);
  bool GetMsg2();
  void setMsg3Time(int time);
  int getMsg3Time();
  void setMsg3RB(int rb);
  int getMsg3RB();

private:
  bool m_msg1Received;
  int m_msg3time;
  int m_msg3RB;
};


#endif /* RANDOM_ACCESS_PREAMBLE_IDEAL_CONTROL_MESSAGES_H */

// ----------------------------------------------------------------------------

#ifndef RANDOM_ACCESS_CONNECTION_REQUEST_IDEAL_CONTROL_MESSAGES_H
#define RANDOM_ACCESS_CONNECTION_REQUEST_IDEAL_CONTROL_MESSAGES_H

class NetworkNode;

class RandomAccessConnectionRequestIdealControlMessage :public IdealControlMessage
{
public:
  enum MotivationRequest{SCHEDULING_REQUEST};
  RandomAccessConnectionRequestIdealControlMessage(void);
  virtual ~RandomAccessConnectionRequestIdealControlMessage(void);

  void CreatUeID();
  int GetUeID();
  void SetMotivationRequest(MotivationRequest motivation);
  MotivationRequest GetMotivationRequest();

private:
  int m_UeID;
  MotivationRequest m_motivation;
};
#endif // RANDOM_ACCESS_CONNECTION_REQUEST_IDEAL_CONTROL_MESSAGES_H

//-----------------------------------------------------------------------------

#ifndef RANDOM_ACCESS_CONNECTION_RESOLUTION_IDEAL_CONTROL_MESSAGES_H
#define RANDOM_ACCESS_CONNECTION_RESOLUTION_IDEAL_CONTROL_MESSAGES_H

class NetworkNode;

class RandomAccessConnectionResolutionIdealControlMessage :public IdealControlMessage
{
public:
  RandomAccessConnectionResolutionIdealControlMessage(void);
  virtual ~RandomAccessConnectionResolutionIdealControlMessage(void);
};
#endif // RANDOM_ACCESS_CONNECTION_RESOLUTION_IDEAL_CONTROL_MESSAGES_H

//-------------------------------------------------------------------------------------------------------
#ifndef NB_IOT_UPLINK_MAP_IDEAL_CONTROL_MESSAGES_H
#define NB_IOT_UPLINK_MAP_IDEAL_CONTROL_MESSAGES_H

#include <list>

class NetworkNode;


class NbIoTMapIdealControlMessage : public IdealControlMessage
{
public:

  NbIoTMapIdealControlMessage (void);
  virtual ~NbIoTMapIdealControlMessage (void);

  struct IdealPdcchRecord
  {
    int m_ru;
    int m_idSubChannel;
    NetworkNode* m_ue;
    double m_mcsIndex;
  };

  typedef list<struct IdealPdcchRecord>  IdealPdcchMessage;

  void AddNewRecord (int ru, NetworkNode* ue, double mcs, int subcarrier);


  IdealPdcchMessage* GetMessage (void);

private:
  IdealPdcchMessage *m_idealPdcchMessage;
};

#endif /* NB_IOT_UPLINK_MAP_IDEAL_CONTROL_MESSAGES_H */

// ----------------------------------------------------------------------------------------------------------
