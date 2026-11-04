/// Apache License 2.0

#include <OpenSpaceToolkit/Simulation/Component.hpp>
#include <OpenSpaceToolkit/Simulation/Component/Geometry.hpp>
#include <OpenSpaceToolkit/Simulation/Simulator.hpp>

#include <OpenSpaceToolkit/Core/Error.hpp>
#include <OpenSpaceToolkit/Core/Utility.hpp>

#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Object/Point.hpp>

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
        geometry_ = ObjectGeometry(aComposite, componentPtr_->getFrame());
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
    aGeometry.print(anOutputStream, true);

    return anOutputStream;
}

bool Geometry::isDefined() const
{
    return (!this->name_.isEmpty()) && this->geometry_.isDefined() && (this->componentPtr_ != nullptr);
}

const Component& Geometry::getComponent() const
{
    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Geometry");
    }

    return *(this->componentPtr_);
}

const String& Geometry::getName() const
{
    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Geometry");
    }

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

    if ((!this->isDefined()) || (!aGeometry.isDefined()))
    {
        throw ostk::core::error::runtime::Undefined("Geometry");
    }

    const Instant instant = this->getComponent().getSimulator().getInstant();

    // TBM: Why GCRF?
    return this->getGeometryIn(Frame::GCRF()).intersects(aGeometry.in(Frame::GCRF(), instant));
}

bool Geometry::intersects(const Celestial& aCelestialObject) const
{
    return this->intersects(aCelestialObject.getGeometry());
}

bool Geometry::contains(const ObjectGeometry& aGeometry) const
{
    using ostk::physics::time::Instant;

    if ((!this->isDefined()) || (!aGeometry.isDefined()))
    {
        throw ostk::core::error::runtime::Undefined("Geometry");
    }

    const Instant instant = this->getComponent().getSimulator().getInstant();

    // TBM: Why GCRF?
    return this->getGeometryIn(Frame::GCRF()).contains(aGeometry.in(Frame::GCRF(), instant));
}

bool Geometry::contains(const Celestial& aCelestialObject) const
{
    return this->contains(aCelestialObject.getGeometry());
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
    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Geometry");
    }

    return this->geometry_.in(aFrameSPtr, this->getComponent().getSimulator().getInstant());
}

ObjectGeometry Geometry::intersectionWith(const ObjectGeometry& aGeometry) const
{
    using ostk::physics::time::Instant;

    if ((!this->isDefined()) || (!aGeometry.isDefined()))
    {
        throw ostk::core::error::runtime::Undefined("Geometry");
    }

    const Instant instant = this->getComponent().getSimulator().getInstant();

    // TBM: Why GCRF?
    return this->getGeometryIn(Frame::GCRF()).intersectionWith(aGeometry.in(Frame::GCRF(), instant));
}

ObjectGeometry Geometry::intersectionWith(const Celestial& aCelestialObject) const
{
    return this->intersectionWith(aCelestialObject.getGeometry());
}

Geometry Geometry::Undefined()
{
    using ostk::mathematics::geometry::d3::object::Point;

    return {String::Empty(), Composite(Point::Undefined()), nullptr};
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
