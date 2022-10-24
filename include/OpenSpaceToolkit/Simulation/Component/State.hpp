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

#define                         DEFAULT_STATUS                                  State::Status::Disabled
#define                         DEFAULT_VERBOSE                                 false

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

                                State                                       (   const   State::Status&              aStatus                                     =   DEFAULT_STATUS,
                                                                                const   bool&                       isVerbose                                   =   DEFAULT_VERBOSE ) ;

                                State                                       (   const   State&                      aState                                      ) ;

        virtual                 ~State                                      ( ) ;

        virtual State*          clone                                       ( ) const ;

        State&                  operator =                                  (   const   State&                      aState                                      ) ;

        /// @brief              Equal to operator
        ///
        /// @code
        ///                     State firstState = ... ;
        ///                     State secondState = ... ;
        ///                     firstState == secondState ; // True
        /// @endcode
        ///
        /// @param              [in] aState A state
        /// @return             True if states are equal

        bool                    operator ==                                 (   const   State&                      aState                                      ) const ;

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
