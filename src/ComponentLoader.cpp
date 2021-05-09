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

#include <QtGlobal>

#include "includes/ComponentSystem/ComponentLoader.h"

#include "includes/ComponentSystem/Component.h"
#include "includes/ComponentSystem/IComponent.h"

#include <QDirIterator>
#include <QJsonArray>
#include <QLibrary>
#include <QLibraryInfo>
#include <QMap>
#include <QMetaEnum>
#include <QPluginLoader>

#include "spdlog.h"

constexpr unsigned int QtMajorBitMask = 0xFFFF0000;
constexpr unsigned int QtMajorBitShift = 16;
constexpr unsigned int QtMinorBitMask = 0x0000FF00;
constexpr unsigned int QtMinorBitShift = 8;
constexpr unsigned int QtPatchBitMask = 0x000000FF;
constexpr unsigned int QtPatchBitShift = 0;

Nedrysoft::ComponentSystem::ComponentLoader::ComponentLoader(QObject *parent) :
        QObject(parent) {

}

Nedrysoft::ComponentSystem::ComponentLoader::~ComponentLoader() {
    unloadComponents();
}

auto Nedrysoft::ComponentSystem::ComponentLoader::addComponents(const QString &componentFolder) -> void {
    auto applicationDebugBuild = QLibraryInfo::isDebugBuild();
#if QT_VERSION >= QT_VERSION_CHECK(5, 8, 0)
    auto applicationQtVersion = QLibraryInfo::version();
#else
    auto applicationQtVersion = QVersionNumber::fromString(qVersion());
#endif

#if defined(Q_OS_UNIX) || (( defined(Q_OS_WIN) && defined(__MINGW32__)))
#if defined(QT_DEBUG)
    if (!applicationDebugBuild) {
        SPDLOG_WARN(QString("Application was built with QT_DEBUG but has loaded RELEASE qt libraries, component system will load DEBUG components instead.").toStdString());

        applicationDebugBuild = true;
    }
#else
    if (applicationDebugBuild) {
        SPDLOG_WARN(tr("Application was built with QT_NO_DEBUG but has loaded DEBUG qt libraries, component system will load RELEASE components instead.").toStdString());

        applicationDebugBuild = false;
    }
#endif
#endif
    SPDLOG_INFO(QString("Searching folder for components %1").arg(componentFolder).toStdString());

    QDirIterator dir(componentFolder);

    // find compatible components, and create a list of components to consider for loading

    while (dir.hasNext()) {
        dir.next();

        auto componentFilename = dir.fileInfo().absoluteFilePath();

        if (!QLibrary::isLibrary(componentFilename)) {
            continue;
        }

        auto pluginLoader = new QPluginLoader(componentFilename);

        SPDLOG_INFO(QString("Found Component %1").arg(componentFilename).toStdString());

        auto metaDataObject = pluginLoader->metaData();

        if (metaDataObject.isEmpty()) {
            delete pluginLoader;

            continue;
        }

        auto componentMetadata = metaDataObject.value("MetaData");
        auto debugBuild = metaDataObject.value("debug");
        auto qtVersion = metaDataObject.value("version");

        if (debugBuild.isNull() || qtVersion.isNull() || componentMetadata.isNull()) {
            delete pluginLoader;

            continue;
        }

        if (debugBuild != applicationDebugBuild) {
            delete pluginLoader;

            continue;
        }

        auto componentName = componentMetadata.toObject().value("Name");

        if (componentName.isNull()) {
            delete pluginLoader;

            continue;
        }

        auto componentQtMajor = static_cast<int>(( qtVersion.toVariant().toUInt() & QtMajorBitMask )
                >> QtMajorBitShift);
        auto componentQtMinor = static_cast<int>(( qtVersion.toVariant().toUInt() & QtMinorBitMask )
                >> QtMinorBitShift);
        auto componentQtPatch = static_cast<int>(( qtVersion.toVariant().toUInt() & QtPatchBitMask )
                >> QtPatchBitShift);

        auto componentQtVersion = QVersionNumber(componentQtMajor, componentQtMinor, componentQtPatch);

        auto component = new Nedrysoft::ComponentSystem::Component(
                componentName.toString(),
                componentFilename,
                metaDataObject );

        if (componentQtVersion.majorVersion() != applicationQtVersion.majorVersion()) {
            component->m_loadFlags.setFlag(IncompatibleQtVersion);
        }

        if (m_componentSearchList.contains(componentName.toString())) {
            component->m_loadFlags.setFlag(NameClash);
        }

        m_componentSearchList[componentName.toString()] = component;

        delete pluginLoader;
    }
}

