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

#ifndef NEDRYSOFT_COMPONENTSYSTEM_COMPONENT_H
#define NEDRYSOFT_COMPONENTSYSTEM_COMPONENT_H

#include "ComponentSystemSpec.h"
#include "ComponentLoader.h"

#include <QDataStream>
#include <QJsonObject>
#include <QList>
#include <QMap>
#include <QString>
#include <QVersionNumber>

namespace Nedrysoft { namespace ComponentSystem {
    /**
     * @brief       The Component class holds the information about a discovered component.
     *
     * @class       Nedrysoft::ComponentSystem::Component Component.h <Component>
     */
    class COMPONENT_SYSTEM_DLLSPEC Component {
        public:
            /**
             * @brief       Constructs a new Component.
             */
            Component();

            /**
             * @brief       Constructs a new Component with the given information.
             *
             * @param[in]   name the name of the component.
             * @param[in]   filename the filename of the component.
             * @param[in]   metadata the metadata retrieved from the component file.
             */
            Component(const QString &name, const QString &filename, const QJsonObject &metadata);

            /**
             * @brief       Adds a component dependency to this component.
             *
             * @param[in]   dependency      The required dependency
             * @param[in]   versionNumber   The required dependency version
             */
            auto addDependency(Component *dependency, QVersionNumber versionNumber) -> void;

            /**
             * @brief       Returns the name of the component.
             *
             * @returns     the component name.
             */
            auto name() -> QString;

            /**
             * @brief       Returns the file name of the component.
             *
             * @returns     the component filename.
             *
             */
            auto filename() -> QString;

            /**
             * @brief       Returns the decoded metadata for the component as a JSON object.
             *
             * @returns     The component metadata.
             */
            auto metadata() -> QJsonObject;

            /**
             * @brief       Returns where the component could be loaded.
             *
             * @details     A component may fail loading if it's dependencies could not be resolved or if a shared
             *              library is missing preventing the component from being loaded.
             *
             * @returns     true if the component is loaded; otherwise false.
             *
             */
            auto isLoaded() -> bool;

            /**
             * @brief       Returns the load status of the component.
             *
             * @details     Returns the bit flags of (ComponentLoader::LoadStatus values) the load status.
             *
             * @returns     the bit field containing information about the load state of the component.
             *
             */
            auto loadStatus() -> int;

            /**
             * @brief       Returns a list of missing dependencies.
             *
             * @details     The list of any missing dependencies, as these will not be available from the
             *              ComponentLoader, this can be used to show the names of any missing dependencies.
             *
             * @returns     The list of missing dependencies.
             */
            auto missingDependencies() -> QStringList;

            /**
             * @brief       Returns the version of the component.
             *
             * @returns     the component version.
             */
            auto version() -> QVersionNumber;

            /**
             * @brief       Returns the version of the component as a formatted string.
             *
             * @returns     the formatted version string.
             */
            auto versionString() -> QString;

            /**
             * @brief       Returns the reverse dns identifier of the component.
             *
             * @returns     the identifier.
             *
             */
            auto identifier() -> QString;

            /**
             * @brief       Returns the category that this component belongs to.
             *
             * @returns     the category of the component.
             */
            auto category() -> QString;

            /**
             * @brief       Returns the vendor of the component.
             *
             * @returns     the vendor.
             *
             */
            auto vendor() -> QString;

            /**
             * @brief       Returns the license text of the component.
             *
             * @returns     the license text.
             */
            auto license() -> QString;

            /**
             * @brief       Returns the copyright information for the component.
             *
             * @returns     the copyright text.
             *
             */
            auto copyright() -> QString;

            /**
             * @brief       Returns the description of the component.
             *
             * @returns     the description text.
             *
             */
            auto description() -> QString;

            /**
             * @brief       Returns the url for the component.
             *
             * @returns     the URL.
             *
             */
            auto url() -> QString;

            /**
             * @brief       Returns the list of dependencies as a string.
             *
             * @returns     the dependencies.
             *
             */
            auto dependencies() -> QString;

            /**
             * @brief       Returns whether the component can be disabled or not.
             *
             * @details     Components such as Core cannot be disabled as they are critical for the application.
             *
             * @returns     true if the component can be disabled; otherwise false.
             *
             */
            auto canBeDisabled() -> bool;

            /**
             * @brief       Validates the dependencies.
             *
             * @details     Validates all dependencies to ensure they are loaded and ensures that the loaded version
             *              meets our minimum requirement.
             */
            auto validateDependencies() -> void;

            friend class ComponentLoader;

        private:
            //! @cond

            QString m_name;
            QString m_filename;
            QList<Nedrysoft::ComponentSystem::Component *> m_dependencies;
            QJsonObject m_metadata;
            bool m_isLoaded;
            Nedrysoft::ComponentSystem::ComponentLoader::LoadFlags m_loadFlags;
            QList<QString> m_missingDependencies;
            QMap<Nedrysoft::ComponentSystem::Component *, QVersionNumber> m_dependencyVersions;

            //! @endcond
    };
}}

Q_DECLARE_METATYPE(Nedrysoft::ComponentSystem::Component *)

#endif // NEDRYSOFT_COMPONENTSYSTEM_COMPONENT_H
