////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Simulation
/// @file           OpenSpaceToolkit/Simulation/Component/State.cpp
/// @author         Lucas Brémond <lucas@loftorbital.com>, Remy Derollez <remy@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <OpenSpaceToolkit/Simulation/Component/State.hpp>

#include <OpenSpaceToolkit/Core/Error.hpp>
#include <OpenSpaceToolkit/Core/Utilities.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace ostk
{
namespace simulation
{
namespace component
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                                State::State                                ( )
                                :   status_(State::Status::Disabled),
                                    verbose_(false)
{

}

                                State::State                                (   const   State::Status&              aStatus                                     )
                                :   status_(aStatus),
                                    verbose_(false)
{

}

                                State::State                                (   const   State&                      aState                                      )
                                :   status_(aState.status_),
                                    verbose_(aState.verbose_)
{

}

                                State::~State                               ( )
{

}

State*                          State::clone                                ( ) const
{
    return new State(*this) ;
}

State&                          State::operator =                           (   const   State&                      aState                                      )
{

    if (this != &aState)
    {
        status_ = aState.status_ ;
        verbose_ = aState.verbose_ ;
    }

    return *this ;

}

bool                            State::isDefined                            ( ) const
{
    return status_ != State::Status::Undefined ;
}

State::Status                   State::getStatus                            ( ) const
{

    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Status") ;
    }

    return status_ ;

}

State                           State::Undefined                            ( )
{
    return { State::Status::Undefined } ;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
