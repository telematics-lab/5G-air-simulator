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


#ifndef PACKETSCHEDULER_H_
#define PACKETSCHEDULER_H_

#include <vector>
#include <memory>
#include "../../../core/idealMessages/ideal-control-messages.h"
#include "../../../device/GNodeB.h"
#include "../../../load-parameters.h"

class GnbMacEntity;
class PacketBurst;
class Packet;
class RadioBearer;
class UserEquipment;
class HarqManager;

class PacketScheduler
{
public:
  PacketScheduler();
  virtual ~PacketScheduler();

  void Destroy (void);

  void SetMacEntity (GnbMacEntity* mac);
  GnbMacEntity* GetMacEntity (void);

  void Schedule (void);
  virtual void DoSchedule (void);

  void StopSchedule ();
  virtual void DoStopSchedule ();

  struct FlowToSchedule
  {
    FlowToSchedule(RadioBearer* bearer,
                   int dataToTransmit);
    virtual ~FlowToSchedule();
    RadioBearer* m_bearer;
    UserEquipment* m_ue;
    GNodeB::UserEquipmentRecord* m_ueRecord;
    int m_allocatedBits;    //bits
    int m_transmittedData;  //bytes
    int m_dataToTransmit;   //bytes
    HarqManager* m_harqManager;
    bool m_isHarqRetransmission;
    int m_harqPid;
    shared_ptr<PacketBurst> m_packets;
    vector< shared_ptr<FlowToSchedule> > m_multicastClonedFlows;

    vector<double> m_spectralEfficiency;
    vector<int> m_listOfAllocatedRBs;
    vector<int> m_listOfSelectedMCS;
    vector<int> m_assignedLayers;
    double widebandSinr;
    vector<int> m_cqiFeedbacks;
    int m_riFeedback;
    vector< vector<int> > m_pmiFeedbacks;
    vector< shared_ptr<arma::cx_fmat> > m_fullCsiFeedbacks;

    RadioBearer* GetBearer (void);

    void SetUeRecord(GNodeB::UserEquipmentRecord* record);
    GNodeB::UserEquipmentRecord* GetUeRecord();

    void SetUe(UserEquipment* ue);
    UserEquipment* GetUe(void);

    void UpdateAllocatedBits (int allocatedBits);
    int GetAllocatedBits (void) const;
    int GetTransmittedData (void) const;
    void SetDataToTransmit (int dataToTransmit);
    int GetDataToTransmit (void) const;

    void SetSpectralEfficiency (vector<double> s);
    vector<double> GetSpectralEfficiency (void);

    vector<int>* GetListOfAllocatedRBs ();
    vector<int>* GetListOfSelectedMCS ();

    void SetAssignedLayers (vector<int> layers);
    vector<int> GetAssignedLayers (void);
    void SetCqiFeedbacks (vector<int> cqiFeedbacks);
    vector<int> GetCqiFeedbacks (void);

    void SetRiFeedback (int riFeedback);
    int GetRiFeedback (void);

    void SetPmiFeedbacks (vector< vector<int> > pmiFeedbacks);
    vector< vector<int> > GetPmiFeedbacks (void);

    void SetHarqManager(HarqManager* harq);
    HarqManager* GetHarqManager(void);

    void SetFullCsiFeedbacks (vector< shared_ptr<arma::cx_fmat> > fullCsiFeedbacks);
    vector< shared_ptr<arma::cx_fmat> > GetFullCsiFeedbacks (void);

    bool IsHarqRetransmission(void);
    void SetHarqRetransmission(bool value);

    void SetHarqPid(int pid);
    int GetHarqPid(void);

    void SetPacketBurst(shared_ptr<PacketBurst> pb);
    shared_ptr<PacketBurst> GetPacketBurst(void);

    vector< shared_ptr<FlowToSchedule> >* GetMulticastClonedFlows();
  };

  typedef vector< shared_ptr<FlowToSchedule> > FlowsToSchedule;

  void CreateFlowsToSchedule (void);
  void DeleteFlowsToSchedule (void);
  void ClearFlowsToSchedule ();

  FlowsToSchedule* GetFlowsToSchedule (void) const;

  void InsertFlowToSchedule (RadioBearer* bearer,
                             int dataToTransmit,
                             vector<double> specEff,
                             vector<int> cqiFeedbacks,
                             int riFeedback,
                             vector< vector<int> > pmiFeedbacks);

  void InsertFlowToSchedule (RadioBearer* bearer,
                             int dataToTransmit,
                             vector<double> specEff,
                             vector<int> cqiFeedbacks,
                             int riFeedback,
                             vector< vector<int> > pmiFeedbacks,
                             vector< shared_ptr<arma::cx_fmat> > fullCsiFeedbacks);


  void InsertFlowForRetransmission (shared_ptr<FlowToSchedule> flow);

  void UpdateAllocatedBits (shared_ptr<FlowToSchedule> scheduledFlow,
                            int allocatedBits,
                            int allocatedRB,
                            int selectedMCS);

  void CheckForDLDropPackets();


private:
  GnbMacEntity *m_mac;
  FlowsToSchedule *m_flowsToSchedule;
};

#endif /* PACKETSCHEDULER_H_ */
