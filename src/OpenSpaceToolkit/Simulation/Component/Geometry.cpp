/// Apache License 2.0

#include <OpenSpaceToolkit/Simulation/Component.hpp>
#include <OpenSpaceToolkit/Simulation/Component/Geometry.hpp>
#include <OpenSpaceToolkit/Simulation/Simulator.hpp>

#include <OpenSpaceToolkit/Core/Error.hpp>
#include <OpenSpaceToolkit/Core/Utility.hpp>

namespace ostk
{
namespace simulation
{
namespace component
{

namespace
{

const Component& ValidateComponent(const Shared<const Component>& aComponentSPtr)
{
    if (aComponentSPtr == nullptr)
    {
        throw ostk::core::error::runtime::Undefined("Component");
    }

    return *aComponentSPtr;
}

const ObjectGeometry& ValidateGeometry(const Celestial& aCelestialObject)
{
    const std::optional<ObjectGeometry>& geometry = aCelestialObject.getGeometry();

    if (!geometry.has_value())
    {
        throw ostk::core::error::runtime::Undefined("Celestial object geometry");
    }

    return geometry.value();
}

}  // namespace

Geometry::Geometry(const String& aName, const Composite& aComposite, const Shared<const Component>& aComponentSPtr)
    : name_(aName),
      geometry_(aComposite, ValidateComponent(aComponentSPtr).getFrame()),
      componentPtr_(aComponentSPtr)
{
    if (name_.isEmpty())
    {
        throw ostk::core::error::runtime::Undefined("Geometry name");
    }
}

bool Geometry::operator==(const Geometry& aGeometry) const
{
    return (this->name_ == aGeometry.name_) && (this->geometry_ == aGeometry.geometry_) &&
           (this->componentPtr_ == aGeometry.componentPtr_);
}

bool Geometry::operator!=(const Geometry& aGeometry) const
{
    return !((*this) == aGeometry);
}

std::ostream& operator<<(std::ostream& anOutputStream, const Geometry& aGeometry)
{
    aGeometry.print(anOutputStream, true);

    return anOutputStream;
}

const Component& Geometry::getComponent() const
{
    return *(this->componentPtr_);
}

const String& Geometry::getName() const
{
    return name_;
}

void Geometry::print(std::ostream& anOutputStream, bool displayDecorators) const
{
    displayDecorators ? ostk::core::utils::Print::Header(anOutputStream, "Geometry") : void();

    ostk::core::utils::Print::Line(anOutputStream) << "Name:" << this->getName();
    ostk::core::utils::Print::Line(anOutputStream) << "Geometry:" << geometry_;
    ostk::core::utils::Print::Line(anOutputStream) << "Component:" << this->getComponent();
}

bool Geometry::intersects(const ObjectGeometry& aGeometry) const
{
    using ostk::physics::time::Instant;

    const Instant instant = this->getComponent().getSimulator().getInstant();

    // TBM: Why GCRF?
    return this->getGeometryIn(Frame::GCRF()).intersects(aGeometry.in(Frame::GCRF(), instant));
}

bool Geometry::intersects(const Celestial& aCelestialObject) const
{
    return this->intersects(ValidateGeometry(aCelestialObject));
}

bool Geometry::contains(const ObjectGeometry& aGeometry) const
{
    using ostk::physics::time::Instant;

    const Instant instant = this->getComponent().getSimulator().getInstant();

    // TBM: Why GCRF?
    return this->getGeometryIn(Frame::GCRF()).contains(aGeometry.in(Frame::GCRF(), instant));
}

bool Geometry::contains(const Celestial& aCelestialObject) const
{
    return this->contains(ValidateGeometry(aCelestialObject));
}

const Composite& Geometry::getComposite() const
{
    return this->geometry_.getComposite();
}

const Shared<const Frame>& Geometry::getFrame() const
{
    return this->geometry_.getFrame();
}

ObjectGeometry Geometry::getGeometryIn(const Shared<const Frame>& aFrameSPtr) const
{
    return this->geometry_.in(aFrameSPtr, this->getComponent().getSimulator().getInstant());
}

ObjectGeometry Geometry::intersectionWith(const ObjectGeometry& aGeometry) const
{
    using ostk::physics::time::Instant;

    const Instant instant = this->getComponent().getSimulator().getInstant();

    // TBM: Why GCRF?
    return this->getGeometryIn(Frame::GCRF()).intersectionWith(aGeometry.in(Frame::GCRF(), instant));
}

ObjectGeometry Geometry::intersectionWith(const Celestial& aCelestialObject) const
{
    return this->intersectionWith(ValidateGeometry(aCelestialObject));
}

Shared<Geometry> Geometry::Configure(
    const GeometryConfiguration& aGeometryConfiguration, const Shared<const Component>& aComponentSPtr
)
{
    if (aComponentSPtr == nullptr)
    {
        throw ostk::core::error::runtime::Undefined("Component");
    }

    return std::make_shared<Geometry>(aGeometryConfiguration.name, aGeometryConfiguration.composite, aComponentSPtr);
}

}  // namespace component
}  // namespace simulation
}  // namespace ostk
