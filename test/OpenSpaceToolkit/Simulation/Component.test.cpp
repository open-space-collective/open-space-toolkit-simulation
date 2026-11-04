/// Apache License 2.0

#include <sstream>

#include <OpenSpaceToolkit/Simulation/Component.hpp>
#include <OpenSpaceToolkit/Simulation/Component/Geometry.hpp>

#include <OpenSpaceToolkit/Physics/Coordinate/Frame.hpp>

#include <Global.test.hpp>

using ostk::core::container::Array;
using ostk::core::type::Shared;
using ostk::core::type::String;

using ostk::physics::coordinate::Frame;

using ostk::simulation::Component;
using ostk::simulation::component::Geometry;
using ostk::simulation::ComponentConfiguration;

class OpenSpaceToolkit_Simulation_Component : public ::testing::Test
{
   protected:
    static Shared<Component> MakeComponent(const String& aName, const std::optional<Component::Type>& aType)
    {
        return std::make_shared<Component>(
            String::Format("{}-id", aName),
            aName,
            aType,
            Array<String>::Empty(),
            Array<Shared<Geometry>>::Empty(),
            Array<Shared<Component>>::Empty(),
            nullptr,
            Frame::GCRF(),
            nullptr
        );
    }

    const Shared<Component> parentSPtr_ = MakeComponent("Parent", Component::Type::Assembly);
};

TEST_F(OpenSpaceToolkit_Simulation_Component, GetType)
{
    {
        EXPECT_EQ(MakeComponent("Sensor", Component::Type::Sensor)->getType(), Component::Type::Sensor);
    }

    {
        EXPECT_EQ(MakeComponent("Unspecified", std::nullopt)->getType(), std::nullopt);
    }
}

TEST_F(OpenSpaceToolkit_Simulation_Component, Configure)
{
    {
        const ComponentConfiguration componentConfiguration = {"camera-id", "Camera"};

        EXPECT_EQ(componentConfiguration.type, std::nullopt);

        const Shared<Component> componentSPtr = Component::Configure(componentConfiguration, parentSPtr_);

        EXPECT_FALSE(componentSPtr->getType().has_value());
    }

    {
        const ComponentConfiguration componentConfiguration = {"camera-id", "Camera", std::nullopt};

        const Shared<Component> componentSPtr = Component::Configure(componentConfiguration, parentSPtr_);

        EXPECT_FALSE(componentSPtr->getType().has_value());
    }

    {
        const ComponentConfiguration componentConfiguration = {"camera-id", "Camera", Component::Type::Sensor};

        const Shared<Component> componentSPtr = Component::Configure(componentConfiguration, parentSPtr_);

        EXPECT_EQ(componentSPtr->getType(), Component::Type::Sensor);
    }
}

TEST_F(OpenSpaceToolkit_Simulation_Component, StreamOperator)
{
    {
        std::ostringstream stream;

        stream << *MakeComponent("Camera", Component::Type::Sensor);

        EXPECT_NE(stream.str().find("Sensor"), std::string::npos);
        EXPECT_EQ(stream.str().find("Undefined"), std::string::npos);
    }

    {
        std::ostringstream stream;

        stream << *MakeComponent("Unspecified", std::nullopt);

        EXPECT_NE(stream.str().find("Undefined"), std::string::npos);
    }
}

TEST_F(OpenSpaceToolkit_Simulation_Component, StringFromType)
{
    {
        EXPECT_EQ(Component::StringFromType(Component::Type::Assembly), "Assembly");
        EXPECT_EQ(Component::StringFromType(Component::Type::Controller), "Controller");
        EXPECT_EQ(Component::StringFromType(Component::Type::Sensor), "Sensor");
        EXPECT_EQ(Component::StringFromType(Component::Type::Actuator), "Actuator");
        EXPECT_EQ(Component::StringFromType(Component::Type::Other), "Other");
    }
}
