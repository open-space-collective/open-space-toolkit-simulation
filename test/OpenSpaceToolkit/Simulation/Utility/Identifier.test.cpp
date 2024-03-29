/// Apache License 2.0

#include <OpenSpaceToolkit/Simulation/Utility/Identifier.hpp>

#include <Global.test.hpp>

TEST(OpenSpaceToolkit_Simulation_Utilities_Identifier, GenerateId)
{
    using ostk::core::type::String;

    using ostk::simulation::utility::generateId;

    {
        const String id_1 = generateId();
        const String id_2 = generateId();

        EXPECT_EQ(id_1.getLength(), 36);
        EXPECT_EQ(id_2.getLength(), 36);

        EXPECT_TRUE(id_1 != id_2);
    }
}
