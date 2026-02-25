/// Apache License 2.0

#ifndef __OpenSpaceToolkit_Simulation_Utilties_Identifier__
#define __OpenSpaceToolkit_Simulation_Utilties_Identifier__

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
namespace utility
{

using ostk::core::type::String;

/// @brief Generate a unique identifier string.
///
/// @code{.cpp}
///     String id = generateId();
/// @endcode
///
/// @return A unique identifier string.
String generateId();

}  // namespace utility
}  // namespace simulation
}  // namespace ostk

#endif
