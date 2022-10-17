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

    using ostk::core::types::String ;
    using ostk::core::ctnr::Array ;

    using ostk::math::geom::d3::Object ;
    using ostk::math::geom::d3::objects::Composite ;

    using ostk::simulation::component::Geometry ;

    class_<Geometry> geometry_class(aModule, "Geometry") ;

    geometry_class.def(init<const String&, const Geometry::Type&, const Object&>())

        .def(init<const String&, const Geometry::Type&, const Composite&>())

        .def("__str__", &(shiftToString<Geometry>))
        .def("__repr__", &(shiftToString<Geometry>))

        .def("is_defined", &Geometry::isDefined)
        .def("get_name", &Geometry::getName)
        .def("get_type", &Geometry::getType)
        .def("get_composite", &Geometry::getComposite)
        .def("get_exclusion_object", &Geometry::getExclusionObject)
        .def("get_exclusion_condition", &Geometry::getExclusionCondition)

        .def_static("undefined", &Geometry::Undefined)

    ;

    enum_<Geometry::Type>(geometry_class, "Type")

        .value("Undefined", Geometry::Type::Undefined)
        .value("Sensing", Geometry::Type::Sensing)
        .value("Exclusion", Geometry::Type::Exclusion)

    ;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
