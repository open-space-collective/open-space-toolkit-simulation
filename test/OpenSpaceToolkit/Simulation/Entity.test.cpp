/// Apache License 2.0

#include <sstream>

#include <OpenSpaceToolkit/Simulation/Entity.hpp>

#include <OpenSpaceToolkit/Core/Error.hpp>

#include <Global.test.hpp>

using ostk::simulation::Entity;

TEST(OpenSpaceToolkit_Simulation_Entity, Constructor)
{
    {
        const Entity entity = {"entity-id", "My Entity"};

        EXPECT_EQ(entity.getId(), "entity-id");
        EXPECT_EQ(entity.getName(), "My Entity");
    }

    {
        const Entity entity = Entity("My Entity");

        EXPECT_FALSE(entity.getId().isEmpty());
        EXPECT_EQ(entity.getName(), "My Entity");
    }

    {
        EXPECT_THROW(Entity("", "My Entity"), ostk::core::error::runtime::Undefined);
        EXPECT_THROW(Entity("entity-id", ""), ostk::core::error::runtime::Undefined);
        EXPECT_THROW(Entity("", ""), ostk::core::error::runtime::Undefined);
        EXPECT_THROW(Entity(""), ostk::core::error::runtime::Undefined);
    }
}

TEST(OpenSpaceToolkit_Simulation_Entity, Print)
{
    {
        std::ostringstream stream;

        Entity("entity-id", "My Entity").print(stream, true);

        EXPECT_NE(stream.str().find("entity-id"), std::string::npos);
        EXPECT_NE(stream.str().find("My Entity"), std::string::npos);
    }
}
