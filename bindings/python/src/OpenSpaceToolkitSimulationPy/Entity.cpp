/// Apache License 2.0

#include <OpenSpaceToolkit/Simulation/Entity.hpp>

inline void OpenSpaceToolkitSimulationPy_Entity(pybind11::module& aModule)
{
    using namespace pybind11;

    using ostk::core::type::Shared;

    using ostk::simulation::Entity;

    {
        class_<Entity, Shared<Entity>>(
            aModule,
            "Entity",
            R"doc(
                Base class representing a simulation entity with ID and name properties.

                An Entity is the fundamental building block in the simulation, providing identification
                and naming capabilities.
            )doc"
        )

            .def(
                "is_defined",
                &Entity::isDefined,
                R"doc(
                    Check if the entity is defined.

                    Returns:
                        bool: True if the entity is defined, False otherwise.

                    Example:
                        >>> entity.is_defined()
                        True
                )doc"
            )

            .def(
                "get_id",
                &Entity::getId,
                R"doc(
                    Get the unique identifier of the entity.

                    Returns:
                        str: The unique identifier.

                    Example:
                        >>> entity.get_id()
                        'my-entity'
                )doc"
            )

            .def(
                "get_name",
                &Entity::getName,
                R"doc(
                    Get the name of the entity.

                    Returns:
                        str: The entity name.

                    Example:
                        >>> entity.get_name()
                        'My Entity'
                )doc"
            )

            .def_static(
                "undefined",
                &Entity::Undefined,
                R"doc(
                    Create an undefined entity.

                    Returns:
                        Entity: An undefined entity.

                    Example:
                        >>> entity = Entity.undefined()
                        >>> entity.is_defined()
                        False
                )doc"
            )

            ;
    }
}
