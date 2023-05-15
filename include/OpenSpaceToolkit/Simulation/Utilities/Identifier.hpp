/// Apache License 2.0

#ifndef __OpenSpaceToolkit_Simulation_Utilties_Identifier__
#define __OpenSpaceToolkit_Simulation_Utilties_Identifier__

#include <OpenSpaceToolkit/Core/Containers/Array.hpp>
#include <OpenSpaceToolkit/Core/Containers/Map.hpp>
#include <OpenSpaceToolkit/Core/Containers/Pair.hpp>
#include <OpenSpaceToolkit/Core/Types/Shared.hpp>
#include <OpenSpaceToolkit/Core/Types/String.hpp>
#include <OpenSpaceToolkit/Core/Types/Unique.hpp>

namespace ostk
{
namespace simulation
{
namespace utilities
{

using ostk::core::types::String;

String generateId();

}  // namespace utilities
}  // namespace simulation
}  // namespace ostk

#endif
