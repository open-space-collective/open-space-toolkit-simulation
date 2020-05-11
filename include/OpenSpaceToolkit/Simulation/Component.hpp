////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Simulation
/// @file           OpenSpaceToolkit/Simulation/Component.hpp
/// @author         Lucas Brémond <lucas@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __OpenSpaceToolkit_Simulation_Component__
#define __OpenSpaceToolkit_Simulation_Component__

#include <OpenSpaceToolkit/Simulation/Component/Geometry.hpp>
#include <OpenSpaceToolkit/Simulation/Component/State.hpp>
#include <OpenSpaceToolkit/Simulation/Utilities/ComponentHolder.hpp>

#include <OpenSpaceToolkit/Core/Containers/Array.hpp>
#include <OpenSpaceToolkit/Core/Types/Weak.hpp>
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
using ostk::core::types::Weak ;
using ostk::core::ctnr::Array ;

using ostk::simulation::utilities::ComponentHolder ;
using ostk::simulation::component::State ;
using ostk::simulation::component::Geometry ;

class Spacecraft ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief                      Component

class Component : public ComponentHolder
{

    public:

        enum class Type
        {

            Undefined,
            Controller,
            Sensor,
            Actuator,
            Other

        } ;

                                Component                                   (   const   String&                     aName,
                                                                                const   Component::Type&            aType                                       ) ;

                                Component                                   (   const   String&                     aName,
                                                                                const   Component::Type&            aType,
                                                                                const   State&                      aState,
                                                                                const   Geometry&                   aGeometry                                   ) ;

                                Component                                   (   const   Component&                  aComponent                                  ) ;

        virtual                 ~Component                                  ( ) ;

        virtual Component*      clone                                       ( ) const ;

        Component&              operator =                                  (   const   Component&                  aComponent                                  ) ;

        bool                    isDefined                                   ( ) const ;

        String                  getName                                     ( ) const ;

        Component::Type         getType                                     ( ) const ;

        State                   getState                                    ( ) const ;

        Geometry                getGeometry                                 ( ) const ;

        const State&            accessState                                 ( ) const ;

        const Geometry&         accessGeometry                              ( ) const ;

        static Component        Undefined                                   ( ) ;

    private:

        String                  name_ ;
        Component::Type         type_ ;

        State                   state_ ;
        Geometry                geometry_ ;

        Weak<const Spacecraft>  spacecraftWPtr_ ;

} ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
