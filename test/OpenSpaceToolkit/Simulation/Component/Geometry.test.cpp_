////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Simulation
/// @file           OpenSpaceToolkit/Simulation/Component/Geometry.test.cpp
/// @author         Remy Derollez <remy@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <OpenSpaceToolkit/Simulation/Component/Geometry.hpp>
#include <OpenSpaceToolkit/Simulation/Component.hpp>


#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Objects/Polygon.hpp>
#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Objects/Pyramid.hpp>
#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Objects/Point.hpp>

#include <Global.test.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST (OpenSpaceToolkit_Simulation_Component_Geometry, Constructor)
{

    using ostk::core::types::Shared ;

    using ostk::math::geom::d3::objects::Point ;
    using ostk::math::geom::d3::objects::Polygon ;
    using ostk::math::geom::d3::objects::Pyramid ;

    using ostk::physics::coord::Frame ;

    using ostk::simulation::Component ;
    using ostk::simulation::component::Geometry ;

    {

        const Shared<const Frame> frameSPtr = nullptr ;
        const Shared<const Component> componentSPtr = nullptr ;

        const Geometry geometry =
        {
            "FOV",
            Pyramid
            {
                Polygon
                {
                    { { { -0.1, -1.0 }, { +0.1, -1.0 }, { +0.1, +1.0 }, { -0.1, +1.0 } } },
                    Point { 0.0, 0.0, 1.0 },
                    { 1.0, 0.0, 0.0 },
                    { 0.0, 1.0, 0.0 }
                },
                Point { 0.0, 0.0, 0.0 }
            },
            frameSPtr,
            componentSPtr
        } ;

    }

}

TEST (OpenSpaceToolkit_Simulation_Component_Geometry, IsDefined)
{

    GTEST_SKIP() ;

}

TEST (OpenSpaceToolkit_Simulation_Component_Geometry, AccessComponent)
{

    GTEST_SKIP() ;

}

TEST (OpenSpaceToolkit_Simulation_Component_Geometry, GetName)
{

    GTEST_SKIP() ;

}

TEST (OpenSpaceToolkit_Simulation_Component_Geometry, Intersects)
{

    GTEST_SKIP() ;

}

TEST (OpenSpaceToolkit_Simulation_Component_Geometry, Contains)
{

    GTEST_SKIP() ;

}

TEST (OpenSpaceToolkit_Simulation_Component_Geometry, IntersectionWith)
{

    GTEST_SKIP() ;

}

TEST (OpenSpaceToolkit_Simulation_Component_Geometry, Undefined)
{

    GTEST_SKIP() ;

}

TEST (OpenSpaceToolkit_Simulation_Component_Geometry, Configure)
{

    GTEST_SKIP() ;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
