#
# Copyright (C) 2020 Adrian Carpenter
#
# This file is part of the Nedrysoft component system (https://github.com/nedrysoft/componentsystem)
#
# A cross-platform plugin system for Qt applications.
#
# Created by Adrian Carpenter on 03/12/2020.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

cmake_policy(SET CMP0077 NEW)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_definitions(-DQT_NO_KEYWORDS)

add_definitions(-DNEDRYSOFT_LIBRARY_COMPONENTSYSTEM_EXPORT)

include_directories(${CMAKE_BINARY_DIR})    # <- allows ui_ files to be available to the editor

# create the component system library

project(ComponentSystem)

find_package(Qt5 COMPONENTS Core REQUIRED)

set(library_SOURCES
    includes/ComponentSystem/Component.h
    includes/ComponentSystem/ComponentLoader.h
    includes/ComponentSystem/ComponentSystemSpec.h
    includes/ComponentSystem/IComponent.h
    includes/ComponentSystem/IComponentManager.h
    includes/ComponentSystem/IInterface.h
    src/Component.cpp
    src/ComponentLoader.cpp
    src/IComponent.cpp
    src/IComponentManager.cpp
)

if(WIN32)
    configure_file("Version.h.in" "Version.h")
    list(APPEND library_SOURCES "version.rc")
    add_definitions("-DNEDRYSOFT_MODULE_FILENAME=\"${PROJECT_NAME}.dll\"")
    add_definitions("-DNEDRYSOFT_MODULE_NAME=\"${PROJECT_NAME}\"")
endif()

set(Qt_LIBS "Qt5::Core")

include_directories("src" "includes")

option(NEDRYSOFT_COMPONENTSYSTEM_COMPONENTVIEWER "Include Component Viewer" OFF)

if (NEDRYSOFT_COMPONENTSYSTEM_COMPONENTVIEWER)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)

    set(Viewer_SOURCES
        includes/ComponentSystem/ComponentDetailsDialog.h
        includes/ComponentSystem/ComponentViewerDialog.h
        src/ComponentDetailsDialog.cpp
        src/ComponentDetailsDialog.ui
        src/ComponentViewerDialog.cpp
        src/ComponentViewerDialog.ui
        src/ComponentSystemFontAwesome.cpp
        src/ComponentSystemFontAwesome.h
        src/ComponentSystemFontAwesome.qrc
    )

    list(APPEND Qt_LIBS "Qt5::Widgets")
else()
    set(Viewer_SOURCES "" ../../common/spdlog.h)
endif()

add_library(${PROJECT_NAME} SHARED
    ${library_SOURCES}
    ${Viewer_SOURCES}
)

if(DEFINED NEDRYSOFT_COMPONENTSYSTEM_LIBRARY_DIR)
    set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${NEDRYSOFT_COMPONENTSYSTEM_LIBRARY_DIR}")
    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${NEDRYSOFT_COMPONENTSYSTEM_LIBRARY_DIR}")
else()
    message(STATUS "Set NEDRYSOFT_COMPONENTSYSTEM_LIBRARY_DIR to set the binary output dir.")
endif()

target_link_libraries(${PROJECT_NAME} ${Qt_LIBS})
