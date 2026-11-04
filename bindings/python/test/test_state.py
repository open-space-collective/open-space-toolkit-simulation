# Apache License 2.0

from ostk.simulation.component import State


class TestState:
    def test_constructor(self):
        state: State = State(State.Status.Idle)

        assert isinstance(state, State)
        assert state.get_status() == State.Status.Idle

    def test_has_no_undefined(self):
        assert not hasattr(State, "undefined")
        assert not hasattr(State, "is_defined")
        assert not hasattr(State.Status, "Undefined")
