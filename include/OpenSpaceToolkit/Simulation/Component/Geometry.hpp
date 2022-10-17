////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Simulation
/// @file           OpenSpaceToolkit/Simulation/Component/Geometry.hpp
/// @author         Lucas Brémond <lucas@loftorbital.com>, Remy Derollez <remy@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __OpenSpaceToolkit_Simulation_Component_Geometry__
#define __OpenSpaceToolkit_Simulation_Component_Geometry__

#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Objects/Composite.hpp>
#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Object.hpp>

#include <OpenSpaceToolkit/Core/Types/String.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace ostk
{
namespace simulation
{
namespace component
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using ostk::core::types::String ;
using ostk::math::geom::d3::Object ;
using ostk::math::geom::d3::objects::Composite ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief                      Component geometry

class Geometry
{

    public:

        enum class Type
        {

            Sensing,
            Exclusion,
            Undefined

        } ;

                                Geometry                                    (   const   String&                     aName,
                                                                                const   Type&                       aType,
                                                                                const   Object&                     aGeometry                                   ) ;

                                Geometry                                    (   const   String&                     aName,
                                                                                const   Type&                       aType,
                                                                                const   Composite&                  aComposite                                  ) ;

        friend std::ostream&    operator <<                                 (           std::ostream&               anOutputStream,
                                                                                const   Geometry&                   aGeometry                                   ) ;

        bool                    isDefined                                   ( ) const ;

        /// @brief              Get name

        String                  getName                                     ( ) const ;

        /// @brief              Get type

        Type                    getType                                     ( ) const ;

        /// @brief              Get geometry composite

        Composite               getComposite                                ( ) const ;

        /// @brief              Get exclusion geometry object (Valid for Exclusion Type only)

        String                  getExclusionObject                          ( ) const ;

        /// @brief              Get exclusion geometry illumination condition (Valid for Exclusion Type only)

        String                  getExclusionCondition                       ( ) const ;

        static Geometry         Undefined                                   ( ) ;

    private:

        String                  name_ ;
        Type                    type_ ;
        Composite               composite_ ;

} ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
