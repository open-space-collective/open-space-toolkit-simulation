////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Simulation
/// @file           OpenSpaceToolkit/Simulation/Entity.cpp
/// @author         Remy Derollez <remy@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <OpenSpaceToolkit/Simulation/Utilities/Identifier.hpp>
#include <OpenSpaceToolkit/Simulation/Entity.hpp>

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

                                Entity::Entity                              (   const   String&                     aName                                       )
                                :   id_(utilities::generateId()),
                                    name_(aName)
{

}

                                Entity::Entity                              (   const   String&                     anId,
                                                                                const   String&                     aName                                       )
                                :   id_(anId),
                                    name_(aName)
{

}

bool                            Entity::isDefined                           ( ) const
{
    return (!id_.isEmpty()) && (!name_.isEmpty()) ;
}

String                          Entity::getId                               ( ) const
{

    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Entity") ;
    }

    return id_ ;

}

String                          Entity::getName                             ( ) const
{

    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Entity") ;
    }

    return name_ ;

}

Entity                          Entity::Undefined                           ( )
{
    return { String::Empty(), String::Empty() } ;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void                            Entity::print                               (           std::ostream&               anOutputStream,
                                                                                        bool                        displayDecorators                           ) const
{

    displayDecorators ? ostk::core::utils::Print::Header(anOutputStream, "Entity") : void () ;

    ostk::core::utils::Print::Line(anOutputStream) << "ID:" << id_ ;
    ostk::core::utils::Print::Line(anOutputStream) << "Name:" << name_ ;

    displayDecorators ? ostk::core::utils::Print::Footer(anOutputStream) : void () ;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
