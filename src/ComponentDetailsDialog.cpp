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

#include "ComponentDetailsDialog.h"

#include "ui_ComponentDetailsDialog.h"

Nedrysoft::ComponentSystem::ComponentDetailsDialog::ComponentDetailsDialog(
            Nedrysoft::ComponentSystem::Component *component,
            QWidget *parent ) :

        QDialog(parent),
        ui(new Nedrysoft::ComponentSystem::Ui::ComponentDetailsDialog) {

    ui->setupUi(this);

    ui->nameLineEdit->setText(component->name());
    ui->versionLineEdit->setText(component->versionString());
    ui->categoryLineEdit->setText(component->category());
    ui->vendorLineEdit->setText(component->vendor());
    ui->copyrightLineEdit->setText(component->copyright());
    ui->URLLineEdit->setText(component->url());
    ui->descriptionTextEdit->setText(component->description());
    ui->licenseTextEdit->setText(component->license());
    ui->dependenciesTextEdit->setText(component->dependencies());
    ui->locationLineEdit->setText(component->filename());
}

Nedrysoft::ComponentSystem::ComponentDetailsDialog::~ComponentDetailsDialog() {
    delete ui;
}
