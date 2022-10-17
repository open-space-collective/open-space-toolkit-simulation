////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Simulation
/// @file           OpenSpaceToolkit/Simulation/Simulator.test.cpp
/// @author         Remy Derollez <remy@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <OpenSpaceToolkit/Simulation/Simulator.hpp>
#include <OpenSpaceToolkit/Simulation/Satellite.hpp>
#include <OpenSpaceToolkit/Simulation/Component.hpp>
#include <OpenSpaceToolkit/Simulation/Component/State.hpp>
#include <OpenSpaceToolkit/Simulation/Component/Geometry.hpp>

#include <OpenSpaceToolkit/Astrodynamics/Flight/Profile.hpp>

#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Objects/Point.hpp>
#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Objects/Pyramid.hpp>
#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Objects/Polygon.hpp>
#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Transformations/Rotations/RotationMatrix.hpp>

#include <Global.test.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST (OpenSpaceToolkit_Simulation_Simulator, Constructor)
{

    using ostk::core::ctnr::Array ;

    using ostk::physics::Environment ;

    using ostk::simulation::Simulator ;
    using ostk::simulation::Satellite ;

    {

        const Environment environment = Environment::Default() ;
        const Array<Satellite> satellites = Array<Satellite>::Empty() ;

        EXPECT_NO_THROW(Simulator simulator = Simulator(environment, satellites)) ;

    }

}

TEST (OpenSpaceToolkit_Simulation_Simulator, Undefined)
{

    using ostk::simulation::Simulator ;

    {

        EXPECT_FALSE(Simulator::Undefined().isDefined()) ;

    }

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST (OpenSpaceToolkit_Simulation_Simulator, Test_1)
{

    using ostk::core::types::String ;
    using ostk::core::ctnr::Array ;

    using ostk::math::geom::d3::objects::Point ;
    using ostk::math::geom::d3::trf::rot::RotationMatrix ;
    using ostk::math::geom::d3::objects::Polygon ;
    using ostk::math::geom::d3::objects::Pyramid ;

    using ostk::physics::Environment ;
    using ostk::physics::units::Length ;
    using ostk::physics::time::Instant ;
    using ostk::physics::time::Scale ;
    using ostk::physics::time::DateTime ;
    using ostk::physics::time::Time ;

    using ostk::astro::trajectory::Orbit ;
    using ostk::astro::flight::Profile ;

    using ostk::simulation::Simulator ;
    using ostk::simulation::Satellite ;
    using ostk::simulation::Component ;
    using ostk::simulation::component::State ;
    using ostk::simulation::component::Geometry ;

    {

        const Environment environment = Environment::Default() ;

        const Orbit orbit = Orbit::SunSynchronous
        (
            Instant::DateTime(DateTime(2020, 1, 1, 0, 0, 0), Scale::UTC), // Epoch
            Length::Kilometers(500.0), // Altitude
            Time(14, 0, 0), // LTAN
            environment.accessCelestialObjectWithName("Earth") // Celestial object
        ) ;

        const Profile profile = Profile::NadirPointing
        (
            orbit,
            Orbit::FrameType::VVLH
        ) ;

        const Satellite satellite =
        {
            "LoftSat-1",
            profile,
            {
                {
                    "Component A",
                    Component::Type::Other,
                    State::Undefined(),
                    {"tag-a", "tag-b"},
                    {
                        Geometry
                        {
                            "FOV",
                            Geometry::Type::Sensing,
                            Pyramid
                            {
                                Polygon { { { { -0.1, -1.0 }, { +0.1, -1.0 }, { +0.1, +1.0 }, { -0.1, +1.0 } } }, Point { 0.0, 0.0, 1.0 }, { 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 } },
                                Point { 0.0, 0.0, 0.0 }
                            }
                        }
                    }
                }
            }
        } ;

        Simulator simulator =
        {
            environment,
            {
                satellite
            }
        } ;

    }

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
