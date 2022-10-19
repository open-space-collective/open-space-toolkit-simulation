////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Simulation
/// @file           OpenSpaceToolkit/Simulation/Component/Geometry.hpp
/// @author         Lucas Brémond <lucas@loftorbital.com>, Remy Derollez <remy@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __OpenSpaceToolkit_Simulation_Component_Geometry__
#define __OpenSpaceToolkit_Simulation_Component_Geometry__

#include <OpenSpaceToolkit/Physics/Environment/Object/Geometry.hpp>

#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Objects/Composite.hpp>
#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Object.hpp>

#include <OpenSpaceToolkit/Core/Types/String.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace ostk
{
namespace simulation
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Component ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace component
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using ostk::core::types::String ;
using ostk::core::types::Shared ;
using ostk::math::geom::d3::Object ;
using ostk::math::geom::d3::objects::Composite ;

using ostk::physics::coord::Frame ;
using ObjectGeometry = ostk::physics::env::object::Geometry ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct GeometryConfiguration ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief                      Component geometry

class Geometry : public ObjectGeometry
{

    public:

                                Geometry                                    (   const   String&                     aName,
                                                                                const   Object&                     anObject,
                                                                                const   Shared<const Frame>&        aFrameSPtr,
                                                                                const   Shared<const Component>&    aComponentSPtr                              ) ;

                                Geometry                                    (   const   String&                     aName,
                                                                                const   Composite&                  aComposite,
                                                                                const   Shared<const Frame>&        aFrameSPtr,
                                                                                const   Shared<const Component>&    aComponentSPtr                              ) ;

        /// @brief              Equal to operator
        ///
        /// @code
        ///                     Geometry firstGeometry = ... ;
        ///                     Geometry secondGeometry = ... ;
        ///                     firstGeometry == secondGeometry ; // True
        /// @endcode
        ///
        /// @param              [in] aGeometry A geometry
        /// @return             True if geometries are equal

        bool                    operator ==                                 (   const   Geometry&                   aGeometry                                   ) const ;

        friend std::ostream&    operator <<                                 (           std::ostream&               anOutputStream,
                                                                                const   Geometry&                   aGeometry                                   ) ;

        bool                    isDefined                                   ( ) const ;

        const Component&        accessComponent                             ( ) const ;

        String                  getName                                     ( ) const ;

        bool                    intersects                                  (   const   ObjectGeometry&             aGeometry                                   ) const ;

        bool                    contains                                    (   const   ObjectGeometry&             aGeometry                                   ) const ;

        ObjectGeometry          intersectionWith                            (   const   ObjectGeometry&             aGeometry                                   ) const ;

        static Geometry         Undefined                                   ( ) ;

        static Shared<Geometry> Configure                                   (   const   GeometryConfiguration&      aGeometryConfiguration,
                                                                                const   Shared<const Component>&    aComponentSPtr                              ) ;

    private:

        String                  name_ ;
        Shared<const Component> componentPtr_ ;

} ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct GeometryConfiguration
{

    const String                name ;
    const Object&               object ;

} ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
