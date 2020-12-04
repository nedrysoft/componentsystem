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

#ifndef NEDRYSOFT_COMPONENTSYSTEM_COMPONENTLOADER_H
#define NEDRYSOFT_COMPONENTSYSTEM_COMPONENTLOADER_H

#include "ComponentSystemSpec.h"

#include <QList>
#include <QMap>
#include <QObject>
#include <QPair>

class QPluginLoader;

namespace Nedrysoft::ComponentSystem {
    class Component;

    /**
     * @brief       Component Loader
     *
     * @details     Generic plugin loader, finds compatible components in a given folder
     *              and using the metadatav ensures that any dependencies are available and
     *              that all dependencies are loaded in the correct order.
     *
     */
    class COMPONENT_SYSTEM_DLLSPEC ComponentLoader :
            public QObject {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       LoadStatus
             *
             * @details     Bit flags for load status
             *
             */
            enum LoadStatus {
                Loaded = 0,
                IncompatibleQtVersion = 1,
                NameClash = 2,
                MissingDependency = 4,
                Disabled = 8,
                IncompatibleVersion = 16,
                LoadStatus = 32,
                MissingInterface = 64
            };

        public:
            /**
             * @brief       Constructor
             *
             * @param[in]   parent the parent object
             *
             */
            explicit ComponentLoader(QObject *parent = nullptr);

            /**
             * @brief       Destructor
             *
             */
            ~ComponentLoader();

            /**
             * @brief       addComponents
             *
             * @details     Searches the given directory and adds any loadable
             *              components to the list of components to be loaded
             *
             * @param[in]   componentFolder         Search folder
             *
             */
            void addComponents(const QString &componentFolder);

            /**
             * @brief       loadComponents
             *
             * @details     Loads the components
             *
             * @param[in]   loadFunction
             */
            void loadComponents(std::function<bool(Nedrysoft::ComponentSystem::Component *)> loadFunction = nullptr);

            /**
             * @brief       components
             *
             * @details     Returns the list of components that were found, the state
             *              of whether the component was loaded is updated along with
             *              an error code for each component if a component could not
             *              be loaded.
             *
             * @return      List of components
             *
             */
            QList<Component *> components();

        private:
            /**
             * @brief       resolve
             *
             * @details     For a given component, returns a list of components in the order
             *              that they must be loaded in order to satisfy all component and
             *              sub component dependencies.
             *
             * @param[in]   component the component to resolve
             * @param[out]  resolvedList ordered list of components
             *
             */
            void resolve(Nedrysoft::ComponentSystem::Component *component,
                         QList<Nedrysoft::ComponentSystem::Component *> &resolvedList);

            /**
             * @brief           resolve
             *
             * @details         For a given component, returns a list of components in the order
             *                  that they must be loaded in order to satisfy all component and
             *                  sub component dependencies.
             *
             *                  This overload uses a list to mark nodes as already processed, this
             *                  allows us to detect circular references.
             *
             * @param[in]       component the component to resolve
             * @param[in,out]   processedList list of nodes that have already been processed
             * @param[out]      resolvedList ordered list of components
             *
             */
            void resolve(Nedrysoft::ComponentSystem::Component *component,
                         QList<Nedrysoft::ComponentSystem::Component *> &resolvedList,
                         QList<Nedrysoft::ComponentSystem::Component *> &processedList);

        private:
            QList<QPair<QPluginLoader *, Nedrysoft::ComponentSystem::Component *> > m_loadOrder;
            QMap<QString, Nedrysoft::ComponentSystem::Component *> m_componentSearchList;
    };
}

#endif // NEDRYSOFT_COMPONENTSYSTEM_COMPONENTLOADER_H