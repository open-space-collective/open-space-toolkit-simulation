////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Simulation
/// @file           OpenSpaceToolkit/Simulation/Component/Geometry.test.cpp
/// @author         Remy Derollez <remy@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <OpenSpaceToolkit/Simulation/Component/Geometry.hpp>

#include <Global.test.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ...

TEST (OpenSpaceToolkit_Simulation_Component_Geometry, getExclusionObject)
{

    using ostk::core::types::String ;
    using ostk::math::geom::d3::objects::Composite ;
    using ostk::simulation::component::Geometry ;

    {
        const String name = "Exclusion Earth Other" ;
        const Geometry::Type type = Geometry::Type::Exclusion ;

        const Geometry geometry = Geometry(name, type, Composite::Undefined()) ;

        EXPECT_EQ("Earth", geometry.getExclusionObject()) ;

    }

    {
        const String name = "Moon Exclusion" ;
        const Geometry::Type type = Geometry::Type::Exclusion ;

        const Geometry geometry = Geometry(name, type, Composite::Undefined()) ;

        EXPECT_EQ("Moon", geometry.getExclusionObject()) ;

    }

    {
        const String name = "Exclusion Sun Full" ;
        const Geometry::Type type = Geometry::Type::Exclusion ;

        const Geometry geometry = Geometry(name, type, Composite::Undefined()) ;

        EXPECT_EQ("Sun", geometry.getExclusionObject()) ;

    }

    {
        const String name = "FOV Earth" ;
        const Geometry::Type type = Geometry::Type::Sensing ;

        const Geometry geometry = Geometry(name, type, Composite::Undefined()) ;

        EXPECT_ANY_THROW(geometry.getExclusionObject()) ;

    }

    {
        const String name = "Exclusion Pluto" ;
        const Geometry::Type type = Geometry::Type::Exclusion ;

        const Geometry geometry = Geometry(name, type, Composite::Undefined()) ;

        EXPECT_ANY_THROW(geometry.getExclusionObject()) ;

    }

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////