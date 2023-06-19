/// Apache License 2.0

#include <OpenSpaceToolkitSimulationPy/Component/State.cpp>
#include <OpenSpaceToolkitSimulationPy/Component/Geometry.cpp>

#include <OpenSpaceToolkit/Simulation/Component.hpp>

inline void                     OpenSpaceToolkitSimulationPy_Component      (           pybind11::module&           aModule                                     )
{

    using namespace pybind11 ;

    using ostk::core::types::Shared ;
    using ostk::core::types::String ;
    using ostk::core::ctnr::Array ;

    using ostk::math::geom::d3::trf::rot::Quaternion ;

    using ostk::physics::coord::Frame ;

    using ostk::simulation::Simulator ;
    using ostk::simulation::Entity ;
    using ostk::simulation::Component ;
    using ostk::simulation::ComponentConfiguration ;
    using ostk::simulation::component::Geometry ;
    using ostk::simulation::component::GeometryConfiguration ;
    using ostk::simulation::component::State ;
    using ostk::simulation::utilities::ComponentHolder ;

    {

        class_<Component, Entity, ComponentHolder, Shared<Component>> component_class(aModule, "Component") ;

        component_class.def
        (
            init
            <
                const String&,
                const String&,
                const Component::Type&,
                const Array<String>&,
                const Array<Shared<Geometry>>&,
                const Array<Shared<Component>>&,
                const Shared<ComponentHolder>&,
                const Shared<const Frame>&,
                const Shared<const Simulator>&
            >(),
            arg("id"),
            arg("name"),
            arg("type"),
            arg("tags"),
            arg("geometries"),
            arg("components"),
            arg("parent"),
            arg("frame"),
            arg("simulator")
        )

            .def("__str__", &(shiftToString<Component>))
            .def("__repr__", &(shiftToString<Component>))

            .def("is_defined", &Component::isDefined)

            .def("access_frame", &Component::accessFrame)
            .def("access_geometry_with_name", &Component::accessGeometryWithName, arg("name"))
            .def("access_simulator", &Component::accessSimulator)

            .def("get_type", &Component::getType)
            .def("get_tags", &Component::getTags)
            .def("get_geometries", &Component::getGeometries)

            .def("set_parent", &Component::setParent, arg("component"))
            .def("add_geometry", &Component::addGeometry, arg("geometry"))
            .def("add_component", &Component::addComponent, arg("component"))

            .def_static("undefined", &Component::Undefined)
            .def_static("configure", &Component::Configure, arg("configuration"), arg("parent_component"))

            .def_static("string_from_type", &Component::StringFromType, arg("type"))

        ;

    enum_<Component::Type>(component_class, "Type")

            .value("Undefined", Component::Type::Undefined)
            .value("Assembly", Component::Type::Assembly)
            .value("Controller", Component::Type::Controller)
            .value("Sensor", Component::Type::Sensor)
            .value("Actuator", Component::Type::Actuator)
            .value("Other", Component::Type::Other)
        ;

    }

    class_<ComponentConfiguration>(aModule, "ComponentConfiguration")

        .def
        (
            init
            <
                const String&,
                const String&,
                const Component::Type&,
                const Array<String>&,
                const Quaternion&,
                const Array<GeometryConfiguration>&,
                const Array<ComponentConfiguration>&
            >(),
            arg("id"),
            arg("name"),
            arg("type") = DEFAULT_COMPONENT_TYPE,
            arg("tags") = DEFAULT_TAGS,
            arg("orientation") = DEFAULT_ORIENTATION,
            arg("geometries") = DEFAULT_GEOMETRIES,
            arg("components") = DEFAULT_COMPONENTS
        )

    ;

    // Create python submodule
    auto component = aModule.def_submodule("component") ;

    // Add __path__ attribute for submodule
    component.attr("__path__") = "ostk.simulation.component" ;

    // Add objects to python submodule
    OpenSpaceToolkitSimulationPy_Component_Geometry(component) ;
    OpenSpaceToolkitSimulationPy_Component_State(component) ;

}
