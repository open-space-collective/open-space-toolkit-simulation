/// Apache License 2.0

#include <OpenSpaceToolkit/Simulation/Component.hpp>

#include <OpenSpaceToolkitSimulationPy/Component/Geometry.cpp>
#include <OpenSpaceToolkitSimulationPy/Component/State.cpp>
#include <OpenSpaceToolkitSimulationPy/Entity.cpp>
#include <OpenSpaceToolkitSimulationPy/Utility/ComponentHolder.cpp>

inline void OpenSpaceToolkitSimulationPy_Component(nanobind::module_& aModule)
{
    using namespace nanobind;

    using ostk::core::container::Array;
    using ostk::core::type::Shared;
    using ostk::core::type::String;

    using ostk::mathematics::geometry::d3::transformation::rotation::Quaternion;

    using ostk::physics::coordinate::Frame;

    using ostk::simulation::Component;
    using ostk::simulation::component::Geometry;
    using ostk::simulation::component::GeometryConfiguration;
    using ostk::simulation::component::State;
    using ostk::simulation::ComponentConfiguration;
    using ostk::simulation::Simulator;

    {
        // nanobind binds a single base class, and only one that sits at offset zero in the derived class. Neither
        // Entity nor ComponentHolder does in Component, so Component has no Python base class, and the Entity and
        // ComponentHolder methods are bound on it below.
        class_<Component> component_class(
            aModule,
            "Component",
            R"doc(
                Simulation component representing any part of a space system.

                Component is the base class for all simulation objects like assemblies, sensors,
                actuators, and controllers. It provides hierarchical structure, geometry attachment,
                and reference frame management.
            )doc"
        );

        component_class
            .def(
                // The parent is taken as a Component, the only ComponentHolder that Python can create, and
                // converted to a ComponentHolder in C++ (see above).
                "__init__",
                [](Component* self,
                   const String& anId,
                   const String& aName,
                   const std::optional<Component::Type>& aType,
                   const Array<String>& aTagArray,
                   const Array<Shared<Geometry>>& aGeometryArray,
                   const Array<Shared<Component>>& aComponentArray,
                   const Shared<Component>& aParentComponentSPtr,
                   const Shared<const Frame>& aFrameSPtr,
                   const Shared<const Simulator>& aSimulatorSPtr)
                {
                    new (self) Component(
                        anId,
                        aName,
                        aType,
                        aTagArray,
                        aGeometryArray,
                        aComponentArray,
                        aParentComponentSPtr,
                        aFrameSPtr,
                        aSimulatorSPtr
                    );
                },
                arg("id"),
                arg("name"),
                arg("type"),
                arg("tags"),
                arg("geometries"),
                arg("components"),
                arg("parent").none(),
                arg("frame").none(),
                arg("simulator").none(),
                R"doc(
                    Create a Component instance.

                    Args:
                        id (str): The unique component identifier. Must not be empty.
                        name (str): The component name. Must not be empty.
                        type (Component.Type | None): The component type (Assembly, Sensor, etc.). None if the type is unspecified.
                        tags (list[str]): Array of classification tags.
                        geometries (list[Geometry]): Array of geometry objects.
                        components (list[Component]): Array of child components.
                        parent (Component | None): The parent component.
                        frame (Frame | None): The component reference frame. None if the component has no frame.
                        simulator (Simulator | None): Reference to the parent simulator.

                    Returns:
                        Component: The component instance.

                    Raises:
                        RuntimeError: If the id or the name is empty.

                    Example:
                        >>> component = Component(
                        ...     id="sensor-1",
                        ...     name="Star Tracker",
                        ...     type=Component.Type.Sensor,
                        ...     tags=["attitude"],
                        ...     geometries=[],
                        ...     components=[],
                        ...     parent=satellite,
                        ...     frame=frame,
                        ...     simulator=sim
                        ... )
                )doc"
            )

            .def("__str__", &(shiftToString<Component>))
            .def("__repr__", &(shiftToString<Component>))

            .def(
                "get_frame",
                &Component::getFrame,
                R"doc(
                    Get the component's reference frame.

                    Returns:
                        Frame: The component frame.

                    Raises:
                        RuntimeError: If the component has no frame.

                    Example:
                        >>> frame = component.get_frame()
                        >>> position = frame.get_origin_in(gcrf)
                )doc"
            )

            .def(
                "get_geometry_with_name",
                &Component::getGeometryWithName,
                arg("name"),
                R"doc(
                    Get a geometry object by name.

                    Args:
                        name (str): The geometry name.

                    Returns:
                        Geometry: The geometry with the specified name.

                    Example:
                        >>> geometry = component.get_geometry_with_name("solar-panel")
                )doc"
            )

            .def(
                "get_simulator",
                &Component::getSimulator,
                R"doc(
                    Get the parent simulator.

                    Returns:
                        Simulator: The parent simulator.

                    Example:
                        >>> simulator = component.get_simulator()
                        >>> instant = simulator.get_instant()
                )doc"
            )

            .def(
                "get_type",
                &Component::getType,
                R"doc(
                    Get the component type.

                    Returns:
                        Component.Type | None: The component type, or None if the type is unspecified.

                    Example:
                        >>> comp_type = component.get_type()
                        >>> comp_type == Component.Type.Sensor
                        True
                )doc"
            )

            .def(
                "get_tags",
                &Component::getTags,
                R"doc(
                    Get the component tags.

                    Returns:
                        list: Array of tag strings.

                    Example:
                        >>> tags = component.get_tags()
                        >>> "attitude" in tags
                        True
                )doc"
            )

            .def(
                "get_geometries",
                &Component::getGeometries,
                R"doc(
                    Get all geometries attached to the component.

                    Returns:
                        list: Array of geometry objects.

                    Example:
                        >>> geometries = component.get_geometries()
                        >>> len(geometries)
                        2
                )doc"
            )

            .def(
                "set_parent",
                &Component::setParent,
                arg("component").none(),
                R"doc(
                    Set the parent component.

                    Args:
                        component (Component): The parent component.

                    Example:
                        >>> component.set_parent(satellite)
                )doc"
            )

            .def(
                "add_geometry",
                &Component::addGeometry,
                arg("geometry"),
                R"doc(
                    Add a geometry to the component.

                    Args:
                        geometry (Geometry): The geometry to add.

                    Example:
                        >>> component.add_geometry(panel_geometry)
                )doc"
            )

            .def(
                "add_component",
                // Component::addComponent calls shared_from_this(), so self is taken as a shared pointer: nanobind
                // gives a Python-created instance a shared owner only when it is converted to one.
                [](const Shared<Component>& self, const Shared<Component>& aComponentSPtr)
                {
                    self->addComponent(aComponentSPtr);
                },
                arg("component"),
                R"doc(
                    Add a child component.

                    Args:
                        component (Component): The child component to add.

                    Example:
                        >>> component.add_component(subsensor)
                )doc"
            );

        OpenSpaceToolkitSimulationPy_Entity_Methods(component_class);
        OpenSpaceToolkitSimulationPy_Utility_ComponentHolder_Methods(component_class);

        component_class

            .def_static(
                "configure",
                &Component::Configure,
                arg("configuration"),
                arg("parent_component"),
                R"doc(
                    Create a component from configuration.

                    Args:
                        configuration (ComponentConfiguration): The component configuration.
                        parent_component (Component): The parent component.

                    Returns:
                        Component: The configured component.

                    Example:
                        >>> config = ComponentConfiguration(
                        ...     id="sensor-1",
                        ...     name="Star Tracker",
                        ...     type=Component.Type.Sensor
                        ... )
                        >>> component = Component.configure(config, parent)
                )doc"
            )

            .def_static(
                "string_from_type",
                &Component::StringFromType,
                arg("type"),
                R"doc(
                    Convert component type to string.

                    Args:
                        type (Component.Type): The component type.

                    Returns:
                        str: String representation of the type.

                    Example:
                        >>> Component.string_from_type(Component.Type.Sensor)
                        'Sensor'
                )doc"
            )

            ;

        enum_<Component::Type>(
            component_class,
            "Type",
            R"doc(
                Enumeration of component types.

                Defines the classification of components within the simulation.
            )doc"
        )

            .value(
                "Assembly",
                Component::Type::Assembly,
                R"doc(
                    Structural assembly or subsystem.
                )doc"
            )

            .value(
                "Controller",
                Component::Type::Controller,
                R"doc(
                    Control system component.
                )doc"
            )

            .value(
                "Sensor",
                Component::Type::Sensor,
                R"doc(
                    Sensing or measurement device.
                )doc"
            )

            .value(
                "Actuator",
                Component::Type::Actuator,
                R"doc(
                    Actuation device (thrusters, reaction wheels, etc.).
                )doc"
            )

            .value(
                "Other",
                Component::Type::Other,
                R"doc(
                    Other component type.
                )doc"
            );
    }

    class_<ComponentConfiguration>(
        aModule,
        "ComponentConfiguration",
        R"doc(
            Configuration structure for creating components.

            ComponentConfiguration defines all parameters needed to construct a Component,
            including type, tags, orientation, geometries, and child components.
        )doc"
    )

        .def(
            init<
                const String&,
                const String&,
                const std::optional<Component::Type>&,
                const Array<String>&,
                const Quaternion&,
                const Array<GeometryConfiguration>&,
                const Array<ComponentConfiguration>&>(),
            arg("id"),
            arg("name"),
            arg("type") = none(),
            arg("tags") = DEFAULT_TAGS,
            arg("orientation") = DEFAULT_ORIENTATION,
            arg("geometries") = DEFAULT_GEOMETRIES,
            arg("components") = DEFAULT_COMPONENTS,
            R"doc(
                Create a ComponentConfiguration instance.

                Args:
                    id (str): The unique component identifier.
                    name (str): The component name.
                    type (Component.Type | None): The component type (default: None, which leaves the type unspecified).
                    tags (list[str]): Array of classification tags (optional).
                    orientation (Quaternion): Orientation relative to parent (optional).
                    geometries (list[GeometryConfiguration]): Array of geometry configurations (optional).
                    components (list[ComponentConfiguration]): Array of child component configurations (optional).

                Returns:
                    ComponentConfiguration: The configuration instance.

                Example:
                    >>> config = ComponentConfiguration(
                    ...     id="sensor-1",
                    ...     name="Star Tracker",
                    ...     type=Component.Type.Sensor,
                    ...     tags=["attitude", "primary"]
                    ... )
            )doc"
        )

        ;

    // Create python submodule
    auto component = aModule.def_submodule("component");

    // Add objects to python submodule
    OpenSpaceToolkitSimulationPy_Component_Geometry(component);
    OpenSpaceToolkitSimulationPy_Component_State(component);
}
