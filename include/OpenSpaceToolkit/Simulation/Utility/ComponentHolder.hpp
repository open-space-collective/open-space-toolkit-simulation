/// Apache License 2.0

#ifndef __OpenSpaceToolkit_Simulation_Utilties_ComponentHolder__
#define __OpenSpaceToolkit_Simulation_Utilties_ComponentHolder__

#include <OpenSpaceToolkit/Core/Container/Array.hpp>
#include <OpenSpaceToolkit/Core/Container/Map.hpp>
#include <OpenSpaceToolkit/Core/Container/Pair.hpp>
#include <OpenSpaceToolkit/Core/Type/Shared.hpp>
#include <OpenSpaceToolkit/Core/Type/String.hpp>
#include <OpenSpaceToolkit/Core/Type/Unique.hpp>

namespace ostk
{
namespace simulation
{

class Component;
class ComponentConfiguration;

namespace utility
{

using ostk::core::type::String;
using ostk::core::type::Unique;
using ostk::core::type::Shared;
using ostk::core::ctnr::Pair;
using ostk::core::ctnr::Array;
using ostk::core::ctnr::Map;

/// @brief                      Generic Component holder

class ComponentHolder
{
   public:
    ComponentHolder(const Array<Shared<Component>>& aComponentArray = Array<Shared<Component>>::Empty());

    ComponentHolder(const ComponentHolder& aComponentHolder);

    ~ComponentHolder();

    bool hasComponentWithId(const String& aComponentId) const;

    bool hasComponentWithName(const String& aComponentName) const;

    bool hasComponentAt(const String& aComponentPath) const;

    Array<Shared<Component>> accessComponents() const;

    const Component& accessComponentWithId(const String& aComponentId) const;

    const Component& accessComponentWithName(const String& aComponentName) const;

    Array<Shared<const Component>> accessComponentsWithTag(const String& aComponentTag) const;

    const Component& accessComponentAt(const String& aComponentPath) const;

    void addComponent(const Shared<Component>& aComponentSPtr);

   private:
    Map<String, Shared<Component>> componentMap_;
};

Pair<String, String> splitComponentPath(const String& aComponentPath);

}  // namespace utility
}  // namespace simulation
}  // namespace ostk

#endif
