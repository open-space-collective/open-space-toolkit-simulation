/// Apache License 2.0

#include <OpenSpaceToolkit/Simulation/Component.hpp>
#include <OpenSpaceToolkit/Simulation/Component/Geometry.hpp>
#include <OpenSpaceToolkit/Simulation/Component/State.hpp>
#include <OpenSpaceToolkit/Simulation/Satellite.hpp>

#include <OpenSpaceToolkit/Core/Error.hpp>

#include <OpenSpaceToolkit/Physics/Environment.hpp>
#include <OpenSpaceToolkit/Physics/Environment/Object/Celestial/Earth.hpp>
#include <OpenSpaceToolkit/Physics/Time/Instant.hpp>
#include <OpenSpaceToolkit/Physics/Time/Time.hpp>
#include <OpenSpaceToolkit/Physics/Unit/Length.hpp>

#include <OpenSpaceToolkit/Astrodynamics/Flight/Profile.hpp>

#include <Global.test.hpp>

using ostk::core::container::Array;
using ostk::core::type::Shared;
using ostk::core::type::String;

using ostk::physics::Environment;
using ostk::physics::environment::object::celestial::Earth;
using ostk::physics::time::Instant;
using ostk::physics::time::Scale;
using ostk::physics::time::Time;
using ostk::physics::unit::Length;

using ostk::astrodynamics::flight::Profile;
using ostk::astrodynamics::trajectory::Orbit;

using ostk::simulation::Component;
using ostk::simulation::component::Geometry;
using ostk::simulation::component::GeometryConfiguration;
using ostk::simulation::ComponentConfiguration;
using ostk::simulation::Satellite;
using ostk::simulation::SatelliteConfiguration;

class OpenSpaceToolkit_Simulation_Satellite : public ::testing::Test
{
   protected:
    void SetUp() override
    {
        const Environment environment = Environment::Default();
        const Orbit orbit = Orbit::SunSynchronous(
            Instant::J2000(),
            Length::Kilometers(500.0),
            Time::Noon(),
            environment.accessCelestialObjectWithName("Earth")
        );

        profile_ = Profile::LocalOrbitalFramePointing(orbit, Orbit::FrameType::VVLH);

        this->satellite_.emplace(
            "87da0b5f-9f65-4c5c-a660-bd254742960b",
            "LoftSat-1",
            Array<String>({"a", "b"}),
            Array<Shared<Geometry>>::Empty(),
            Array<Shared<Component>>::Empty(),
            nullptr,
            std::make_shared<Profile>(profile_),
            nullptr
        );
    }

    Profile profile_ = Profile::Undefined();
    std::optional<Satellite> satellite_;
};

TEST_F(OpenSpaceToolkit_Simulation_Satellite, Constructor)
{
    {
        EXPECT_THROW(
            Satellite(
                "",
                "LoftSat-1",
                {"a", "b"},
                Array<Shared<Geometry>>::Empty(),
                Array<Shared<Component>>::Empty(),
                nullptr,
                std::make_shared<Profile>(profile_),
                nullptr
            ),
            ostk::core::error::runtime::Undefined
        );
    }

    {
        EXPECT_THROW(
            Satellite(
                "87da0b5f-9f65-4c5c-a660-bd254742960b",
                "",
                {"a", "b"},
                Array<Shared<Geometry>>::Empty(),
                Array<Shared<Component>>::Empty(),
                nullptr,
                std::make_shared<Profile>(profile_),
                nullptr
            ),
            ostk::core::error::runtime::Undefined
        );
    }
}

TEST_F(OpenSpaceToolkit_Simulation_Satellite, getId)
{
    {
        EXPECT_EQ(satellite_->getId(), "87da0b5f-9f65-4c5c-a660-bd254742960b");
        EXPECT_EQ(&satellite_->getId(), &satellite_->getId());
    }
}

TEST_F(OpenSpaceToolkit_Simulation_Satellite, getName)
{
    {
        EXPECT_EQ(satellite_->getName(), "LoftSat-1");
        EXPECT_EQ(&satellite_->getName(), &satellite_->getName());
    }
}

TEST_F(OpenSpaceToolkit_Simulation_Satellite, WithoutFrame)
{
    // The fixture satellite is built without a frame.

    {
        EXPECT_EQ(satellite_->getType(), Component::Type::Assembly);
        EXPECT_EQ(satellite_->getTags(), Array<String>({"a", "b"}));
        EXPECT_TRUE(satellite_->getGeometries().isEmpty());
        EXPECT_NE(satellite_->getProfile(), nullptr);
    }

    {
        EXPECT_THROW(satellite_->getFrame(), ostk::core::error::runtime::Undefined);
    }

    {
        const Shared<Satellite> satelliteSPtr = std::make_shared<Satellite>(satellite_.value());

        EXPECT_THROW(
            Component::Configure({"camera-id", "Camera"}, satelliteSPtr), ostk::core::error::runtime::Undefined
        );
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

    EXPECT_EQ(satellite->getName(), "LoftSat-1");
    EXPECT_NE(satellite->getFrame(), nullptr);
}

TEST_F(OpenSpaceToolkit_Simulation_Satellite, GetProfile)
{
    {
        EXPECT_NO_THROW(satellite_->getProfile()->isDefined());
        EXPECT_EQ(&satellite_->getProfile(), &satellite_->getProfile());
    }
}

// TEST_F (OpenSpaceToolkit_Simulation_Satellite, GenerateFrame)
