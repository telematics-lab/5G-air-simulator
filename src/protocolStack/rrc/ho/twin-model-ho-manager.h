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
 * Author: Gabriele Roncella <gabriele.roncella@student.unisi.it>
 * Author: Dinesh Tamang <dinesh.tamang@student.unisi.it>
 */

#include "ho-manager.h"
#ifndef twin_model_ho_manager_h
#define twin_model_ho_manager_h



class TwinModelHoManager: public HoManager
{
public:
    TwinModelHoManager();
    virtual ~TwinModelHoManager() = default;
    
    virtual bool CheckHandoverNeed (UserEquipment* ue);
    virtual bool CheckDetachTimeForTwin (UserEquipment * twin);
    virtual void CalculatePower(UserEquipment* ue);
    
    void SetHandoverThresholdBefore(double thresholdBefore);
    void SetHandoverThresholdAfter(double thresholdAfter);
    double GetHandoverThresholdBefore(void);
    double GetHandoverThresholdAfter(void);
    
    
private:
    double m_handoverThresholdBefore;
    double m_handoverThresholdAfter;
};

#endif /* twin_model_ho_manager_h */


