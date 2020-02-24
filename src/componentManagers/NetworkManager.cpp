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



#include "NetworkManager.h"
#include "../core/eventScheduler/simulator.h"
#include "../device/Gateway.h"
#include "../device/GNodeB.h"
#include "../device/HeNodeB.h"
#include "../device/UserEquipment.h"
#include "../networkTopology/Cell.h"
#include "../networkTopology/Femtocell.h"
#include "../networkTopology/Building.h"
#include "../phy/phy.h"
#include "../protocolStack/rrc/ho/handover-entity.h"
#include "../protocolStack/rrc/ho/ho-manager.h"
#include "../channel/RadioChannel.h"
#include "../channel/propagation-model/channel-realization-helper.h"
#include "../channel/propagation-model/propagation-loss-model.h"
#include "../flows/radio-bearer.h"
#include "../flows/radio-bearer-sink.h"
#include "../utility/frequency-reuse-helper.h"
#include "../device/CqiManager/cqi-manager.h"
#include "../device/CqiManager/fullband-cqi-manager.h"
#include "../device/CqiManager/wideband-cqi-manager.h"
#include "../load-parameters.h"
#include "../flows/application/Application.h"
#include "../utility/IndoorScenarios.h"
#include "../networkTopology/Street.h"
#include "FrameManager.h"

NetworkManager* NetworkManager::ptr=nullptr;

NetworkManager::NetworkManager()
{
  m_cellContainer = new vector<Cell*>;
  m_femtoCellContainer = new vector<Femtocell*>;
  m_gNodeBContainer = new vector<GNodeB*>;
  m_homeGNodeBContainer = new vector<HeNodeB*>;
  m_userEquipmentContainer = new vector<UserEquipment*>;
  m_gatewayContainer = new vector<Gateway*>;
  m_buildingContainer = new vector<Building*>;
  m_WrapAroundType = WRAPAROUND_NONE;
  m_WrapAroundDistanceX = 0;
  m_WrapAroundDistanceY = 0;
}

NetworkManager::~NetworkManager()
{
  for (auto i : *m_cellContainer)
    {
      delete i;
    }
  delete m_cellContainer;

  for (auto i : *m_gatewayContainer)
    {
      delete i;
    }
  delete m_gatewayContainer;

  for (auto i : *m_gNodeBContainer)
    {
      delete i;
    }
  delete m_gNodeBContainer;

  for (auto i : *m_userEquipmentContainer)
    {
      delete i;
    }
  delete m_userEquipmentContainer;
}


Cell*
NetworkManager::CreateCell (int idCell, double radius, double minDistance, double X, double Y)
{
  Cell *cell = new Cell (idCell, radius, minDistance, X, Y);

  GetCellContainer()->push_back (cell);

  return cell;
}

GNodeB*
NetworkManager::CreateGnodeb (int id,
                              Cell* cell,
                              double pos_X, double pos_Y,
                              int numTxAntennas, int numRxAntennas,
                              RadioChannel *dlCh, RadioChannel *ulCh,
                              BandwidthManager *bm)
{
  GNodeB* gnb = new GNodeB (id, cell, pos_X, pos_Y); //GNodeB (1, cell);
  gnb->GetPhy ()->SetTxAntennas (numTxAntennas);
  gnb->GetPhy ()->SetRxAntennas (numRxAntennas);
  gnb->GetPhy ()->SetDlChannel (dlCh);
//  XXX gnb->GetPhy ()->SetDlMcChannel (dlMcCh);
  gnb->GetPhy ()->SetUlChannel (ulCh);
  gnb->GetPhy ()->SetBandwidthManager (bm);
  ulCh->AddDevice (gnb);

  GetGNodeBContainer ()->push_back (gnb);

  return gnb;
}

Gateway*
NetworkManager::CreateGateway (void)
{
  Gateway *gw = new Gateway ();

  GetGatewayContainer ()->push_back (gw);

  return gw;
}

