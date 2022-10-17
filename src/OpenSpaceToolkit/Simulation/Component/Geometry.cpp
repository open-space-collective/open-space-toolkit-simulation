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

bool                            Geometry::isDefined                         ( ) const
{
    return !name_.isEmpty() ;
}

String                          Geometry::getName                           ( ) const
{
    return name_ ;
}

Geometry::Type                  Geometry::getType                           ( ) const
{
    return type_ ;
}

Composite                       Geometry::getComposite                      ( ) const
{
    return composite_ ;
}

String                          Geometry::getExclusionObject                ( ) const
{

    if (this->getType() != Geometry::Type::Exclusion)
    {
        throw ostk::core::error::RuntimeError("Not a Geometry with Type Exclusion") ;
    }

    String geometryName = this->getName() ;
    String celestialObjectName = "" ;

    // std::cout << geometryName << std::endl ;
    // std::cout << geometryName.find("Sun") << std::endl ;
    // std::cout << geometryName.getLength() ;

    if (geometryName.find("Earth") <= geometryName.getLength())
    {

        celestialObjectName = "Earth" ;

    }

    else if (geometryName.find("Sun") <= geometryName.getLength())
    {

        celestialObjectName = "Sun" ;

    }

    else if (geometryName.find("Moon") <= geometryName.getLength())
    {

        celestialObjectName = "Moon" ;

    }

    else
    {

        throw ostk::core::error::runtime::Undefined("Celestial Object for Exclusion") ;

    }

    return celestialObjectName ;

}

String                          Geometry::getExclusionCondition             ( ) const
{

    if (this->getType() != Geometry::Type::Exclusion)
    {
        throw ostk::core::error::RuntimeError("Not a Geometry with Type Exclusion") ;
    }

    String geometryName = this->getName() ;
    String illuminationCondition = "" ;

    if (geometryName.find("Illuminated") <= geometryName.getLength())
    {

        illuminationCondition = "Illuminated" ;

    }

    else if (geometryName.find("Dark") <= geometryName.getLength())
    {

        illuminationCondition = "Dark" ;

    }

    else
    {

        throw ostk::core::error::runtime::Undefined("Celestial Object for Exclusion") ;

    }

    return illuminationCondition ;

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
