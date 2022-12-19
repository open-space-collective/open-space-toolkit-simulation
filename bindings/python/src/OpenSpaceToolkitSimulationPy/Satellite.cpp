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

    using ostk::core::types::Shared ;
    using ostk::core::types::String ;
    using ostk::core::ctnr::Array ;

    using ostk::physics::coord::Frame ;

    using ostk::astro::flight::Profile ;

    using ostk::simulation::Simulator ;
    using ostk::simulation::Satellite ;
    using ostk::simulation::SatelliteConfiguration ;
    using ostk::simulation::Component ;
    using ostk::simulation::ComponentConfiguration ;
    using ostk::simulation::component::Geometry ;
    using ostk::simulation::component::GeometryConfiguration ;

    class_<Satellite, Component, Shared<Satellite>>(aModule, "Satellite")

        .def
        (
            init
            <
                const String&,
                const String&,
                const Array<String>&,
                const Array<Shared<Geometry>>&,
                const Array<Shared<Component>>&,
                const Shared<const Frame>&,
                const Shared<Profile>&,
                const Shared<const Simulator>&
            >(),
            arg("id"),
            arg("name"),
            arg("tags"),
            arg("geometries"),
            arg("components"),
            arg("frame"),
            arg("profile"),
            arg("simulator")
        )

        .def_static("undefined", &Satellite::Undefined)
        .def_static("configure", &Satellite::Configure, arg("configuration"), arg("simulator") = nullptr)

    ;

    class_<SatelliteConfiguration>(aModule, "SatelliteConfiguration")

        .def
        (
            init<const String&, const String&, const Profile&, const Array<ComponentConfiguration>&, const Array<String>&, const Array<GeometryConfiguration>&>(),
            arg("id"),
            arg("name"),
            arg("profile"),
            arg("components") = DEFAULT_COMPONENTS,
            arg("tags") = DEFAULT_TAGS,
            arg("geometries") = DEFAULT_GEOMETRIES
        )

    ;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
