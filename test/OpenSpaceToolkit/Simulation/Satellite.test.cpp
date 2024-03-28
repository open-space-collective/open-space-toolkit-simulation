/// Apache License 2.0

#include <OpenSpaceToolkit/Simulation/Component.hpp>
#include <OpenSpaceToolkit/Simulation/Component/Geometry.hpp>
#include <OpenSpaceToolkit/Simulation/Component/State.hpp>
#include <OpenSpaceToolkit/Simulation/Satellite.hpp>

#include <OpenSpaceToolkit/Physics/Environment/Object/Celestial/Earth.hpp>
#include <OpenSpaceToolkit/Physics/Time/Instant.hpp>
#include <OpenSpaceToolkit/Physics/Time/Time.hpp>
#include <OpenSpaceToolkit/Physics/Unit/Length.hpp>

#include <OpenSpaceToolkit/Astrodynamics/Flight/Profile.hpp>

#include <Global.test.hpp>

using ostk::core::type::Shared;
using ostk::core::type::String;
using ostk::core::container::Array;

using ostk::physics::unit::Length;
using ostk::physics::time::Time;
using ostk::physics::time::Instant;
using ostk::physics::environment::object::celestial::Earth;

using ostk::astrodynamics::flight::Profile;
using ostk::astrodynamics::trajectory::Orbit;

using ostk::simulation::Component;
using ostk::simulation::ComponentConfiguration;
using ostk::simulation::component::Geometry;
using ostk::simulation::component::GeometryConfiguration;
using ostk::simulation::Satellite;
using ostk::simulation::SatelliteConfiguration;

class OpenSpaceToolkit_Simulation_Satellite : public ::testing::Test
{
   protected:
    void SetUp() override
    {
        const Orbit orbit = Orbit::SunSynchronous(
            Instant::J2000(), Length::Kilometers(7000.0), Time::Noon(), std::make_shared<Earth>(Earth::Default())
        );

        profile_ = Profile::NadirPointing(orbit, Orbit::FrameType::VVLH);

        this->satellite_ = Satellite(
            "87da0b5f-9f65-4c5c-a660-bd254742960b",
            "LoftSat-1",
            {"a", "b"},
            Array<Shared<Geometry>>::Empty(),
            Array<Shared<Component>>::Empty(),
            nullptr,
            std::make_shared<Profile>(profile_),
            nullptr
        );
    }

    Profile profile_ = Profile::Undefined();
    Satellite satellite_ = Satellite::Undefined();
};

TEST_F(OpenSpaceToolkit_Simulation_Satellite, IsDefined)
{
    {
        EXPECT_TRUE(satellite_.isDefined());
    }

    {
        EXPECT_FALSE(Satellite::Undefined().isDefined());
    }
}

TEST_F(OpenSpaceToolkit_Simulation_Satellite, getId)
{
    {
        EXPECT_EQ(satellite_.getId(), "87da0b5f-9f65-4c5c-a660-bd254742960b");
    }

    {
        EXPECT_ANY_THROW(Satellite::Undefined().getId());
    }
}

TEST_F(OpenSpaceToolkit_Simulation_Satellite, getName)
{
    {
        EXPECT_EQ(satellite_.getName(), "LoftSat-1");
    }

    {
        EXPECT_ANY_THROW(Satellite::Undefined().getName());
    }
}

TEST_F(OpenSpaceToolkit_Simulation_Satellite, Undefined)
{
    {
        EXPECT_FALSE(Satellite::Undefined().isDefined());
    }
}

TEST_F(OpenSpaceToolkit_Simulation_Satellite, Configure)
{
    const SatelliteConfiguration satelliteConfiguration = {
        "87da0b5f-9f65-4c5c-a660-bd254742960b",
        "LoftSat-1",
        profile_,
        Array<ComponentConfiguration>::Empty(),
        {"a", "b"},
        Array<GeometryConfiguration>::Empty(),
    };

    const Shared<Satellite> satellite = Satellite::Configure(satelliteConfiguration, nullptr);

    EXPECT_TRUE(satellite->isDefined());
}

TEST_F(OpenSpaceToolkit_Simulation_Satellite, AccessProfile)
{
    {
        EXPECT_NO_THROW(satellite_.accessProfile()->isDefined());
    }

    {
        EXPECT_ANY_THROW(Satellite::Undefined().accessProfile());
    }
}

// TEST_F (OpenSpaceToolkit_Simulation_Satellite, GenerateFrame)
