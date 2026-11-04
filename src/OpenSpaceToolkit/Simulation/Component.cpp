/// Apache License 2.0

#include <OpenSpaceToolkit/Simulation/Component.hpp>
#include <OpenSpaceToolkit/Simulation/Utility/Identifier.hpp>

#include <OpenSpaceToolkit/Core/Error.hpp>
#include <OpenSpaceToolkit/Core/Utility.hpp>

#include <OpenSpaceToolkit/Physics/Coordinate/Frame/Provider.hpp>
#include <OpenSpaceToolkit/Physics/Coordinate/Frame/Provider/Dynamic.hpp>

namespace ostk
{
namespace simulation
{

using ostk::physics::coordinate::frame::Provider;
using ostk::physics::coordinate::Transform;
using DynamicProvider = ostk::physics::coordinate::frame::provider::Dynamic;

using namespace ostk::simulation::utility;

Component::Component(
    const String& anId,
    const String& aName,
    const std::optional<Component::Type>& aType,
    const Array<String>& aTagArray,
    const Array<Shared<Geometry>>& aGeometryArray,
    const Array<Shared<Component>>& aComponentArray,
    const Shared<ComponentHolder>& aParentComponentSPtr,
    const Shared<const Frame>& aFrameSPtr,
    const Shared<const Simulator>& aSimulatorSPtr
)
    : Entity(anId, aName),
      ComponentHolder(aComponentArray),
      type_(aType),
      tags_(aTagArray),
      geometries_(aGeometryArray),
      parentWPtr_(aParentComponentSPtr),
      frameSPtr_(aFrameSPtr),
      simulatorSPtr_(aSimulatorSPtr)
{
}

Component::Component(const Component& aComponent)
    : Entity(aComponent),
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

Component::~Component() {}

Component* Component::clone() const
{
    return new Component(*this);
}

std::ostream& operator<<(std::ostream& anOutputStream, const Component& aComponent)
{
    aComponent.print(anOutputStream, true);

    return anOutputStream;
}

const Shared<const Frame>& Component::getFrame() const
{
    if (!this->frameSPtr_)
    {
        throw ostk::core::error::runtime::Undefined("Frame");
    }

    return this->frameSPtr_;
}

const Geometry& Component::getGeometryWithName(const String& aName) const
{
    for (const auto& geometryStr : this->geometries_)
    {
        if (geometryStr->getName() == aName)
        {
            return *geometryStr;
        }
    }

    throw ostk::core::error::RuntimeError("No Geometry found with name [{}].", aName);
}

const Simulator& Component::getSimulator() const
{
    if (!simulatorSPtr_)
    {
        throw ostk::core::error::runtime::Undefined("Simulator");
    }

    return *(this->simulatorSPtr_);
}

const std::optional<Component::Type>& Component::getType() const
{
    return this->type_;
}

const Array<String>& Component::getTags() const
{
    return this->tags_;
}

const Array<Shared<Geometry>>& Component::getGeometries() const
{
    return this->geometries_;
}

void Component::setParent(const Shared<Component>& aComponentSPtr)
{
    this->parentWPtr_ = aComponentSPtr;
}

void Component::addGeometry(const Shared<Geometry>& aGeometrySPtr)
{
    if ((!aGeometrySPtr) || (!aGeometrySPtr->isDefined()))
    {
        throw ostk::core::error::runtime::Undefined("Geometry");
    }

    this->geometries_.add(aGeometrySPtr);
}

void Component::addComponent(const Shared<Component>& aComponentSPtr)
{
    aComponentSPtr->setParent(this->shared_from_this());

    ComponentHolder::addComponent(aComponentSPtr);
}

Shared<Component> Component::Configure(
    const ComponentConfiguration& aComponentConfiguration, const Shared<Component>& aParentComponentSPtr
)
{
    if (!aParentComponentSPtr)
    {
        throw ostk::core::error::runtime::Undefined("Parent component");
    }

    const Shared<Component> componentSPtr = std::make_shared<Component>(
        aComponentConfiguration.id,
        aComponentConfiguration.name,
        aComponentConfiguration.type,
        aComponentConfiguration.tags,
        Array<Shared<Geometry>>::Empty(),
        Array<Shared<Component>>::Empty(),
        aParentComponentSPtr,
        Component::GenerateFrame(
            String::Format("Component [{}]", aComponentConfiguration.id),
            aComponentConfiguration.orientation,
            aParentComponentSPtr->getFrame()
        ),
        aParentComponentSPtr->simulatorSPtr_
    );

    for (const auto& geometryConfiguration : aComponentConfiguration.geometries)
    {
        componentSPtr->addGeometry(Geometry::Configure(geometryConfiguration, componentSPtr));
    }

    for (const auto& componentConfiguration : aComponentConfiguration.components)
    {
        componentSPtr->addComponent(Component::Configure(componentConfiguration, componentSPtr));
    }

    return componentSPtr;
}

String Component::StringFromType(const Component::Type& aType)
{
    static const Map<Component::Type, String> typeStringMap = {
        {Component::Type::Assembly, "Assembly"},
        {Component::Type::Controller, "Controller"},
        {Component::Type::Sensor, "Sensor"},
        {Component::Type::Actuator, "Actuator"},
        {Component::Type::Other, "Other"}
    };

    return typeStringMap.at(aType);
}

Shared<const Frame> Component::GenerateFrame(
    const String& aName, const Quaternion& aQuaternion, const Shared<const Frame>& aParentFrameSPtr
)
{
    using ostk::physics::time::Instant;

    if (Frame::Exists(aName))
    {
        Frame::Destruct(aName);
    }

    if (aParentFrameSPtr == nullptr)
    {
        throw ostk::core::error::runtime::Undefined("Frame");
    }

    const Weak<const Frame> frameWPtr = aParentFrameSPtr;

    const Shared<const DynamicProvider> transformProviderSPtr = std::make_shared<const DynamicProvider>(
        [frameWPtr, aQuaternion](const Instant& anInstant) -> Transform
        {
            if (Shared<const Frame> frameSPtr = frameWPtr.lock())
            {
                return Transform::Passive(anInstant, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, aQuaternion, {0.0, 0.0, 0.0});
            }
            else
            {
                throw ostk::core::error::RuntimeError("Cannot get pointer to Profile.");
            }
        }
    );

    return Frame::Construct(aName, false, aParentFrameSPtr, transformProviderSPtr);
}

void Component::print(std::ostream& anOutputStream, bool displayDecorators) const
{
    displayDecorators ? ostk::core::utils::Print::Header(anOutputStream, "Component") : void();

    Entity::print(anOutputStream, false);

    ostk::core::utils::Print::Line(anOutputStream)
        << "Type:" << (type_.has_value() ? Component::StringFromType(type_.value()) : "Undefined");

    displayDecorators ? ostk::core::utils::Print::Footer(anOutputStream) : void();
}

}  // namespace simulation
}  // namespace ostk
