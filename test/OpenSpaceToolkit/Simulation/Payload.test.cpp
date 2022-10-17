////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Simulation
/// @file           OpenSpaceToolkit/Simulation/Payload.test.cpp
/// @author         Remy Derollez <remy@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Objects/Point.hpp>
#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Objects/Pyramid.hpp>
#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Objects/Polygon.hpp>
#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Transformations/Rotations/RotationMatrix.hpp>

#include <OpenSpaceToolkit/Simulation/Satellite.hpp>
#include <OpenSpaceToolkit/Simulation/Payload.hpp>
#include <OpenSpaceToolkit/Simulation/Component.hpp>
#include <OpenSpaceToolkit/Simulation/Component/State.hpp>
#include <OpenSpaceToolkit/Simulation/Component/Geometry.hpp>

#include <Global.test.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST (OpenSpaceToolkit_Simulation_Payload, Constructor)
{

    using ostk::core::ctnr::Array ;
    using ostk::core::types::String ;

    using ostk::math::geom::d3::objects::Point ;
    using ostk::math::geom::d3::trf::rot::RotationMatrix ;
    using ostk::math::geom::d3::objects::Polygon ;
    using ostk::math::geom::d3::objects::Pyramid ;

    using ostk::simulation::Payload ;
    using ostk::simulation::Satellite ;
    using ostk::simulation::component::Geometry ;

    {

        const String payloadId = "234jbb5f-9f65-4c5c-a660-bd2547sfsifs" ;
        const String payloadName = "Payload A" ;
        const String payloadModel = "Model 345-e" ;
        const Array<Geometry> geometryArray =
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
        } ;

        EXPECT_NO_THROW(const Payload payload = Payload(payloadId, payloadName, payloadModel, geometryArray) ;) ;

    }

}

TEST (OpenSpaceToolkit_Simulation_Payload, IsDefined)
{

    using ostk::core::ctnr::Array ;
    using ostk::core::types::String ;

    using ostk::math::geom::d3::objects::Point ;
    using ostk::math::geom::d3::trf::rot::RotationMatrix ;
    using ostk::math::geom::d3::objects::Polygon ;
    using ostk::math::geom::d3::objects::Pyramid ;

    using ostk::simulation::Payload ;
    using ostk::simulation::Satellite ;
    using ostk::simulation::component::Geometry ;

    {

        const Payload payload =
        Payload
        {
            "234jbb5f-9f65-4c5c-a660-bd2547sfsifs",
            "Payload A",
            "Model 345-e",
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
        } ;

        EXPECT_TRUE(payload.isDefined()) ;

    }

}

TEST (OpenSpaceToolkit_Simulation_Payload, getId)
{

    using ostk::core::ctnr::Array ;
    using ostk::core::types::String ;

    using ostk::math::geom::d3::objects::Point ;
    using ostk::math::geom::d3::trf::rot::RotationMatrix ;
    using ostk::math::geom::d3::objects::Polygon ;
    using ostk::math::geom::d3::objects::Pyramid ;

    using ostk::simulation::Payload ;
    using ostk::simulation::Satellite ;
    using ostk::simulation::component::Geometry ;

    {

        const Payload payload =
        Payload
        {
            "234jbb5f-9f65-4c5c-a660-bd2547sfsifs",
            "Payload A",
            "Model 345-e",
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
        } ;

        EXPECT_EQ(payload.getId(), "234jbb5f-9f65-4c5c-a660-bd2547sfsifs") ;

    }

}

TEST (OpenSpaceToolkit_Simulation_Payload, getName)
{

    using ostk::core::ctnr::Array ;
    using ostk::core::types::String ;

    using ostk::math::geom::d3::objects::Point ;
    using ostk::math::geom::d3::trf::rot::RotationMatrix ;
    using ostk::math::geom::d3::objects::Polygon ;
    using ostk::math::geom::d3::objects::Pyramid ;

    using ostk::simulation::Payload ;
    using ostk::simulation::Satellite ;
    using ostk::simulation::component::Geometry ;

    {

        const Payload payload =
        Payload
        {
            "234jbb5f-9f65-4c5c-a660-bd2547sfsifs",
            "Payload A",
            "Model 345-e",
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
        } ;

        EXPECT_EQ(payload.getName(), "Payload A") ;

    }

}

TEST (OpenSpaceToolkit_Simulation_Payload, getModel)
{

    using ostk::core::ctnr::Array ;
    using ostk::core::types::String ;

    using ostk::math::geom::d3::objects::Point ;
    using ostk::math::geom::d3::trf::rot::RotationMatrix ;
    using ostk::math::geom::d3::objects::Polygon ;
    using ostk::math::geom::d3::objects::Pyramid ;

    using ostk::simulation::Payload ;
    using ostk::simulation::Satellite ;
    using ostk::simulation::component::Geometry ;

    {

        const Payload payload =
        Payload
        {
            "234jbb5f-9f65-4c5c-a660-bd2547sfsifs",
            "Payload A",
            "Model 345-e",
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
        } ;

        EXPECT_EQ(payload.getModel(), "Model 345-e") ;

    }

}

TEST (OpenSpaceToolkit_Simulation_Payload, Undefined)
{
    using ostk::core::types::String ;
    using ostk::simulation::Payload ;

    {

        const Payload payload = Payload::Undefined() ;

        EXPECT_EQ(payload.isDefined(), false) ;

    }

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////