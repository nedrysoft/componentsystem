# Nedrysoft Component System

The component system library provides an abstracted layer to build modular applications through plugins; it consists of two major parts:

* The "Components" are dynamic libraries that implement the IComponent interface.
* The "Object Registry" which provides a centralised store for object instances that allows other components to discover them.

This library takes the Qt plugin system and provides a higher-level way to use it.  Because plugins all derive from the IComponent interface, the plugin does not have fixed functionality like a standard plugin would, instead the combining of the IComponent interface with the object registry allows much more abstracted and scalable applications.

Components can and should define custom interfaces which other components can find instances of in the object registry.

The application Pingnoo (https://github.com/nedrysoft/pingnoo) uses this component system (and indeed was where it was born), and the main application performs just two tasks.

Firstly it invokes the component loader, which figures out the dependencies and load order, and secondly, it starts the Qt event loop; the components themselves provide the main window and all other functionality.

## The Component

Each "component" is a dynamically loaded library complete with extra metadata which allows the component system to resolve dependencies and ensure that loading occurs in the correct order.

Each component is required to implement the IComponent interface.

An embedded metadata blob inside the component binary provides information about the component.  The loader retrieves this before loading; this metadata contains information such as the name of the component, vendor, version, dependencies etc.

## The Object Registry

The object registry allows components to register objects which can then be found by other components; this is a fundamental concept to the modularisation.

By using the registry, there is no "hard" linking to objects, this dynamic yet straightforward architecture allows the application (and it's components) to discover objects at runtime.

***NOTE: You can only store objects that are subclasses of QObject in the registry***

```c++
// adding an object to the registry

Nedrysoft::ComponentSystem::addObject(new Label);

// get a list of all objects in the registry

QList<QObject *> objects = Nedrysoft::ComponentSystem::allObjects();

// get the first matching object of the given type

auto myLabel = Nedrysoft::ComponentSystem::getObject<QLabel>();

// get all objects of the given type

QList<QLabel *> labels = Nedrysoft::ComponentSystem::getObjects<QLabel>();

```

## Architecture Diagram

![architecture diagram](https://user-images.githubusercontent.com/55795671/101220097-f76ea400-367c-11eb-9d47-059649d7e1ab.png)

### Nedrysoft::ComponentSystem::ComponentLoader

The Nedrysoft::ComponentSystem::ComponentLoader class is responsible for loading components.  You can add as many search locations as you need, it will open each component that it finds in turn and figure out dependencies and the correct load order.

### Nedrysoft::ComponentSystem::IInterface

*All components should implement IInterface; it is not currently used but will provide future functionality.*

### Nedrysoft::ComponentSystem::IComponent

The IComponent interface currently exposes two methods to the component loader:

* **initialiseEvent** - *called by the component loader after loading the component, this allows the component to perform any initialisation tasks that may be required, the component should add objects here to the registry that it wishes to expose to the application.*
* **initialisationFinishedEvent** - *called by the component loader after every component has been loaded and initialised, components may search the object registry at this point to find any objects of interest.*

## Loading Components

Use the ComponentLoader class to discover and load components.  It has methods to add search folders before loading.  The application can store components in multiple locations such as a system-wide folder, users home folder, etc.,

```c++
auto loader = new Nedrysoft::ComponentSystem::ComponentLoader();

loader->addComponents("./components");
loader->addComponents("/usr/local/myapp/components");

loader->loadComponents([=](Nedrysoft::ComponentSystem::Component *component){
// the function allows the software to selectively load components, this may be because
// a component has been disabled or possibly that it needs a registration key and it's not valid etc.
});
```
## Creating a Component

Creating a component is simple, create a new class in your dynamic library and make it a subclass of IComponent.

**MyComponent.h**

```c++
#include "ComponentSystem/IComponent.h"

class Q_DECL_EXPORT MyComponent :
        public QObject,
        public Nedrysoft::ComponentSystem::IComponent {

    private:
        Q_OBJECT
        
        Q_PLUGIN_METADATA(IID NedrysoftComponentInterfaceIID FILE "metadata.json")
        
        Q_INTERFACES(Nedrysoft::ComponentSystem::IComponent)

    public:
        MyComponent();
        
        ~MyComponent();
        
        void initialiseEvent() override;
        
        void initialisationFinishedEvent() override;
};
```

**MyComponent.cpp**

```c++
#include "MyComponent.h"

#include "ComponentSystem/IComponentManager.h"

MyComponent::MyComponent() {
    // called when the component is instantiated by the loader
}

MyComponent::~MyComponent() {
    // called when the component is unloaded by the loader
}

void MyComponent::initialiseEvent() {
    // as an example, create q QLabel and add it to the registry, the object can then
    // be found by other components in their initialisationFinishedEvent method

    Nedrysoft::ComponentSystem::addObject(new QLabel);
}


void MyComponent::initialisationFinishedEvent() {
    // called by the loader after all components have been loaded and initialised.
    // use this function to find objects in the registry that other components have
}

```

**Metadata.json**

The metadata is a JSON formatted structure that is retrieved by the Component Loader before it loads the component.

```
{
    "Name" : "MyComponent",
    "Version" : "1.2.3",
    "Branch" : "master",
    "Revision" : "2b244cf6",
    "CompatVersion" : "1.0.0",
    "Vendor" : "nedrysoft.com",
    "Copyright" : "(C) 2020 Adrian Carpenter",
    "License" : [
        "Copyright (C) 2020 Adrian Carpenter",
        "",
        "This program is free software: you can redistribute it and/or modify",
        "it under the terms of the GNU General Public License as published by",
        "the Free Software Foundation, either version 3 of the License, or",
        "(at your option) any later version.",
        "",
        "This program is distributed in the hope that it will be useful,",
        "but WITHOUT ANY WARRANTY; without even the implied warranty of",
        "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the",
        "GNU General Public License for more details.",
        "",
        "You should have received a copy of the GNU General Public License",
        "along with this program.  If not, see <http://www.gnu.org/licenses/>.",
        ""
    ],
    "Category" : "My Category",
    "Dependencies" : [
        "Core"
    ],
    "Description" : [
        "This is a sample component."
    ],
    "Url" : "https://github.com/nedrysoft/componentsystem"
}

```

* **Name** - *the name of the component, this is displayed in the component viewer.*
* **Version** - *the version of the component.  Versioning allows you to require a base version of a dependency.*
* **Branch** - *the version control branch that the component was build from.*
* **Revision** - *the commit revision.*
* **CompatVersion** - *an API level that this component supports.*
* **Vendor** - *a reverse DNS string identifiying the provider of the component.*
* **Copyright** - *the copyright holder.*
* **License** - *the license under which the component is provided.*
* **Category** - *the component viewer groups components by category, this allows logical grouping of different components.*
* **Dependencies** - *a list of dependencies that the component has.*
* **Description** - *a description of the purpose of the component.*
* **url** - *the url of where to find information about the component.*

## Component Viewer

The Nedrysoft::ComponentSystem::ComponentViewerDialog
dialog provides a list of all the components that were found by the loader, along with their status (loaded, disabled, incompatible etc.). Additionally, it provides the means to enable or disable components from being used.

![component viewer](https://user-images.githubusercontent.com/55795671/101047358-b17ce780-3579-11eb-8044-24f8263c7004.png)

Double-clicking on a component in the viewer opens the detail view, which displays the metadata that the component exposes in a user-friendly manner.

![component-detail](https://user-images.githubusercontent.com/55795671/101047381-b5a90500-3579-11eb-9c45-4dc6ce246440.png)

## Requirements

* Qt 5
* CMake

## Building

To build the library, invoke CMake or open the CMakeLists.txt file in your preferred IDE.

Setting the following CMake variables allows the customisation of the build.

```
NEDRYSOFT_COMPONENTSYSTEM_LIBRARY_DIR=<dir>
```

Sets the output folder for the dynamic library; if omitted, you can find the binaries in the default location.

```
NEDRYSOFT_COMPONENTSYSTEM_VIEWER=ON|OFF
```

By default, the viewer is disabled; this removes the need for linking the library with the Qt widgets module.

The application can use the viewer to display information to the user about the discovered components.  It also additionally provides the user with the ability to disable specific components from being loaded.

# License

This project is open source and released under the GPLv3 licence.

Distributed as-is; no warranty is given.