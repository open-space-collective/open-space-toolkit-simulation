/// Apache License 2.0

#include <OpenSpaceToolkit/Simulation/Entity.hpp>
#include <OpenSpaceToolkit/Simulation/Utility/Identifier.hpp>

#include <OpenSpaceToolkit/Core/Error.hpp>
#include <OpenSpaceToolkit/Core/Utility.hpp>

namespace ostk
{
namespace simulation
{

using namespace ostk::simulation::utility;

Entity::Entity(const String& aName)
    : id_(utility::generateId()),
      name_(aName)
{
    if (name_.isEmpty())
    {
        throw ostk::core::error::runtime::Undefined("Entity name");
    }
}

Entity::Entity(const String& anId, const String& aName)
    : id_(anId),
      name_(aName)
{
    if (id_.isEmpty())
    {
        throw ostk::core::error::runtime::Undefined("Entity ID");
    }

    if (name_.isEmpty())
    {
        throw ostk::core::error::runtime::Undefined("Entity name");
    }
}

const String& Entity::getId() const
{
    return id_;
}

const String& Entity::getName() const
{
    return name_;
}

void Entity::print(std::ostream& anOutputStream, bool displayDecorators) const
{
    displayDecorators ? ostk::core::utils::Print::Header(anOutputStream, "Entity") : void();

    ostk::core::utils::Print::Line(anOutputStream) << "ID:" << id_;
    ostk::core::utils::Print::Line(anOutputStream) << "Name:" << name_;

    displayDecorators ? ostk::core::utils::Print::Footer(anOutputStream) : void();
}

}  // namespace simulation
}  // namespace ostk
