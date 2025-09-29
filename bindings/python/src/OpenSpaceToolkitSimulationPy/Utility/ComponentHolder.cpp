/// Apache License 2.0

#include <OpenSpaceToolkit/Simulation/Utility/ComponentHolder.hpp>

inline void OpenSpaceToolkitSimulationPy_Utility_ComponentHolder(pybind11::module& aModule)
{
    using namespace pybind11;

    using ostk::core::type::Shared;

    using ostk::simulation::utility::ComponentHolder;

    class_<ComponentHolder, Shared<ComponentHolder>>(
        aModule,
        "ComponentHolder",
        R"doc(
            Mixin class providing component management functionality for hierarchical component structures.

            ComponentHolder enables storage and retrieval of child components, supporting both flat
            and hierarchical component organization with path-based access.
        )doc"
    )

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
            &ComponentHolder::addComponent,
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
