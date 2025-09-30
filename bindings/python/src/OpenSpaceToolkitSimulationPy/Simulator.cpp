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

    class_<Simulator, Shared<Simulator>>(
        aModule,
        "Simulator",
        R"doc(
            Main simulation orchestrator managing satellites and environment.

            The Simulator coordinates the overall simulation, managing a collection of satellites,
            the physics environment, and the simulation time. It provides methods to step forward
            in time and manage satellite lifecycles.
        )doc"
    )

        .def(
            init<const Environment&, const Array<Shared<Satellite>>&>(),
            arg("environment"),
            arg("satellites"),
            R"doc(
                Create a Simulator instance.

                Args:
                    environment (Environment): The physics environment (celestial bodies, gravity, etc.).
                    satellites (list[Satellite]): Array of satellite objects to simulate.

                Returns:
                    Simulator: The simulator instance.

                Example:
                    >>> simulator = Simulator(
                    ...     environment=environment,
                    ...     satellites=[satellite1, satellite2]
                    ... )
            )doc"
        )

        .def(
            "is_defined",
            &Simulator::isDefined,
            R"doc(
                Check if the simulator is defined.

                Returns:
                    bool: True if the simulator is defined, False otherwise.

                Example:
                    >>> simulator.is_defined()
                    True
            )doc"
        )

        .def(
            "has_satellite_with_name",
            &Simulator::hasSatelliteWithName,
            arg("name"),
            R"doc(
                Check if a satellite with the given name exists.

                Args:
                    name (str): The satellite name to search for.

                Returns:
                    bool: True if a satellite with the name exists, False otherwise.

                Example:
                    >>> simulator.has_satellite_with_name("sat-1")
                    True
            )doc"
        )

        .def(
            "access_environment",
            &Simulator::accessEnvironment,
            R"doc(
                Access the physics environment.

                Returns:
                    Environment: The simulation environment.

                Example:
                    >>> environment = simulator.access_environment()
                    >>> earth = environment.access_celestial_object_with_name("Earth")
            )doc"
        )

        .def(
            "access_satellite_map",
            &Simulator::accessSatelliteMap,
            R"doc(
                Access the satellite map (name to satellite mapping).

                Returns:
                    dict: Map of satellite names to satellite objects.

                Example:
                    >>> satellite_map = simulator.access_satellite_map()
                    >>> len(satellite_map)
                    2
            )doc"
        )

        .def(
            "access_satellite_with_name",
            &Simulator::accessSatelliteWithName,
            arg("name"),
            R"doc(
                Access a satellite by name.

                Args:
                    name (str): The satellite name.

                Returns:
                    Satellite: The satellite with the specified name.

                Example:
                    >>> satellite = simulator.access_satellite_with_name("sat-1")
            )doc"
        )

        .def(
            "get_instant",
            &Simulator::getInstant,
            R"doc(
                Get the current simulation time.

                Returns:
                    Instant: The current simulation instant.

                Example:
                    >>> instant = simulator.get_instant()
                    >>> print(instant)
            )doc"
        )

        .def(
            "set_instant",
            &Simulator::setInstant,
            arg("instant"),
            R"doc(
                Set the current simulation time.

                Args:
                    instant (Instant): The new simulation instant.

                Example:
                    >>> simulator.set_instant(start_instant)
            )doc"
        )

        .def(
            "step_forward",
            &Simulator::stepForward,
            arg("duration"),
            R"doc(
                Step the simulation forward by a duration.

                Args:
                    duration (Duration): The time step duration.

                Example:
                    >>> from ostk.physics.time import Duration
                    >>> simulator.step_forward(Duration.seconds(60.0))
            )doc"
        )

        .def(
            "add_satellite",
            &Simulator::addSatellite,
            arg("satellite"),
            R"doc(
                Add a satellite to the simulation.

                Args:
                    satellite (Satellite): The satellite to add.

                Example:
                    >>> simulator.add_satellite(new_satellite)
            )doc"
        )

        .def(
            "remove_satellite_with_name",
            &Simulator::removeSatelliteWithName,
            arg("name"),
            R"doc(
                Remove a satellite from the simulation by name.

                Args:
                    name (str): The satellite name.

                Example:
                    >>> simulator.remove_satellite_with_name("sat-1")
            )doc"
        )

        .def(
            "clear_satellites",
            &Simulator::clearSatellites,
            R"doc(
                Remove all satellites from the simulation.

                Example:
                    >>> simulator.clear_satellites()
                    >>> len(simulator.access_satellite_map())
                    0
            )doc"
        )

        .def_static(
            "undefined",
            &Simulator::Undefined,
            R"doc(
                Create an undefined simulator.

                Returns:
                    Simulator: An undefined simulator.

                Example:
                    >>> simulator = Simulator.undefined()
                    >>> simulator.is_defined()
                    False
            )doc"
        )

        .def_static(
            "configure",
            &Simulator::Configure,
            arg("configuration"),
            R"doc(
                Create a simulator from configuration.

                Args:
                    configuration (SimulatorConfiguration): The simulator configuration.

                Returns:
                    Simulator: The configured simulator.

                Example:
                    >>> config = SimulatorConfiguration(
                    ...     environment=environment,
                    ...     satellites=[sat_config1, sat_config2]
                    ... )
                    >>> simulator = Simulator.configure(config)
            )doc"
        )

        ;

    class_<SimulatorConfiguration>(
        aModule,
        "SimulatorConfiguration",
        R"doc(
            Configuration structure for creating simulators.

            SimulatorConfiguration defines the environment and satellite configurations
            needed to construct a Simulator.
        )doc"
    )

        .def(
            init<const Environment&, const Array<SatelliteConfiguration>&>(),
            arg("environment"),
            arg("satellites") = DEFAULT_SATELLITES,
            R"doc(
                Create a SimulatorConfiguration instance.

                Args:
                    environment (Environment): The physics environment.
                    satellites (list[SatelliteConfiguration]): Array of satellite configurations (optional).

                Returns:
                    SimulatorConfiguration: The configuration instance.

                Example:
                    >>> config = SimulatorConfiguration(
                    ...     environment=environment,
                    ...     satellites=[sat_config1, sat_config2]
                    ... )
            )doc"
        )

        ;
}
