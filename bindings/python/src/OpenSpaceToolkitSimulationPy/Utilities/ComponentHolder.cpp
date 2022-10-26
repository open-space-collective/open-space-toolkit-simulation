////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Simulation
/// @file           bindings/python/src/OpenSpaceToolkitSimulationPy/Utilities/ComponentHolder.cpp
/// @author         Lucas Brémond <lucas@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <OpenSpaceToolkit/Simulation/Utilities/ComponentHolder.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void                     OpenSpaceToolkitSimulationPy_Utilities_ComponentHolder ( pybind11::module&          aModule                                     )
{

    using namespace pybind11 ;

    using ostk::core::types::Shared ;

    using ostk::simulation::Component ;
    using ostk::simulation::utilities::ComponentHolder ;

    class_<ComponentHolder, Shared<ComponentHolder>>(aModule, "ComponentHolder")

        .def("has_component_with_id", &ComponentHolder::hasComponentWithId)
        .def("has_component_with_name", &ComponentHolder::hasComponentWithName)
        .def("has_component_at", &ComponentHolder::hasComponentAt)

        .def("access_components", &ComponentHolder::accessComponents)
        .def("access_component_with_id", &ComponentHolder::accessComponentWithId)
        .def("access_component_with_name", &ComponentHolder::accessComponentWithName)
        .def("access_components_with_tag", &ComponentHolder::accessComponentsWithTag)
        .def("access_component_at", &ComponentHolder::accessComponentAt)

        .def("add_component", &ComponentHolder::addComponent)

    ;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////