/// Apache License 2.0

#include <OpenSpaceToolkit/Simulation/Satellite.hpp>

inline void OpenSpaceToolkitSimulationPy_Satellite(nanobind::module_& aModule)
{
    using namespace nanobind;

    using ostk::core::container::Array;
    using ostk::core::type::Shared;
    using ostk::core::type::String;

    using ostk::physics::coordinate::Frame;

    using ostk::astrodynamics::flight::Profile;

    using ostk::simulation::Component;
    using ostk::simulation::component::Geometry;
    using ostk::simulation::component::GeometryConfiguration;
    using ostk::simulation::ComponentConfiguration;
    using ostk::simulation::Satellite;
    using ostk::simulation::SatelliteConfiguration;
    using ostk::simulation::Simulator;

    class_<Satellite, Component>(
        aModule,
        "Satellite",
        R"doc(
            Satellite component representing a spacecraft entity.

            A Satellite extends Component with an astrodynamics Profile that defines its
            trajectory and state information over time.
        )doc"
    )

        .def(
            init<
                const String&,
                const String&,
                const Array<String>&,
                const Array<Shared<Geometry>>&,
                const Array<Shared<Component>>&,
                const Shared<const Frame>&,
                const Shared<Profile>&,
                const Shared<const Simulator>&>(),
            arg("id"),
            arg("name"),
            arg("tags"),
            arg("geometries"),
            arg("components"),
            arg("frame").none(),
            arg("profile").none(),
            arg("simulator").none(),
            R"doc(
                Create a Satellite instance.

                Args:
                    id (str): The unique satellite identifier. Must not be empty.
                    name (str): The satellite name. Must not be empty.
                    tags (list[str]): Array of classification tags.
                    geometries (list[Geometry]): Array of geometry objects.
                    components (list[Component]): Array of child components.
                    frame (Frame | None): The satellite reference frame. None if the satellite has no frame.
                    profile (Profile | None): The astrodynamics profile (trajectory).
                    simulator (Simulator | None): Reference to the parent simulator.

                Returns:
                    Satellite: The satellite instance.

                Raises:
                    RuntimeError: If the id or the name is empty.

                Example:
                    >>> satellite = Satellite(
                    ...     id="sat-1",
                    ...     name="My Satellite",
                    ...     tags=["LEO"],
                    ...     geometries=[],
                    ...     components=[],
                    ...     frame=frame,
                    ...     profile=profile,
                    ...     simulator=sim
                    ... )
            )doc"
        )

        .def(
            "get_profile",
            &Satellite::getProfile,
            R"doc(
                Get the satellite's astrodynamics profile.

                Returns:
                    Profile: The satellite trajectory profile.

                Example:
                    >>> profile = satellite.get_profile()
                    >>> state = profile.get_state_at(instant)
            )doc"
        )

        .def_static(
            "configure",
            &Satellite::Configure,
            arg("configuration"),
            arg("simulator").none() = none(),
            R"doc(
                Create a satellite from configuration.

                Args:
                    configuration (SatelliteConfiguration): The satellite configuration.
                    simulator (Simulator | None): Optional reference to the parent simulator.

                Returns:
                    Satellite: The configured satellite.

                Example:
                    >>> config = SatelliteConfiguration(
                    ...     id="sat-1",
                    ...     name="My Satellite",
                    ...     profile=profile
                    ... )
                    >>> satellite = Satellite.configure(config, simulator)
            )doc"
        )

        ;

    class_<SatelliteConfiguration>(
        aModule,
        "SatelliteConfiguration",
        R"doc(
            Configuration structure for creating satellites.

            SatelliteConfiguration defines all parameters needed to construct a Satellite,
            including its profile, components, tags, and geometries.
        )doc"
    )

        .def(
            init<
                const String&,
                const String&,
                const Profile&,
                const Array<ComponentConfiguration>&,
                const Array<String>&,
                const Array<GeometryConfiguration>&>(),
            arg("id"),
            arg("name"),
            arg("profile"),
            arg("components") = DEFAULT_COMPONENTS,
            arg("tags") = DEFAULT_TAGS,
            arg("geometries") = DEFAULT_GEOMETRIES,
            R"doc(
                Create a SatelliteConfiguration instance.

                Args:
                    id (str): The unique satellite identifier.
                    name (str): The satellite name.
                    profile (Profile): The astrodynamics profile (trajectory).
                    components (list[ComponentConfiguration]): Array of component configurations (optional).
                    tags (list[str]): Array of classification tags (optional).
                    geometries (list[GeometryConfiguration]): Array of geometry configurations (optional).

                Returns:
                    SatelliteConfiguration: The configuration instance.

                Example:
                    >>> config = SatelliteConfiguration(
                    ...     id="sat-1",
                    ...     name="My Satellite",
                    ...     profile=profile,
                    ...     tags=["LEO", "science"]
                    ... )
            )doc"
        )

        ;
}