UserEquipment*
NetworkManager::CreateUserEquipment (int id,
                                     double pos_X, double pos_Y, double speed, double speedDirection, int numTxAntennas, int numRxAntennas,
                                     Cell* cell, GNodeB* gnb)
{
  UserEquipment* ue = new UserEquipment (id,
                                         pos_X, pos_Y, speed, speedDirection,
                                         cell, gnb,
                                         0,
                                         //Mobility::RANDOM_DIRECTION
                                         Mobility::CONSTANT_POSITION);

  ue->GetPhy ()->SetDlChannel (gnb->GetPhy()->GetDlChannel ());
  ue->GetPhy ()->SetDlMcChannel (gnb->GetPhy()->GetDlMcChannel ());
  ue->GetPhy ()->SetUlChannel (gnb->GetPhy()->GetUlChannel ());
  ue->GetPhy ()->SetTxAntennas(numTxAntennas);
  ue->GetPhy ()->SetRxAntennas(numRxAntennas);

  FullbandCqiManager *cqiManager = new FullbandCqiManager ();
  cqiManager->SetCqiReportingMode (CqiManager::PERIODIC);
  cqiManager->SetReportingInterval (1);
  cqiManager->SetDevice (ue);
  ue->SetCqiManager (cqiManager);

  gnb->RegisterUserEquipment (ue);

  ChannelRealization* c_dl = new ChannelRealization (gnb, ue, ChannelRealization::CHANNEL_MODEL_MACROCELL_URBAN);
  gnb->GetPhy()->GetDlChannel ()->GetPropagationLossModel ()->AddChannelRealization (c_dl);
  if (gnb->GetPhy()->GetDlMcChannel () != nullptr)
    {
      ChannelRealization* c_mcdl = new ChannelRealization (gnb, ue, ChannelRealization::CHANNEL_MODEL_MACROCELL_URBAN, true);
      gnb->GetPhy()->GetDlMcChannel ()->GetPropagationLossModel ()->AddChannelRealization (c_mcdl);
    }
  ChannelRealization* c_ul = new ChannelRealization (ue, gnb, ChannelRealization::CHANNEL_MODEL_MACROCELL_URBAN);
  gnb->GetPhy()->GetUlChannel ()->GetPropagationLossModel ()->AddChannelRealization (c_ul);

  GetUserEquipmentContainer ()->push_back (ue);

  return ue;
}

void
NetworkManager::CreateStreet (int idStreet, CartesianCoordinates* center, double theta,
                              int buildingtype, int nbBuildings, double street_width, double building_distance,
                              int nbFemtoInBuilding, double apartment_side, int firstFemtoIDInStreet)
{
  Street* street = new Street (idStreet, center, theta, nbBuildings, buildingtype, street_width, building_distance, apartment_side);

  vector<CartesianCoordinates*> *building_positions = street->GetBuildingDistributionInStreet();

  int idBuilding = 0;
  int firstFemtoinBuildingID = firstFemtoIDInStreet;

  while( idBuilding < 2*nbBuildings )
    {
      // Get Building Positions
      double buildingCenter_X = building_positions->at (idBuilding)->GetCoordinateX ();
      double buildingCenter_Y = building_positions->at (idBuilding)->GetCoordinateY ();

      CreateBuildingForFemtocells( idBuilding, buildingtype, apartment_side, 1, buildingCenter_X, buildingCenter_Y, firstFemtoinBuildingID, nbFemtoInBuilding);

      cout << "Created Building, id " << idBuilding
                <<", position: " << buildingCenter_X
                << " " << buildingCenter_Y
                << " and " << nbFemtoInBuilding << " femtocells" << endl;

      idBuilding++;
      firstFemtoinBuildingID += nbFemtoInBuilding;
    }

}

