////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Simulation
/// @file           OpenSpaceToolkit/Simulation/Satellite.cpp
/// @author         Remy Derollez <remy@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <OpenSpaceToolkit/Simulation/Utilities/Identifier.hpp>
#include <OpenSpaceToolkit/Simulation/Satellite.hpp>

#include <OpenSpaceToolkit/Core/Error.hpp>
#include <OpenSpaceToolkit/Core/Utilities.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace ostk
{
namespace simulation
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using namespace ostk::simulation::utilities ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                                Satellite::Satellite                        (   const   String&                     aName,
                                                                                const   Array<Component>&           aComponentArray                             )
                                :   Entity(aName),
                                    ComponentHolder(aComponentArray)
{

}

                                Satellite::Satellite                        (   const   String&                     anId,
                                                                                const   String&                     aName,
                                                                                const   Array<Component>&           aComponentArray                             )
                                :   Entity(anId, aName),
                                    ComponentHolder(aComponentArray)
{

}

                                Satellite::~Satellite                       ( )
{

}

bool                            Satellite::isDefined                        ( ) const
{
    return Entity::isDefined() ;
}

Satellite                       Satellite::Undefined                        ( )
{
    return { String::Empty(), String::Empty() } ;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void                            Satellite::print                            (           std::ostream&               anOutputStream,
                                                                                        bool                        displayDecorators                           ) const
{

    displayDecorators ? ostk::core::utils::Print::Header(anOutputStream, "Satellite") : void () ;

    Entity::print(anOutputStream, false) ;

    displayDecorators ? ostk::core::utils::Print::Footer(anOutputStream) : void () ;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
