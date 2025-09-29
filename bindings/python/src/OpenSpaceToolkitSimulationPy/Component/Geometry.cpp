/// Apache License 2.0

#include <OpenSpaceToolkit/Simulation/Component/Geometry.hpp>

inline void OpenSpaceToolkitSimulationPy_Component_Geometry(pybind11::module& aModule)
{
    using namespace pybind11;

    using ostk::core::container::Array;
    using ostk::core::type::Shared;
    using ostk::core::type::String;

    using ostk::mathematics::geometry::d3::Object;
    using ostk::mathematics::geometry::d3::object::Composite;

    using ostk::physics::coordinate::Frame;
    using ObjectGeometry = ostk::physics::environment::object::Geometry;
    using ostk::physics::environment::object::Celestial;

    using ostk::simulation::Component;
    using ostk::simulation::component::Geometry;
    using ostk::simulation::component::GeometryConfiguration;

    class_<Geometry, Shared<Geometry>>(
        aModule,
        "Geometry",
        R"doc(
            Physical geometry attached to a component.

            Geometry represents the physical shape and structure of a component, enabling
            spatial operations like intersection and containment checks with other geometries
            or celestial objects.
        )doc"
    )

        .def(
            init<const String&, const Composite&, const Shared<const Component>&>(),
            arg("name"),
            arg("composite"),
            arg("component"),
            R"doc(
                Create a Geometry instance.

                Args:
                    name (str): The geometry name.
                    composite (Composite): The 3D composite geometry object.
                    component (Component): The parent component.

                Returns:
                    Geometry: The geometry instance.

                Example:
                    >>> geometry = Geometry(
                    ...     name="solar-panel",
                    ...     composite=panel_composite,
                    ...     component=satellite
                    ... )
            )doc"
        )

        .def("__str__", &(shiftToString<Geometry>))
        .def("__repr__", &(shiftToString<Geometry>))

        .def(
            "is_defined",
            &Geometry::isDefined,
            R"doc(
                Check if the geometry is defined.

                Returns:
                    bool: True if the geometry is defined, False otherwise.

                Example:
                    >>> geometry.is_defined()
                    True
            )doc"
        )

        .def(
            "access_component",
            &Geometry::accessComponent,
            R"doc(
                Access the parent component.

                Returns:
                    Component: The parent component.

                Example:
                    >>> component = geometry.access_component()
            )doc"
        )

        .def(
            "get_name",
            &Geometry::getName,
            R"doc(
                Get the geometry name.

                Returns:
                    str: The geometry name.

                Example:
                    >>> name = geometry.get_name()
                    >>> name
                    'solar-panel'
            )doc"
        )

        .def(
            "intersects",
            overload_cast<const ObjectGeometry&>(&Geometry::intersects, const_),
            arg("geometry"),
            R"doc(
                Check if this geometry intersects another geometry.

                Args:
                    geometry (Geometry): The other geometry.

                Returns:
                    bool: True if geometries intersect, False otherwise.

                Example:
                    >>> geometry.intersects(other_geometry)
                    False
            )doc"
        )

        .def(
            "intersects",
            overload_cast<const Celestial&>(&Geometry::intersects, const_),
            arg("celestial_object"),
            R"doc(
                Check if this geometry intersects a celestial object.

                Args:
                    celestial_object (Celestial): The celestial object (planet, moon, etc.).

                Returns:
                    bool: True if geometry intersects the celestial object, False otherwise.

                Example:
                    >>> geometry.intersects(earth)
                    False
            )doc"
        )

        .def(
            "contains",
            overload_cast<const ObjectGeometry&>(&Geometry::contains, const_),
            arg("geometry"),
            R"doc(
                Check if this geometry contains another geometry.

                Args:
                    geometry (Geometry): The other geometry.

                Returns:
                    bool: True if this geometry contains the other, False otherwise.

                Example:
                    >>> geometry.contains(small_geometry)
                    True
            )doc"
        )

        .def(
            "contains",
            overload_cast<const Celestial&>(&Geometry::contains, const_),
            arg("geometry"),
            R"doc(
                Check if this geometry contains a celestial object.

                Args:
                    geometry (Celestial): The celestial object.

                Returns:
                    bool: True if this geometry contains the celestial object, False otherwise.

                Example:
                    >>> geometry.contains(moon)
                    False
            )doc"
        )

        .def(
            "access_composite",
            &Geometry::accessComposite,
            return_value_policy::reference,
            R"doc(
                Access the underlying composite geometry.

                Returns:
                    Composite: The 3D composite geometry object.

                Example:
                    >>> composite = geometry.access_composite()
            )doc"
        )

        .def(
            "access_frame",
            &Geometry::accessFrame,
            R"doc(
                Access the geometry's reference frame.

                Returns:
                    Frame: The geometry frame.

                Example:
                    >>> frame = geometry.access_frame()
            )doc"
        )

        .def(
            "get_geometry_in",
            &Geometry::getGeometryIn,
            arg("frame"),
            R"doc(
                Get the geometry expressed in a different reference frame.

                Args:
                    frame (Frame): The target reference frame.

                Returns:
                    Geometry: The geometry in the target frame.

                Example:
                    >>> geometry_in_gcrf = geometry.get_geometry_in(gcrf)
            )doc"
        )

        .def(
            "intersection_with",
            overload_cast<const ObjectGeometry&>(&Geometry::intersectionWith, const_),
            arg("geometry"),
            R"doc(
                Compute the intersection of this geometry with another geometry.

                Args:
                    geometry (Geometry): The other geometry.

                Returns:
                    Geometry: The intersection geometry.

                Example:
                    >>> intersection = geometry.intersection_with(other_geometry)
            )doc"
        )

        .def(
            "intersection_with",
            overload_cast<const Celestial&>(&Geometry::intersectionWith, const_),
            arg("celestial_object"),
            R"doc(
                Compute the intersection of this geometry with a celestial object.

                Args:
                    celestial_object (Celestial): The celestial object.

                Returns:
                    Geometry: The intersection geometry.

                Example:
                    >>> intersection = geometry.intersection_with(earth)
            )doc"
        )

        .def_static(
            "undefined",
            &Geometry::Undefined,
            R"doc(
                Create an undefined geometry.

                Returns:
                    Geometry: An undefined geometry.

                Example:
                    >>> geometry = Geometry.undefined()
                    >>> geometry.is_defined()
                    False
            )doc"
        )

        .def_static(
            "configure",
            &Geometry::Configure,
            arg("configuration"),
            arg("component"),
            R"doc(
                Create a geometry from configuration.

                Args:
                    configuration (GeometryConfiguration): The geometry configuration.
                    component (Component): The parent component.

                Returns:
                    Geometry: The configured geometry.

                Example:
                    >>> config = GeometryConfiguration(
                    ...     name="solar-panel",
                    ...     composite=panel_composite
                    ... )
                    >>> geometry = Geometry.configure(config, component)
            )doc"
        )

        ;

    class_<GeometryConfiguration>(
        aModule,
        "GeometryConfiguration",
        R"doc(
            Configuration structure for creating geometries.

            GeometryConfiguration defines the name and composite geometry needed to
            construct a Geometry object.
        )doc"
    )

        .def(
            init<const String&, const Composite&>(),
            arg("name"),
            arg("composite"),
            R"doc(
                Create a GeometryConfiguration instance.

                Args:
                    name (str): The geometry name.
                    composite (Composite): The 3D composite geometry object.

                Returns:
                    GeometryConfiguration: The configuration instance.

                Example:
                    >>> config = GeometryConfiguration(
                    ...     name="solar-panel",
                    ...     composite=panel_composite
                    ... )
            )doc"
        )

        ;
}