void
NetworkManager::CreateBuildingForFemtocells (int idBuilding,
    int buildingType,
    double apartmentSide,
    int nbFloors,
    double pos_X,
    double pos_Y,
    int firstFemtoCellID,
    int nbOfFemtocells)
{

  double side_X;
  double side_Y;

  if ( buildingType == 0)
    {
      side_X = 5 * apartmentSide;
      side_Y = 5 * apartmentSide;
    }
  else
    {
      side_X = 10 * apartmentSide;
      side_Y = 5 * apartmentSide;
    }

  Building* building = new Building (idBuilding,
                                     buildingType,
                                     nbFloors,
                                     side_X,
                                     side_Y,
                                     pos_X,
                                     pos_Y);

  // CREATE FEMTO _CELLS
  int idCell;

  for (int floor = 0; floor < nbFloors; floor++ )
    {
      int femtocellIndex = 0;
      while ( femtocellIndex < nbOfFemtocells )
        {
          idCell = femtocellIndex + firstFemtoCellID;

          double* offset = GetFemtoCellOffsetInBuilding ( building->GetBuildingType(), femtocellIndex, apartmentSide);

          building->AddFemtoCell( CreateFemtoCell( idCell,
                                  apartmentSide,
                                  offset[0] + pos_X,
                                  offset[1] + pos_Y) );

          GetFemtoCellByID(idCell)->GetCellCenterPosition()->SetFloor( floor );

          delete [] offset;

          femtocellIndex++;
        }
    }

  GetBuildingContainer()->push_back(building);

}

Femtocell*
NetworkManager::CreateFemtoCell(int idCell, double side, double X, double Y)
{
  Femtocell *femtocell = new Femtocell (idCell, side, X, Y);

  GetFemtoCellContainer()->push_back (femtocell);

  return femtocell;
}


int
NetworkManager::GetNbCell (void)
{
  return m_cellContainer->size ();
}

vector<Cell*>*
NetworkManager::GetCellContainer (void)
{
  return m_cellContainer;
}

vector<Femtocell*>*
NetworkManager::GetFemtoCellContainer (void)
{
  return m_femtoCellContainer;
}

vector<GNodeB*>*
NetworkManager::GetGNodeBContainer (void)
{
  return m_gNodeBContainer;
}

vector<HeNodeB*>*
NetworkManager::GetHomeGNodeBContainer (void)
{
  return m_homeGNodeBContainer;
}

vector<UserEquipment*>*
NetworkManager::GetUserEquipmentContainer (void)
{
  return m_userEquipmentContainer;
}

vector<Gateway*>*
NetworkManager::GetGatewayContainer (void)
{
  return m_gatewayContainer;
}

vector<Building*>*
NetworkManager::GetBuildingContainer (void)
{
  return m_buildingContainer;
}


Cell*
NetworkManager::GetCellByID (int idCell)
{
  for (auto cell : *GetCellContainer ())
    {
      if (cell->GetIdCell() == idCell)
        return cell;
    }
  return nullptr;
}

Femtocell*
NetworkManager::GetFemtoCellByID (int idFemtoCell)
{
  for (auto cell : *GetFemtoCellContainer())
    {
      if (cell->GetIdCell() == idFemtoCell)
        return cell;
    }
  return nullptr;
}

GNodeB*
NetworkManager::GetGNodeBByID (int idGNodeB)
{
  for (auto gNodeB : *GetGNodeBContainer ())
    {
      if (gNodeB->GetIDNetworkNode() == idGNodeB)
        {
          return gNodeB;
        }
    }
  return nullptr;
}

GNodeB*
NetworkManager::GetGNodeBByCellID (int idCell)
{
  for (auto gNodeB : *GetGNodeBContainer ())
    {
      if (gNodeB->GetCell ()->GetIdCell ()== idCell)
        {
          return gNodeB;
        }
    }
  return nullptr;
}

UserEquipment*
NetworkManager::GetUserEquipmentByID (int idUE)
{
  for (auto userEquipment : *GetUserEquipmentContainer())
    {
      if (userEquipment->GetIDNetworkNode() == idUE)
        {
          return userEquipment;
        }
    }
  return nullptr;
}

Gateway*
NetworkManager::GetGatewayByID (int idGW)
{
  for (auto gateway : *GetGatewayContainer())
    {
      if (gateway->GetIDNetworkNode() == idGW)
        {
          return gateway;
        }
    }
  return nullptr;
}

Building*
NetworkManager::GetBuildingByID (int idBuilding)
{
  for (auto building : *GetBuildingContainer())
    {
      if (building->GetIdBuilding() == idBuilding)
        {
          return building;
        }
    }
  return nullptr;
}

