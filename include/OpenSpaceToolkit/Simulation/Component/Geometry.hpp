////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Simulation
/// @file           OpenSpaceToolkit/Simulation/Component/Geometry.hpp
/// @author         Lucas Brémond <lucas@loftorbital.com>, Remy Derollez <remy@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __OpenSpaceToolkit_Simulation_Component_Geometry__
#define __OpenSpaceToolkit_Simulation_Component_Geometry__

#include <OpenSpaceToolkit/Physics/Environment/Objects/Celestial.hpp>
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
using ostk::physics::env::obj::Celestial ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct GeometryConfiguration ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief                      Component geometry

class Geometry
{

    public:

                                Geometry                                    (   const   String&                     aName,
                                                                                const   Composite&                  aComposite,
                                                                                const   Shared<const Component>&    aComponentSPtr                              ) ;

        /// @brief              Copy assignment operator
        ///
        /// @param              [in] aGeometry A geometry
        /// @return             Reference to geometry

        /// @brief              Equal to operator
        ///
        /// @param              [in] aGeometry A geometry
        /// @return             True if geometries are equal

        bool                    operator ==                                 (   const   Geometry&                   aGeometry                                   ) const ;

        /// @brief              Not equal to operator
        ///
        /// @param              [in] aGeometry A geometry
        /// @return             True if geometries are not equal

        bool                    operator !=                                 (   const   Geometry&                   aGeometry                                   ) const ;

        /// @brief              Output stream operator
        ///
        /// @param              [in] anOutputStream An output stream
        /// @param              [in] aGeometry A geometry
        /// @return             A reference to output stream

        friend std::ostream&    operator <<                                 (           std::ostream&               anOutputStream,
                                                                                const   Geometry&                   aGeometry                                   ) ;

        /// @brief              Check if geometry is defined
        ///
        /// @return             True if geometry is defined

        bool                    isDefined                                   ( ) const ;

        ///                     TBI

        const Component&        accessComponent                             ( ) const ;

        ///                     TBI

        String                  getName                                     ( ) const ;

        /// @brief              Check if geometry intersects another geometry
        ///
        /// @param              [in] aGeometry A geometry
        /// @return             True if geometry intersects another geometry

        bool                    intersects                                  (   const   ObjectGeometry&             aGeometry                                   ) const ;

        /// @brief              Check if geometry intersects a celestial object
        ///
        /// @param              [in] aCelestialObject A celestial object
        /// @return             True if geometry intersects a celestial object

        bool                    intersects                                  (   const   Celestial&                  aCelestialObject                            ) const ;

        /// @brief              Check if geometry contains another geometry
        ///
        /// @param              [in] aGeometry A geometry
        /// @return             True if geometry contains another geometry

        bool                    contains                                    (   const   ObjectGeometry&             aGeometry                                   ) const ;

        /// @brief              Check if geometry contains a celestial object
        ///
        /// @param              [in] aCelestialObject A celestial object
        /// @return             True if geometry contains a celestial object

        bool                    contains                                    (   const   Celestial&                  aCelestialObject                            ) const ;

        /// @brief              Access composite
        ///
        /// @return             Reference to composite

        const Composite&        accessComposite                             ( ) const ;

        /// @brief              Access frame
        ///
        /// @return             Shared pointer to frame

        Shared<const Frame>     accessFrame                                 ( ) const ;

        /// @brief              Compute intersection of geometry with another geometry
        ///
        /// @param              [in] aGeometry A geometry
        /// @return             Intersection of geometry with another geometry

        ObjectGeometry          intersectionWith                            (   const   ObjectGeometry&             aGeometry                                   ) const ;

        /// @brief              Compute intersection of geometry with a celestial object
        ///
        /// @param              [in] aCelestialObject A celestial object
        /// @return             Intersection of geometry with a celestial object

        ObjectGeometry          intersectionWith                            (   const   Celestial&                  aCelestialObject                            ) const ;

        static Geometry         Undefined                                   ( ) ;

        static Shared<Geometry> Configure                                   (   const   GeometryConfiguration&      aGeometryConfiguration,
                                                                                const   Shared<const Component>&    aComponentSPtr                              ) ;

    private:

        String                  name_ ;
        ObjectGeometry          geometry_ ;
        Shared<const Component> componentPtr_ ;

} ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct GeometryConfiguration
{

    const String                name ;
    const Composite             composite ;

} ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
