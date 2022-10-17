////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Simulation
/// @file           OpenSpaceToolkit/Simulation/Component/State.hpp
/// @author         Lucas Brémond <lucas@loftorbital.com>, Remy Derollez <remy@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __OpenSpaceToolkit_Simulation_Component_State__
#define __OpenSpaceToolkit_Simulation_Component_State__

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace ostk
{
namespace simulation
{
namespace component
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief                      Component state

class State
{

    public:

        enum class Status
        {

            Undefined,
            Disabled,
            Idle,
            Busy,
            Error

        } ;

                                State                                       ( ) ;

                                State                                       (   const   State::Status&              aStatus                                     ) ;

                                State                                       (   const   State&                      aState                                      ) ;

        virtual                 ~State                                      ( ) ;

        virtual State*          clone                                       ( ) const ;

        State&                  operator =                                  (   const   State&                      aState                                      ) ;

        bool                    isDefined                                   ( ) const ;

        State::Status           getStatus                                   ( ) const ;

        static State            Undefined                                   ( ) ;

    private:

        State::Status           status_ ;

        bool                    verbose_ ;

} ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