Building*
NetworkManager::GetBuildingByFemtoCellID (int idFemtoCell)
{
  for (auto building : *GetBuildingContainer())
    {
      if (building->GetFemtoIdInBuilding(idFemtoCell) != -1)
        {
          return building;
        }
    }
  return nullptr;
}

NetworkNode*
NetworkManager::GetNetworkNodeByID (int id)
{
  for (auto gNodeB : *GetGNodeBContainer())
    {
      if (gNodeB->GetIDNetworkNode() == id)
        {
          return gNodeB;
        }
    }

  for (auto userEquipment : *GetUserEquipmentContainer())
    {
      if (userEquipment->GetIDNetworkNode() == id)
        {
          return userEquipment;
        }
    }

  for (auto gateway : *GetGatewayContainer())
    {
      if (gateway->GetIDNetworkNode() == id)
        {
          return gateway;
        }
    }
  return nullptr;
}


NetworkManager::WrapAroundType
NetworkManager::GetWrapAroundType(void)
{
  return m_WrapAroundType;
}

void
NetworkManager::SetWrapAroundType(NetworkManager::WrapAroundType type)
{
  m_WrapAroundType = type;
}

void
NetworkManager::SetWrapAroundDistanceX(double distance)
{
  m_WrapAroundDistanceX = distance;
}

double
NetworkManager::GetWrapAroundDistanceX(void)
{
  return m_WrapAroundDistanceX;
}

void
NetworkManager::SetWrapAroundDistanceY(double distance)
{
  m_WrapAroundDistanceY = distance;
}

double
NetworkManager::GetWrapAroundDistanceY(void)
{
  return m_WrapAroundDistanceY;
}

vector<UserEquipment*>*
NetworkManager::GetRegisteredUEToGNodeB (int idENB)
{
  vector<UserEquipment*>* UElist = new vector<UserEquipment*>;
  for (auto userEquipment : *GetUserEquipmentContainer())
    {
      if (userEquipment->GetTargetNode ()->GetIDNetworkNode() == idENB)
        {
          UElist->push_back(userEquipment);
        }
    }
  return UElist;
}


vector<int>
NetworkManager::GetCellIDFromPosition (CartesianCoordinates *position)
{
  vector<int> CellsID;
  double distance;
  for (auto cell : *GetCellContainer())
    {
      distance =  sqrt (pow ((cell->GetCellCenterPosition()->GetCoordinateX() - position->GetCoordinateX()),2)
                        + pow ((cell->GetCellCenterPosition()->GetCoordinateY() - position->GetCoordinateY()),2));

      if (distance <= cell->GetRadius())
        {
          CellsID.push_back(cell->GetIdCell());
        }
    }

  return CellsID;
}

int
NetworkManager::GetCellIDFromPosition (double posX,
                                       double posY)
{
  double target_distance = 10000.0;
  double distance;
  int cellID;

  for (auto cell : *GetCellContainer())
    {
      distance = cell->GetCellCenterPosition()->GetDistance(posX, posY);

      if (distance < target_distance)
        {
          cellID = cell->GetIdCell ();
          target_distance = distance;
        }
    }
  return cellID;
}

bool
NetworkManager::CheckHandoverPermissions (GNodeB* target, UserEquipment* user)
{
  if ( target->GetNodeType() == NetworkNode::TYPE_GNODEB )
    {
      return true;
    }
  else if (target->GetNodeType() == NetworkNode::TYPE_HOME_BASE_STATION)
    {
      HenbMacEntity* henb_mac = (HenbMacEntity*)target->GetMacEntity();

      return henb_mac->CheckSubscribedUser(user);
    }
  else
    {
      cout<< " Wrong Node Type for handover. Exiting..."<<endl;
      exit(1);
    }
  return false;
}

bool
NetworkManager::CheckIndoorUsers (UserEquipment *ue)
{
  vector<Building*>* buildingContainer = GetBuildingContainer ();
  if (buildingContainer->size()>0)
    {
      for (auto building : *buildingContainer)
        {
          if ( ( abs( building->GetCenterPosition()->GetCoordinateX()
                      - ue->GetMobilityModel()->GetAbsolutePosition()->GetCoordinateX() ) < building->GetSide()[0]/2 ) &&
               ( abs( building->GetCenterPosition()->GetCoordinateY()
                      - ue->GetMobilityModel()->GetAbsolutePosition()->GetCoordinateY() ) < building->GetSide()[1]/2 ) )
            {
              return true;
            }
        }
    }
  return false;
}

