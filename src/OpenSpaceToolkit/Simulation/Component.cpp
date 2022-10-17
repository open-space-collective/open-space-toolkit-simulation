////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Simulation
/// @file           OpenSpaceToolkit/Simulation/Component.cpp
/// @author         Lucas Brémond <lucas@loftorbital.com>, Remy Derollez <remy@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <OpenSpaceToolkit/Simulation/Utilities/Identifier.hpp>
#include <OpenSpaceToolkit/Simulation/Component.hpp>

#include <OpenSpaceToolkit/Core/Error.hpp>
#include <OpenSpaceToolkit/Core/Utilities.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace ostk
{
namespace simulation
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using namespace ostk::simulation::utilities ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                                Component::Component                        (   const   String&                     anId,
                                                                                const   String&                     aName,
                                                                                const   Component::Type&            aType,
                                                                                const   State&                      aState,
                                                                                const   Array<String>&              aTagArray,
                                                                                const   Array<Geometry>&            aGeometryArray,
                                                                                const   RotationMatrix&             aRotationMatrix,
                                                                                const   Array<Component>&           aComponentArray,
                                                                                const   Shared<Component>&          aParentComponent                            )
                                :   Entity(anId, aName),
                                    ComponentHolder(aComponentArray),
                                    type_(aType),
                                    state_(aState),
                                    tags_(aTagArray),
                                    geometries_(aGeometryArray),
                                    rotationMatrix_(aRotationMatrix),
                                    parentWPtr_(aParentComponent)
{

}

                                Component::Component                        (   const   String&                     aName,
                                                                                const   Component::Type&            aType,
                                                                                const   State&                      aState,
                                                                                const   Array<String>&              aTagArray,
                                                                                const   Array<Geometry>&            aGeometryArray,
                                                                                const   RotationMatrix&             aRotationMatrix,
                                                                                const   Array<Component>&           aComponentArray,
                                                                                const   Shared<Component>&          aParentComponent                            )
                                :   Entity(aName),
                                    ComponentHolder(aComponentArray),
                                    type_(aType),
                                    state_(aState),
                                    tags_(aTagArray),
                                    geometries_(aGeometryArray),
                                    rotationMatrix_(aRotationMatrix),
                                    parentWPtr_(aParentComponent)
{

}

                                Component::Component                        (   const   Component&                  aComponent                                  )
                                :   Entity(aComponent),
                                    ComponentHolder(aComponent),
                                    type_(aComponent.type_),
                                    state_(aComponent.state_),
                                    tags_(aComponent.tags_),
                                    geometries_(aComponent.geometries_),
                                    rotationMatrix_(aComponent.rotationMatrix_),
                                    parentWPtr_(aComponent.parentWPtr_)
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

        this->Entity::operator = (aComponent) ;
        this->ComponentHolder::operator = (aComponent) ;

        type_ = aComponent.type_ ;
        state_ = aComponent.state_ ;
        tags_ = aComponent.tags_ ;
        geometries_ = aComponent.geometries_ ;
        rotationMatrix_ = aComponent.rotationMatrix_ ;
        parentWPtr_ = aComponent.parentWPtr_ ;

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
    return Entity::isDefined() ;
}

Component::Type                 Component::getType                          ( ) const
{

    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Component") ;
    }

    return type_ ;

}

State                           Component::getState                         ( ) const
{

    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Component") ;
    }

    return state_ ;

}

Array<String>                   Component::getTags                          ( ) const
{

    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Component") ;
    }

    return tags_ ;

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

void                            Component::setParent                        (   const   Shared<Component>&          aParentComponent                            )
{

    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Component") ;
    }

    parentWPtr_ = aParentComponent ;

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
        { Component::Type::Assembly,   "Assembly" },
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

    Entity::print(anOutputStream, false) ;

    ostk::core::utils::Print::Line(anOutputStream) << "Type:" << Component::StringFromType(type_) ;

    displayDecorators ? ostk::core::utils::Print::Footer(anOutputStream) : void () ;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
