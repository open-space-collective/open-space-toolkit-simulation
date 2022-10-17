////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Simulation
/// @file           bindings/python/src/OpenSpaceToolkitSimulationPy/Payload.cpp
/// @author         Lucas Brémond <lucas@loftorbital.com>, Remy Derollez <remy@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <OpenSpaceToolkit/Simulation/Payload.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void                     OpenSpaceToolkitSimulationPy_Payload        (           pybind11::module&           aModule                                     )
{

    using namespace pybind11 ;

    using ostk::core::types::String ;
    using ostk::core::ctnr::Array ;

    using ostk::math::geom::d3::Object ;

    using ostk::simulation::Component ;
    using ostk::simulation::Payload ;
    using ostk::simulation::component::Geometry ;

    class_<Payload, Component>(aModule, "Payload")

        .def(init<const String&, const String&, const String&, const Array<Geometry>&>())

        .def("__str__", &(shiftToString<Payload>))
        .def("__repr__", &(shiftToString<Payload>))

        .def("is_defined", &Payload::isDefined)

        .def("get_id", &Payload::getId)
        .def("get_model", &Payload::getModel)

        .def_static("undefined", &Payload::Undefined)
    ;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
