////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Simulation
/// @file           OpenSpaceToolkit/Simulation/Component.cpp
/// @author         Lucas Brémond <lucas@loftorbital.com>, Remy Derollez <remy@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <OpenSpaceToolkit/Simulation/Utilities/Identifier.hpp>
#include <OpenSpaceToolkit/Simulation/Component.hpp>

#include <OpenSpaceToolkit/Physics/Coordinate/Frame/Providers/Dynamic.hpp>
#include <OpenSpaceToolkit/Physics/Coordinate/Frame/Provider.hpp>

#include <OpenSpaceToolkit/Core/Error.hpp>
#include <OpenSpaceToolkit/Core/Utilities.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace ostk
{
namespace simulation
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using ostk::physics::coord::Transform ;
using ostk::physics::coord::frame::Provider ;
using DynamicProvider = ostk::physics::coord::frame::provider::Dynamic ;

using namespace ostk::simulation::utilities ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                                Component::Component                        (   const   String&                     anId,
                                                                                const   String&                     aName,
                                                                                const   Component::Type&            aType,
                                                                                const   Array<String>&              aTagArray,
                                                                                const   Array<Shared<Geometry>>&    aGeometryArray,
                                                                                const   Array<Shared<Component>>&   aComponentArray,
                                                                                const   Shared<ComponentHolder>&    aParentComponentSPtr,
                                                                                const   Shared<const Frame>&        aFrameSPtr,
                                                                                const   Shared<const Simulator>&    aSimulatorSPtr                              )
                                :   Entity(anId, aName),
                                    ComponentHolder(aComponentArray),
                                    type_(aType),
                                    tags_(aTagArray),
                                    geometries_(aGeometryArray),
                                    parentWPtr_(aParentComponentSPtr),
                                    frameSPtr_(aFrameSPtr),
                                    simulatorSPtr_(aSimulatorSPtr)
{

}

                                Component::Component                        (   const   Component&                  aComponent                                  )
                                :   Entity(aComponent),
                                    ComponentHolder(aComponent),
                                    enable_shared_from_this(aComponent),
                                    type_(aComponent.type_),
                                    tags_(aComponent.tags_),
                                    geometries_(aComponent.geometries_),
                                    parentWPtr_(aComponent.parentWPtr_),
                                    frameSPtr_(aComponent.frameSPtr_),
                                    simulatorSPtr_(aComponent.simulatorSPtr_)
{

}

                                Component::~Component                       ( )
{

}

Component*                      Component::clone                            ( ) const
{
    return new Component(*this) ;
}

std::ostream&                   operator <<                                 (           std::ostream&               anOutputStream,
                                                                                const   Component&                  aComponent                                  )
{

    aComponent.print(anOutputStream, true) ;

    return anOutputStream ;

}

bool                            Component::isDefined                        ( ) const
{
    return Entity::isDefined() && frameSPtr_ && simulatorSPtr_ ;
}

const Shared<const Frame>&      Component::accessFrame                      ( ) const
{

    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Component") ;
    }

    return this->frameSPtr_ ;

}

const Geometry&                 Component::accessGeometryWithName           (   const   String&                     aName                                       ) const
{

    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Component") ;
    }

    for (const auto& geometryStr : this->geometries_)
    {
        if (geometryStr->getName() == aName)
        {
            return *geometryStr ;
        }
    }

    throw ostk::core::error::RuntimeError("No Geometry found with name [{}].", aName) ;

}

const Simulator&                Component::accessSimulator                  ( ) const
{

    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Component") ;
    }

    return *(this->simulatorSPtr_) ;

}

Component::Type                 Component::getType                          ( ) const
{

    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Component") ;
    }

    return this->type_ ;

}

Array<String>                   Component::getTags                          ( ) const
{

    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Component") ;
    }

    return this->tags_ ;

}

Array<Shared<Geometry>>         Component::getGeometries                    ( ) const
{

    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Component") ;
    }

    return this->geometries_ ;

}

