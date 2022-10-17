////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Simulation
/// @file           OpenSpaceToolkit/Simulation/Component.test.cpp
/// @author         Remy Derollez <remy@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <OpenSpaceToolkit/Simulation/Component/State.hpp>
#include <OpenSpaceToolkit/Simulation/Component/Geometry.hpp>
#include <OpenSpaceToolkit/Simulation/Component.hpp>

#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Objects/Point.hpp>
#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Objects/Pyramid.hpp>
#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Objects/Polygon.hpp>
#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Transformations/Rotations/RotationMatrix.hpp>

#include <Global.test.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST (OpenSpaceToolkit_Simulation_Component, Constructor)
{

    using ostk::core::types::String ;
    using ostk::core::ctnr::Array ;

    using ostk::simulation::component::State ;
    using ostk::simulation::Component ;

    // Construct using name and type

    {

        const String name = "OBC" ;
        const Component::Type type = Component::Type::Other ;

        EXPECT_NO_THROW(const Component component = Component(name, type) ;) ;

    }

    // Construct using name, type and State

    {

        const String name = "Camera" ;
        const Component::Type type = Component::Type::Sensor ;
        const State state = State::Undefined() ;

        EXPECT_NO_THROW(const Component component = Component(name, type, state) ;) ;

    }

    // Construct using name, type, State and tags

    {

        const String name = "Reaction Wheel" ;
        const Component::Type type = Component::Type::Controller ;
        const State state = State(State::Status::Busy) ;
        const Array<String> tags = { "Bus", "ADCS" } ;

        EXPECT_NO_THROW(const Component component = Component(name, type, state, tags) ;) ;

    }

    // Construct using name, type, State, tags, geometries and rotation matrix

    {

        using ostk::math::geom::d3::objects::Point ;
        using ostk::math::geom::d3::trf::rot::RotationMatrix ;
        using ostk::math::geom::d3::objects::Polygon ;
        using ostk::math::geom::d3::objects::Pyramid ;
        using ostk::simulation::component::Geometry ;

        const String name = "Camera" ;
        const Component::Type type = Component::Type::Sensor ;
        const Array<String> tags = { "Hub", "Customer" } ;
        const State state = State(State::Status::Busy) ;
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

    using ostk::simulation::Component ;

    {

        EXPECT_TRUE(Component("OBC", Component::Type::Other).isDefined()) ;

    }

    {

        EXPECT_FALSE(Component::Undefined().isDefined()) ;

    }

}

TEST (OpenSpaceToolkit_Simulation_Component, getId)
{

    using ostk::simulation::Component ;

    {

        const Component component = Component("87da0b5f-9f65-4c5c-a660-bd254742960b", "STT") ;

        EXPECT_EQ(component.getId(), "87da0b5f-9f65-4c5c-a660-bd254742960b") ;

    }

    {

        EXPECT_ANY_THROW(Component::Undefined().getId()) ;

    }

}

TEST (OpenSpaceToolkit_Simulation_Component, getName)
{

    using ostk::simulation::Component ;

    {

        const Component component = Component("Camera") ;

        EXPECT_EQ(component.getName(), "Camera") ;

    }

    {

        EXPECT_ANY_THROW(Component::Undefined().getName()) ;

    }

}

TEST (OpenSpaceToolkit_Simulation_Component, getType)
{

    using ostk::simulation::Component ;

    {

        const Component component = Component("Camera", Component::Type::Sensor) ;

        EXPECT_EQ(component.getType(), Component::Type::Sensor) ;

    }

    {

        EXPECT_ANY_THROW(Component::Undefined().getType()) ;

    }

}

TEST (OpenSpaceToolkit_Simulation_Component, getTags)
{

    using ostk::core::types::String ;
    using ostk::core::ctnr::Array ;

    using ostk::simulation::Component ;
    using ostk::simulation::component::State ;

    {

        const String name = "Camera" ;
        const Component::Type type = Component::Type::Sensor ;
        const State state = State(State::Status::Busy) ;
        const Array<String> tags = { "Hub", "Customer" } ;

        const Component component = Component(name, type, state, tags) ;

        EXPECT_EQ(component.getTags(), tags) ;

    }

    {

        EXPECT_ANY_THROW(Component::Undefined().getTags()) ;

    }

}

TEST (OpenSpaceToolkit_Simulation_Component, getState)
{

    using ostk::core::types::String ;
    using ostk::core::ctnr::Array ;

    using ostk::simulation::Component ;
    using ostk::simulation::component::State ;

    {

        const String name = "Camera" ;
        const Component::Type type = Component::Type::Sensor ;
        const State state = State(State::Status::Busy) ;
        const Array<String> tags = { "Hub", "Customer" } ;

        const Component component = Component(name, type, state, tags) ;

        EXPECT_EQ(component.getState(), state) ;

    }

    {

        EXPECT_ANY_THROW(Component::Undefined().getState()) ;

    }

}

TEST (OpenSpaceToolkit_Simulation_Component, getGeometries)
{

    using ostk::core::types::String ;
    using ostk::core::ctnr::Array ;

    using ostk::math::geom::d3::objects::Point ;
    using ostk::math::geom::d3::objects::Polygon ;
    using ostk::math::geom::d3::objects::Pyramid ;

    using ostk::simulation::Component ;
    using ostk::simulation::component::State ;
    using ostk::simulation::component::Geometry ;

    {

        const String name = "Camera" ;
        const Component::Type type = Component::Type::Sensor ;
        const State state = State(State::Status::Busy) ;
        const Array<String> tags = { "Hub", "Customer" } ;
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

        const Component component = Component(name, type, state, tags, geometries) ;

        EXPECT_EQ(component.getGeometries(), geometries) ;

    }

    {

        EXPECT_ANY_THROW(Component::Undefined().getGeometries()) ;

    }

}

TEST (OpenSpaceToolkit_Simulation_Component, getRotationMatrix)
{

    using ostk::core::types::String ;
    using ostk::core::ctnr::Array ;

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
        const Array<String> tags = { "Hub", "Customer" } ;
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

    {

        EXPECT_ANY_THROW(Component::Undefined().getRotationMatrix()) ;

    }

}

TEST (OpenSpaceToolkit_Simulation_Component, Undefined)
{

    using ostk::simulation::Component ;

    {

        EXPECT_FALSE(Component::Undefined().isDefined()) ;

    }

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////