int
NetworkManager::GetBuildingIDForUE (UserEquipment *ue)
{
  vector<Building*> *buildingContainer = GetBuildingContainer();
  if (buildingContainer->size()>0)
    {
      for (auto building : *buildingContainer)
        {

          if ( ( abs( building->GetCenterPosition()->GetCoordinateX()
                      - ue->GetMobilityModel()->GetAbsolutePosition()->GetCoordinateX() ) < building->GetSide()[0]/2 ) &&
               ( abs( building->GetCenterPosition()->GetCoordinateY()
                      - ue->GetMobilityModel()->GetAbsolutePosition()->GetCoordinateY() ) < building->GetSide()[1]/2 ) )
            {
              return building->GetIdBuilding();
            }
        }
    }
  return -1;
}

Cell*
NetworkManager::GetBelongingCellFromPosition(UserEquipment* ue)
{
  CartesianCoordinates* ue_pos = ue->GetMobilityModel()->GetAbsolutePosition();
  int buildingID = GetBuildingIDForUE(ue);

  if ( buildingID != -1 )
    {
      //user is indoor and it will be attached to a femtocell
      vector<Femtocell*>* femtocellContainer = GetBuildingByID(buildingID)->GetFemtoCellsInBuilding();
      Femtocell *cell;
      double distance = 9999999;
      for (auto iter : *femtocellContainer)
        {
          if ( ue_pos->GetDistance(iter->GetCellCenterPosition()) < distance )
            {
              distance = ue_pos->GetDistance(iter->GetCellCenterPosition());
              cell = iter;
            }
        }
      return (Cell*) cell ;
    }
  else
    {
      // ue is outdoor and it will be attached to a macrocell
      return GetCellByID( GetCellIDFromPosition( ue_pos->GetCoordinateX(), ue_pos->GetCoordinateY() ) );
    }

  return nullptr;
}

void
NetworkManager::SelectTargetNode (UserEquipment* ue)
{
  GNodeB* targetNode = ue->GetTargetNode ();

  if (targetNode->GetProtocolStack ()->GetRrcEntity ()->
      GetHandoverEntity ()->CheckHandoverNeed (ue))
    {
      GNodeB* newTargetNode =
        targetNode->GetProtocolStack ()->GetRrcEntity ()->
        GetHandoverEntity ()->GetHoManager ()->m_target;

      ue->SetTargetNode (newTargetNode);
    }
}

void
NetworkManager::HandoverProcedure(double time, UserEquipment* ue, GNodeB* oldTarget, GNodeB* newTarget)
{

  if (ue->GetNodeState() == UserEquipment::STATE_DETACHED)
    {
      return;
    }

DEBUG_LOG_START_2(SIM_ENV_HANDOVER_DEBUG,SIM_ENV_HANDOVER_TARGET_DEBUG)
  cout << "** HO ** \t HandoverProcedure for user "
            <<  ue->GetIDNetworkNode ()
            << " from gNB " << oldTarget->GetIDNetworkNode()
            << " to gNB " << newTarget->GetIDNetworkNode() << endl;
DEBUG_LOG_END

  // 1 - deactivate the UE for a time equal to 30 ms
  bool detach = false;
  if (ue->GetNodeState() == UserEquipment::STATE_ACTIVE||
      ue->GetNodeState() == UserEquipment::STATE_IDLE)
    {
      //XXX Admission Control is not implemented
      ue->SetNodeState (UserEquipment::STATE_DETACHED);
      detach = true;
    }

DEBUG_LOG_START_1(SIM_ENV_MOBILITY_DEBUG)
  cout << "user " <<  ue->GetIDNetworkNode () << " starts HO "
            "\n\t old gNB = " << ue->GetTargetNode ()->GetIDNetworkNode () <<
            "\n\t target gNB = " <<  newTarget->GetIDNetworkNode () << endl;
DEBUG_LOG_END

  // 2 - transfer all active radio bearer
  Simulator::Init()->Schedule(0.001,
                              &NetworkManager::TransferBearerInfo,
                              this,
                              ue,
                              newTarget);

  if (detach == true)
    {
      double detachTime = ue->GetProtocolStack()->GetRrcEntity()->GetHandoverEntity()->GetDetachTime();
      Simulator::Init()->Schedule(detachTime, &NetworkNode::MakeActive, ue);
    }

  ue->GetMobilityModel ()->SetLastHandoverTime(time);
}


