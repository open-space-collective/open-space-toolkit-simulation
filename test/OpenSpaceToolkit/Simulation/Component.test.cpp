////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Simulation
/// @file           OpenSpaceToolkit/Simulation/Component.test.cpp
/// @author         Remy Derollez <remy@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Objects/Point.hpp>
#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Objects/Pyramid.hpp>
#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Objects/Polygon.hpp>
#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Transformations/Rotations/RotationMatrix.hpp>

#include <OpenSpaceToolkit/Simulation/Component/State.hpp>
#include <OpenSpaceToolkit/Simulation/Component/Geometry.hpp>
#include <OpenSpaceToolkit/Simulation/Component.hpp>

#include <Global.test.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST (OpenSpaceToolkit_Simulation_Component, Constructor)
{

    using ostk::core::ctnr::Array ;
    using ostk::core::types::String ;

    using ostk::simulation::component::State ;
    using ostk::simulation::Component ;

    {

        // Construct using aName and aType
        const String name = "OBC" ;
        const Component::Type type = Component::Type::Other ;

        EXPECT_NO_THROW(const Component component = Component(name, type) ;) ;

    }

    {

        // Construct using aName and aType and aState
        const String name = "Camera" ;
        const Component::Type type = Component::Type::Sensor ;
        const State state = State(State::Status::Undefined) ;

        EXPECT_NO_THROW(const Component component = Component(name, type, state) ;) ;

    }

    {

        // Construct using aName and aType and anArrayOfTags
        const String name = "Camera" ;
        const Component::Type type = Component::Type::Sensor ;
        const Array<String> tags = { "Bus", "ADCS" } ;

        EXPECT_NO_THROW(const Component component = Component(name, type, tags) ;) ;

    }

    {

        // Construct using aName and aType and aState and anArrayOfTags
        const String name = "Reaction Wheel" ;
        const Component::Type type = Component::Type::Controller ;
        const State state = State(State::Status::Busy) ;
        const Array<String> tags = { "Bus", "ADCS" } ;

        EXPECT_NO_THROW(const Component component = Component(name, type, state, tags) ;) ;

    }

    {

        using ostk::math::geom::d3::objects::Point ;
        using ostk::math::geom::d3::trf::rot::RotationMatrix ;
        using ostk::math::geom::d3::objects::Polygon ;
        using ostk::math::geom::d3::objects::Pyramid ;
        using ostk::simulation::component::Geometry ;

        // Construct using aName and aType and aState and anArrayOfTags and anArrayOfGeometries and aRotationMatrix
        const String name = "Camera" ;
        const Component::Type type = Component::Type::Sensor ;
        const State state = State(State::Status::Busy) ;
        const Array<String> tags = { "PLHUB", "Customer" } ;
        const Array<Geometry> geometries =
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
        const RotationMatrix rotationMatrix = RotationMatrix::Unit() ;

        EXPECT_NO_THROW(const Component component = Component(name, type, state, tags, geometries, rotationMatrix) ;) ;

    }

}

TEST (OpenSpaceToolkit_Simulation_Component, CopyConstructor)
{

    using ostk::core::types::String ;

    using ostk::simulation::component::State ;
    using ostk::simulation::Component ;

   {

        const String name = "OBC" ;
        const Component::Type type = Component::Type::Other ;

        const Component component = Component(name, type) ;

        EXPECT_TRUE(component.isDefined()) ;

        EXPECT_EQ("OBC", component.getName()) ;
        EXPECT_EQ(Component::Type::Other, component.getType()) ;

        const Component componentCopy(component) ;

        EXPECT_TRUE(componentCopy.isDefined()) ;

        EXPECT_EQ("OBC", componentCopy.getName()) ;
        EXPECT_EQ(Component::Type::Other, componentCopy.getType()) ;


    }

}

