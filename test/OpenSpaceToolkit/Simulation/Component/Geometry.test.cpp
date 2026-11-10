/// Apache License 2.0

#include <OpenSpaceToolkit/Simulation/Component.hpp>
#include <OpenSpaceToolkit/Simulation/Component/Geometry.hpp>

#include <OpenSpaceToolkit/Core/Error.hpp>

#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Object/Composite.hpp>
#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Object/Point.hpp>

#include <OpenSpaceToolkit/Physics/Coordinate/Frame.hpp>

#include <Global.test.hpp>

using ostk::core::container::Array;
using ostk::core::type::Shared;
using ostk::core::type::String;

using ostk::mathematics::geometry::d3::object::Composite;
using ostk::mathematics::geometry::d3::object::Point;

using ostk::physics::coordinate::Frame;

using ostk::simulation::Component;
using ostk::simulation::component::Geometry;

class OpenSpaceToolkit_Simulation_Component_Geometry : public ::testing::Test
{
   protected:
    static Shared<const Component> MakeComponent(const Shared<const Frame>& aFrameSPtr)
    {
        return std::make_shared<Component>(
            "camera-id",
            "Camera",
            Component::Type::Sensor,
            Array<String>::Empty(),
            Array<Shared<Geometry>>::Empty(),
            Array<Shared<Component>>::Empty(),
            nullptr,
            aFrameSPtr,
            nullptr
        );
    }

    const Shared<const Component> componentSPtr_ = MakeComponent(Frame::GCRF());
    const Composite composite_ = Composite {Point {0.0, 0.0, 1.0}};
};

TEST_F(OpenSpaceToolkit_Simulation_Component_Geometry, Constructor)
{
    {
        const Geometry geometry = {"FOV", composite_, componentSPtr_};

        EXPECT_EQ(geometry.getName(), "FOV");
        EXPECT_EQ(&geometry.getComponent(), componentSPtr_.get());
        EXPECT_EQ(geometry.getFrame(), componentSPtr_->getFrame());
        EXPECT_EQ(geometry.getComposite(), composite_);
    }

    {
        EXPECT_THROW(Geometry("", composite_, componentSPtr_), ostk::core::error::runtime::Undefined);
    }

    {
        EXPECT_THROW(Geometry("FOV", composite_, nullptr), ostk::core::error::runtime::Undefined);
    }

    {
        EXPECT_THROW(Geometry("FOV", composite_, MakeComponent(nullptr)), ostk::core::error::runtime::Undefined);
    }
}

TEST_F(OpenSpaceToolkit_Simulation_Component_Geometry, EqualToOperator)
{
    {
        const Geometry geometry = {"FOV", composite_, componentSPtr_};

        EXPECT_TRUE(geometry == geometry);
        EXPECT_FALSE(geometry != geometry);
    }

    {
        const Geometry firstGeometry = {"FOV", composite_, componentSPtr_};
        const Geometry secondGeometry = {"Other", composite_, componentSPtr_};

        EXPECT_FALSE(firstGeometry == secondGeometry);
        EXPECT_TRUE(firstGeometry != secondGeometry);
    }
}
