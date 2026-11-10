/// Apache License 2.0

#include <OpenSpaceToolkit/Simulation/Component/State.hpp>

#include <Global.test.hpp>

using ostk::simulation::component::State;

TEST(OpenSpaceToolkit_Simulation_Component_State, Constructor)
{
    {
        EXPECT_EQ(State().getStatus(), State::Status::Disabled);
    }

    {
        EXPECT_EQ(State(State::Status::Disabled).getStatus(), State::Status::Disabled);
        EXPECT_EQ(State(State::Status::Idle).getStatus(), State::Status::Idle);
        EXPECT_EQ(State(State::Status::Busy).getStatus(), State::Status::Busy);
        EXPECT_EQ(State(State::Status::Error).getStatus(), State::Status::Error);
    }
}

TEST(OpenSpaceToolkit_Simulation_Component_State, EqualToOperator)
{
    {
        EXPECT_TRUE(State(State::Status::Idle) == State(State::Status::Idle));
        EXPECT_TRUE(State(State::Status::Idle, true) == State(State::Status::Idle, true));
        EXPECT_TRUE(State() == State(State::Status::Disabled, false));
    }

    {
        EXPECT_FALSE(State(State::Status::Idle) == State(State::Status::Busy));
        EXPECT_FALSE(State(State::Status::Idle, true) == State(State::Status::Idle, false));
    }
}

TEST(OpenSpaceToolkit_Simulation_Component_State, Copy)
{
    {
        const State state = {State::Status::Busy, true};

        const State copiedState(state);

        EXPECT_TRUE(copiedState == state);

        State assignedState;
        assignedState = state;

        EXPECT_TRUE(assignedState == state);

        const State* clonedStatePtr = state.clone();

        EXPECT_TRUE(*clonedStatePtr == state);

        delete clonedStatePtr;
    }
}
