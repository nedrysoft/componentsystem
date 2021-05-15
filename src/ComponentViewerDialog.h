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

#ifndef NEDRYSOFT_COMPONENTSYSTEM_COMPONENTVIEWERDIALOG_H
#define NEDRYSOFT_COMPONENTSYSTEM_COMPONENTVIEWERDIALOG_H

#include "ComponentSystemSpec.h"

#include <QDialog>
#include <QTreeWidgetItem>

namespace Nedrysoft { namespace ComponentSystem {
    namespace Ui {
        class ComponentViewerDialog;
    }

    class FontAwesome;

    /**
     * @brief       The ComponentViewerDialog provides a dialog which shows all discovered components.
     *
     * @brief       Dialog that shows the list of components detected by the system (although not necessarily loaded).
     *              Allows further detailed information to be displayed.
     *
     * @class       Nedrysoft::ComponentSystem::ComponentViewerDialog ComponentViewerDialog.h <ComponentViewerDialog>
     */
    class COMPONENT_SYSTEM_DLLSPEC ComponentViewerDialog :
            public QDialog {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Constructs a new ComponentViewerDialog which is child of the parent.
             *
             * @param[in]   parent the parent widget.
             */
            explicit ComponentViewerDialog(QWidget *parent = nullptr);

            /**
             * @brief       Destroys the ComponentViewerDialog.
             */
            ~ComponentViewerDialog();

            /**
             * @brief       Returns the list of disabled components.
             *
             * @details     The dialog allows components to be enabled and disabled from being loaded at the
             *              next startup of the software.  This function returns a list of disabled components
             *              in a reverse domain name format which should be stored and queried by the loader
             *              at startup.
             *
             * @returns     the list of disabled components.
             */
            auto disabledComponents() -> QStringList;

        private:

            /**
             * @brief       Component List double click handler.
             *
             * @details     Opens the ComponentDetailsDialog for the given component that has been double clicked.
             *
             * @param[in]   item the item that was double clicked.
             * @param[in]   column the column that was double clicked.
             */
            Q_SLOT void on_componentsTreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

        private:
            //! @cond

            Ui::ComponentViewerDialog *ui;

            FontAwesome *m_fontAwesome;

            //! @endcond
    };
}}

#endif // NEDRYSOFT_COMPONENTSYSTEM_COMPONENTVIEWERDIALOG_H
