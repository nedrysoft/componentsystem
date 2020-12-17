/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of the Nedrysoft component system (https://github.com/fizzyade/componentsystem)
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

#ifndef NEDRYSOFT_COMPONENTSYSTEM_ICOMPONENT_H
#define NEDRYSOFT_COMPONENTSYSTEM_ICOMPONENT_H

#include "ComponentSystemSpec.h"

#include <QObject>

#define NedrysoftComponentInterfaceIID  "com.nedrysoft.IComponent/1.0"

namespace Nedrysoft::ComponentSystem {
    /**
     * @brief       The IComponent interface defines the contract for a loadable component.
     *
     * @details     Interface that a component must implement, the plugin system will call various functions to
     *              load and initialise the plugin at the appropriate time.
     */
    class COMPONENT_SYSTEM_DLLSPEC IComponent {
        public:
            /**
             * @brief       Destroys the IComponent.
             */
            virtual ~IComponent();

            /**
             * @brief       The initialiseEvent is called by the component loader to initialise the component.
             *
             * @details     Called by the component loader after all components have been loaded, called in load order.
             */
            virtual void initialiseEvent();

            /**
             * @brief       The initialisationFinishedEvent function is called by the component loader after all
             *              components have been initialised.
             *
             * @details     Called by the component loader after all components have been
             *              initialised, called in reverse load order.
             */
            virtual void initialisationFinishedEvent();
    };
}

Q_DECLARE_INTERFACE(Nedrysoft::ComponentSystem::IComponent, NedrysoftComponentInterfaceIID)

#endif // NEDRYSOFT_COMPONENTSYSTEM_ICOMPONENT_H
