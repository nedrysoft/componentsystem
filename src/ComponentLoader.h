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

#ifndef NEDRYSOFT_COMPONENTSYSTEM_COMPONENTLOADER_H
#define NEDRYSOFT_COMPONENTSYSTEM_COMPONENTLOADER_H

#include "ComponentSystemSpec.h"

#include <QList>
#include <QMap>
#include <QObject>
#include <QPair>
#include <functional>

class QPluginLoader;

namespace Nedrysoft { namespace ComponentSystem {
    class Component;

    /**
     * @brief       The ComponentLoader loads the discovered components.
     *
     * @details     Generic plugin loader, finds compatible components in a given folder and using the metadata
     *              ensures that any dependencies are available and that all dependencies are loaded in the correct
     *              order.
     *
     * @class       Nedrysoft::ComponentSystem::ComponentLoader ComponentLoader.h <ComponentLoader>
     */
    class COMPONENT_SYSTEM_DLLSPEC ComponentLoader :
            public QObject {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       The load status flags.
             *
             * @details     Bit flags for load status.
             */
            enum LoadFlag {
                Unloaded = 0,
                Loaded = 1,
                IncompatibleQtVersion = 2,
                NameClash = 4,
                MissingDependency = 8,
                Disabled = 16,
                IncompatibleVersion = 32,
                UnableToLoad = 64,
                MissingInterface = 128
            };
            Q_ENUM(LoadFlag)
            Q_DECLARE_FLAGS(LoadFlags, LoadFlag)
            Q_FLAGS(LoadFlags)

        public:
            /**
             * @brief       Constructs a ComponentLoader which is a child of the parent.
             *
             * @param[in]   parent the parent object.
             */
            explicit ComponentLoader(QObject *parent = nullptr);

            /**
             * @brief       Destroys the ComponentLoader.
             */
            ~ComponentLoader();

            /**
             * @brief       Add all components in the given folder to the load list.
             *
             * @details     Searches the given directory and adds any loadable components to the list of components
             *              to be loaded.
             *
             * @param[in]   componentFolder the search folder.
             */
            auto addComponents(const QString &componentFolder) -> void;

            /**
             * @brief       Loads all discovered components.
             *
             * @param[in]   loadFunction the load function is a callback that allows the application to selectively
             *              load components, i.e the user can disable certain components.
             */
            auto loadComponents(std::function<bool(Nedrysoft::ComponentSystem::Component *)> loadFunction = nullptr) -> void;

            /**
             * @brief       Returns the list of all discovered components.
             *
             * @details     Returns the list of components that were found, the state of whether the component was
             *              loaded is updated along with an error code for each component if a component could not
             *              be loaded.
             *
             * @returns     the list of components.
             *
             */
            auto components() -> QList<Component *>;

            /**
             * @brief       Unloads all loaded components.
             */
            auto unloadComponents() -> void;

        private:
            /**
             * @brief       Resolves the dependencies of the loaded components.
             *
             * @details     For a given component, returns a list of components in the order that they must be loaded
             *              in order to satisfy all component and sub component dependencies.
             *
             * @param[in]   component the component to resolve.
             * @param[out]  resolvedList the ordered list of components.
             */
            auto resolve(Nedrysoft::ComponentSystem::Component *component,
                         QList<Nedrysoft::ComponentSystem::Component *> &resolvedList) -> void;

            /**
             * @brief           Resolves the dependencies of the loaded components.
             *
             * @details         For a given component, returns a list of components in the order that they must be
             *                  loaded in order to satisfy all component and sub component dependencies.
             *
             *                  This overload uses a list to mark nodes as already processed, this allows us to detect
             *                  circular references.
             *
             * @param[in]       component the component to resolve.
             * @param[in,out]   processedList list of nodes that have already been processed.
             * @param[out]      resolvedList ordered list of components.
             */
            auto resolve(Nedrysoft::ComponentSystem::Component *component,
                         QList<Nedrysoft::ComponentSystem::Component *> &resolvedList,
                         QList<Nedrysoft::ComponentSystem::Component *> &processedList) -> void;

            /**
             * @brief       Returns a string containing the flags that were set.
             *
             * @param[in]   flags the flags value to be converted to a string.
             *
             * @returns     a string containing the list of flags that were set.
             */
            auto loadFlagString(Nedrysoft::ComponentSystem::ComponentLoader::LoadFlags flags) -> QString;

        private:
            //! @cond

            QList<QPair<QPluginLoader *, Nedrysoft::ComponentSystem::Component *> > m_loadOrder;
            QMap<QString, Nedrysoft::ComponentSystem::Component *> m_componentSearchList;

            //! @endcond
    };
}}

Q_DECLARE_OPERATORS_FOR_FLAGS(Nedrysoft::ComponentSystem::ComponentLoader::LoadFlags)
Q_DECLARE_METATYPE(Nedrysoft::ComponentSystem::ComponentLoader::LoadFlags)
Q_DECLARE_METATYPE(Nedrysoft::ComponentSystem::ComponentLoader::LoadFlag)

#endif // NEDRYSOFT_COMPONENTSYSTEM_COMPONENTLOADER_H
