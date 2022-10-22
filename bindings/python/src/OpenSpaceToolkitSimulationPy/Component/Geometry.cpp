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

    using ostk::simulation::Component ;
    using ostk::simulation::component::Geometry ;
    using ostk::simulation::component::GeometryConfiguration ;

    // class_<Geometry, ObjectGeometry, Shared<Geometry>>(aModule, "Geometry")
    class_<Geometry, ObjectGeometry>(aModule, "Geometry")

        .def
        (
            init<const String&, const Object&, const Shared<const Frame>&, const Shared<const Component>&>(),
            arg("name"),
            arg("object"),
            arg("frame"),
            arg("component")
        )

        // .def
        // (
        //     init<const String&, const Composite&, const Shared<const Frame>&, const Shared<const Component>&>(),
        //     arg("name"),
        //     arg("composite"),
        //     arg("frame"),
        //     arg("component")
        // )

        .def("__str__", &(shiftToString<Geometry>))
        .def("__repr__", &(shiftToString<Geometry>))

        .def("is_defined", &Geometry::isDefined)

        .def("access_component", &Geometry::accessComponent)

        .def("get_name", &Geometry::getName)

        .def("intersects", &Geometry::intersects, arg("geometry"))
        .def("contains", &Geometry::contains, arg("geometry"))
        .def("intersection_with", &Geometry::intersectionWith, arg("geometry"))

        .def_static("undefined", &Geometry::Undefined)
        // .def_static("configure", &Geometry::Configure)

    ;

    class_<GeometryConfiguration>(aModule, "GeometryConfiguration")

        .def
        (
            init<const String&, const Object&>(),
            arg("name"),
            arg("object")
        )

    ;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