TEST (OpenSpaceToolkit_Simulation_Component, IsDefined)
{

    using ostk::core::ctnr::Array ;
    using ostk::core::types::String ;

    using ostk::simulation::component::State ;
    using ostk::simulation::Component ;

    {

        const String name = "OBC" ;
        const Component::Type type = Component::Type::Other ;
        const Component component = Component(name, type) ;

        EXPECT_TRUE(component.isDefined()) ;

    }

}

TEST (OpenSpaceToolkit_Simulation_Component, getName)
{

    using ostk::core::ctnr::Array ;
    using ostk::core::types::String ;

    using ostk::math::geom::d3::objects::Point ;
    using ostk::math::geom::d3::objects::Polygon ;
    using ostk::math::geom::d3::objects::Pyramid ;
    using ostk::math::geom::d3::trf::rot::RotationMatrix ;

    using ostk::simulation::Component ;
    using ostk::simulation::component::State ;
    using ostk::simulation::component::Geometry ;

    {

        const String name = "Camera" ;
        const Component::Type type = Component::Type::Sensor ;
        const State state = State(State::Status::Busy) ;
        const Array<String> tags = { "PLHUB", "Customer" } ;
        const Array<Geometry> geometries =
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
        const RotationMatrix rotationMatrix = RotationMatrix::Unit() ;

        const Component component = Component(name, type, state, tags, geometries, rotationMatrix) ;

        EXPECT_EQ(component.getName(), "Camera") ;

    }

}

TEST (OpenSpaceToolkit_Simulation_Component, getType)
{

    using ostk::core::ctnr::Array ;
    using ostk::core::types::String ;

    using ostk::math::geom::d3::objects::Point ;
    using ostk::math::geom::d3::objects::Polygon ;
    using ostk::math::geom::d3::objects::Pyramid ;
    using ostk::math::geom::d3::trf::rot::RotationMatrix ;

    using ostk::simulation::Component ;
    using ostk::simulation::component::State ;
    using ostk::simulation::component::Geometry ;

    {

        const String name = "Camera" ;
        const Component::Type type = Component::Type::Sensor ;
        const State state = State(State::Status::Busy) ;
        const Array<String> tags = { "PLHUB", "Customer" } ;
        const Array<Geometry> geometries =
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
        const RotationMatrix rotationMatrix = RotationMatrix::Unit() ;

        const Component component = Component(name, type, state, tags, geometries, rotationMatrix) ;

        EXPECT_EQ(component.getType(), Component::Type::Sensor) ;

    }

}

TEST (OpenSpaceToolkit_Simulation_Component, getTags)
{

    using ostk::core::ctnr::Array ;
    using ostk::core::types::String ;

    using ostk::math::geom::d3::objects::Point ;
    using ostk::math::geom::d3::objects::Polygon ;
    using ostk::math::geom::d3::objects::Pyramid ;
    using ostk::math::geom::d3::trf::rot::RotationMatrix ;

    using ostk::simulation::Component ;
    using ostk::simulation::component::State ;
    using ostk::simulation::component::Geometry ;

    {

        const String name = "Camera" ;
        const Component::Type type = Component::Type::Sensor ;
        const State state = State(State::Status::Busy) ;
        const Array<String> tags = { "PLHUB", "Customer" } ;
        const Array<Geometry> geometries =
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
        const RotationMatrix rotationMatrix = RotationMatrix::Unit() ;

        const Component component = Component(name, type, state, tags, geometries, rotationMatrix) ;

        EXPECT_EQ(component.getTags(), tags) ;

    }

}

// TEST (OpenSpaceToolkit_Simulation_Component, getState)
// {

//     using ostk::core::ctnr::Array ;
//     using ostk::core::types::String ;

//     using ostk::math::geom::d3::objects::Point ;
//     using ostk::math::geom::d3::objects::Polygon ;
//     using ostk::math::geom::d3::objects::Pyramid ;
//     using ostk::math::geom::d3::trf::rot::RotationMatrix ;

//     using ostk::simulation::Component ;
//     using ostk::simulation::component::State ;
//     using ostk::simulation::component::Geometry ;

//     {

