////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Simulation
/// @file           OpenSpaceToolkit/Simulation/Simulator.hpp
/// @author         Remy Derollez <remy@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __OpenSpaceToolkit_Simulation_Simulator__
#define __OpenSpaceToolkit_Simulation_Simulator__

#include <OpenSpaceToolkit/Simulation/Satellite.hpp>

#include <OpenSpaceToolkit/Physics/Time/Duration.hpp>
#include <OpenSpaceToolkit/Physics/Time/Instant.hpp>
#include <OpenSpaceToolkit/Physics/Environment.hpp>

#include <OpenSpaceToolkit/Core/Containers/Array.hpp>
#include <OpenSpaceToolkit/Core/Containers/Map.hpp>
#include <OpenSpaceToolkit/Core/Types/Shared.hpp>
#include <OpenSpaceToolkit/Core/Types/String.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace ostk
{
namespace simulation
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using ostk::core::types::String ;
using ostk::core::types::Shared ;
using ostk::core::ctnr::Array ;
using ostk::core::ctnr::Map ;

using ostk::physics::Environment ;
using ostk::physics::time::Instant ;
using ostk::physics::time::Duration ;

using ostk::simulation::Satellite ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief                      Simulator

class Simulator
{

    public:

                                Simulator                                   (   const   Environment&                anEnvironment,
                                                                                const   Array<Satellite>&           aSatelliteArray                             ) ;

        friend std::ostream&    operator <<                                 (           std::ostream&               anOutputStream,
                                                                                const   Simulator&                  aSimulator                                  ) ;

        bool                    isDefined                                   ( ) const ;

        bool                    hasSatelliteWithName                        (   const   String&                     aSatelliteName                              ) const ;

        const Environment&      accessEnvironment                           ( ) const ;

        Satellite&              accessSatelliteWithName                     (   const   String&                     aSatelliteName                              ) const ;

        /// @brief              Print simulator
        ///
        /// @param              [in] anOutputStream An output stream
        /// @param              [in] (optional) displayDecorators If true, display decorators

        void                    print                                       (           std::ostream&               anOutputStream,
                                                                                        bool                        displayDecorators                           =   true ) const ;

        void                    setInstant                                  (   const   Instant&                    anInstant                                   ) ;

        void                    step                                        (   const   Duration&                   aDuration                                   ) ;

        static Simulator        Undefined                                   ( ) ;

    private:

        Environment             environment_ ;
        Map<String, Shared<Satellite>> satelliteMap_ ;

} ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
