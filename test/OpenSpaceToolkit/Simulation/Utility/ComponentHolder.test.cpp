/// Apache License 2.0

#include <OpenSpaceToolkit/Simulation/Component.hpp>
#include <OpenSpaceToolkit/Simulation/Component/Geometry.hpp>
#include <OpenSpaceToolkit/Simulation/Utility/ComponentHolder.hpp>

#include <OpenSpaceToolkit/Physics/Coordinate/Frame.hpp>

#include <Global.test.hpp>

using ostk::core::container::Array;
using ostk::core::container::Pair;
using ostk::core::type::Shared;
using ostk::core::type::String;

using ostk::physics::coordinate::Frame;

using ostk::simulation::Component;
using ostk::simulation::component::Geometry;
using ostk::simulation::utility::ComponentHolder;
using ostk::simulation::utility::splitComponentPath;

class OpenSpaceToolkit_Simulation_Utility_ComponentHolder : public ::testing::Test
{
   protected:
    static Shared<Component> MakeComponent(const String& aName, const Array<Shared<Component>>& aComponentArray)
    {
        return std::make_shared<Component>(
            String::Format("{}-id", aName),
            aName,
            Component::Type::Assembly,
            Array<String>::Empty(),
            Array<Shared<Geometry>>::Empty(),
            aComponentArray,
            nullptr,
            Frame::GCRF(),
            nullptr
        );
    }

    // Camera
    // └── Lens
    //     └── Detector
    const Shared<Component> detectorSPtr_ = MakeComponent("Detector", Array<Shared<Component>>::Empty());
    const Shared<Component> lensSPtr_ = MakeComponent("Lens", {detectorSPtr_});
    const Shared<Component> cameraSPtr_ = MakeComponent("Camera", {lensSPtr_});

    const ComponentHolder componentHolder_ = ComponentHolder(Array<Shared<Component>> {cameraSPtr_});
};

TEST_F(OpenSpaceToolkit_Simulation_Utility_ComponentHolder, HasComponentAt)
{
    {
        EXPECT_TRUE(componentHolder_.hasComponentAt("Camera"));
        EXPECT_TRUE(componentHolder_.hasComponentAt("/Camera"));
        EXPECT_TRUE(componentHolder_.hasComponentAt("Camera/"));
    }

    {
        EXPECT_TRUE(componentHolder_.hasComponentAt("Camera/Lens"));
        EXPECT_TRUE(componentHolder_.hasComponentAt("Camera/Lens/Detector"));
        EXPECT_TRUE(componentHolder_.hasComponentAt("/Camera/Lens/Detector/"));
    }

    {
        EXPECT_TRUE(componentHolder_.hasComponentAt("Camera.Lens"));
        EXPECT_TRUE(componentHolder_.hasComponentAt("Camera.Lens.Detector"));
        EXPECT_TRUE(componentHolder_.hasComponentAt(".Camera.Lens.Detector."));
    }

    {
        EXPECT_TRUE(componentHolder_.hasComponentAt("Camera/Lens.Detector"));
        EXPECT_TRUE(componentHolder_.hasComponentAt("Camera.Lens/Detector"));
    }

    {
        EXPECT_FALSE(componentHolder_.hasComponentAt("Lens"));
        EXPECT_FALSE(componentHolder_.hasComponentAt("Camera/Detector"));
        EXPECT_FALSE(componentHolder_.hasComponentAt("Camera.Detector"));
        EXPECT_FALSE(componentHolder_.hasComponentAt("Camera/Lens/Unknown"));
        EXPECT_FALSE(componentHolder_.hasComponentAt("Camera.Lens.Unknown"));
    }

    {
        EXPECT_ANY_THROW(componentHolder_.hasComponentAt(""));
    }
}

TEST_F(OpenSpaceToolkit_Simulation_Utility_ComponentHolder, AccessComponentAt)
{
    {
        EXPECT_EQ(componentHolder_.accessComponentAt("Camera").getName(), "Camera");
    }

    {
        EXPECT_EQ(componentHolder_.accessComponentAt("Camera/Lens").getName(), "Lens");
        EXPECT_EQ(componentHolder_.accessComponentAt("Camera/Lens/Detector").getName(), "Detector");
        EXPECT_EQ(componentHolder_.accessComponentAt("/Camera/Lens/Detector/").getName(), "Detector");
    }

    {
        EXPECT_EQ(componentHolder_.accessComponentAt("Camera.Lens").getName(), "Lens");
        EXPECT_EQ(componentHolder_.accessComponentAt("Camera.Lens.Detector").getName(), "Detector");
        EXPECT_EQ(componentHolder_.accessComponentAt(".Camera.Lens.Detector.").getName(), "Detector");
    }

    {
        EXPECT_EQ(componentHolder_.accessComponentAt("Camera/Lens.Detector").getName(), "Detector");
        EXPECT_EQ(componentHolder_.accessComponentAt("Camera.Lens/Detector").getName(), "Detector");
    }

    {
        EXPECT_ANY_THROW(componentHolder_.accessComponentAt(""));
        EXPECT_ANY_THROW(componentHolder_.accessComponentAt("Lens"));
        EXPECT_ANY_THROW(componentHolder_.accessComponentAt("Camera/Detector"));
        EXPECT_ANY_THROW(componentHolder_.accessComponentAt("Camera.Detector"));
    }
}

TEST_F(OpenSpaceToolkit_Simulation_Utility_ComponentHolder, SplitComponentPath)
{
    using PathSplit = Pair<String, String>;

    {
        EXPECT_EQ(splitComponentPath(""), PathSplit("", ""));
        EXPECT_EQ(splitComponentPath("/"), PathSplit("", ""));
        EXPECT_EQ(splitComponentPath("."), PathSplit("", ""));
    }

    {
        EXPECT_EQ(splitComponentPath("Camera"), PathSplit("Camera", ""));
        EXPECT_EQ(splitComponentPath("/Camera"), PathSplit("Camera", ""));
        EXPECT_EQ(splitComponentPath(".Camera"), PathSplit("Camera", ""));
        EXPECT_EQ(splitComponentPath("Camera/"), PathSplit("Camera", ""));
        EXPECT_EQ(splitComponentPath("Camera."), PathSplit("Camera", ""));
    }

    {
        EXPECT_EQ(splitComponentPath("Camera/Lens"), PathSplit("Camera", "Lens"));
        EXPECT_EQ(splitComponentPath("/Camera/Lens"), PathSplit("Camera", "Lens"));
        EXPECT_EQ(splitComponentPath("Camera/Lens/Detector"), PathSplit("Camera", "Lens/Detector"));
    }

    {
        EXPECT_EQ(splitComponentPath("Camera.Lens"), PathSplit("Camera", "Lens"));
        EXPECT_EQ(splitComponentPath(".Camera.Lens"), PathSplit("Camera", "Lens"));
        EXPECT_EQ(splitComponentPath("Camera.Lens.Detector"), PathSplit("Camera", "Lens.Detector"));
    }

    {
        EXPECT_EQ(splitComponentPath("Camera/Lens.Detector"), PathSplit("Camera", "Lens.Detector"));
        EXPECT_EQ(splitComponentPath("Camera.Lens/Detector"), PathSplit("Camera", "Lens/Detector"));
        EXPECT_EQ(splitComponentPath("/Camera.Lens"), PathSplit("Camera", "Lens"));
        EXPECT_EQ(splitComponentPath(".Camera/Lens"), PathSplit("Camera", "Lens"));
    }
}
