/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of the Nedrysoft component system (https://github.com/nedrysoft/componentsystem)
 *
 * A cross-platform plugin system for Qt applications.
 *
 * Created by Adrian Carpenter on 04/12/2020.
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

#ifndef NEDRYSOFT_COMPONENTSYSTEM_IINTERFACE_H
#define NEDRYSOFT_COMPONENTSYSTEM_IINTERFACE_H

#include "ComponentSystemSpec.h"

#include <QObject>

namespace Nedrysoft { namespace ComponentSystem {
    /**
     * @brief       The IInterface class is the root level object for all objects.
     *
     * @details     All objects that reside in the component system should inherit this interface for future
     *              compatibility.
     *
     * @class       Nedrysoft::ComponentSystem::IInterface IInterface.h <IInterface>
     */
    class COMPONENT_SYSTEM_DLLSPEC IInterface :
            public QObject {

        private:
            Q_OBJECT

        public:
            /**
             * @brief   Destroys the IInterface.
             *
             */
            virtual ~IInterface() = default;

    };
}}

Q_DECLARE_INTERFACE(Nedrysoft::ComponentSystem::IInterface, "com.nedrysoft.IInterface/1.0.0")

#endif // NEDRYSOFT_COMPONENTSYSTEM_IINTERFACE_H