void
NetworkManager::TransferBearerInfo (UserEquipment* ue, GNodeB* target)
{
DEBUG_LOG_START_1(SIM_ENV_HANDOVER_DEBUG)
  cout << "** HO ** \t TransferBearerInfo for user "
            <<  ue->GetIDNetworkNode () << endl;
DEBUG_LOG_END

  if ( (target->GetNodeType() == NetworkNode::TYPE_GNODEB && ue->GetTargetNode ()->GetNodeType() == NetworkNode::TYPE_GNODEB) ||
       (target->GetNodeType() == NetworkNode::TYPE_HOME_BASE_STATION && ue->GetTargetNode ()->GetNodeType() == NetworkNode::TYPE_GNODEB) ||
       (target->GetNodeType() == NetworkNode::TYPE_GNODEB && ue->GetTargetNode ()->GetNodeType() == NetworkNode::TYPE_HOME_BASE_STATION))
    {
      GNodeB *oldTargetNode = ue->GetTargetNode ();
      GNodeB *newTargetNode = target;

      // 1 - update spectrum, channels and propagation loss model
DEBUG_LOG_START_1(SIM_ENV_HANDOVER_DEBUG)
      cout << "update spectrum, channels and propagation loss model"<< endl;
DEBUG_LOG_END
      ue->GetPhy ()->SetBandwidthManager (newTargetNode->GetPhy ()->GetBandwidthManager ());

      RadioChannel *oldDl, *oldUl, *oldMcDl, *newDl, *newUl, *newMcDl;

      oldDl = oldTargetNode->GetPhy ()->GetDlChannel ();
      oldMcDl = oldTargetNode->GetPhy ()->GetDlMcChannel ();
      oldUl = oldTargetNode->GetPhy ()->GetUlChannel ();
      newDl = newTargetNode->GetPhy ()->GetDlChannel ();
      newMcDl = newTargetNode->GetPhy ()->GetDlMcChannel ();
      newUl = newTargetNode->GetPhy ()->GetUlChannel ();

      if (newDl != nullptr && oldDl != nullptr)
        {
          ue->GetPhy ()->SetDlChannel (newDl);
          if (oldDl->IsAttached (ue))
            {
              newDl->AddDevice (ue);
              oldDl->DelDevice (ue);
            }
          if (newDl->GetPropagationLossModel () != nullptr)
            {
              if(newDl->GetPropagationLossModel ()->GetChannelRealization(newTargetNode,ue) == nullptr)
                {
                  newDl->GetPropagationLossModel ()->AddChannelRealization (CreateChannelRealization (newTargetNode, ue));
                  oldDl->GetPropagationLossModel ()->DelChannelRealization (oldTargetNode,ue);
                }
              newDl->GetPropagationLossModel ()->GetChannelRealization(newTargetNode, ue)->enableFastFading();
              if (oldDl->GetPropagationLossModel ()->GetChannelRealization (oldTargetNode,ue) != nullptr)
                {
                  oldDl->GetPropagationLossModel ()->GetChannelRealization (oldTargetNode,ue)->disableFastFading();
                }
            }
        }

      if (newMcDl != nullptr && oldMcDl != nullptr)
        {
          ue->GetPhy ()->SetDlMcChannel(newMcDl);
          if (oldMcDl->IsAttached (ue))
            {
              newMcDl->AddDevice (ue);
              oldMcDl->DelDevice (ue);
            }
          if (newMcDl->GetPropagationLossModel () != nullptr)
            {
              if(newMcDl->GetPropagationLossModel ()->GetChannelRealization(newTargetNode,ue) == nullptr)
                {
                  newMcDl->GetPropagationLossModel ()-> AddChannelRealization (CreateChannelRealization (newTargetNode, ue));
                  oldMcDl->GetPropagationLossModel ()->DelChannelRealization (oldTargetNode,ue);
                }
            }
        }

      if (newUl != nullptr && oldUl != nullptr)
        {
          ue->GetPhy ()->SetUlChannel (newUl);
          if (oldUl->IsAttached (ue))
            {
              newUl->AddDevice (ue);
              oldUl->DelDevice (ue);
            }
          if (newUl->GetPropagationLossModel () != nullptr && UPLINK)
            {
              if(newUl->GetPropagationLossModel ()->GetChannelRealization(newTargetNode,ue) == nullptr)
                {
                  newUl->GetPropagationLossModel ()->AddChannelRealization (CreateChannelRealization (ue, newTargetNode));

                  oldUl->GetPropagationLossModel ()->DelChannelRealization (ue,oldTargetNode);
                }
              newUl->GetPropagationLossModel ()->GetChannelRealization(ue, newTargetNode)->enableFastFading();
              if (oldUl->GetPropagationLossModel ()->GetChannelRealization (ue, oldTargetNode) != nullptr)
                {
                  oldUl->GetPropagationLossModel ()->GetChannelRealization (ue, oldTargetNode)->disableFastFading();
                }
            }
        }


      // 2 - add ue record to the new Gnb
DEBUG_LOG_START_1(SIM_ENV_HANDOVER_DEBUG)
      cout << "add ue record to the new Gnb"<< endl;
DEBUG_LOG_END
      newTargetNode->RegisterUserEquipment (ue);

      // 3 - delete ue record form the old gnb
DEBUG_LOG_START_1(SIM_ENV_HANDOVER_DEBUG)
      cout << "delete ue record form the old gnb"<< endl;
DEBUG_LOG_END
      oldTargetNode->DeleteUserEquipment (ue);

      // 4 - update cell and new target gnb for the ue
DEBUG_LOG_START_1(SIM_ENV_HANDOVER_DEBUG)
      cout << "update cell and new target gnb for the ue"<< endl;
DEBUG_LOG_END
      ue->SetTargetNode (newTargetNode);

      // MOVE RRC CONTEXT FOR THE OLD TARGET NODE TO THE NEWER ONE
DEBUG_LOG_START_1(SIM_ENV_HANDOVER_DEBUG)
      cout << "MOVE RRC CONTEXT"<< endl;
DEBUG_LOG_END
      RrcEntity * oldGnbRrc = oldTargetNode->GetProtocolStack ()->GetRrcEntity ();
      RrcEntity * newGnbRrc = newTargetNode->GetProtocolStack ()->GetRrcEntity ();
      RrcEntity * ueRrc = ue->GetProtocolStack  ()->GetRrcEntity ();


      // --> move dl bearers
DEBUG_LOG_START_1(SIM_ENV_HANDOVER_DEBUG)
      cout << "\t DL radio bearers " << oldGnbRrc->GetRadioBearerContainer ()->size () << endl;
DEBUG_LOG_END
      vector<RadioBearer* > *dlBearerToDelete = new vector<RadioBearer* > ();
      for (auto bearer : *oldGnbRrc->GetRadioBearerContainer ())
        {
          if (bearer->GetDestination ()->GetIDNetworkNode () == ue->GetIDNetworkNode ())
            {
              bearer->SetSource (newTargetNode);
              bearer->GetRlcEntity ()->SetDevice (newTargetNode);
              newGnbRrc->AddRadioBearer (bearer);
              dlBearerToDelete->push_back (bearer);
            }
        }
      for (auto bearer : *dlBearerToDelete)
        {
          oldGnbRrc->DelRadioBearer (bearer);
        }
      dlBearerToDelete->clear ();
      delete dlBearerToDelete;



      // --> move ul bearers
DEBUG_LOG_START_1(SIM_ENV_HANDOVER_DEBUG)
      cout << "\t UL radio bearers"<< endl;
DEBUG_LOG_END
      vector<RadioBearerSink* > *ulBearerToDelete = new vector<RadioBearerSink* > ();
      for (auto bearer : *oldGnbRrc->GetRadioBearerSinkContainer ())
        {
          if (bearer->GetSource ()->GetIDNetworkNode () == ue->GetIDNetworkNode ())
            {
              bearer->SetDestination (newTargetNode);
              newGnbRrc->AddRadioBearerSink (bearer);
              ulBearerToDelete->push_back (bearer);
            }
        }
      for (auto bearer : *ulBearerToDelete)
        {
          oldGnbRrc->DelRadioBearerSink (bearer);
        }
      ulBearerToDelete->clear ();
      delete ulBearerToDelete;


      // UPDATE THE RRC CONTEXT FOR THE UE
DEBUG_LOG_START_1(SIM_ENV_HANDOVER_DEBUG)
      cout << "\t UE updates DL radio bearers "<< endl;
DEBUG_LOG_END
      for (auto bearer : *ueRrc->GetRadioBearerSinkContainer ())
        {
          bearer->SetSource (newTargetNode);
        }
DEBUG_LOG_START_1(SIM_ENV_HANDOVER_DEBUG)
      cout << "\t UE updates UL radio bearers "<< endl;
DEBUG_LOG_END
      for (auto bearer : *ueRrc->GetRadioBearerContainer ())
        {
          bearer->SetDestination (newTargetNode);
        }
    }

}

