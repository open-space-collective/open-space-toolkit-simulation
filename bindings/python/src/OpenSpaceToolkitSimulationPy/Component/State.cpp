/// Apache License 2.0

#include <OpenSpaceToolkit/Simulation/Component/State.hpp>

inline void                     OpenSpaceToolkitSimulationPy_Component_State    (           pybind11::module&       aModule                                     )
{

    using namespace pybind11 ;

    using ostk::simulation::component::State ;

    class_<State> state(aModule, "State") ;

    state.def(init<const State::Status&>())

        // .def("__str__", &(shiftToString<State>))
        // .def("__repr__", &(shiftToString<State>))

        .def("is_defined", &State::isDefined)

        .def("get_status", &State::getStatus)

        .def_static("undefined", &State::Undefined)

    ;

    enum_<State::Status>(state, "Status")

        .value("Undefined", State::Status::Undefined)
        .value("Disabled", State::Status::Disabled)
        .value("Idle", State::Status::Idle)
        .value("Busy", State::Status::Busy)
        .value("Error", State::Status::Error)

    ;

}
