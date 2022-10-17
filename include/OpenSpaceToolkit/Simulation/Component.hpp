////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Simulation
/// @file           OpenSpaceToolkit/Simulation/Component.hpp
/// @author         Lucas Brémond <lucas@loftorbital.com>, Remy Derollez <remy@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __OpenSpaceToolkit_Simulation_Component__
#define __OpenSpaceToolkit_Simulation_Component__

#include <OpenSpaceToolkit/Simulation/Component/Geometry.hpp>
#include <OpenSpaceToolkit/Simulation/Component/State.hpp>
#include <OpenSpaceToolkit/Simulation/Utilities/ComponentHolder.hpp>

#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Transformations/Rotations/RotationMatrix.hpp>

#include <OpenSpaceToolkit/Core/Containers/Array.hpp>
#include <OpenSpaceToolkit/Core/Containers/Map.hpp>
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
using ostk::core::ctnr::Map ;

using ostk::math::geom::d3::trf::rot::RotationMatrix ;

using ostk::simulation::utilities::ComponentHolder ;
using ostk::simulation::component::State ;
using ostk::simulation::component::Geometry ;

class Satellite;

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
                                                                                const   State&                      aState                                      ) ;

                                Component                                   (   const   String&                     aName,
                                                                                const   Component::Type&            aType,
                                                                                const   Array<String>&              anArrayOfTags                               ) ;

                                Component                                   (   const   String&                     aName,
                                                                                const   Component::Type&            aType,
                                                                                const   State&                      aState,
                                                                                const   Array<String>&              anArrayofTags                               ) ;

                                Component                                   (   const   String&                     aName,
                                                                                const   Component::Type&            aType,
                                                                                const   State&                      aState,
                                                                                const   Array<String>&              anArrayofTags,
                                                                                const   Array<Geometry>&            anArrayOfGeometries,
                                                                                const   RotationMatrix&             aRotationMatrix                             ) ;

                                Component                                   (   const   Component&                  aComponent                                  ) ;

        virtual                 ~Component                                  ( ) ;

        virtual Component*      clone                                       ( ) const ;

        Component&              operator =                                  (   const   Component&                  aComponent                                  ) ;

        friend std::ostream&    operator <<                                 (           std::ostream&               anOutputStream,
                                                                                const   Component&                  aComponent                                  ) ;

        bool                    isDefined                                   ( ) const ;

        String                  getName                                     ( ) const ;

        Component::Type         getType                                     ( ) const ;

        Array<String>           getTags                                     ( ) const ;

        State                   getState                                    ( ) const ;

        Array<Geometry>         getGeometries                               ( ) const ;

        RotationMatrix          getRotationMatrix                           ( ) const ;

        const State&            accessState                                 ( ) const ;

        const Geometry&         accessGeometryWithName                      ( ) const ;

        static Component        Undefined                                   ( ) ;

        static String           StringFromType                              (   const   Component::Type&            aType                                       ) ;

    protected:

        /// @brief              Print component
        ///
        /// @param              [in] anOutputStream An output stream
        /// @param              [in] (optional) displayDecorators If true, display decorators

        virtual void            print                                       (           std::ostream&               anOutputStream,
                                                                                        bool                        displayDecorators                           =   true ) const ;

    private:

        String                  name_ ;
        Component::Type         type_ ;
        Array<String>           tags_ ;
        State                   state_ ;
        Array<Geometry>         geometries_ ; // Array of Geometries defined in Component Frame
        RotationMatrix          rotationMatrix_ ; // Rotation from Body Frame to Component Frame
        Weak<const Satellite>   satelliteWPtr_ ;

} ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