vector <BandwidthManager*>
RunFrequencyReuse (int nodes, int cluster, double bandwidth)
{
  return RunFrequencyReuseTechniques (nodes, cluster, bandwidth);
}

void
NetworkManager::PrintUEsForEachCell (void)
{
  cout << "UEsForCell: ";
  for (auto record : *GetGNodeBContainer())
    {
      cout << " " << record->GetNbOfUserEquipmentRecords();
    }
  cout << endl;
}


/*
 *  DEBUG
 */
/*
void
NetworkManager::Print (void)
{
  cout << " TOTAL Network Manager Debug: " << endl;

  for (auto cell:*GetCellContainer ())
    {
      cell->Print();
    }

  for (auto gnb : *GetGNodeBContainer())
    {
      gnb->Print();
    }

  for (auto ue : *GetUserEquipmentContainer())
    {
      ue->Print();
    }
}
*/

void
NetworkManager::Print (NetworkManager::m_debugInfo info)
{
  cout << " Network Manager Debug: " << endl;

  switch (info)
    {
    case NetworkManager::ALL_NETWORK:
//      this->Print();
      cout << " TOTAL Network Manager Debug: " << endl;

      for (auto cell:*GetCellContainer ())
        {
          cell->Print();
        }

      for (auto gnb : *GetGNodeBContainer())
        {
          gnb->Print();
        }

      for (auto ue : *GetUserEquipmentContainer())
        {
          ue->Print();
        }

      break;

    case NetworkManager::ONLY_CELL_DETAILS:
      for (auto cell : *GetCellContainer())
        {
          cell->Print();
        }
      break;

    case NetworkManager::ONLY_GNODEB_DETAILS:
      for (auto gnb : *GetGNodeBContainer())
        {
          gnb->Print();
        }
      break;

    case NetworkManager::ONLY_USER_EQUIPMENT_DETAILS:
      for (auto ue : *GetUserEquipmentContainer())
        {
          ue->Print();
        }
      break;

    default:
    {
      cout << " INVALID NETWORK DEBUG CODE " << endl;
      break;
    }

    }
}

void
NetworkManager::PrintFrequencyMask (void)
{
  for (auto gNodeB : *GetGNodeBContainer())
    {
      cout << "ENODEB " <<  gNodeB->GetIDNetworkNode() << ", frequencyMask:" << endl;
    }
}

void
NetworkManager::PrintUserPosition (void)
{
  cout << " UserPosition X [at " << Simulator::Init()->Now() << "] ";
  for (auto user : *GetUserEquipmentContainer())
    {
      cout << user->GetMobilityModel ()->GetAbsolutePosition()->GetCoordinateX() << " ";
    }

  cout << "\n UserPosition Y [at " << Simulator::Init()->Now() << "] ";
  for (auto user : *GetUserEquipmentContainer())
    {
      cout << user->GetMobilityModel ()->GetAbsolutePosition()->GetCoordinateY() << " ";
    }
  cout << endl;
}

