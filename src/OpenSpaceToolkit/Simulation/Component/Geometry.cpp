////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Simulation
/// @file           OpenSpaceToolkit/Simulation/Component/Geometry.cpp
/// @author         Lucas Brémond <lucas@loftorbital.com>, Remy Derollez <remy@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <OpenSpaceToolkit/Simulation/Component/Geometry.hpp>
#include <OpenSpaceToolkit/Simulation/Component.hpp>
#include <OpenSpaceToolkit/Simulation/Simulator.hpp>

#include <OpenSpaceToolkit/Core/Error.hpp>
#include <OpenSpaceToolkit/Core/Utilities.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace ostk
{
namespace simulation
{
namespace component
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                                Geometry::Geometry                          (   const   String&                     aName,
                                                                                const   Object&                     anObject,
                                                                                const   Shared<const Frame>&        aFrameSPtr,
                                                                                const   Shared<const Component>&    aComponentSPtr                              )
                                :   ObjectGeometry(anObject, aFrameSPtr),
                                    name_(aName),
                                    componentPtr_(aComponentSPtr)
{

}

                                Geometry::Geometry                          (   const   String&                     aName,
                                                                                const   Composite&                  aComposite,
                                                                                const   Shared<const Frame>&        aFrameSPtr,
                                                                                const   Shared<const Component>&    aComponentSPtr                              )
                                :   ObjectGeometry(aComposite, aFrameSPtr),
                                    name_(aName),
                                    componentPtr_(aComponentSPtr)
{

}

bool                            Geometry::operator ==                       (   const   Geometry&                   aGeometry                                   ) const
{

    if ((!this->isDefined()) || (!aGeometry.isDefined()))
    {
        return false ;
    }

    return ObjectGeometry::operator == (aGeometry) && (this->name_ == aGeometry.name_) ;


}

std::ostream&                   operator <<                                 (           std::ostream&               anOutputStream,
                                                                                const   Geometry&                   aGeometry                                   )
{

    // TBI

    (void) aGeometry ;

    return anOutputStream ;

}

bool                            Geometry::isDefined                         ( ) const
{

    return ObjectGeometry::isDefined() && !name_.isEmpty() && (componentPtr_ != nullptr) ;

}

const Component&                Geometry::accessComponent                   ( ) const
{

    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Geometry") ;
    }

    return *(this->componentPtr_) ;

}

String                          Geometry::getName                           ( ) const
{

    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Geometry") ;
    }

    return name_ ;

}

bool                            Geometry::intersects                        (   const   ObjectGeometry&             aGeometry                                   ) const
{

    using ostk::physics::time::Instant ;

    if ((!this->isDefined()) || (!aGeometry.isDefined()))
    {
        throw ostk::core::error::runtime::Undefined("Geometry") ;
    }

    const Instant instant = this->accessComponent().accessSimulator().getInstant() ;

    // TBM: Why GCRF?
    return this->in(Frame::GCRF(), instant).intersects(aGeometry.in(Frame::GCRF(), instant)) ;

}

bool                            Geometry::contains                          (   const   ObjectGeometry&             aGeometry                                   ) const
{

    using ostk::physics::time::Instant ;

    if ((!this->isDefined()) || (!aGeometry.isDefined()))
    {
        throw ostk::core::error::runtime::Undefined("Geometry") ;
    }

    const Instant instant = this->accessComponent().accessSimulator().getInstant() ;

    // TBM: Why GCRF?
    return this->in(Frame::GCRF(), instant).contains(aGeometry.in(Frame::GCRF(), instant)) ;

}

ObjectGeometry                  Geometry::intersectionWith                  (   const   ObjectGeometry&             aGeometry                                   ) const
{

    using ostk::physics::time::Instant ;

    if ((!this->isDefined()) || (!aGeometry.isDefined()))
    {
        throw ostk::core::error::runtime::Undefined("Geometry") ;
    }

    const Instant instant = this->accessComponent().accessSimulator().getInstant() ;

    // TBM: Why GCRF?
    return this->in(Frame::GCRF(), instant).intersectionWith(aGeometry.in(Frame::GCRF(), instant)) ;

}

Geometry                        Geometry::Undefined                         ( )
{

    return
    {
        String::Empty(),
        Composite::Undefined(),
        Frame::Undefined(),
        nullptr
    } ;

}

Shared<Geometry>                Geometry::Configure                         (   const   GeometryConfiguration&      aGeometryConfiguration,
                                                                                const   Shared<const Component>&    aComponentSPtr                              )
{

    if (aComponentSPtr == nullptr)
    {
        throw ostk::core::error::runtime::Undefined("Component") ;
    }

    return std::make_shared<Geometry>
    (
        aGeometryConfiguration.name,
        aGeometryConfiguration.object,
        aComponentSPtr->accessFrame(),
        aComponentSPtr
    ) ;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
