/// Apache License 2.0

#include <OpenSpaceToolkit/Simulation/Component.hpp>

#include <OpenSpaceToolkitSimulationPy/Component/Geometry.cpp>
#include <OpenSpaceToolkitSimulationPy/Component/State.cpp>

inline void OpenSpaceToolkitSimulationPy_Component(pybind11::module& aModule)
{
    using namespace pybind11;

    using ostk::core::container::Array;
    using ostk::core::type::Shared;
    using ostk::core::type::String;

    using ostk::mathematics::geometry::d3::transformation::rotation::Quaternion;

    using ostk::physics::coordinate::Frame;

    using ostk::simulation::Component;
    using ostk::simulation::component::Geometry;
    using ostk::simulation::component::GeometryConfiguration;
    using ostk::simulation::component::State;
    using ostk::simulation::ComponentConfiguration;
    using ostk::simulation::Entity;
    using ostk::simulation::Simulator;
    using ostk::simulation::utility::ComponentHolder;

    {
        class_<Component, Entity, ComponentHolder, Shared<Component>> component_class(aModule, "Component");

        component_class
            .def(
                init<
                    const String&,
                    const String&,
                    const Component::Type&,
                    const Array<String>&,
                    const Array<Shared<Geometry>>&,
                    const Array<Shared<Component>>&,
                    const Shared<ComponentHolder>&,
                    const Shared<const Frame>&,
                    const Shared<const Simulator>&>(),
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
            .value("Other", Component::Type::Other);
    }

    class_<ComponentConfiguration>(aModule, "ComponentConfiguration")

        .def(
            init<
                const String&,
                const String&,
                const Component::Type&,
                const Array<String>&,
                const Quaternion&,
                const Array<GeometryConfiguration>&,
                const Array<ComponentConfiguration>&>(),
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
    auto component = aModule.def_submodule("component");

    // Add objects to python submodule
    OpenSpaceToolkitSimulationPy_Component_Geometry(component);
    OpenSpaceToolkitSimulationPy_Component_State(component);
}
