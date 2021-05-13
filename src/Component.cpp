/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of the Nedrysoft component system (https://github.com/nedrysoft/componentsystem)
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

#include "Component.h"

#include <QJsonArray>

Nedrysoft::ComponentSystem::Component::Component() :
        m_isLoaded(false),
        m_loadFlags(ComponentLoader::Unloaded) {

}

Nedrysoft::ComponentSystem::Component::Component(const QString &name, const QString &filename, const QJsonObject &metadata) :
        m_name(name),
        m_filename(filename),
        m_metadata(metadata),
        m_isLoaded(false),
        m_loadFlags(ComponentLoader::Unloaded) {

}

void Nedrysoft::ComponentSystem::Component::addDependency(Component *dependency, QVersionNumber versionNumber) {
    m_dependencyVersions[dependency] = std::move(versionNumber);
    m_dependencies.append(dependency);
}

auto Nedrysoft::ComponentSystem::Component::name() -> QString {
    return m_name;
}

auto Nedrysoft::ComponentSystem::Component::filename() -> QString {
    return m_filename;
}

auto Nedrysoft::ComponentSystem::Component::metadata() -> QJsonObject  {
    return m_metadata;
}

auto Nedrysoft::ComponentSystem::Component::isLoaded() -> bool {
    return m_isLoaded;
}

auto Nedrysoft::ComponentSystem::Component::loadStatus() ->int {
    return m_loadFlags;
}

auto Nedrysoft::ComponentSystem::Component::missingDependencies() -> QStringList {
    return m_missingDependencies;
}

auto Nedrysoft::ComponentSystem::Component::version() -> QVersionNumber {
    auto componentMetadata = m_metadata["MetaData"].toObject();
    auto componentVersion = componentMetadata["Version"].toString();

    return QVersionNumber::fromString(componentVersion);
}

auto Nedrysoft::ComponentSystem::Component::versionString() -> QString {
    auto componentMetadata = m_metadata["MetaData"].toObject();
    auto componentVersion = componentMetadata["Version"].toString();
    auto componentBranch = componentMetadata["Branch"].toString();
    auto componentRevision = componentMetadata["Revision"].toString();

    return QString("%1-%2 (%3)").arg(componentVersion).arg(componentBranch).arg(componentRevision);
}

auto Nedrysoft::ComponentSystem::Component::identifier() -> QString {
    auto componentMetadata = m_metadata["MetaData"].toObject();

    return ( componentMetadata["Name"].toString() + "." + componentMetadata["Vendor"].toString()).toLower();
}

auto Nedrysoft::ComponentSystem::Component::category() -> QString {
    auto componentMetadata = m_metadata["MetaData"].toObject();

    return componentMetadata["Category"].toString();
}

auto Nedrysoft::ComponentSystem::Component::vendor() -> QString {
    auto componentMetadata = m_metadata["MetaData"].toObject();

    return componentMetadata["Vendor"].toString();
}

auto Nedrysoft::ComponentSystem::Component::license() -> QString {
    QString licenseText;
    auto componentMetadata = m_metadata["MetaData"].toObject();

    auto licenseContent = componentMetadata["License"].toArray();

    for (auto object : licenseContent) {
        licenseText += object.toString();
    }

    return licenseText;
}

auto Nedrysoft::ComponentSystem::Component::copyright() -> QString {
    auto componentMetadata = m_metadata["MetaData"].toObject();

    return componentMetadata["Copyright"].toString();
}

auto Nedrysoft::ComponentSystem::Component::description() -> QString {
    QString descriptionText;
    auto componentMetadata = m_metadata["MetaData"].toObject();

    auto licenseContent = componentMetadata["Description"].toArray();

    for (auto object : licenseContent) {
        descriptionText += object.toString() + "\r\n";
    }

    return descriptionText;
}

auto Nedrysoft::ComponentSystem::Component::url() -> QString {
    auto componentMetadata = m_metadata["MetaData"].toObject();

    return componentMetadata["Url"].toString();
}

auto Nedrysoft::ComponentSystem::Component::dependencies() -> QString {
    QString dependencyText;
    auto componentMetadata = m_metadata["MetaData"].toObject();

    auto licenseContent = componentMetadata["Dependencies"].toArray();

    for (auto object : licenseContent) {
        auto dependency = object.toObject();

        dependencyText += QString("%1 (%2)\r\n").arg(dependency["Name"].toString()).arg(
                dependency["Version"].toString());
    }

    return dependencyText;
}

auto Nedrysoft::ComponentSystem::Component::canBeDisabled() -> bool {
    auto componentMetadata = m_metadata["MetaData"].toObject();

    if (componentMetadata.contains("CanBeDisabled")) {
        return componentMetadata["CanBeDisabled"].toBool();
    }

    return true;
}

auto Nedrysoft::ComponentSystem::Component::validateDependencies() -> void {
    for (auto dependency : m_dependencies) {
        if (!dependency->isLoaded()) {
            m_loadFlags |= Nedrysoft::ComponentSystem::ComponentLoader::MissingDependency;
        } else {
            if (dependency->version() < m_dependencyVersions[dependency]) {
                m_loadFlags |= Nedrysoft::ComponentSystem::ComponentLoader::IncompatibleVersion;
            }
        }
    }
}


