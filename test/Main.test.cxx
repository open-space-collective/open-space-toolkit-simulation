////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Simulation
/// @file           OpenSpaceToolkit/Simulation/Main.test.cxx
/// @author         Lucas Brémond <lucas@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Setup.test.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int                             main                                        (           int                         argc,
                                                                                        char**                      argv                                )
{

    ::testing::InitGoogleTest(&argc, argv) ;

    ::testing::AddGlobalTestEnvironment(new ostk::simulation::test::Environment(argc >= 2 ? argv[1] : "")) ;

    return RUN_ALL_TESTS() ;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////