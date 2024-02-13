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
namespace utilities
{

using ostk::core::type::String;

String generateId();

}  // namespace utilities
}  // namespace simulation
}  // namespace ostk

#endif
