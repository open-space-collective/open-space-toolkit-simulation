/// Apache License 2.0

#include <OpenSpaceToolkit/Simulation/Simulator.hpp>

inline void OpenSpaceToolkitSimulationPy_Simulator(pybind11::module& aModule)
{
    using namespace pybind11;

    using ostk::core::ctnr::Array;
    using ostk::core::types::Shared;

    using ostk::physics::Environment;

    using ostk::simulation::Simulator;
    using ostk::simulation::SimulatorConfiguration;
    using ostk::simulation::Satellite;
    using ostk::simulation::SatelliteConfiguration;

    class_<Simulator, Shared<Simulator>>(aModule, "Simulator")

        .def(init<const Environment&, const Array<Shared<Satellite>>&>(), arg("environment"), arg("satellites"))

        .def("is_defined", &Simulator::isDefined)
        .def("has_satellite_with_name", &Simulator::hasSatelliteWithName, arg("name"))

        .def("access_environment", &Simulator::accessEnvironment)
        .def("access_satellite_with_name", &Simulator::accessSatelliteWithName, arg("name"))

        .def("get_instant", &Simulator::getInstant)

        .def("set_instant", &Simulator::setInstant, arg("instant"))
        .def("step_forward", &Simulator::stepForward, arg("duration"))
        .def("add_satellite", &Simulator::addSatellite, arg("satellite"))

        .def_static("undefined", &Simulator::Undefined)
        .def_static("configure", &Simulator::Configure, arg("configuration"))

        ;

    class_<SimulatorConfiguration>(aModule, "SimulatorConfiguration")

        .def(
            init<const Environment&, const Array<SatelliteConfiguration>&>(),
            arg("environment"),
            arg("satellites") = DEFAULT_SATELLITES
        )

        ;
}
