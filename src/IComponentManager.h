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

#ifndef NEDRYSOFT_COMPONENTSYSTEM_ICOMPONENTMANAGER_H
#define NEDRYSOFT_COMPONENTSYSTEM_ICOMPONENTMANAGER_H

#include "ComponentSystemSpec.h"
#include "IComponent.h"

#include <QObject>

namespace Nedrysoft { namespace ComponentSystem {
    /**
     * @brief       The IComponentManager defines the contract for a class to manage loaded components.
     *
     * @details     In addition to handling the management of components, this class also provides a global
     *              registry for components.
     *
     * @class       Nedrysoft::ComponentSystem::IComponentManager IComponentManager.h <IComponentManager>
     */
    class COMPONENT_SYSTEM_DLLSPEC IComponentManager :
            public QObject {

        private:
            Q_OBJECT

        private:
            /**
             * @brief       `Constructs a new IComponentManager.
             */
            IComponentManager() = default;

            /**
             * @brief       Destroys the IComponentManager.
             */
            ~IComponentManager();

        public:
            /**
             * @brief       Add an object to the object registry.
             *
             * @param[in]   object object to store.
             */
            auto addObject(QObject *object) -> void;

            /**
             * @brief       Removes an object to the object registry.
             *
             * @param[in]   object object to remove.
             */
            auto removeObject(QObject *object) -> void;

            /**
             * @brief       Returns a list of all objects in the registry.
             *
             * @returns     returns a list of all objects.
             */
            auto allObjects() -> QList<QObject *>;

            /**
             * @brief       Returns the singleton instance to the ComponentManager object.
             *
             * @returns     the singleton instance
             */
            static auto getInstance() -> IComponentManager *;

        private:
            QList<QObject *> m_objectList;
    };
}}

/**
 * @brief       Convenience functions to manipulate the object registry.
 *
 * @code(.cpp)
 *              Nedrysoft::ComponentSystem::addObject(object);
 *
 *              QList<QObject *> objectList = Nedrysoft::ComponentSystem::allObjects();
 *
 *              auto object = Nedrysoft::ComponentSystem::getObject<IInterface>();
 *
 *              QList<IInterface *> objectList = Nedrysoft::ComponentSystem::getObjects<IInterface>();
 * @endcode
 */
namespace Nedrysoft { namespace ComponentSystem {
    /**
     * @brief       Adds an object to the registry.
     *
     * @param[in]   object the object to add to the registry.
     */
    inline auto addObject(QObject *object) -> void {
        IComponentManager::getInstance()->addObject(object);
    }

    /**
     * @brief       Removes an object to the registry.
     *
     * @param[in]   object the object to remove from the registry.
     */
    inline auto removeObject(QObject *object) -> void{
        IComponentManager::getInstance()->removeObject(object);
    }

    /**
     * @brief       Returns all registered objects.
     *
     * @returns     the list of objects.
     */
    inline auto allObjects() -> QList<QObject *> {
        return IComponentManager::getInstance()->allObjects();
    }

    /**
     * @brief       Returns the first matching object of type T.
     *
     * @returns     the object of type T.
     */
    template<typename T>
    inline auto getObject() -> T* {
        for (auto object : IComponentManager::getInstance()->allObjects()) {

            auto castObject = qobject_cast<T *>(object);

            if (castObject)
                return castObject;
        }

        return nullptr;
    }

    /**
     * @brief       Returns all objects that implement type T.
     *
     * @returns     the list of objects implementing type T.
     */
    template<typename T>
    inline auto getObjects() -> QList<T *> {
        QList<T *> objectList;

        for (auto object : IComponentManager::getInstance()->allObjects()) {

            auto castObject = qobject_cast<T *>(object);

            if (castObject)
                objectList.append(castObject);
        }

        return objectList;
    }
}}

#endif // NEDRYSOFT_COMPONENTSYSTEM_ICOMPONENTMANAGER_H
