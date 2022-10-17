////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Simulation
/// @file           OpenSpaceToolkit/Simulation/Satellite.cpp
/// @author         Remy Derollez <remy@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <OpenSpaceToolkit/Simulation/Satellite.hpp>

#include <OpenSpaceToolkit/Core/Error.hpp>
#include <OpenSpaceToolkit/Core/Utilities.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace ostk
{
namespace simulation
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                                Satellite::Satellite                        (   const   String&                     anId,
                                                                                const   String&                     aName,
                                                                                const   Array<Payload>&             aPayloadArray                               )
                                :   ComponentHolder(),
                                    id_(anId),
                                    name_(aName),
                                    payloads_(aPayloadArray)
{

}

                                Satellite::Satellite                        (   const   String&                     anId,
                                                                                const   String&                     aName                                       )
                                :   ComponentHolder(),
                                    id_(anId),
                                    name_(aName),
                                    payloads_(Array<Payload>::Empty())
{

}

                                Satellite::~Satellite                       ( )
{

}

bool                            Satellite::isDefined                        ( ) const
{
    return !name_.isEmpty() ;
}

String                          Satellite::getId                            ( ) const
{
    return id_ ;
}

String                          Satellite::getName                          ( ) const
{

    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Satellite") ;
    }

    return name_ ;

}

Array<Payload>                  Satellite::getPayloads                      ( ) const
{
    return payloads_ ;
}

Payload                         Satellite::getPayloadWithId                 (   const   String&                     aPayloadId                                  ) const
{

    for (const auto& payload : payloads_)
    {

        if (payload.getId() == aPayloadId)
        {
            return payload ;
        }

    }

    throw ostk::core::error::RuntimeError("No Payload found with ID [{}].", aPayloadId) ;

}

Satellite                       Satellite::Undefined                        ( )
{
    return { String::Empty(), String::Empty(), Array<Payload>::Empty() } ;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void                            Satellite::print                            (           std::ostream&               anOutputStream,
                                                                                        bool                        displayDecorators                           ) const
{

    displayDecorators ? ostk::core::utils::Print::Header(anOutputStream, "Satellite") : void () ;

    ostk::core::utils::Print::Line(anOutputStream) << "ID:" << id_ ;
    ostk::core::utils::Print::Line(anOutputStream) << "Name:" << name_ ;

    displayDecorators ? ostk::core::utils::Print::Footer(anOutputStream) : void () ;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
