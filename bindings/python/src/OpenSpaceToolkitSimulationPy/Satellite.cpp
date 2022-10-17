////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Simulation
/// @file           bindings/python/src/OpenSpaceToolkitSimulationPy/Satellite.cpp
/// @author         Lucas Brémond <lucas@loftorbital.com>, Remy Derollez <remy@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <OpenSpaceToolkit/Simulation/Satellite.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void                     OpenSpaceToolkitSimulationPy_Satellite      (           pybind11::module&           aModule                                     )
{

    using namespace pybind11 ;

    using ostk::core::types::String ;
    using ostk::core::ctnr::Array ;

    using ostk::simulation::Satellite ;

    class_<Satellite>(aModule, "Satellite")

        .def(init<const String&, const String&>())

        // Issue to be investigated between the following and simulation/Simulation
        // .def("__str__", &(shiftToString<Satellite>))
        // .def("__repr__", &(shiftToString<Satellite>))

        .def("is_defined", &Satellite::isDefined)

        .def("get_id", &Satellite::getId)
        .def("get_name", &Satellite::getName)

        .def_static("undefined", &Satellite::Undefined)

    ;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
