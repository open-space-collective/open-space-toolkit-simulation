/// Apache License 2.0

#include <OpenSpaceToolkit/Simulation/Component.hpp>
#include <OpenSpaceToolkit/Simulation/Component/Geometry.hpp>
#include <OpenSpaceToolkit/Simulation/Component/State.hpp>
#include <OpenSpaceToolkit/Simulation/Satellite.hpp>
#include <OpenSpaceToolkit/Simulation/Simulator.hpp>

#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Object/Composite.hpp>
#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Object/LineString.hpp>
#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Object/Point.hpp>
#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Object/Polygon.hpp>
#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Object/Pyramid.hpp>
#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Transformation/Rotation/Quaternion.hpp>

#include <OpenSpaceToolkit/Astrodynamics/Flight/Profile.hpp>

#include <Global.test.hpp>

using ostk::core::container::Array;
using ostk::core::container::Map;
using ostk::core::type::Shared;
using ostk::core::type::String;

using ostk::mathematics::geometry::d3::object::Composite;
using ostk::mathematics::geometry::d3::object::LineString;
using ostk::mathematics::geometry::d3::object::Point;
using ostk::mathematics::geometry::d3::object::Polygon;
using ostk::mathematics::geometry::d3::object::Pyramid;
using ostk::mathematics::geometry::d3::transformation::rotation::Quaternion;

using ostk::physics::coordinate::Frame;
using ostk::physics::Environment;
using ostk::physics::time::DateTime;
using ostk::physics::time::Duration;
using ostk::physics::time::Instant;
using ostk::physics::time::Scale;
using ostk::physics::time::Time;
using ostk::physics::unit::Length;

using ostk::astrodynamics::flight::Profile;
using ostk::astrodynamics::trajectory::Orbit;

using ostk::simulation::Component;
using ostk::simulation::component::Geometry;
using ostk::simulation::component::State;
using ostk::simulation::Satellite;
using ostk::simulation::SatelliteConfiguration;
using ostk::simulation::Simulator;
using ostk::simulation::SimulatorConfiguration;

class OpenSpaceToolkit_Simulation_Simulator : public ::testing::Test
{
   protected:
    const Environment environment_ = Environment::Default();

    const String satelliteName_ = "LoftSat-1";

    const Orbit orbit_ = Orbit::SunSynchronous(
        Instant::DateTime(DateTime(2020, 1, 1, 0, 0, 0), Scale::UTC),  // Epoch
        Length::Kilometers(500.0),                                     // Altitude
        Time(14, 0, 0),                                                // LTAN
        environment_.accessCelestialObjectWithName("Earth")            // Celestial object
    );

    const Shared<Simulator> simulatorSPtr_ = Simulator::Configure(
        {environment_,
         {{"1",
           satelliteName_,
           Profile::NadirPointing(orbit_, Orbit::FrameType::VVLH),
           {{"2",
             "Camera",
             Component::Type::Sensor,
             {"tag-a", "tag-b"},
             Quaternion::Unit(),
             {{"FOV",
               Composite {Pyramid {
                   Polygon {
                       {{{-0.1, -1.0}, {+0.1, -1.0}, {+0.1, +1.0}, {-0.1, +1.0}}},
                       Point {0.0, 0.0, 1.0},
                       {1.0, 0.0, 0.0},
                       {0.0, 1.0, 0.0}
                   },
                   Point {0.0, 0.0, 0.0}
               }}}}}}}}}
    );
};

TEST_F(OpenSpaceToolkit_Simulation_Simulator, Constructor)
{
    {
        const Array<Shared<Satellite>> satellites = Array<Shared<Satellite>>::Empty();

        EXPECT_NO_THROW(Simulator simulator = Simulator(environment_, satellites));
    }
}

TEST_F(OpenSpaceToolkit_Simulation_Simulator, StreamOperator)
{
    {
        testing::internal::CaptureStdout();

        EXPECT_NO_THROW(std::cout << *simulatorSPtr_ << std::endl);

        EXPECT_FALSE(testing::internal::GetCapturedStdout().empty());
    }
}

TEST_F(OpenSpaceToolkit_Simulation_Simulator, IsDefined)
{
    {
        EXPECT_TRUE(simulatorSPtr_->isDefined());
    }
}

TEST_F(OpenSpaceToolkit_Simulation_Simulator, HasSatelliteWithName)
{
    {
        EXPECT_TRUE(simulatorSPtr_->hasSatelliteWithName(satelliteName_));
    }
}

TEST_F(OpenSpaceToolkit_Simulation_Simulator, AccessEnvironment)
{
    {
        EXPECT_NO_THROW(simulatorSPtr_->accessEnvironment());
    }
}

