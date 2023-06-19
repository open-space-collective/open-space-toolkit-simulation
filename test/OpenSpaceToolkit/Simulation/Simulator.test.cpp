/// Apache License 2.0

#include <OpenSpaceToolkit/Simulation/Component.hpp>
#include <OpenSpaceToolkit/Simulation/Component/Geometry.hpp>
#include <OpenSpaceToolkit/Simulation/Component/State.hpp>
#include <OpenSpaceToolkit/Simulation/Satellite.hpp>
#include <OpenSpaceToolkit/Simulation/Simulator.hpp>

#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Objects/Composite.hpp>
#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Objects/LineString.hpp>
#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Objects/Point.hpp>
#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Objects/Polygon.hpp>
#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Objects/Pyramid.hpp>
#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Transformations/Rotations/Quaternion.hpp>

#include <OpenSpaceToolkit/Astrodynamics/Flight/Profile.hpp>

#include <Global.test.hpp>

TEST(OpenSpaceToolkit_Simulation_Simulator, Constructor)
{
    using ostk::core::types::Shared;
    using ostk::core::ctnr::Array;

    using ostk::physics::Environment;

    using ostk::simulation::Simulator;
    using ostk::simulation::Satellite;

    {
        const Environment environment = Environment::Default();
        const Array<Shared<Satellite>> satellites = Array<Shared<Satellite>>::Empty();

        EXPECT_NO_THROW(Simulator simulator = Simulator(environment, satellites));
    }
}

TEST(OpenSpaceToolkit_Simulation_Simulator, Undefined)
{
    using ostk::simulation::Simulator;

    {
        EXPECT_FALSE(Simulator::Undefined().isDefined());
    }
}

TEST(OpenSpaceToolkit_Simulation_Simulator, Test_1)
{
    using ostk::core::types::String;
    using ostk::core::types::Shared;
    using ostk::core::ctnr::Array;

    using ostk::math::geom::d3::trf::rot::Quaternion;
    using ostk::math::geom::d3::objects::Point;
    using ostk::math::geom::d3::objects::LineString;
    using ostk::math::geom::d3::objects::Polygon;
    using ostk::math::geom::d3::objects::Pyramid;
    using ostk::math::geom::d3::objects::Composite;

    using ostk::physics::Environment;
    using ostk::physics::units::Length;
    using ostk::physics::time::Instant;
    using ostk::physics::time::Scale;
    using ostk::physics::time::DateTime;
    using ostk::physics::time::Time;
    using ostk::physics::coord::Frame;

    using ostk::astro::trajectory::Orbit;
    using ostk::astro::flight::Profile;

    using ostk::simulation::Simulator;
    using ostk::simulation::Satellite;
    using ostk::simulation::Component;
    using ostk::simulation::component::State;
    using ostk::simulation::component::Geometry;

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
                           {0.0, 1.0, 0.0}},
                       Point {0.0, 0.0, 0.0}}}}}}}}}}
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
