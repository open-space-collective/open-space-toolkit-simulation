/// Apache License 2.0

#include <OpenSpaceToolkit/Simulation/Simulator.hpp>

inline void OpenSpaceToolkitSimulationPy_Simulator(pybind11::module& aModule)
{
    using namespace pybind11;

    using ostk::core::container::Array;
    using ostk::core::type::Shared;

    using ostk::physics::Environment;

    using ostk::simulation::Satellite;
    using ostk::simulation::SatelliteConfiguration;
    using ostk::simulation::Simulator;
    using ostk::simulation::SimulatorConfiguration;

    class_<Simulator, Shared<Simulator>>(aModule, "Simulator")

        .def(init<const Environment&, const Array<Shared<Satellite>>&>(), arg("environment"), arg("satellites"))

        .def("is_defined", &Simulator::isDefined)
        .def("has_satellite_with_name", &Simulator::hasSatelliteWithName, arg("name"))

        .def("access_environment", &Simulator::accessEnvironment)
        .def("access_satellite_map", &Simulator::accessSatelliteMap)
        .def("access_satellite_with_name", &Simulator::accessSatelliteWithName, arg("name"))

        .def("get_instant", &Simulator::getInstant)

        .def("set_instant", &Simulator::setInstant, arg("instant"))
        .def("step_forward", &Simulator::stepForward, arg("duration"))
        .def("add_satellite", &Simulator::addSatellite, arg("satellite"))
        .def("remove_satellite_with_name", &Simulator::removeSatelliteWithName, arg("name"))
        .def("clear_satellites", &Simulator::clearSatellites)

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
