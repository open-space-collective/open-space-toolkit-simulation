////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Simulation
/// @file           bindings/python/src/OpenSpaceToolkitSimulationPy/Component.cpp
/// @author         Lucas Brémond <lucas@loftorbital.com>, Remy Derollez <remy@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <OpenSpaceToolkitSimulationPy/Component/Geometry.cpp>
#include <OpenSpaceToolkitSimulationPy/Component/State.cpp>

#include <OpenSpaceToolkit/Simulation/Component.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void                     OpenSpaceToolkitSimulationPy_Component      (           pybind11::module&           aModule                                     )
{

    using namespace pybind11 ;

    using ostk::core::types::String ;
    using ostk::core::ctnr::Array ;

    using ostk::math::geom::d3::trf::rot::RotationMatrix ;

    using ostk::simulation::Component ;

    using ostk::simulation::component::Geometry ;
    using ostk::simulation::component::State ;

    {

        class_<Component> component_class(aModule, "Component") ;

        component_class.def(init<const String&, const Component::Type&>())

            .def(init<const String&, const Component::Type&, const State&>())
            .def(init<const String&, const Component::Type&, const Array<String>&>())
            .def(init<const String&, const Component::Type&, const State&, const Array<String>&>())
            .def(init<const String&, const Component::Type&, const State&, const Array<String>&, const Array<Geometry>, const RotationMatrix>())

            .def("__str__", &(shiftToString<Component>))
            .def("__repr__", &(shiftToString<Component>))

            .def("is_defined", &Component::isDefined)
            .def("get_name", &Component::getName)
            .def("get_type", &Component::getType)
            .def("get_tags", &Component::getTags)
            .def("get_state", &Component::getState)
            .def("get_geometries", &Component::getGeometries)
            .def("get_rotation_matrix", &Component::getRotationMatrix)

            .def_static("undefined", &Component::Undefined)
        ;

        enum_<Component::Type>(component_class, "Type")

            .value("Undefined", Component::Type::Undefined)
            .value("Controller", Component::Type::Controller)
            .value("Sensor", Component::Type::Sensor)
            .value("Actuator", Component::Type::Actuator)
            .value("Other", Component::Type::Other)
        ;

    }

    // Create python submodule
    auto component = aModule.def_submodule("component") ;

    // Add __path__ attribute for submodule
    component.attr("__path__") = "ostk.simulation.component" ;

    // Add objects to python submodule
    OpenSpaceToolkitSimulationPy_Component_Geometry(component) ;
    OpenSpaceToolkitSimulationPy_Component_State(component) ;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
