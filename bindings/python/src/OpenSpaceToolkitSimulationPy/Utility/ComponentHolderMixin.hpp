/// Apache License 2.0

#ifndef __OpenSpaceToolkitSimulationPy_Utility_ComponentHolderMixin__
#define __OpenSpaceToolkitSimulationPy_Utility_ComponentHolderMixin__

#include <OpenSpaceToolkit/Simulation/Utility/ComponentHolder.hpp>
#include <nanobind/nanobind.h>

/// @brief                      Add ComponentHolder's interface to a class deriving from it.
///
///                             nanobind supports single inheritance only, so a class cannot declare
///                             both its own base and this mixin. It takes the methods directly
///                             instead, which is why they live in a header rather than alongside
///                             the ComponentHolder binding.

template <class ClassType>
inline void OpenSpaceToolkitSimulationPy_Utility_ComponentHolder_AddMethods(ClassType& aClass)
{
    using namespace nanobind;

    using ostk::simulation::utility::ComponentHolder;

    aClass

        .def(
            "has_component_with_id",
            &ComponentHolder::hasComponentWithId,
            R"doc(
                Check if a component with the given ID exists.

                Args:
                    id (str): The component ID to search for.

                Returns:
                    bool: True if a component with the ID exists, False otherwise.

                Example:
                    >>> holder.has_component_with_id("sensor-1")
                    True
            )doc"
        )

        .def(
            "has_component_with_name",
            &ComponentHolder::hasComponentWithName,
            R"doc(
                Check if a component with the given name exists.

                Args:
                    name (str): The component name to search for.

                Returns:
                    bool: True if a component with the name exists, False otherwise.

                Example:
                    >>> holder.has_component_with_name("Main Sensor")
                    True
            )doc"
        )

        .def(
            "has_component_at",
            &ComponentHolder::hasComponentAt,
            R"doc(
                Check if a component exists at the given path.

                Args:
                    path (str): The component path (e.g., "parent/child").

                Returns:
                    bool: True if a component exists at the path, False otherwise.

                Example:
                    >>> holder.has_component_at("payload/sensor-1")
                    True
            )doc"
        )

        .def(
            "access_components",
            &ComponentHolder::accessComponents,
            R"doc(
                Access all child components.

                Returns:
                    list: Array of all child components.

                Example:
                    >>> components = holder.access_components()
                    >>> len(components)
                    3
            )doc"
        )

        .def(
            "access_component_with_id",
            &ComponentHolder::accessComponentWithId,
            R"doc(
                Access a component by its ID.

                Args:
                    id (str): The component ID.

                Returns:
                    Component: The component with the specified ID.

                Example:
                    >>> component = holder.access_component_with_id("sensor-1")
            )doc"
        )

        .def(
            "access_component_with_name",
            &ComponentHolder::accessComponentWithName,
            R"doc(
                Access a component by its name.

                Args:
                    name (str): The component name.

                Returns:
                    Component: The component with the specified name.

                Example:
                    >>> component = holder.access_component_with_name("Main Sensor")
            )doc"
        )

        .def(
            "access_components_with_tag",
            &ComponentHolder::accessComponentsWithTag,
            R"doc(
                Access all components with a specific tag.

                Args:
                    tag (str): The tag to filter by.

                Returns:
                    list: Array of components with the specified tag.

                Example:
                    >>> sensors = holder.access_components_with_tag("sensor")
                    >>> len(sensors)
                    2
            )doc"
        )

        .def(
            "access_component_at",
            &ComponentHolder::accessComponentAt,
            R"doc(
                Access a component at a given path.

                Args:
                    path (str): The component path (e.g., "parent/child").

                Returns:
                    Component: The component at the specified path.

                Example:
                    >>> component = holder.access_component_at("payload/sensor-1")
            )doc"
        )

        .def(
            "add_component",
            // `self` is taken as a Shared<> so that `Component::addComponent`'s
            // `shared_from_this()` has an owner under nanobind, which does not hold bound
            // instances in a shared_ptr the way pybind11 did.
            [](const ostk::core::type::Shared<typename ClassType::Type>& aHolderSPtr,
               const ostk::core::type::Shared<ostk::simulation::Component>& aComponentSPtr)
            {
                aHolderSPtr->addComponent(aComponentSPtr);
            },
            R"doc(
                Add a child component.

                Args:
                    component (Component): The component to add.

                Example:
                    >>> holder.add_component(sensor)
            )doc"
        )

        ;
}

#endif
