/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of the Nedrysoft component system (https://github.com/fizzyade/componentsystem)
 *
 * A cross-platform plugin system for Qt applications.
 *
 * Created by Adrian Carpenter on 03/12/2020.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ComponentSystem/IComponentManager.h"

Nedrysoft::ComponentSystem::IComponentManager::~IComponentManager() = default;

auto Nedrysoft::ComponentSystem::IComponentManager::getInstance() -> Nedrysoft::ComponentSystem::IComponentManager * {
    static IComponentManager componentManager;

    return &componentManager;
}

auto Nedrysoft::ComponentSystem::IComponentManager::addObject(QObject *object) -> void {
    m_objectList.append(object);
}

auto Nedrysoft::ComponentSystem::IComponentManager::removeObject(QObject *object) -> void {
    m_objectList.removeAll(object);
}

auto Nedrysoft::ComponentSystem::IComponentManager::allObjects() -> QList<QObject *> {
    return m_objectList;
}