auto Nedrysoft::ComponentSystem::ComponentLoader::loadComponents(
        std::function<bool(Nedrysoft::ComponentSystem::Component *)> loadFunction) -> void {

    QList<Nedrysoft::ComponentSystem::Component *> componentLoadList;
    QList<Nedrysoft::ComponentSystem::Component *> resolvedLoadList;

    // find and add dependencies from the search

    auto componentIterator = QMapIterator<QString, Nedrysoft::ComponentSystem::Component *>(m_componentSearchList);

    while (componentIterator.hasNext()) {
        componentIterator.next();

        auto component = componentIterator.value();

        if (component->m_loadFlags) {
            continue;
        }

        auto metadata = component->metadata().value("MetaData").toObject();

        auto dependencies = metadata.value("Dependencies").toArray();

        for (auto dependency : dependencies) {
            auto dependencyName = dependency.toObject().value("Name").toString();
            auto dependencyVersion = dependency.toObject().value("Version").toString();

            if (m_componentSearchList.contains(dependencyName)) {
                component->addDependency(
                        m_componentSearchList[dependencyName],
                        QVersionNumber::fromString(dependencyVersion) );
            } else {
                component->m_missingDependencies.append(dependencyName);
                component->m_loadFlags |= MissingDependency;
            }
        }

        if (!component->m_loadFlags) {
            componentLoadList.append(component);
        }
    }

    // resolve the dependencies to create a load order

    for (auto current : componentLoadList) {
        QList<Nedrysoft::ComponentSystem::Component *> dependencyResolveList;

        if (!resolvedLoadList.contains(current)) {
            dependencyResolveList.clear();

            resolve(current, dependencyResolveList);

            for (auto dependency : dependencyResolveList) {
                if (!resolvedLoadList.contains(dependency)) {
                    resolvedLoadList.append(dependency);
                }
            }
        }
    }

    // load the components that we have satisfied dependencies for

    for (auto component : resolvedLoadList) {
        if (component->m_loadFlags) {
            SPDLOG_INFO(QString("component %1 was not loaded. (%2)").arg(component->name()).arg(loadFlagString(component->m_loadFlags)).toStdString());

            continue;
        }

        component->validateDependencies();

        if (component->m_loadFlags) {
            SPDLOG_INFO(QString("component %1 was not loaded. (%2)").arg(component->name()).arg(loadFlagString(component->m_loadFlags)).toStdString());

            continue;
        }

        if (loadFunction) {
            if (!loadFunction(component)) {
                component->m_loadFlags.setFlag(Nedrysoft::ComponentSystem::ComponentLoader::Disabled);

                SPDLOG_INFO(QString("component %1 was not loaded. (%2)").arg(component->name()).arg(loadFlagString(component->m_loadFlags)).toStdString());

                continue;
            }
        }

        // check if dependencies are loaded, if not then this component cannot be loaded

        auto pluginLoader = new QPluginLoader(component->filename());

        if (!pluginLoader->load()) {
            component->m_loadFlags.setFlag(Nedrysoft::ComponentSystem::ComponentLoader::UnableToLoad);

            SPDLOG_INFO(QString("component %1 was not loaded. (%2) [%3]").arg(component->name()).arg(loadFlagString(component->m_loadFlags)).arg(pluginLoader->errorString()).toStdString());

            delete pluginLoader;

            continue;
        }

        auto componentInterface = qobject_cast<Nedrysoft::ComponentSystem::IComponent *>(pluginLoader->instance());

        if (!componentInterface) {
            component->m_loadFlags.setFlag(Nedrysoft::ComponentSystem::ComponentLoader::MissingInterface);

            delete pluginLoader;

            SPDLOG_INFO(QString("component %1 was not loaded. (%2)").arg(component->name()).arg(loadFlagString(component->m_loadFlags)).toStdString());

            continue;
        }

        component->m_loadFlags.setFlag(Nedrysoft::ComponentSystem::ComponentLoader::Loaded);

        m_loadOrder.append(QPair<QPluginLoader *, Component *>(pluginLoader, component));

        component->m_isLoaded = true;
    }

    // call initialiseEvent for each component (in load order)

    for (auto componentPair : m_loadOrder) {
        auto componentInterface = qobject_cast<Nedrysoft::ComponentSystem::IComponent *>(
                componentPair.first->instance());

        componentInterface->initialiseEvent();
    }

    // call initialisationFinishedEvent for each component (in reverse load order)

    for (auto loadedComponentIterator = m_loadOrder.rbegin();
         loadedComponentIterator < m_loadOrder.rend(); loadedComponentIterator++) {
        auto componentInterface = qobject_cast<Nedrysoft::ComponentSystem::IComponent *>(
                loadedComponentIterator->first->instance());

        componentInterface->initialisationFinishedEvent();
    }
}

