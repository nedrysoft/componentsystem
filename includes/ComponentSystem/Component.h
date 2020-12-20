/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of the Nedrysoft component system (https://github.com/fizzyade/componentsystem)
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

namespace Nedrysoft::ComponentSystem {
    /**
     * @brief           The Component class holds the information about a discovered component.
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
            Component(QString name, QString filename, QJsonObject metadata);

            /**
             * @brief       Adds a component dependency to this component.
             *
             * @param[in]   dependency      The required dependency
             * @param[in]   versionNumber   The required dependency version
             */
            void addDependency(Component *dependency, QVersionNumber versionNumber);

            /**
             * @brief       Returns the name of the component.
             *
             * @returns     the component name.
             */
            QString name();

            /**
             * @brief       Returns the file name of the component.
             *
             * @returns     the component filename.
             *
             */
            QString filename();

            /**
             * @brief       Returns the decoded metadata for the component as a JSON object.
             *
             * @returns     The component metadata.
             */
            QJsonObject metadata();

            /**
             * @brief       Returns where the component could be loaded.
             *
             * @details     A component may fail loading if it's dependencies could not be resolved or if a shared
             *              library is missing preventing the component from being loaded.
             *
             * @returns     true if the component is loaded; otherwise false.
             *
             */
            bool isLoaded();

            /**
             * @brief       Returns the load status of the component.
             *
             * @details     Returns the bit flags of (ComponentLoader::LoadStatus values) the load status.
             *
             * @returns     the bit field containing information about the load state of the component.
             *
             */
            int loadStatus();

            /**
             * @brief       Returns a list of missing dependencies.
             *
             * @details     The list of any missing dependencies, as these will not be available from the
             *              ComponentLoader, this can be used to show the names of any missing dependencies.
             *
             * @returns     The list of missing dependencies.
             */
            QStringList missingDependencies();

            /**
             * @brief       Returns the version of the component.
             *
             * @returns     the component version.
             */
            QVersionNumber version();

            /**
             * @brief       Returns the version of the component as a formatted string.
             *
             * @return      the formatted version string.
             */
            QString versionString();

            /**
             * @brief       Returns the reverse dns identifier of the component.
             *
             * @returns     the identifier.
             *
             */
            QString identifier();

            /**
             * @brief       Returns the category that this component belongs to.
             *
             * @returns     the category of the component.
             */
            QString category();

            /**
             * @brief       Returns the vendor of the component.
             *
             * @returns     the vendor.
             *
             */
            QString vendor();

            /**
             * @brief       Returns the license text of the component.
             *
             * @returns     the license text.
             */
            QString license();

            /**
             * @brief       Returns the copyright information for the component.
             *
             * @returns     the copyright text.
             *
             */
            QString copyright();

            /**
             * @brief       Returns the description of the component.
             *
             * @returns     the description text.
             *
             */
            QString description();

            /**
             * @brief       Returns the url for the component.
             *
             * @returns     the URL.
             *
             */
            QString url();

            /**
             * @brief       Returns the list of dependencies as a string.
             *
             * @returns     the dependencies.
             *
             */
            QString dependencies();

            /**
             * @brief       Returns whether the component can be disabled or not.
             *
             * @details     Componetns such as Core cannot be disabled as they are critical for the application.
             *
             * @return      true if the component can be disabled; otherwise false.
             *
             */
            bool canBeDisabled();

            /**
             * @brief       Validates the dependencies.
             *
             * @details     Validates all dependencies to ensure they are loaded and ensures that the loaded version
             *              meets our minimum requirement.
             */
            void validateDependencies();

            friend class ComponentLoader;

        private:
            QString m_name;
            QString m_filename;
            QList<Nedrysoft::ComponentSystem::Component *> m_dependencies;
            QJsonObject m_metadata;
            bool m_isLoaded;
            Nedrysoft::ComponentSystem::ComponentLoader::LoadFlags m_loadFlags;
            QList<QString> m_missingDependencies;
            QMap<Nedrysoft::ComponentSystem::Component *, QVersionNumber> m_dependencyVersions;
    };
}

Q_DECLARE_METATYPE(Nedrysoft::ComponentSystem::Component *)

#endif // NEDRYSOFT_COMPONENTSYSTEM_COMPONENT_H
