/// Apache License 2.0

#include <OpenSpaceToolkit/Simulation/Utility/ComponentHolder.hpp>

/// @brief Bind the ComponentHolder lookup methods (everything but `add_component`) on a class whose C++ type derives
///        from ComponentHolder.
///
/// nanobind supports a single base class per bound type, and passes an instance to a base class method without
/// adjusting the pointer. `Component` derives from both Entity and ComponentHolder, and neither sits at offset zero
/// in it, so it cannot list ComponentHolder as its Python base. The methods are bound on it directly, through member
/// pointers of the derived type, which apply the base class adjustment in C++.
template <typename Class>
inline void OpenSpaceToolkitSimulationPy_Utility_ComponentHolder_Methods(Class& aClass)
{
    using namespace nanobind;

    using ostk::core::container::Array;
    using ostk::core::type::Shared;
    using ostk::core::type::String;

    using ostk::simulation::Component;
    using ostk::simulation::utility::ComponentHolder;

    using T = typename Class::Type;

    aClass

        .def(
            "has_component_with_id",
            static_cast<bool (T::*)(const String&) const>(&ComponentHolder::hasComponentWithId),
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
            static_cast<bool (T::*)(const String&) const>(&ComponentHolder::hasComponentWithName),
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
            static_cast<bool (T::*)(const String&) const>(&ComponentHolder::hasComponentAt),
            R"doc(
                Check if a component exists at the given path.

                Args:
                    path (str): The component path, with segments separated by "/" or "."
                        (e.g., "parent/child" or "parent.child").

                Returns:
                    bool: True if a component exists at the path, False otherwise.

                Example:
                    >>> holder.has_component_at("payload/sensor-1")
                    True
                    >>> holder.has_component_at("payload.sensor-1")
                    True
            )doc"
        )

        .def(
            "get_components",
            static_cast<Array<Shared<Component>> (T::*)() const>(&ComponentHolder::getComponents),
            R"doc(
                Get all child components.

                Returns:
                    list: Array of all child components.

                Example:
                    >>> components = holder.get_components()
                    >>> len(components)
                    3
            )doc"
        )

        .def(
            "get_component_with_id",
            static_cast<const Component& (T::*)(const String&) const>(&ComponentHolder::getComponentWithId),
            R"doc(
                Get a component by its ID.

                Args:
                    id (str): The component ID.

                Returns:
                    Component: The component with the specified ID.

                Example:
                    >>> component = holder.get_component_with_id("sensor-1")
            )doc"
        )

        .def(
            "get_component_with_name",
            static_cast<const Component& (T::*)(const String&) const>(&ComponentHolder::getComponentWithName),
            R"doc(
                Get a component by its name.

                Args:
                    name (str): The component name.

                Returns:
                    Component: The component with the specified name.

                Example:
                    >>> component = holder.get_component_with_name("Main Sensor")
            )doc"
        )

        .def(
            "get_components_with_tag",
            static_cast<Array<Shared<const Component>> (T::*)(const String&) const>(
                &ComponentHolder::getComponentsWithTag
            ),
            R"doc(
                Get all components with a specific tag.

                Args:
                    tag (str): The tag to filter by.

                Returns:
                    list: Array of components with the specified tag.

                Example:
                    >>> sensors = holder.get_components_with_tag("sensor")
                    >>> len(sensors)
                    2
            )doc"
        )

        .def(
            "get_component_at",
            static_cast<const Component& (T::*)(const String&) const>(&ComponentHolder::getComponentAt),
            R"doc(
                Get a component at a given path.

                Args:
                    path (str): The component path, with segments separated by "/" or "."
                        (e.g., "parent/child" or "parent.child").

                Returns:
                    Component: The component at the specified path.

                Example:
                    >>> component = holder.get_component_at("payload/sensor-1")
                    >>> component = holder.get_component_at("payload.sensor-1")
            )doc"
        );
}

inline void OpenSpaceToolkitSimulationPy_Utility_ComponentHolder(nanobind::module_& aModule)
{
    using namespace nanobind;

    using ostk::simulation::utility::ComponentHolder;

    class_<ComponentHolder> component_holder_class(
        aModule,
        "ComponentHolder",
        R"doc(
            Mixin class providing component management functionality for hierarchical component structures.

            ComponentHolder enables storage and retrieval of child components, supporting both flat
            and hierarchical component organization with path-based access.
        )doc"
    );

    OpenSpaceToolkitSimulationPy_Utility_ComponentHolder_Methods(component_holder_class);

    component_holder_class

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
