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

#include "application-entity.h"

ApplicationEntity::ApplicationEntity()
{
  m_app_sources.clear();
  m_app_destinations.clear();
}

void
ApplicationEntity::InsertApplicationSource(Application* app)
{
  m_app_sources.push_back(app);
}

vector<Application*>*
ApplicationEntity::GetApplicationSources(void)
{
  return &m_app_sources;
}

void
ApplicationEntity::InsertApplicationDestinations(Application* app)
{
  m_app_destinations.push_back(app);
}

vector<Application*>*
ApplicationEntity::GetApplicationDestinations(void)
{
  return &m_app_destinations;
}