//         const String name = "Camera" ;
//         const Component::Type type = Component::Type::Sensor ;
//         const State state = State(State::Status::Busy) ;
//         const Array<String> tags = { "PLHUB", "Customer" } ;
//         const Array<Geometry> geometries =
//         {
//             Geometry
//             {
//                 "FOV",
//                 Geometry::Type::Sensing,
//                 Pyramid
//                 {
//                     Polygon { { { { -0.1, -1.0 }, { +0.1, -1.0 }, { +0.1, +1.0 }, { -0.1, +1.0 } } }, Point { 0.0, 0.0, 1.0 }, { 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 } },
//                     Point { 0.0, 0.0, 0.0 }
//                 }
//             }
//         } ;
//         const RotationMatrix rotationMatrix = RotationMatrix::Unit() ;

//         const Component component = Component(name, type, state, tags, geometries, rotationMatrix) ;

//         EXPECT_EQ(component.getState(), state) ;

//     }

// }

// TEST (OpenSpaceToolkit_Simulation_Component, getGeometries)
// {

//     using ostk::core::ctnr::Array ;
//     using ostk::core::types::String ;

//     using ostk::math::geom::d3::objects::Point ;
//     using ostk::math::geom::d3::objects::Polygon ;
//     using ostk::math::geom::d3::objects::Pyramid ;
//     using ostk::math::geom::d3::trf::rot::RotationMatrix ;

//     using ostk::simulation::Component ;
//     using ostk::simulation::component::State ;
//     using ostk::simulation::component::Geometry ;

//     {

//         const String name = "Camera" ;
//         const Component::Type type = Component::Type::Sensor ;
//         const State state = State(State::Status::Busy) ;
//         const Array<String> tags = { "PLHUB", "Customer" } ;
//         const Array<Geometry> geometries =
//         {
//             Geometry
//             {
//                 "FOV",
//                 Geometry::Type::Sensing,
//                 Pyramid
//                 {
//                     Polygon { { { { -0.1, -1.0 }, { +0.1, -1.0 }, { +0.1, +1.0 }, { -0.1, +1.0 } } }, Point { 0.0, 0.0, 1.0 }, { 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 } },
//                     Point { 0.0, 0.0, 0.0 }
//                 }
//             }
//         } ;
//         const RotationMatrix rotationMatrix = RotationMatrix::Unit() ;

//         const Component component = Component(name, type, state, tags, geometries, rotationMatrix) ;

//         EXPECT_EQ(component.getGeometries(), geometries) ;

//     }

// }

TEST (OpenSpaceToolkit_Simulation_Component, getRotationMatrix)
{

    using ostk::core::ctnr::Array ;
    using ostk::core::types::String ;

    using ostk::math::geom::d3::objects::Point ;
    using ostk::math::geom::d3::objects::Polygon ;
    using ostk::math::geom::d3::objects::Pyramid ;
    using ostk::math::geom::d3::trf::rot::RotationMatrix ;

    using ostk::simulation::Component ;
    using ostk::simulation::component::State ;
    using ostk::simulation::component::Geometry ;

    {

        const String name = "Camera" ;
        const Component::Type type = Component::Type::Sensor ;
        const State state = State(State::Status::Busy) ;
        const Array<String> tags = { "PLHUB", "Customer" } ;
        const Array<Geometry> geometries =
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
        const RotationMatrix rotationMatrix = RotationMatrix::Unit() ;

        const Component component = Component(name, type, state, tags, geometries, rotationMatrix) ;

        EXPECT_EQ(component.getRotationMatrix(), rotationMatrix) ;

    }

}

TEST (OpenSpaceToolkit_Simulation_Component, Undefined)
{
    using ostk::core::types::String ;
    using ostk::simulation::Component ;

    {

        const Component component = Component::Undefined() ;

        EXPECT_EQ(component.isDefined(), false) ;

        EXPECT_ANY_THROW(component.getName()) ;
        EXPECT_ANY_THROW(component.getType()) ;

    }

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////