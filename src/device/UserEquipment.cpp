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


#include "UserEquipment.h"
#include "NetworkNode.h"
#include "GNodeB.h"
#include "HeNodeB.h"
#include "Gateway.h"
#include "../phy/ue-phy.h"
#include "CqiManager/cqi-manager.h"
#include "../core/eventScheduler/simulator.h"
#include "../componentManagers/NetworkManager.h"
#include "../protocolStack/rrc/ho/handover-entity.h"
#include "../protocolStack/rrc/ho/ho-manager.h"


UserEquipment::UserEquipment (int idElement,
                              double posx,
                              double posy,
                              Cell *cell,
                              GNodeB* target,
                              bool handover,
                              Mobility::MobilityModel model)
{
  SetIDNetworkNode (idElement);
  SetNodeType(NetworkNode::TYPE_UE);
  SetCell(cell);

  m_targetNode = target;

  ProtocolStack *stack = new ProtocolStack (this);
  SetProtocolStack (stack);

  Classifier *classifier = new Classifier ();
  classifier->SetDevice (this);
  SetClassifier (classifier);
  SetNodeState(STATE_IDLE);

  //Setup Mobility Model
  Mobility *m;
  if (model == Mobility::RANDOM_DIRECTION)
    {
      m = new RandomDirection ();
    }
  else if (model == Mobility::RANDOM_WALK)
    {
      m = new RandomWalk ();
    }
  else if (model == Mobility::RANDOM_WAYPOINT)
    {
      m = new RandomWaypoint ();
    }
  else if (model == Mobility::CONSTANT_POSITION)
    {
      m = new ConstantPosition ();
    }
  else if (model == Mobility::MANHATTAN)
    {
      m = new Manhattan ();
    }
  else if (model == Mobility::LINEAR_MOVEMENT)
    {
      m = new LinearMovement ();
    }
  else
    {
      cout << "ERROR: incorrect Mobility Model"<< endl;
      exit(1);
    }
  CartesianCoordinates *position = new CartesianCoordinates (posx, posy, 1.5);
  m->SetHandover (handover);
  m->SetAbsolutePosition (position);
  m->SetDevice (this);
  SetMobilityModel (m);

  m_timePositionUpdate = 0.001;
  Simulator::Init()->Schedule(m_timePositionUpdate,
                              &UserEquipment::UpdateUserPosition,
                              this,
                              Simulator::Init ()->Now());

  delete position;

  UePhy *phy = new UePhy ();
  phy->SetDevice(this);
  phy->SetBandwidthManager (target->GetPhy ()->GetBandwidthManager ());
  SetPhy(phy);

  m_cqiManager = nullptr;
  m_isIndoor = false;

  m_activityTimeout = 10;
  m_activityTimeoutEvent = NULL;
}

UserEquipment::UserEquipment (int idElement,
                              double posx,
                              double posy,
                              int speed,
                              double speedDirection,
                              Cell *cell,
                              GNodeB* target,
                              bool handover,
                              Mobility::MobilityModel model)
    : UserEquipment::UserEquipment (idElement,
                                    posx,
                                    posy,
                                    cell,
                                    target,
                                    handover,
                                    model)
{
  GetMobilityModel()->SetSpeed(speed);
  GetMobilityModel()->SetSpeedDirection(speedDirection);
}

UserEquipment::~UserEquipment()
{
  m_targetNode = nullptr;
  delete m_cqiManager;
  Destroy ();
}

void
UserEquipment::SetTargetNode (GNodeB* n)
{
  m_targetNode = n;
  SetCell (n->GetCell ());
}

GNodeB*
UserEquipment::GetTargetNode (void)
{
  return m_targetNode;
}

void
UserEquipment::SetTimePositionUpdate (double time)
{
  m_timePositionUpdate = time;
}

double
UserEquipment::GetTimePositionUpdate (void)
{
  return m_timePositionUpdate;
}
//TODO: CHECK GD for the moment I only comment the original method and implemented the new one (leave this todo)
//void
//UserEquipment::UpdateUserPosition (double time)
//{
//  GetMobilityModel ()->UpdatePosition (time);
//
//  SetIndoorFlag(NetworkManager::Init()->CheckIndoorUsers(this));
//
//  if (GetMobilityModel ()->GetHandover () == true)
//    {
//      GNodeB* targetNode = GetTargetNode ();
//
//      if (targetNode->GetProtocolStack ()->GetRrcEntity ()->
//          GetHandoverEntity ()->CheckHandoverNeed (this))
//        {
//          GNodeB* newTargetNode = targetNode->GetProtocolStack ()
//                                       ->GetRrcEntity ()->GetHandoverEntity ()->GetHoManager ()->m_target;
//DEBUG_LOG_START_1(SIM_ENV_HANDOVER_DEBUG)
//          cout << "** HO ** \t time: " << time << " user " <<  GetIDNetworkNode () <<
//                    " old gNB " << targetNode->GetIDNetworkNode () <<
//                    " new gNB " << newTargetNode->GetIDNetworkNode () << endl;
//DEBUG_LOG_END
//          NetworkManager::Init()->HandoverProcedure(time, this, targetNode, newTargetNode);
//        }
//    }
//
//  if (GetMobilityModel ()-> GetMobilityModel() != Mobility::CONSTANT_POSITION) {
//    //schedule the new update after m_timePositionUpdate
//    Simulator::Init()->Schedule(m_timePositionUpdate,
//                              &UserEquipment::UpdateUserPosition,
//                              this,
//                              Simulator::Init ()->Now());
//  }
//}


