/// Apache License 2.0

#include <OpenSpaceToolkit/Simulation/Component/State.hpp>

inline void OpenSpaceToolkitSimulationPy_Component_State(nanobind::module_& aModule)
{
    using namespace nanobind;

    using ostk::simulation::component::State;

    class_<State> state(
        aModule,
        "State",
        R"doc(
            Component state representing operational status.

            State tracks the operational condition of a component: disabled, idle, busy, or error.
        )doc"
    );

    state
        .def(
            init<const State::Status&>(),
            arg("status"),
            R"doc(
                Create a State instance.

                Args:
                    status (State.Status): The operational status.

                Returns:
                    State: The state instance.

                Example:
                    >>> state = State(State.Status.Idle)
            )doc"
        )

        // .def("__str__", &(shiftToString<State>))
        // .def("__repr__", &(shiftToString<State>))

        .def(
            "get_status",
            &State::getStatus,
            R"doc(
                Get the operational status.

                Returns:
                    State.Status: The current status.

                Example:
                    >>> status = state.get_status()
                    >>> status == State.Status.Idle
                    True
            )doc"
        )

        ;

    enum_<State::Status>(
        state,
        "Status",
        R"doc(
            Enumeration of component operational states.

            Defines the possible operational states a component can be in.
        )doc"
    )

        .value(
            "Disabled",
            State::Status::Disabled,
            R"doc(
                Component is disabled.
            )doc"
        )

        .value(
            "Idle",
            State::Status::Idle,
            R"doc(
                Component is idle and ready.
            )doc"
        )

        .value(
            "Busy",
            State::Status::Busy,
            R"doc(
                Component is actively processing.
            )doc"
        )

        .value(
            "Error",
            State::Status::Error,
            R"doc(
                Component is in error state.
            )doc"
        )

        ;
}
