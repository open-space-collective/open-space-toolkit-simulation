////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Simulation
/// @file           OpenSpaceToolkit/Simulation/Satellite.test.cpp
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

TEST (OpenSpaceToolkit_Simulation_Satellite, Constructor)
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

        // Construct using anId, aName and aPayloadArray
        const String satelliteId = "87da0b5f-9f65-4c5c-a660-bd254742960b" ;
        const String satelliteName = "Loft Sat-1" ;
        const Array<Payload> payloadArray =
        {
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
            }
        } ;

        EXPECT_NO_THROW(const Satellite satellite = Satellite(satelliteId, satelliteName, payloadArray) ;) ;

    }

    {

        // Construct using anId and aName
        const String satelliteId = "87da0b5f-9f65-4c5c-a660-bd254742960b" ;
        const String satelliteName = "Loft Sat-1" ;

        EXPECT_NO_THROW(const Satellite satellite = Satellite(satelliteId, satelliteName) ;) ;

    }

}

TEST (OpenSpaceToolkit_Simulation_Satellite, IsDefined)
{

    using ostk::core::types::String ;

    using ostk::simulation::Satellite ;

    {

        const String satelliteId = "87da0b5f-9f65-4c5c-a660-bd254742960b" ;
        const String satelliteName = "Loft Sat-1" ;
        const Satellite satellite = Satellite(satelliteId, satelliteName) ;

        EXPECT_TRUE(satellite.isDefined()) ;

    }

}

TEST (OpenSpaceToolkit_Simulation_Satellite, getId)
{

    using ostk::core::types::String ;

    using ostk::simulation::Satellite ;

    {

        const String satelliteId = "87da0b5f-9f65-4c5c-a660-bd254742960b" ;
        const String satelliteName = "Loft Sat-1" ;
        const Satellite satellite = Satellite(satelliteId, satelliteName) ;

        EXPECT_EQ(satellite.getId(), satelliteId) ;

    }

}

TEST (OpenSpaceToolkit_Simulation_Satellite, getName)
{

    using ostk::core::types::String ;

    using ostk::simulation::Satellite ;

    {

        const String satelliteId = "87da0b5f-9f65-4c5c-a660-bd254742960b" ;
        const String satelliteName = "Loft Sat-1" ;
        const Satellite satellite = Satellite(satelliteId, satelliteName) ;

        EXPECT_EQ(satellite.getName(), satelliteName) ;

    }

}

TEST (OpenSpaceToolkit_Simulation_Satellite, getPayloads)
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

        const String satelliteId = "87da0b5f-9f65-4c5c-a660-bd254742960b" ;
        const String satelliteName = "Loft Sat-1" ;
        const Satellite satellite = Satellite(satelliteId, satelliteName) ;

        EXPECT_EQ(0, satellite.getPayloads().getSize()) ;

    }

    {

        const String satelliteId = "87da0b5f-9f65-4c5c-a660-bd254742960b" ;
        const String satelliteName = "Loft Sat-1" ;
        const Array<Payload> payloadArray =
        {
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
            }
        } ;

        const Satellite satellite = Satellite(satelliteId, satelliteName, payloadArray) ;

        EXPECT_EQ(1, satellite.getPayloads().getSize()) ;

    }

}

TEST (OpenSpaceToolkit_Simulation_Satellite, getPayloadWithId)
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

        const String satelliteId = "87da0b5f-9f65-4c5c-a660-bd254742960b" ;
        const String satelliteName = "Loft Sat-1" ;
        const Array<Payload> payloadArray =
        {
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
            }
        } ;

        const Satellite satellite = Satellite(satelliteId, satelliteName, payloadArray) ;

        const Payload payload = satellite.getPayloadWithId("234jbb5f-9f65-4c5c-a660-bd2547sfsifs") ;

        EXPECT_EQ(payload.getId(), "234jbb5f-9f65-4c5c-a660-bd2547sfsifs") ;
        EXPECT_EQ(payload.getName(), "Payload A") ;
        EXPECT_EQ(payload.getModel(), "Model 345-e") ;

    }

    {

        const String satelliteId = "87da0b5f-9f65-4c5c-a660-bd254742960b" ;
        const String satelliteName = "Loft Sat-1" ;
        const Array<Payload> payloadArray =
        {
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
            }
        } ;

        const Satellite satellite = Satellite(satelliteId, satelliteName, payloadArray) ;

        EXPECT_ANY_THROW(satellite.getPayloadWithId("9f65-4c5c-a660-bd2547sfsifs")) ;

    }

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////