UeMacEntity*
UserEquipment::GetMacEntity(void) const
{
  return (UeMacEntity*)GetProtocolStack()->GetMacEntity();
}

void
UserEquipment::SetTargetNodeRecord (GNodeB::UserEquipmentRecord *r)
{
  m_targetNodeRecord = r;
}

GNodeB::UserEquipmentRecord*
UserEquipment::GetTargetNodeRecord (void)
{
  return m_targetNodeRecord;
}


void
UserEquipment::SetCqiManager (CqiManager *cm)
{
  m_cqiManager = cm;
}

CqiManager*
UserEquipment::GetCqiManager (void)
{
  return m_cqiManager;
}

void
UserEquipment::SetIndoorFlag ( bool flag)
{
  m_isIndoor = flag;
}

bool
UserEquipment::IsIndoor (void)
{
  return m_isIndoor;
}

void
UserEquipment::SetLastActivity()
{
  if (m_activityTimeoutEvent != NULL)
    {
      m_activityTimeoutEvent->MarkDeleted();
    }
  m_activityTimeoutEvent = Simulator::Init()->Schedule(m_activityTimeout, &UserEquipment::SetNodeState, this, NetworkNode::STATE_IDLE );
}

void
UserEquipment::SetActivityTimeout(double timeout)
{
  m_activityTimeout = timeout;
}

double
UserEquipment::GetActivityTimeout()
{
  return m_activityTimeout;
}

void UserEquipment::SetRandomAccessType(UeRandomAccess::RandomAccessType type)
{
  GetMacEntity ()->SetRandomAccessType(type);
}

//Debug
void
UserEquipment::Print (void)
{
  cout << " UserEquipment object:"
            "\n\t m_idNetworkNode = " << GetIDNetworkNode () <<
            "\n\t idCell = " << GetCell ()->GetIdCell () <<
            "\n\t idtargetNode = " << GetTargetNode ()->GetIDNetworkNode () <<
            "\n\t m_AbsolutePosition_X = " <<  GetMobilityModel ()->GetAbsolutePosition()->GetCoordinateX()<<
            "\n\t m_AbsolutePosition_Y = " << GetMobilityModel ()->GetAbsolutePosition()->GetCoordinateY()<<
            "\n\t m_speed = " << GetMobilityModel ()->GetSpeed () <<
            "\n\t m_speedDirection = " << GetMobilityModel ()->GetSpeedDirection () <<
            endl;
}

