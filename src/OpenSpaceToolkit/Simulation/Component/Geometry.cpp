////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Simulation
/// @file           OpenSpaceToolkit/Simulation/Component/Geometry.cpp
/// @author         Lucas Brémond <lucas@loftorbital.com>, Remy Derollez <remy@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <OpenSpaceToolkit/Simulation/Component/Geometry.hpp>

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
                                                                                const   Geometry::Type&             aType,
                                                                                const   Object&                     aGeometry                                   )
                                :   name_(aName),
                                    type_(aType),
                                    composite_(aGeometry)
{

}

                                Geometry::Geometry                          (   const   String&                     aName,
                                                                                const   Geometry::Type&             aType,
                                                                                const   Composite&                  aComposite                                  )
                                :   name_(aName),
                                    type_(aType),
                                    composite_(aComposite)
{

}

std::ostream&                   operator <<                                 (           std::ostream&               anOutputStream,
                                                                                const   Geometry&                   aGeometry                                   )
{

    // TBI

    (void) aGeometry ;

    return anOutputStream ;

}

bool                            Geometry::operator ==                       (   const   Geometry&                   aGeometry                                   ) const
{

    if ((!this->isDefined()) || (!aGeometry.isDefined()))
    {
        return false ;
    }

    return (this->name_ == aGeometry.name_) && (this->type_ == aGeometry.type_) && (this->composite_ == aGeometry.composite_) ;


}

bool                            Geometry::isDefined                         ( ) const
{
    return !name_.isEmpty() ;
}

String                          Geometry::getName                           ( ) const
{

    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Geometry") ;
    }

    return name_ ;

}

Geometry::Type                  Geometry::getType                           ( ) const
{

    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Geometry") ;
    }

    return type_ ;

}

Composite                       Geometry::getComposite                      ( ) const
{

    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Geometry") ;
    }

    return composite_ ;

}

String                          Geometry::getExclusionObject                ( ) const
{

    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Geometry") ;
    }

    if (this->getType() != Geometry::Type::Exclusion)
    {
        throw ostk::core::error::RuntimeError("Not a Geometry with Type Exclusion.") ;
    }

    if (this->getName().find("Earth") <= this->getName().getLength())
    {
        return "Earth" ;
    }
    else if (this->getName().find("Sun") <= this->getName().getLength())
    {
        return "Sun" ;
    }
    else if (this->getName().find("Moon") <= this->getName().getLength())
    {
        return "Moon" ;
    }

    throw ostk::core::error::runtime::Undefined("Celestial Object for Exclusion") ;

}

String                          Geometry::getExclusionCondition             ( ) const
{

    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Geometry") ;
    }

    if (this->getType() != Geometry::Type::Exclusion)
    {
        throw ostk::core::error::RuntimeError("Not a Geometry with Type Exclusion") ;
    }

    if (this->getName().find("Illuminated") <= this->getName().getLength())
    {
        return "Illuminated" ;
    }
    else if (this->getName().find("Dark") <= this->getName().getLength())
    {
        return "Dark" ;
    }

    throw ostk::core::error::runtime::Undefined("Celestial Object for Exclusion") ;

}


Geometry                        Geometry::Undefined                         ( )
{
    return { String::Empty(), Geometry::Type::Undefined, Composite::Empty() } ;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