void                            Component::setParent                        (   const   Shared<Component>&          aComponentSPtr                              )
{

    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Component") ;
    }

    this->parentWPtr_ = aComponentSPtr ;

}

void                            Component::addGeometry                      (   const   Shared<Geometry>&           aGeometrySPtr                               )
{

    if ((!aGeometrySPtr) || (!aGeometrySPtr->isDefined()))
    {
        throw ostk::core::error::runtime::Undefined("Geometry") ;
    }

    this->geometries_.add(aGeometrySPtr) ;

}

void                            Component::addComponent                     (   const   Shared<Component>&          aComponentSPtr                              )
{

    aComponentSPtr->setParent(this->shared_from_this()) ;

    ComponentHolder::addComponent(aComponentSPtr) ;

}

Component                       Component::Undefined                        ( )
{

    return
    {
        String::Empty(),
        String::Empty(),
        Component::Type::Undefined,
        Array<String>::Empty(),
        Array<Shared<Geometry>>::Empty(),
        Array<Shared<Component>>::Empty(),
        nullptr,
        nullptr,
        nullptr
    } ;

}

Shared<Component>               Component::Configure                        (   const   ComponentConfiguration&     aComponentConfiguration,
                                                                                const   Shared<Component>&          aParentComponentSPtr                        )
{

    if ((!aParentComponentSPtr) || (!aParentComponentSPtr->isDefined()))
    {
        throw ostk::core::error::runtime::Undefined("Parent component") ;
    }

    const Shared<Component> componentSPtr = std::make_shared<Component>
    (
        aComponentConfiguration.id,
        aComponentConfiguration.name,
        aComponentConfiguration.type,
        aComponentConfiguration.tags,
        Array<Shared<Geometry>>::Empty(),
        Array<Shared<Component>>::Empty(),
        aParentComponentSPtr,
        Component::GenerateFrame
        (
            aComponentConfiguration.id,
            aComponentConfiguration.orientation,
            aParentComponentSPtr->accessFrame()
        ),
        aParentComponentSPtr->simulatorSPtr_
    ) ;

    for (const auto& geometryConfiguration : aComponentConfiguration.geometries)
    {
        componentSPtr->addGeometry(Geometry::Configure(geometryConfiguration, componentSPtr)) ;
    }

    for (const auto& componentConfiguration : aComponentConfiguration.components)
    {
        componentSPtr->addComponent(Component::Configure(componentConfiguration, componentSPtr)) ;
    }

    return componentSPtr ;

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

Shared<const Frame>             Component::GenerateFrame                    (   const   String&                     aName,
                                                                                const   Quaternion&                 aQuaternion,
                                                                                const   Shared<const Frame>&        aParentFrameSPtr                                )
{

    using ostk::physics::time::Instant ;

    if (Frame::Exists(aName))
    {
        Frame::Destruct(aName) ;
    }

    if (aParentFrameSPtr == nullptr)
    {
        throw ostk::core::error::runtime::Undefined("Frame") ;
    }

    const Weak<const Frame> frameWPtr = aParentFrameSPtr ;

    const Shared<const DynamicProvider> transformProviderSPtr = std::make_shared<const DynamicProvider>
    (
        [frameWPtr, aQuaternion] (const Instant& anInstant) -> Transform
        {

            if (Shared<const Frame> frameSPtr = frameWPtr.lock())
            {

                return Transform::Passive
                (
                    anInstant,
                    { 0.0, 0.0, 0.0 },
                    { 0.0, 0.0, 0.0 },
                    aQuaternion,
                    { 0.0, 0.0, 0.0 }
                ) ;

            }
            else
            {
                throw ostk::core::error::RuntimeError("Cannot get pointer to Profile.") ;
            }

        }
    ) ;

    return Frame::Construct(String::Format("Component [{}]", aName), false, aParentFrameSPtr, transformProviderSPtr) ;

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