auto Nedrysoft::ComponentSystem::ComponentLoader::components() -> QList<Nedrysoft::ComponentSystem::Component *> {
    return m_componentSearchList.values();
}

auto Nedrysoft::ComponentSystem::ComponentLoader::resolve(Nedrysoft::ComponentSystem::Component *component,
        QList<Nedrysoft::ComponentSystem::Component *> &resolvedList) ->void {

    QList<Nedrysoft::ComponentSystem::Component *> processedList;

    resolve(component, resolvedList, processedList);
}

auto Nedrysoft::ComponentSystem::ComponentLoader::resolve(
        Nedrysoft::ComponentSystem::Component *component,
        QList<Nedrysoft::ComponentSystem::Component *> &resolvedList,
        QList<Nedrysoft::ComponentSystem::Component *> &processedList) -> void {

    processedList.append(component);

    for (auto dependency : component->m_dependencies) {
        if (!resolvedList.contains(dependency)) {
            if (processedList.contains(dependency)) {
                continue;
            }

            resolve(dependency, resolvedList, processedList);
        }
    }

    resolvedList.append(component);
}

auto Nedrysoft::ComponentSystem::ComponentLoader::unloadComponents() -> void {
    for (auto loadedComponentIterator = m_loadOrder.rbegin();
        loadedComponentIterator < m_loadOrder.rend(); loadedComponentIterator++) {

        auto pluginLoader = qobject_cast<QPluginLoader *>(loadedComponentIterator->first);

        auto componentInterface = qobject_cast<Nedrysoft::ComponentSystem::IComponent *>(pluginLoader->instance());

        if (!componentInterface) {
            continue;
        }

        componentInterface->finaliseEvent();

        if (pluginLoader) {
            // TODO: crash under macOS, may be sql related.
            
            //pluginLoader->unload();
            delete pluginLoader;
        }
    }

    m_loadOrder.clear();
}

auto Nedrysoft::ComponentSystem::ComponentLoader::loadFlagString(
        Nedrysoft::ComponentSystem::ComponentLoader::LoadFlags flags) -> QString {

    auto metaEnum = QMetaEnum::fromType<Nedrysoft::ComponentSystem::ComponentLoader::LoadFlag>();

    return QString::fromLatin1(metaEnum.valueToKeys(flags)).replace("|"," | ");
}
