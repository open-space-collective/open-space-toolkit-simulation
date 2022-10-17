////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Simulation
/// @file           OpenSpaceToolkit/Simulation/Payload.cpp
/// @author         Lucas Brémond <lucas@loftorbital.com>, Remy Derollez <remy@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <OpenSpaceToolkit/Simulation/Payload.hpp>

#include <OpenSpaceToolkit/Core/Error.hpp>
#include <OpenSpaceToolkit/Core/Utilities.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace ostk
{
namespace simulation
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                                Payload::Payload                            (   const   String&                     anId,
                                                                                const   String&                     aName,
                                                                                const   String&                     aModel,
                                                                                const   Array<Geometry>&            aGeometryArray                              )
                                :   Component(
                                    aName,
                                    Component::Type::Undefined,
                                    State::Undefined(),
                                    {""},
                                    aGeometryArray,
                                    RotationMatrix::Undefined()),
                                    id_(anId),
                                    model_(aModel)
{

}

std::ostream&                   operator <<                                 (           std::ostream&               anOutputStream,
                                                                                const   Payload&                    aPayload                                    )
{

    aPayload.print(anOutputStream, true) ;

    return anOutputStream ;

}

bool                            Payload::isDefined                          ( ) const
{
    return !id_.isEmpty() ;
}

String                          Payload::getId                              ( ) const
{
    return id_ ;
}

String                          Payload::getModel                           ( ) const
{
    return model_ ;
}

Payload                         Payload::Undefined                          ( )
{
    return { String::Empty(), String::Empty(), String::Empty(), Array<Geometry>::Empty() } ;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void                            Payload::print                              (           std::ostream&               anOutputStream,
                                                                                        bool                        displayDecorators                           ) const
{

    displayDecorators ? ostk::core::utils::Print::Header(anOutputStream, "Payload") : void () ;

    ostk::core::utils::Print::Line(anOutputStream) << "ID:"    << id_ ;
    ostk::core::utils::Print::Line(anOutputStream) << "Name:"  << this->getName() ;
    ostk::core::utils::Print::Line(anOutputStream) << "Model:" << model_ ;

    displayDecorators ? ostk::core::utils::Print::Footer(anOutputStream) : void () ;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
