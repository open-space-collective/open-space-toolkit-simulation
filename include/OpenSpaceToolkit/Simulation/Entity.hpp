/// Apache License 2.0

#ifndef __OpenSpaceToolkit_Simulation_Entity__
#define __OpenSpaceToolkit_Simulation_Entity__

#include <OpenSpaceToolkit/Core/Type/String.hpp>

namespace ostk
{
namespace simulation
{

using ostk::core::type::String;

/// @brief A base class representing a named entity in the simulation.
/// @details An Entity has an identifier and a name. It serves as the base class for
///          Components and other simulation objects.
///
/// @code{.cpp}
///     Entity entity("my-entity");
///     String name = entity.getName();
///     String id = entity.getId();
/// @endcode
class Entity
{
   public:
    /// @brief Construct an entity with a name.
    /// @details An identifier is automatically generated.
    ///          Throws ostk::core::error::runtime::Undefined if the name is empty.
    ///
    /// @code{.cpp}
    ///     Entity entity("my-entity");
    /// @endcode
    ///
    /// @param aName A name for the entity (must not be empty).
    Entity(const String& aName);

    /// @brief Construct an entity with an identifier and a name.
    /// @details Throws ostk::core::error::runtime::Undefined if the identifier or the name is empty.
    ///
    /// @code{.cpp}
    ///     Entity entity("entity-id", "my-entity");
    /// @endcode
    ///
    /// @param anId An identifier (must not be empty).
    /// @param aName A name (must not be empty).
    Entity(const String& anId, const String& aName);

    /// @brief Get the identifier of the entity.
    ///
    /// @code{.cpp}
    ///     Entity entity("entity-id", "my-entity");
    ///     String id = entity.getId(); // "entity-id"
    /// @endcode
    ///
    /// @return The identifier.
    String getId() const;

    /// @brief Get the name of the entity.
    ///
    /// @code{.cpp}
    ///     Entity entity("my-entity");
    ///     String name = entity.getName(); // "my-entity"
    /// @endcode
    ///
    /// @return The name.
    String getName() const;

    /// @brief Print the entity to an output stream.
    ///
    /// @code{.cpp}
    ///     Entity entity("my-entity");
    ///     entity.print(std::cout, true);
    /// @endcode
    ///
    /// @param [in] anOutputStream An output stream.
    /// @param [in] displayDecorators If true, display decorators.
    void print(std::ostream& anOutputStream, bool displayDecorators = true) const;

   private:
    String id_;
    String name_;
};

}  // namespace simulation
}  // namespace ostk

#endif
