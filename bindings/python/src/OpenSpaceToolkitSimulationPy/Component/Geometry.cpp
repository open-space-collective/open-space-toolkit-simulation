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

    using ostk::simulation::Component ;
    using ostk::simulation::component::Geometry ;

    class_<Geometry>(aModule, "Geometry")

        .def(init<const String&, const Object&, const Shared<const Frame>&, const Shared<const Component>&>())
        .def(init<const String&, const Composite&, const Shared<const Frame>&, const Shared<const Component>&>())

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

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
