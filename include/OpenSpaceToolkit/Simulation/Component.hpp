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
#include <OpenSpaceToolkit/Simulation/Entity.hpp>

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
using ostk::core::types::Unique ;
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

#define                         DEFAULT_TYPE                                    Component::Type::Undefined
#define                         DEFAULT_TAG_ARRAY                               Array<String>::Empty()
#define                         DEFAULT_STATE                                   State::Undefined()
#define                         DEFAULT_GEOMETRY_ARRAY                          Array<Geometry>::Empty()
#define                         DEFAULT_ROTATION_MATRIX                         RotationMatrix::Unit()
#define                         DEFAULT_COMPONENT_ARRAY                         Array<Component>::Empty()
#define                         DEFAULT_PARENT_COMPONENT                        nullptr

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief                      Component

class Component : public Entity, public ComponentHolder
{

    public:

        enum class Type
        {

            Undefined,
            Assembly,
            Controller,
            Sensor,
            Actuator,
            Other

        } ;

                                Component                                   (   const   String&                     anId,
                                                                                const   String&                     aName,
                                                                                const   Component::Type&            aType                                       =   DEFAULT_TYPE,
                                                                                const   State&                      aState                                      =   DEFAULT_STATE,
                                                                                const   Array<String>&              aTagArray                                   =   DEFAULT_TAG_ARRAY,
                                                                                const   Array<Geometry>&            aGeometryArray                              =   DEFAULT_GEOMETRY_ARRAY,
                                                                                const   RotationMatrix&             aRotationMatrix                             =   DEFAULT_ROTATION_MATRIX,
                                                                                const   Array<Component>&           aComponentArray                             =   DEFAULT_COMPONENT_ARRAY,
                                                                                const   Shared<Component>&          aParentComponent                            =   DEFAULT_PARENT_COMPONENT ) ;

                                Component                                   (   const   String&                     aName,
                                                                                const   Component::Type&            aType                                       =   DEFAULT_TYPE,
                                                                                const   State&                      aState                                      =   DEFAULT_STATE,
                                                                                const   Array<String>&              aTagArray                                   =   DEFAULT_TAG_ARRAY,
                                                                                const   Array<Geometry>&            aGeometryArray                              =   DEFAULT_GEOMETRY_ARRAY,
                                                                                const   RotationMatrix&             aRotationMatrix                             =   DEFAULT_ROTATION_MATRIX,
                                                                                const   Array<Component>&           aComponentArray                             =   DEFAULT_COMPONENT_ARRAY,
                                                                                const   Shared<Component>&          aParentComponent                            =   DEFAULT_PARENT_COMPONENT ) ;

                                Component                                   (   const   Component&                  aComponent                                  ) ;

        virtual                 ~Component                                  ( ) ;

        virtual Component*      clone                                       ( ) const ;

        Component&              operator =                                  (   const   Component&                  aComponent                                  ) ;

        friend std::ostream&    operator <<                                 (           std::ostream&               anOutputStream,
                                                                                const   Component&                  aComponent                                  ) ;

        bool                    isDefined                                   ( ) const ;

        Component::Type         getType                                     ( ) const ;

        State                   getState                                    ( ) const ;

        Array<String>           getTags                                     ( ) const ;

        Array<Geometry>         getGeometries                               ( ) const ;

        RotationMatrix          getRotationMatrix                           ( ) const ;

        const State&            accessState                                 ( ) const ;

        const Geometry&         accessGeometryWithName                      ( ) const ;

        void                    setParent                                   (   const   Shared<Component>&          aParentComponent                            ) ;

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

        Component::Type         type_ ;
        State                   state_ ;
        Array<String>           tags_ ;
        Array<Geometry>         geometries_ ; // Array of Geometries defined in Component Frame
        RotationMatrix          rotationMatrix_ ; // Rotation from Body Frame to Component Frame

        Weak<const Component>   parentWPtr_ ;

} ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
