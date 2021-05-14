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

if(WIN32)
    configure_file("Version.h.in" "Version.h")
    list(APPEND library_SOURCES "version.rc")
    add_definitions("-DNEDRYSOFT_MODULE_FILENAME=\"${PROJECT_NAME}.dll\"")
    add_definitions("-DNEDRYSOFT_MODULE_NAME=\"${PROJECT_NAME}\"")
endif()

# discover which Qt version is available

if (NOT DEFINED QT_VERSION_MAJOR)
    if (DEFINED USE_QT_VERSION)
        set(QT_VERSION_MAJOR ${USE_QT_VERSION})
        message(STATUS "Qt${QT_VERSION_MAJOR} has been manually selected")
    else()
        message(STATUS "Detecting Qt version")

        find_package(Qt6 COMPONENTS Core QUIET)
        find_package(Qt5 COMPONENTS Core QUIET)

        if (Qt6_FOUND)
            set(QT_VERSION_MAJOR 6)
        elseif(Qt5_FOUND)
            set(QT_VERSION_MAJOR 5)
        else()
            message(FATAL_ERROR "No valid Qt version was set, and none could be found")
        endif()
        message(STATUS "Detecting Qt version - done")
    endif()

    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core QUIET)

    if (NOT Qt${QT_VERSION_MAJOR}_FOUND)
        message(FATAL_ERROR "Qt${QT_VERSION_MAJOR} could not be found")
    endif()

    message(STATUS "Qt major version: ${QT_VERSION_MAJOR}")
endif()

# end of qt selection/detection

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

set(Qt_LIBS "Qt${QT_VERSION_MAJOR}::Core")

include_directories("src" "include")

option(NEDRYSOFT_COMPONENTSYSTEM_COMPONENTVIEWER "Include Component Viewer" OFF)

if (NEDRYSOFT_COMPONENTSYSTEM_COMPONENTVIEWER)
    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

    set(Viewer_SOURCES
        src/ComponentDetailsDialog.h
        src/ComponentViewerDialog.h
        src/ComponentDetailsDialog.cpp
        src/ComponentDetailsDialog.ui
        src/ComponentViewerDialog.cpp
        src/ComponentViewerDialog.ui
        src/ComponentSystemFontAwesome.cpp
        src/ComponentSystemFontAwesome.h
        src/ComponentSystemFontAwesome.qrc
    )

    list(APPEND Qt_LIBS "Qt${QT_VERSION_MAJOR}::Widgets")
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
