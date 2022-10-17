////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Simulation
/// @file           OpenSpaceToolkit/Simulation/Satellite.hpp
/// @author         Remy Derollez <remy@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __OpenSpaceToolkit_Simulation_Satellite__
#define __OpenSpaceToolkit_Simulation_Satellite__

#include <OpenSpaceToolkit/Simulation/Component.hpp>
#include <OpenSpaceToolkit/Simulation/Entity.hpp>
#include <OpenSpaceToolkit/Simulation/Utilities/ComponentHolder.hpp>

#include <OpenSpaceToolkit/Astrodynamics/Flight/Profile.hpp>

#include <OpenSpaceToolkit/Physics/Coordinate/Frame.hpp>

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

using ostk::physics::coord::Frame ;

using ostk::astro::flight::Profile ;

using ostk::simulation::utilities::ComponentHolder ;
using ostk::simulation::Component ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief                      Satellite

class Satellite : public Entity, public ComponentHolder
{

    public:

                                Satellite                                   (   const   String&                     aName,
                                                                                const   Profile&                    aProfile,
                                                                                const   Array<Component>&           aComponentArray                             =   Array<Component>::Empty() ) ;

                                Satellite                                   (   const   String&                     anId,
                                                                                const   String&                     aName,
                                                                                const   Profile&                    aProfile,
                                                                                const   Array<Component>&           aComponentArray                             =   Array<Component>::Empty() ) ;

                                Satellite                                   (   const   Satellite&                  aSatellite                                 ) = default ;

                                ~Satellite                                  ( ) ;

        Satellite*              clone                                       ( ) const ;

        Satellite&              operator =                                  (   const   Satellite&                  aSatellite                                 ) = default ;

        friend std::ostream&    operator <<                                 (           std::ostream&               anOutputStream,
                                                                                const   Satellite&                  aSatellite                                 ) ;

        bool                    isDefined                                   ( ) const ;

        const Shared<const Frame>& accessFrame                              ( ) const ;

        /// @brief              Print satellite
        ///
        /// @param              [in] anOutputStream An output stream
        /// @param              [in] (optional) displayDecorators If true, display decorators

        void                    print                                       (           std::ostream&               anOutputStream,
                                                                                        bool                        displayDecorators                           =   true ) const ;

        static Satellite        Undefined                                   ( ) ;

        static Shared<const Frame> GenerateFrame                            (   const   String&                     aName,
                                                                                const   Profile&                    aProfile                                    ) ;

    private:

        Shared<const Frame>     frameSPtr_ ;

} ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
