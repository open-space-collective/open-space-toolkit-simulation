/// Apache License 2.0

#include <OpenSpaceToolkit/Simulation/Entity.hpp>

inline void OpenSpaceToolkitSimulationPy_Entity(pybind11::module& aModule)
{
    using namespace pybind11;

    using ostk::core::types::Shared;

    using ostk::simulation::Entity;

    {
        class_<Entity, Shared<Entity>>(aModule, "Entity")

            .def("is_defined", &Entity::isDefined)

            .def("get_id", &Entity::getId)
            .def("get_name", &Entity::getName)

            .def_static("undefined", &Entity::Undefined)

            ;
    }
}
