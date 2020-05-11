////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Simulation
/// @file           OpenSpaceToolkit/Simulation/Spacecraft.test.cpp
/// @author         Lucas Brémond <lucas@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <OpenSpaceToolkit/Simulation/Spacecraft.hpp>

#include <Global.test.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST (OpenSpaceToolkit_Simulation_Spacecraft, Constructor)
{

    using ostk::simulation::Spacecraft ;

    {

        EXPECT_NO_THROW(Spacecraft spacecraft ("My Spacecraft") ;) ;

    }

}

TEST (OpenSpaceToolkit_Simulation_Spacecraft, IsDefined)
{

    using ostk::simulation::Spacecraft ;

    {

        const Spacecraft spacecraft = { "My Spacecraft" } ;

        EXPECT_TRUE(spacecraft.isDefined()) ;

    }

    {

        EXPECT_FALSE(Spacecraft::Undefined().isDefined()) ;

    }

}

TEST (OpenSpaceToolkit_Simulation_Spacecraft, GetName)
{

    using ostk::simulation::Spacecraft ;

    {

        const Spacecraft spacecraft = { "My Spacecraft" } ;

        EXPECT_EQ("My Spacecraft", spacecraft.getName()) ;

    }

    {

        EXPECT_ANY_THROW(Spacecraft::Undefined().getName()) ;

    }

}

TEST (OpenSpaceToolkit_Simulation_Spacecraft, Undefined)
{

    using ostk::simulation::Spacecraft ;

    EXPECT_FALSE(Spacecraft::Undefined().isDefined()) ;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
