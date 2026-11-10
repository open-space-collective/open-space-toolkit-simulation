/// Apache License 2.0

#include <OpenSpaceToolkit/Simulation/Entity.hpp>

/// @brief Bind the methods that every entity exposes (`get_id`, `get_name`) on a class whose C++ type derives from
///        Entity.
///
/// nanobind supports a single base class per bound type, and passes an instance to a base class method without
/// adjusting the pointer. `Component` derives from both Entity and ComponentHolder, and neither sits at offset zero
/// in it, so it cannot list Entity as its Python base. The methods are bound on it directly, through member pointers
/// of the derived type, which apply the base class adjustment in C++.
template <typename Class>
inline void OpenSpaceToolkitSimulationPy_Entity_Methods(Class& aClass)
{
    using namespace nanobind;

    using ostk::core::type::String;

    using ostk::simulation::Entity;

    using T = typename Class::Type;

    aClass

        .def(
            "get_id",
            static_cast<const String& (T::*)() const>(&Entity::getId),
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
            static_cast<const String& (T::*)() const>(&Entity::getName),
            R"doc(
                Get the name of the entity.

                Returns:
                    str: The entity name.

                Example:
                    >>> entity.get_name()
                    'My Entity'
            )doc"
        );
}

inline void OpenSpaceToolkitSimulationPy_Entity(nanobind::module_& aModule)
{
    using namespace nanobind;

    using ostk::simulation::Entity;

    {
        class_<Entity> entity_class(
            aModule,
            "Entity",
            R"doc(
                Base class representing a simulation entity with ID and name properties.

                An Entity is the fundamental building block in the simulation, providing identification
                and naming capabilities.
            )doc"
        );

        OpenSpaceToolkitSimulationPy_Entity_Methods(entity_class);
    }
}
