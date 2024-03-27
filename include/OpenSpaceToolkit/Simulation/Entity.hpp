/// Apache License 2.0

#ifndef __OpenSpaceToolkit_Simulation_Entity__
#define __OpenSpaceToolkit_Simulation_Entity__

#include <OpenSpaceToolkit/Core/Type/String.hpp>

namespace ostk
{
namespace simulation
{

using ostk::core::type::String;

/// @brief                      Entity

class Entity
{
   public:
    Entity(const String& aName);

    Entity(const String& anId, const String& aName);

    bool isDefined() const;

    String getId() const;

    String getName() const;

    /// @brief              Print entity
    ///
    /// @param              [in] anOutputStream An output stream
    /// @param              [in] (optional) displayDecorators If true, display decorators

    void print(std::ostream& anOutputStream, bool displayDecorators = true) const;

    static Entity Undefined();

   private:
    String id_;
    String name_;
};

}  // namespace simulation
}  // namespace ostk

#endif
