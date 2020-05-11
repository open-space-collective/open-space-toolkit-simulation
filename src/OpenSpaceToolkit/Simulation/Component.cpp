////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Simulation
/// @file           OpenSpaceToolkit/Simulation/Component.cpp
/// @author         Lucas Brémond <lucas@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <OpenSpaceToolkit/Simulation/Component.hpp>

#include <OpenSpaceToolkit/Core/Error.hpp>
#include <OpenSpaceToolkit/Core/Utilities.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace ostk
{
namespace simulation
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                                Component::Component                        (   const   String&                     aName,
                                                                                const   Component::Type&            aType                                       )
                                :   ComponentHolder(),
                                    name_(aName),
                                    type_(aType),
                                    state_(State::Undefined()),
                                    geometry_(Geometry::Undefined()),
                                    spacecraftWPtr_()
{

}

                                Component::Component                        (   const   String&                     aName,
                                                                                const   Component::Type&            aType,
                                                                                const   State&                      aState,
                                                                                const   Geometry&                   aGeometry                                   )
                                :   ComponentHolder(),
                                    name_(aName),
                                    type_(aType),
                                    state_(aState),
                                    geometry_(aGeometry),
                                    spacecraftWPtr_()
{

}

                                Component::Component                        (   const   Component&                  aComponent                                  )
                                :   ComponentHolder(aComponent),
                                    name_(aComponent.name_),
                                    type_(aComponent.type_),
                                    state_(aComponent.state_),
                                    geometry_(aComponent.geometry_),
                                    spacecraftWPtr_(aComponent.spacecraftWPtr_)
{

}

                                Component::~Component                       ( )
{

}

Component*                      Component::clone                            ( ) const
{
    return new Component(*this) ;
}

Component&                      Component::operator =                       (   const   Component&                  aComponent                                  )
{

    if (this != &aComponent)
    {

        this->ComponentHolder::operator = (aComponent) ;

        name_ = aComponent.name_ ;
        type_ = aComponent.type_ ;
        state_ = aComponent.state_ ;
        geometry_ = aComponent.geometry_ ;
        spacecraftWPtr_ = aComponent.spacecraftWPtr_ ;

    }

    return *this ;

}

bool                            Component::isDefined                        ( ) const
{
    return !name_.isEmpty() ;
}

String                          Component::getName                          ( ) const
{

    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Component") ;
    }

    return name_ ;

}

Component                       Component::Undefined                        ( )
{
    return { String::Empty(), Component::Type::Undefined } ;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
