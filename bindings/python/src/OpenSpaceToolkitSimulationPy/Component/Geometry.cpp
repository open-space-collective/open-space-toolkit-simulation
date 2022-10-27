////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Simulation
/// @file           bindings/python/src/OpenSpaceToolkitSimulationPy/Component/Geometry.cpp
/// @author         Lucas Brémond <lucas@loftorbital.com>, Remy Derollez <remy@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <OpenSpaceToolkit/Simulation/Component/Geometry.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void                     OpenSpaceToolkitSimulationPy_Component_Geometry    (           pybind11::module&    aModule                                     )
{

    using namespace pybind11 ;

    using ostk::core::types::Shared ;
    using ostk::core::types::String ;
    using ostk::core::ctnr::Array ;

    using ostk::math::geom::d3::Object ;
    using ostk::math::geom::d3::objects::Composite ;

    using ostk::physics::coord::Frame ;
    using ObjectGeometry = ostk::physics::env::object::Geometry ;
    using ostk::physics::env::obj::Celestial ;

    using ostk::simulation::Component ;
    using ostk::simulation::component::Geometry ;
    using ostk::simulation::component::GeometryConfiguration ;

    class_<Geometry, Shared<Geometry>>(aModule, "Geometry")

        .def
        (
            init<const String&, const Composite&, const Shared<const Component>&>(),
            arg("name"),
            arg("composite"),
            arg("component")
        )

        .def("__str__", &(shiftToString<Geometry>))
        .def("__repr__", &(shiftToString<Geometry>))

        .def("is_defined", &Geometry::isDefined)

        .def("access_component", &Geometry::accessComponent)

        .def("get_name", &Geometry::getName)

        .def("intersects", overload_cast<const ObjectGeometry&>(&Geometry::intersects, const_), arg("geometry"))
        .def("intersects", overload_cast<const Celestial&>(&Geometry::intersects, const_), arg("celestial_object"))
        .def("contains", overload_cast<const ObjectGeometry&>(&Geometry::contains, const_), arg("geometry"))
        .def("contains", overload_cast<const ObjectGeometry&>(&Geometry::contains, const_), arg("geometry"))
        .def("access_composite", &Geometry::accessComposite, return_value_policy::reference)
        .def("access_frame", &Geometry::accessFrame)
        .def("get_geometry_in", &Geometry::getGeometryIn, arg("frame"))
        .def("intersection_with", overload_cast<const ObjectGeometry&>(&Geometry::intersectionWith, const_), arg("geometry"))
        .def("intersection_with", overload_cast<const Celestial&>(&Geometry::intersectionWith, const_), arg("celestial_object"))

        .def_static("undefined", &Geometry::Undefined)
        .def_static("configure", &Geometry::Configure, arg("configuration"), arg("component"))

    ;

    class_<GeometryConfiguration>(aModule, "GeometryConfiguration")

        .def
        (
            init<const String&, const Composite&>(),
            arg("name"),
            arg("composite")
        )

    ;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
