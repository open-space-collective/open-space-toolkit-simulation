////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Simulation
/// @file           OpenSpaceToolkit/Simulation/Satellite.test.cpp
/// @author         Remy Derollez <remy@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

TEST (OpenSpaceToolkit_Simulation_Satellite, Constructor)
{

    using ostk::core::types::String ;
    using ostk::core::ctnr::Array ;

    using ostk::math::geom::d3::objects::Point ;
    using ostk::math::geom::d3::trf::rot::RotationMatrix ;
    using ostk::math::geom::d3::objects::Polygon ;
    using ostk::math::geom::d3::objects::Pyramid ;

    using ostk::astro::flight::Profile ;

    using ostk::simulation::Component ;
    using ostk::simulation::Satellite ;
    using ostk::simulation::component::State ;
    using ostk::simulation::component::Geometry ;

    const String satelliteId = "87da0b5f-9f65-4c5c-a660-bd254742960b" ;
    const String satelliteName = "LoftSat-1" ;
    const Array<Component> components =
    {
        Component
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
    } ;

    {

        EXPECT_NO_THROW(const Satellite satellite = Satellite(satelliteId, satelliteName, Profile::Undefined(), components) ;) ;

    }

    {

        EXPECT_NO_THROW(const Satellite satellite = Satellite(satelliteId, satelliteName, Profile::Undefined()) ;) ;

    }

    {

        EXPECT_NO_THROW(const Satellite satellite = Satellite(satelliteName, Profile::Undefined()) ;) ;

    }

}

TEST (OpenSpaceToolkit_Simulation_Satellite, IsDefined)
{

    using ostk::core::types::String ;

    using ostk::astro::flight::Profile ;

    using ostk::simulation::Satellite ;

    {

        const String satelliteId = "87da0b5f-9f65-4c5c-a660-bd254742960b" ;
        const String satelliteName = "LoftSat-1" ;

        EXPECT_TRUE(Satellite(satelliteId, satelliteName, Profile::Undefined()).isDefined()) ;

    }

    {

        EXPECT_FALSE(Satellite::Undefined().isDefined()) ;

    }

}

TEST (OpenSpaceToolkit_Simulation_Satellite, getId)
{

    using ostk::core::types::String ;

    using ostk::astro::flight::Profile ;

    using ostk::simulation::Satellite ;

    {

        const String satelliteId = "87da0b5f-9f65-4c5c-a660-bd254742960b" ;
        const String satelliteName = "LoftSat-1" ;

        const Satellite satellite = Satellite(satelliteId, satelliteName, Profile::Undefined()) ;

        EXPECT_EQ(satellite.getId(), satelliteId) ;

    }

    {

        EXPECT_ANY_THROW(Satellite::Undefined().getId()) ;

    }

}

TEST (OpenSpaceToolkit_Simulation_Satellite, getName)
{

    using ostk::core::types::String ;

    using ostk::astro::flight::Profile ;

    using ostk::simulation::Satellite ;

    {

        const String satelliteId = "87da0b5f-9f65-4c5c-a660-bd254742960b" ;
        const String satelliteName = "LoftSat-1" ;

        const Satellite satellite = Satellite(satelliteId, satelliteName, Profile::Undefined()) ;

        EXPECT_EQ(satellite.getName(), satelliteName) ;

    }

    {

        EXPECT_ANY_THROW(Satellite::Undefined().getName()) ;

    }

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////