void
UserEquipment::UpdateUserPosition (double time) {
    if(hasTwin()) {               // when the twinmodel ho is activated
        if(IsTwin()) {   // if it is a twin
            UserEquipment* ue = NetworkManager::Init()->GetTwinUserEquipmentByID(GetIDNetworkNode());
            GetMobilityModel ()->UpdatePosition (time);
            SetIndoorFlag(NetworkManager::Init()->CheckIndoorUsers(this));
            GetTargetNode()->GetProtocolStack()->GetRrcEntity()->GetHandoverEntity()->GetHoManager()->CalculatePower(this);
            if(IsTwinTransmitting()&& ue->IsTwinTransmitting()) {  // If Both are transmitting
                if(GetTargetNode()->GetProtocolStack()->GetRrcEntity()->GetHandoverEntity()->GetHoManager()->CheckDetachTimeForTwin(this)) {
                    NetworkManager::Init()->CheckPacketsInTwin(this);
                    NetworkManager::Init()->DoHandover(this);
                }
                else {
                    if(!IsTwinTransmitting()&&!ue->IsTwinTransmitting()) {
                        NetworkManager::Init()->CheckPacketsInTwin(this);
                        NetworkManager::Init()->DeleteTwinBearerOnly(this);
                    }
                }
            }
            
            if (GetMobilityModel ()-> GetMobilityModel() != Mobility::CONSTANT_POSITION) {
                //schedule the new update after m_timePositionUpdate
                Simulator::Init()->Schedule(m_timePositionUpdate,
                                            &UserEquipment::UpdateUserPosition,
                                            this,
                                            Simulator::Init ()->Now());
            }
        }
        else {  // if it is a original node
            UserEquipment* twin = NetworkManager::Init()->GetTwinUserEquipmentByID(GetIDNetworkNode());
            GetMobilityModel ()->UpdatePosition (time);
            SetIndoorFlag(NetworkManager::Init()->CheckIndoorUsers(this));
            GNodeB* targetNode = GetTargetNode ();
            
            GetTargetNode()->GetProtocolStack()->GetRrcEntity()->GetHandoverEntity()->GetHoManager()->CalculatePower(this);
            
            
            if(IsTwinTransmitting()&&!twin->IsTwinTransmitting()) {  // If only UE is transmitting
                if (GetMobilityModel ()->GetHandover () == true) {
                    GNodeB* targetNode = GetTargetNode ();
                    if (targetNode->GetProtocolStack ()->GetRrcEntity ()->
                        GetHandoverEntity ()->CheckHandoverNeed (this)) {
                        SetTwinTransmittingFlag(false);
                        UserEquipment* twin = NetworkManager::Init()->GetTwinUserEquipmentByID(GetIDNetworkNode());
                        GNodeB* newTargetNode = targetNode->GetProtocolStack ()->GetRrcEntity ()->GetHandoverEntity ()->GetHoManager ()->m_target;
                        NetworkManager::Init()->DualTransmission(this,newTargetNode);
                    }
                }
            }
            
            
            if (GetMobilityModel ()-> GetMobilityModel() != Mobility::CONSTANT_POSITION) {
                //schedule the new update after m_timePositionUpdate
                Simulator::Init()->Schedule(m_timePositionUpdate,
                                            &UserEquipment::UpdateUserPosition,
                                            this,
                                            Simulator::Init ()->Now());
            }
            
        }
    }
    
    else {  // when the LTE HO is acivated
        
        if(!IsTwin()) { // if this a original node
            GetMobilityModel ()->UpdatePosition (time);
            SetIndoorFlag(NetworkManager::Init()->CheckIndoorUsers(this));
            GetTargetNode()->GetProtocolStack()->GetRrcEntity()->GetHandoverEntity()->GetHoManager()->CalculatePower(this);
            
            if (GetMobilityModel ()->GetHandover () == true) {
                
                if(Simulator::Init()->Now()-GetMobilityModel()->GetLastHandoverTime()>GetTargetNode()->GetProtocolStack()->GetRrcEntity()->GetHandoverEntity()->GetDetachTime()) {
                    GNodeB* targetNode = GetTargetNode ();
                    
                    if (targetNode->GetProtocolStack ()->GetRrcEntity ()->
                        GetHandoverEntity ()->CheckHandoverNeed (this)) {
                        
                        GNodeB* newTargetNode = targetNode->GetProtocolStack ()
                        ->GetRrcEntity ()->GetHandoverEntity ()->GetHoManager ()->m_target;
                        
                        DEBUG_LOG_START_1(SIM_ENV_HANDOVER_DEBUG)
                        cout << "** HO ** \t time: " << time << " user " <<  GetIDNetworkNode () <<
                        " old eNB " << targetNode->GetIDNetworkNode () <<
                        " new eNB " << newTargetNode->GetIDNetworkNode () << endl;
                        DEBUG_LOG_END
                        NetworkManager::Init()->HandoverProcedure(time, this, targetNode, newTargetNode);
                    }
                    
                }
            }
            
            if (GetMobilityModel ()-> GetMobilityModel() != Mobility::CONSTANT_POSITION) {
                //schedule the new update after m_timePositionUpdate
                Simulator::Init()->Schedule(m_timePositionUpdate,
                                            &UserEquipment::UpdateUserPosition,
                                            this,
                                            Simulator::Init ()->Now());
            }
            
        }
        else {  // if this is a twin, just update the positiion
            GetMobilityModel ()->UpdatePosition (time);
            SetIndoorFlag(NetworkManager::Init()->CheckIndoorUsers(this));
            GetTargetNode()->GetProtocolStack()->GetRrcEntity()->GetHandoverEntity()->GetHoManager()->CalculatePower(this);
            if (GetMobilityModel ()-> GetMobilityModel() != Mobility::CONSTANT_POSITION) {
                //schedule the new update after m_timePositionUpdate
                Simulator::Init()->Schedule(m_timePositionUpdate,
                                            &UserEquipment::UpdateUserPosition,
                                            this,
                                            Simulator::Init ()->Now());
            }
            
        }
        
    }
}



double
UserEquipment::GetDistanceFromServingCell() {
    return m_distanceFromServingCell;
}

void
UserEquipment::SetDistanceFromServingCell(double distance) {
    m_distanceFromServingCell = distance;
}

void
UserEquipment::SetTwinFlag ( bool flag) {
    m_isTwin = flag;
}

bool
UserEquipment::IsTwin(void) {
    return m_isTwin;
}
void
UserEquipment::SetTwinTransmittingFlag ( bool flag) {
    m_isTwinTransmitting = flag;
}

bool
UserEquipment::IsTwinTransmitting(void) {
    return m_isTwinTransmitting;
}

void
UserEquipment::SethasTwin(bool flag) {
    m_hasTwin = flag;
}

bool
UserEquipment::hasTwin() {
    return m_hasTwin;
}
