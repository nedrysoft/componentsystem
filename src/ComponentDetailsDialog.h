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

#ifndef NEDRYSOFT_COMPONENTSYSTEM_COMPONENTDETAILSDIALOG_H
#define NEDRYSOFT_COMPONENTSYSTEM_COMPONENTDETAILSDIALOG_H

#include "Component.h"

#include "ComponentSystemSpec.h"

#include <QDialog>

namespace Nedrysoft { namespace ComponentSystem {
    namespace Ui {
        class ComponentDetailsDialog;
    }

    /**
     * @brief       The ComponentDetailsDialog displays the metadata for a component.
     *
     * @class       Nedrysoft::ComponentSystem::ComponentDetailsDialog ComponentDetailsDialog.h <ComponentDetailsDialog>
     */
    class COMPONENT_SYSTEM_DLLSPEC ComponentDetailsDialog :
            public QDialog {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Constructs a new ComponentDetailsDialog.
             *
             * @param[in]   component the component instance to be viewed.
             * @param[in]   parent the parent widget.
             */
            explicit ComponentDetailsDialog(
                    Nedrysoft::ComponentSystem::Component *component,
                    QWidget *parent = nullptr );

            /**
             * @brief       Destroys the ComponentDetailsDialog.
             */
            ~ComponentDetailsDialog();

        private:
            //! @cond

            Ui::ComponentDetailsDialog *ui;

            //! @endcond
    };
}}

#endif // NEDRYSOFT_COMPONENTSYSTEM_COMPONENTDETAILSDIALOG_H
