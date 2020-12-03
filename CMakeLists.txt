#
# Copyright (C) 2020 Adrian Carpenter
#
# This file is part of the Nedrysoft component system (https://github.com/fizzyade/componentsystem)
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

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

ADD_DEFINITIONS(-DQT_NO_KEYWORDS)

set(NEDRYSOFT_LIBRARY_COMPONENTSYSTEM_EXPORT)

# create the component system library

project(ComponentSystem)

find_package(Qt5 COMPONENTS Core REQUIRED)

set(library_SOURCES
    src/Component.cpp
    src/Component.h
    src/ComponentLoader.cpp
    src/ComponentLoader.h
    src/ComponentSystemSpec.h
    src/IComponent.cpp
    src/IComponent.h
    src/IComponentManager.cpp
    src/IComponentManager.h
    src/IInterface.h
)

set(Qt_LIBS "Qt5::Core")

option(NEDRYSOFT_COMPONENTSYSTEM_COMPONENTVIEWER "Include Component Viewer" OFF)

if (NEDRYSOFT_COMPONENTSYSTEM_COMPONENTVIEWER)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)

    set(Viewer_SOURCES
        src/ComponentDetailsDialog.cpp
        src/ComponentDetailsDialog.h
        src/ComponentDetailsDialog.ui
        src/ComponentViewerDialog.cpp
        src/ComponentViewerDialog.h
        src/ComponentViewerDialog.ui
        src/FontAwesome.cpp
        src/FontAwesome.h
        src/FontAwesome.qrc
    )

    list(APPEND Qt_LIBS "Qt5::Widgets")
else()
    set(Viewer_SOURCES "")
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
