/// Apache License 2.0

#include <OpenSpaceToolkit/Simulation/Component.hpp>
#include <OpenSpaceToolkit/Simulation/Component/Geometry.hpp>
#include <OpenSpaceToolkit/Simulation/Simulator.hpp>

#include <OpenSpaceToolkit/Core/Error.hpp>
#include <OpenSpaceToolkit/Core/Utilities.hpp>

namespace ostk
{
namespace simulation
{
namespace component
{

Geometry::Geometry(const String& aName, const Composite& aComposite, const Shared<const Component>& aComponentSPtr)
    : name_(aName),
      geometry_(ObjectGeometry::Undefined()),
      componentPtr_(aComponentSPtr)
{
    if (componentPtr_ != nullptr)
    {
        geometry_ = ObjectGeometry(aComposite, componentPtr_->accessFrame());
    }
}

bool Geometry::operator==(const Geometry& aGeometry) const
{
    if ((!this->isDefined()) || (!aGeometry.isDefined()))
    {
        return false;
    }

    return (this->name_ == aGeometry.name_) && (this->geometry_ == aGeometry.geometry_) &&
           (this->componentPtr_ == aGeometry.componentPtr_);
}

std::ostream& operator<<(std::ostream& anOutputStream, const Geometry& aGeometry)
{
    // TBI

    (void)aGeometry;

    return anOutputStream;
}

bool Geometry::isDefined() const
{
    return (!this->name_.isEmpty()) && this->geometry_.isDefined() && (this->componentPtr_ != nullptr);
}

const Component& Geometry::accessComponent() const
{
    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Geometry");
    }

    return *(this->componentPtr_);
}

String Geometry::getName() const
{
    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Geometry");
    }

    return name_;
}

bool Geometry::intersects(const ObjectGeometry& aGeometry) const
{
    using ostk::physics::time::Instant;

    if ((!this->isDefined()) || (!aGeometry.isDefined()))
    {
        throw ostk::core::error::runtime::Undefined("Geometry");
    }

    const Instant instant = this->accessComponent().accessSimulator().getInstant();

    // TBM: Why GCRF?
    return this->getGeometryIn(Frame::GCRF()).intersects(aGeometry.in(Frame::GCRF(), instant));
}

bool Geometry::intersects(const Celestial& aCelestialObject) const
{
    return this->intersects(aCelestialObject.accessGeometry());
}

bool Geometry::contains(const ObjectGeometry& aGeometry) const
{
    using ostk::physics::time::Instant;

    if ((!this->isDefined()) || (!aGeometry.isDefined()))
    {
        throw ostk::core::error::runtime::Undefined("Geometry");
    }

    const Instant instant = this->accessComponent().accessSimulator().getInstant();

    // TBM: Why GCRF?
    return this->getGeometryIn(Frame::GCRF()).contains(aGeometry.in(Frame::GCRF(), instant));
}

bool Geometry::contains(const Celestial& aCelestialObject) const
{
    return this->contains(aCelestialObject.accessGeometry());
}

const Composite& Geometry::accessComposite() const
{
    return this->geometry_.accessComposite();
}

Shared<const Frame> Geometry::accessFrame() const
{
    return this->geometry_.accessFrame();
}

ObjectGeometry Geometry::getGeometryIn(const Shared<const Frame>& aFrameSPtr) const
{
    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Geometry");
    }

    return this->geometry_.in(aFrameSPtr, this->accessComponent().accessSimulator().getInstant());
}

ObjectGeometry Geometry::intersectionWith(const ObjectGeometry& aGeometry) const
{
    using ostk::physics::time::Instant;

    if ((!this->isDefined()) || (!aGeometry.isDefined()))
    {
        throw ostk::core::error::runtime::Undefined("Geometry");
    }

    const Instant instant = this->accessComponent().accessSimulator().getInstant();

    // TBM: Why GCRF?
    return this->getGeometryIn(Frame::GCRF()).intersectionWith(aGeometry.in(Frame::GCRF(), instant));
}

ObjectGeometry Geometry::intersectionWith(const Celestial& aCelestialObject) const
{
    return this->intersectionWith(aCelestialObject.accessGeometry());
}

Geometry Geometry::Undefined()
{
    return {String::Empty(), Composite::Undefined(), nullptr};
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
