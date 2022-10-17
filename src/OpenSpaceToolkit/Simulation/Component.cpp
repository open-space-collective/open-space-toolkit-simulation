////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Simulation
/// @file           OpenSpaceToolkit/Simulation/Component.cpp
/// @author         Lucas Brémond <lucas@loftorbital.com>, Remy Derollez <remy@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <OpenSpaceToolkit/Simulation/Component.hpp>

#include <OpenSpaceToolkit/Core/Error.hpp>
#include <OpenSpaceToolkit/Core/Utilities.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace ostk
{
namespace simulation
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                                Component::Component                        (   const   String&                     aName,
                                                                                const   Component::Type&            aType                                       )
                                :   ComponentHolder(),
                                    name_(aName),
                                    type_(aType),
                                    tags_({""}),
                                    state_(State::Undefined()),
                                    geometries_({Geometry::Undefined()}),
                                    rotationMatrix_(RotationMatrix::Unit()),
                                    satelliteWPtr_()
{

}

                                Component::Component                        (   const   String&                     aName,
                                                                                const   Component::Type&            aType,
                                                                                const   State&                      aState                                      )
                                :   ComponentHolder(),
                                    name_(aName),
                                    type_(aType),
                                    tags_({""}),
                                    state_(aState),
                                    geometries_({Geometry::Undefined()}),
                                    rotationMatrix_(RotationMatrix::Unit()),
                                    satelliteWPtr_()
{

}

                                Component::Component                        (   const   String&                     aName,
                                                                                const   Component::Type&            aType,
                                                                                const   Array<String>&              anArrayOfTags                               )
                                :   ComponentHolder(),
                                    name_(aName),
                                    type_(aType),
                                    tags_(anArrayOfTags),
                                    state_(State::Undefined()),
                                    geometries_({Geometry::Undefined()}),
                                    rotationMatrix_(RotationMatrix::Unit()),
                                    satelliteWPtr_()
{

}

                                Component::Component                        (   const   String&                     aName,
                                                                                const   Component::Type&            aType,
                                                                                const   State&                      aState,
                                                                                const   Array<String>&              anArrayofTags                               )
                                :   ComponentHolder(),
                                    name_(aName),
                                    type_(aType),
                                    tags_(anArrayofTags),
                                    state_(aState),
                                    geometries_({Geometry::Undefined()}),
                                    rotationMatrix_(RotationMatrix::Unit()),
                                    satelliteWPtr_()
{

}

                                Component::Component                        (   const   String&                     aName,
                                                                                const   Component::Type&            aType,
                                                                                const   State&                      aState,
                                                                                const   Array<String>&              anArrayofTags,
                                                                                const   Array<Geometry>&            anArrayOfGeometries,
                                                                                const   RotationMatrix&             aRotationMatrix                             )
                                :   ComponentHolder(),
                                    name_(aName),
                                    type_(aType),
                                    tags_(anArrayofTags),
                                    state_(aState),
                                    geometries_(anArrayOfGeometries),
                                    rotationMatrix_(aRotationMatrix),
                                    satelliteWPtr_()
{

}

                                Component::Component                        (   const   Component&                  aComponent                                  )
                                :   ComponentHolder(aComponent),
                                    name_(aComponent.name_),
                                    type_(aComponent.type_),
                                    tags_(aComponent.tags_),
                                    state_(aComponent.state_),
                                    geometries_(aComponent.geometries_),
                                    rotationMatrix_(aComponent.rotationMatrix_),
                                    satelliteWPtr_(aComponent.satelliteWPtr_)
{

}

                                Component::~Component                       ( )
{

}

Component*                      Component::clone                            ( ) const
{
    return new Component(*this) ;
}

Component&                      Component::operator =                       (   const   Component&                  aComponent                                  )
{

    if (this != &aComponent)
    {

        this->ComponentHolder::operator = (aComponent) ;

        name_ = aComponent.name_ ;
        type_ = aComponent.type_ ;
        tags_ = aComponent.tags_ ;
        state_ = aComponent.state_ ;
        geometries_ = aComponent.geometries_ ;
        rotationMatrix_ = aComponent.rotationMatrix_ ;
        satelliteWPtr_ = aComponent.satelliteWPtr_ ;

    }

    return *this ;

}

std::ostream&                   operator <<                                 (           std::ostream&               anOutputStream,
                                                                                const   Component&                  aComponent                                  )
{

    aComponent.print(anOutputStream, true) ;

    return anOutputStream ;

}

bool                            Component::isDefined                        ( ) const
{
    return !name_.isEmpty() ;
}

String                          Component::getName                          ( ) const
{

    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Component") ;
    }

    return name_ ;

}

Component::Type                 Component::getType                          ( ) const
{

    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Component") ;
    }

    return type_ ;

}

Array<String>                   Component::getTags                          ( ) const
{

    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Component") ;
    }

    return tags_ ;

}

State                           Component::getState                         ( ) const
{

    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Component") ;
    }

    return state_ ;

}

Array<Geometry>                Component::getGeometries                    ( ) const
{

    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Component") ;
    }

    return geometries_ ;

}

RotationMatrix                  Component::getRotationMatrix                ( ) const
{

    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Component") ;
    }

    return rotationMatrix_ ;

}

Component                       Component::Undefined                        ( )
{
    return { String::Empty(), Component::Type::Undefined } ;
}

String                          Component::StringFromType                   (   const   Component::Type&            aType                                       )
{

    static const Map<Component::Type, String> typeStringMap =
    {
        { Component::Type::Undefined,  "Undefined" },
        { Component::Type::Controller, "Controller" },
        { Component::Type::Sensor,     "Sensor" },
        { Component::Type::Actuator,   "Actuator" },
        { Component::Type::Other,      "Other" }
    } ;

    return typeStringMap.at(aType) ;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void                            Component::print                            (           std::ostream&               anOutputStream,
                                                                                        bool                        displayDecorators                           ) const
{

    displayDecorators ? ostk::core::utils::Print::Header(anOutputStream, "Component") : void () ;

    ostk::core::utils::Print::Line(anOutputStream) << "Name:"       << name_ ;
    // ostk::core::utils::Print::Line(anOutputStream) << "ID:"         << id_ ;
    ostk::core::utils::Print::Line(anOutputStream) << "Type:"       << Component::StringFromType(type_) ;
    // ostk::core::utils::Print::Line(anOutputStream) << "State:"      << ;
    // ostk::core::utils::Print::Line(anOutputStream) << "Geometries:" << ;

    displayDecorators ? ostk::core::utils::Print::Footer(anOutputStream) : void () ;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
