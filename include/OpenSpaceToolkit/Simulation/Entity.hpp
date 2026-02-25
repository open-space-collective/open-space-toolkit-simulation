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
    ///
    /// @code{.cpp}
    ///     Entity entity("my-entity");
    /// @endcode
    ///
    /// @param aName A name for the entity.
    Entity(const String& aName);

    /// @brief Construct an entity with an identifier and a name.
    ///
    /// @code{.cpp}
    ///     Entity entity("entity-id", "my-entity");
    /// @endcode
    ///
    /// @param anId An identifier.
    /// @param aName A name.
    Entity(const String& anId, const String& aName);

    /// @brief Check if the entity is defined.
    ///
    /// @code{.cpp}
    ///     Entity entity("my-entity");
    ///     bool defined = entity.isDefined(); // true
    /// @endcode
    ///
    /// @return True if the entity is defined.
    bool isDefined() const;

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

    /// @brief Construct an undefined entity.
    ///
    /// @code{.cpp}
    ///     Entity entity = Entity::Undefined();
    ///     bool defined = entity.isDefined(); // false
    /// @endcode
    ///
    /// @return An undefined entity.
    static Entity Undefined();

   private:
    String id_;
    String name_;
};

}  // namespace simulation
}  // namespace ostk

#endif