TEST_F(OpenSpaceToolkit_Simulation_Simulator, AccessSatelliteMap)
{
    {
        EXPECT_THROW(Simulator::Undefined().accessSatelliteMap(), ostk::core::error::runtime::Undefined);
    }

    {
        EXPECT_EQ(simulatorSPtr_->accessSatelliteMap().size(), 1);
    }
}

TEST_F(OpenSpaceToolkit_Simulation_Simulator, AccessSatelliteWithName)
{
    {
        EXPECT_EQ(satelliteName_, simulatorSPtr_->accessSatelliteWithName(satelliteName_).getName());
    }
}

TEST_F(OpenSpaceToolkit_Simulation_Simulator, GetInstant)
{
    {
        const Instant instant = Instant::DateTime(DateTime(2020, 1, 1, 0, 0, 0), Scale::UTC);

        simulatorSPtr_->setInstant(instant);

        EXPECT_EQ(instant, simulatorSPtr_->getInstant());
    }
}

TEST_F(OpenSpaceToolkit_Simulation_Simulator, SetInstant)
{
    {
        const Instant instant = Instant::DateTime(DateTime(2020, 1, 1, 0, 0, 0), Scale::UTC);

        simulatorSPtr_->setInstant(instant);

        EXPECT_EQ(instant, simulatorSPtr_->getInstant());
    }
}

TEST_F(OpenSpaceToolkit_Simulation_Simulator, StepForward)
{
    {
        const Instant instant = Instant::DateTime(DateTime(2020, 1, 1, 0, 0, 0), Scale::UTC);

        simulatorSPtr_->setInstant(instant);

        const Duration duration = Duration::Seconds(60.0);

        simulatorSPtr_->stepForward(duration);

        EXPECT_EQ(instant + duration, simulatorSPtr_->getInstant());
    }
}

TEST_F(OpenSpaceToolkit_Simulation_Simulator, Test_1)
{
    {
        const Environment environment = Environment::Default();

        const Orbit orbit = Orbit::SunSynchronous(
            Instant::DateTime(DateTime(2020, 1, 1, 0, 0, 0), Scale::UTC),  // Epoch
            Length::Kilometers(500.0),                                     // Altitude
            Time(14, 0, 0),                                                // LTAN
            environment.accessCelestialObjectWithName("Earth")             // Celestial object
        );

        Shared<Simulator> simulatorSPtr = Simulator::Configure(
            {environment,
             {{"1",
               "LoftSat-1",
               Profile::NadirPointing(orbit, Orbit::FrameType::VVLH),
               {{"2",
                 "Camera",
                 Component::Type::Sensor,
                 {"tag-a", "tag-b"},
                 Quaternion::Unit(),
                 {{"FOV",
                   Composite {Pyramid {
                       Polygon {
                           {{{-0.1, -1.0}, {+0.1, -1.0}, {+0.1, +1.0}, {-0.1, +1.0}}},
                           Point {0.0, 0.0, 1.0},
                           {1.0, 0.0, 0.0},
                           {0.0, 1.0, 0.0}
                       },
                       Point {0.0, 0.0, 0.0}
                   }}}}}}}}}
        );

        const Instant instant = Instant::DateTime(DateTime(2020, 1, 1, 0, 0, 0), Scale::UTC);

        simulatorSPtr->setInstant(instant);

        const auto camera = simulatorSPtr->accessSatelliteWithName("LoftSat-1").accessComponentWithName("Camera");

        const auto cameraGeometry = camera.accessGeometryWithName("FOV");
        const auto earthGeometry = environment.accessCelestialObjectWithName("Earth")->accessGeometry();

        EXPECT_TRUE(cameraGeometry.intersects(earthGeometry));

        EXPECT_EQ(2, cameraGeometry.intersectionWith(earthGeometry).accessComposite().getObjectCount());
        EXPECT_TRUE(cameraGeometry.intersectionWith(earthGeometry).accessComposite().accessObjectAt(0).is<LineString>()
        );
        EXPECT_TRUE(cameraGeometry.intersectionWith(earthGeometry).accessComposite().accessObjectAt(1).is<LineString>()
        );
    }
}

TEST_F(OpenSpaceToolkit_Simulation_Simulator, Undefined)
{
    {
        EXPECT_FALSE(Simulator::Undefined().isDefined());
    }
}

TEST_F(OpenSpaceToolkit_Simulation_Simulator, Configure)
{
    {
        const Array<SatelliteConfiguration> satelliteConfiguraitons = Array<SatelliteConfiguration>::Empty();

        EXPECT_NO_THROW(Simulator::Configure(SimulatorConfiguration {environment_, satelliteConfiguraitons}));
    }
}
