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

using ostk::core::container::Array;
using ostk::core::container::Map;
using ostk::core::container::Pair;
using ostk::core::type::Shared;
using ostk::core::type::String;
using ostk::core::type::Unique;

/// @brief A container that holds and manages a collection of components.
/// @details Provides lookup by identifier, name, tag, and hierarchical path.
///
/// @code{.cpp}
///     ComponentHolder holder(componentArray);
///     bool exists = holder.hasComponentWithName("sensor");
///     const Component& sensor = holder.accessComponentWithName("sensor");
/// @endcode
class ComponentHolder
{
   public:
    /// @brief Construct a component holder.
    ///
    /// @code{.cpp}
    ///     ComponentHolder holder(componentArray);
    /// @endcode
    ///
    /// @param [in] aComponentArray An array of components (default: empty).
    ComponentHolder(const Array<Shared<Component>>& aComponentArray = Array<Shared<Component>>::Empty());

    /// @brief Copy constructor.
    ///
    /// @code{.cpp}
    ///     ComponentHolder copy(holder);
    /// @endcode
    ///
    /// @param [in] aComponentHolder A component holder.
    ComponentHolder(const ComponentHolder& aComponentHolder);

    /// @brief Destructor.
    ~ComponentHolder();

    /// @brief Check if a component with the given identifier exists.
    ///
    /// @code{.cpp}
    ///     bool exists = holder.hasComponentWithId("component-id");
    /// @endcode
    ///
    /// @param [in] aComponentId A component identifier.
    /// @return True if a component with the identifier exists.
    bool hasComponentWithId(const String& aComponentId) const;

    /// @brief Check if a component with the given name exists.
    ///
    /// @code{.cpp}
    ///     bool exists = holder.hasComponentWithName("sensor");
    /// @endcode
    ///
    /// @param [in] aComponentName A component name.
    /// @return True if a component with the name exists.
    bool hasComponentWithName(const String& aComponentName) const;

    /// @brief Check if a component exists at the given hierarchical path.
    ///
    /// @code{.cpp}
    ///     bool exists = holder.hasComponentAt("assembly.sensor");
    /// @endcode
    ///
    /// @param [in] aComponentPath A dot-separated component path (e.g., "assembly.sensor").
    /// @return True if a component exists at the path.
    bool hasComponentAt(const String& aComponentPath) const;

    /// @brief Access all components.
    ///
    /// @code{.cpp}
    ///     Array<Shared<Component>> components = holder.accessComponents();
    /// @endcode
    ///
    /// @return An array of shared pointers to the components.
    Array<Shared<Component>> accessComponents() const;

    /// @brief Access a component by identifier.
    ///
    /// @code{.cpp}
    ///     const Component& component = holder.accessComponentWithId("component-id");
    /// @endcode
    ///
    /// @param [in] aComponentId A component identifier.
    /// @return A reference to the component.
    const Component& accessComponentWithId(const String& aComponentId) const;

    /// @brief Access a component by name.
    ///
    /// @code{.cpp}
    ///     const Component& sensor = holder.accessComponentWithName("sensor");
    /// @endcode
    ///
    /// @param [in] aComponentName A component name.
    /// @return A reference to the component.
    const Component& accessComponentWithName(const String& aComponentName) const;

    /// @brief Access all components matching a tag.
    ///
    /// @code{.cpp}
    ///     Array<Shared<const Component>> sensors = holder.accessComponentsWithTag("sensor");
    /// @endcode
    ///
    /// @param [in] aComponentTag A tag.
    /// @return An array of shared pointers to the matching components.
    Array<Shared<const Component>> accessComponentsWithTag(const String& aComponentTag) const;

    /// @brief Access a component at a hierarchical path.
    ///
    /// @code{.cpp}
    ///     const Component& sensor = holder.accessComponentAt("assembly.sensor");
    /// @endcode
    ///
    /// @param [in] aComponentPath A dot-separated component path (e.g., "assembly.sensor").
    /// @return A reference to the component.
    const Component& accessComponentAt(const String& aComponentPath) const;

    /// @brief Add a component.
    ///
    /// @code{.cpp}
    ///     holder.addComponent(componentSPtr);
    /// @endcode
    ///
    /// @param [in] aComponentSPtr A shared pointer to a component.
    void addComponent(const Shared<Component>& aComponentSPtr);

   private:
    Map<String, Shared<Component>> componentMap_;
};

/// @brief Split a hierarchical component path into its first segment and the remainder.
///
/// @code{.cpp}
///     Pair<String, String> result = splitComponentPath("assembly.sensor");
///     // result.first == "assembly", result.second == "sensor"
/// @endcode
///
/// @param [in] aComponentPath A dot-separated component path (e.g., "assembly.sensor").
/// @return A pair containing the first segment and the remaining path.
Pair<String, String> splitComponentPath(const String& aComponentPath);

}  // namespace utility
}  // namespace simulation
}  // namespace ostk

#endif
