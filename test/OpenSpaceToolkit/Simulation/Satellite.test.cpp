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

using ostk::simulation::Satellite ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class OpenSpaceToolkit_Simulation_Satellite : public ::testing::Test
{

    protected:

        void                    SetUp                                       ( ) override
        {

            using ostk::core::types::Shared ;
            using ostk::core::types::String ;
            using ostk::core::ctnr::Array ;

            using ostk::astro::flight::Profile ;

            using ostk::simulation::Component ;
            using ostk::simulation::component::Geometry ;

            this->satellite = Satellite
            (
                "87da0b5f-9f65-4c5c-a660-bd254742960b",
                "LoftSat-1",
                {"a", "b"},
                Array<Shared<Geometry>>::Empty(),
                Array<Shared<Component>>::Empty(),
                nullptr,
                std::make_shared<Profile>(Profile::Undefined()),
                nullptr
            ) ;

        }

        Satellite satellite = Satellite::Undefined() ;

} ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_F (OpenSpaceToolkit_Simulation_Satellite, IsDefined)
{

    {

        EXPECT_TRUE(satellite.isDefined()) ;

    }

    {

        EXPECT_FALSE(Satellite::Undefined().isDefined()) ;

    }

}

TEST_F (OpenSpaceToolkit_Simulation_Satellite, getId)
{

    {

        EXPECT_EQ(satellite.getId(), "87da0b5f-9f65-4c5c-a660-bd254742960b") ;

    }

    {

        EXPECT_ANY_THROW(Satellite::Undefined().getId()) ;

    }

}

TEST_F (OpenSpaceToolkit_Simulation_Satellite, getName)
{

    {

        EXPECT_EQ(satellite.getName(), "LoftSat-1") ;

    }

    {

        EXPECT_ANY_THROW(Satellite::Undefined().getName()) ;

    }

}

TEST_F (OpenSpaceToolkit_Simulation_Satellite, Undefined)
{

    {

        EXPECT_FALSE(Satellite::Undefined().isDefined()) ;

    }

}

// TEST_F (OpenSpaceToolkit_Simulation_Satellite, Configure)
// TEST_F (OpenSpaceToolkit_Simulation_Satellite, GenerateFrame)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
