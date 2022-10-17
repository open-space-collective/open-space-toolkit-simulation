////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Simulation
/// @file           OpenSpaceToolkit/Simulation/Satellite.hpp
/// @author         Remy Derollez <remy@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __OpenSpaceToolkit_Simulation_Satellite__
#define __OpenSpaceToolkit_Simulation_Satellite__

#include <OpenSpaceToolkit/Simulation/Payload.hpp>
#include <OpenSpaceToolkit/Simulation/Component.hpp>
#include <OpenSpaceToolkit/Simulation/Utilities/ComponentHolder.hpp>

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
using ostk::core::ctnr::Map ;
using ostk::core::ctnr::Array ;

using ostk::simulation::utilities::ComponentHolder ;
using ostk::simulation::Component ;
using ostk::simulation::Payload ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief                      Satellite

class Satellite : public ComponentHolder
{

    public:

                                Satellite                                   (   const   String&                     anId,
                                                                                const   String&                     aName,
                                                                                const   Array<Payload>&             aPayloadArray                               ) ;

                                Satellite                                   (   const   String&                     anId,
                                                                                const   String&                     aName                                       ) ;

        friend std::ostream&    operator <<                                 (           std::ostream&               anOutputStream,
                                                                                const   Satellite&                  aSatellite                                 ) ;

                                Satellite                                   (   const   Satellite&                  aSatellite                                 ) = default ;

                                ~Satellite                                  ( ) ;

        Satellite&              operator =                                  (   const   Satellite&                  aSatellite                                 ) = default ;

        bool                    isDefined                                   ( ) const ;

        String                  getId                                       ( ) const ;

        String                  getName                                     ( ) const ;

        Array<Payload>          getPayloads                                 ( ) const ;

        Payload                 getPayloadWithId                            (   const   String&                     aPayloadId                                  ) const ;

        /// @brief              Print satellite
        ///
        /// @param              [in] anOutputStream An output stream
        /// @param              [in] (optional) displayDecorators If true, display decorators

        void                    print                                       (           std::ostream&               anOutputStream,
                                                                                        bool                        displayDecorators                           =   true ) const ;

        static Satellite        Undefined                                   ( ) ;

    private:

        String                  id_ ;
        String                  name_ ;
        Array<Payload>          payloads_ ;

} ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
