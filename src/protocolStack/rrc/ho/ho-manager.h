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


#ifndef HOMANAGER_H_
#define HOMANAGER_H_

class UserEquipment;
class GNodeB;

class HoManager
{
public:
    HoManager() = default;
    virtual ~HoManager() = default;
    
    virtual bool CheckHandoverNeed (UserEquipment* ue) = 0;
    //TODO: CHECK GD are you sure these go in this class?
    virtual bool CheckDetachTimeForTwin(UserEquipment* twin)=0;
    virtual void CalculatePower(UserEquipment* ue)=0;
    
    //for twin model
    void SetHandoverThresholdBefore(double thresholdBefore);
    void SetHandoverThresholdAfter(double thresholdAfter);
    double GetHandoverThresholdBefore(void);
    double GetHandoverThresholdAfter(void);
    
    GNodeB* m_target;
    
    void SetHandoverMargin(double margin);
    double GetHandoverMargin(void);
private:
    double m_handoverMargin;
    
    double m_handoverThresholdBefore;
    double m_handoverThresholdAfter;

};

#endif /* HOMANAGER_H_ */
