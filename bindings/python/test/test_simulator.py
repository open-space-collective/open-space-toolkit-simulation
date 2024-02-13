# Apache License 2.0

import pytest

from datetime import datetime

from ostk.mathematics.geometry.d2.object import Point as Point2d
from ostk.mathematics.geometry.d2.object import Polygon as Polygon2d
from ostk.mathematics.geometry.d3.object import Point
from ostk.mathematics.geometry.d3.object import Polygon
from ostk.mathematics.geometry.d3.object import Pyramid
from ostk.mathematics.geometry.d3.object import Composite
from ostk.mathematics.geometry.d3.transformation.rotation import Quaternion

from ostk.physics import Environment
from ostk.physics.unit import Length
from ostk.physics.time import Instant
from ostk.physics.time import Scale
from ostk.physics.time import Time
from ostk.physics.time import Duration
from ostk.physics.coordinate import Frame

from ostk.astrodynamics.trajectory import Orbit
from ostk.astrodynamics.flight import Profile

from ostk.simulation import Simulator
from ostk.simulation import SimulatorConfiguration
from ostk.simulation import Satellite
from ostk.simulation import SatelliteConfiguration
from ostk.simulation import Component
from ostk.simulation import ComponentConfiguration
from ostk.simulation.component import Geometry
from ostk.simulation.component import GeometryConfiguration


@pytest.fixture
def environment() -> Environment:
    return Environment.default()


@pytest.fixture
def orbit(environment: Environment) -> Orbit:
    return Orbit.sun_synchronous(
        epoch=Instant.date_time(datetime(2020, 1, 1, 0, 0, 0), Scale.UTC),
        altitude=Length.kilometers(500.0),
        local_time_at_descending_node=Time(14, 0, 0),
        celestial_object=environment.access_celestial_object_with_name("Earth"),
    )


@pytest.fixture
def satellite_name() -> str:
    return "LoftSat-1"


@pytest.fixture
def satellite_configuration(
    satellite_name: str,
    orbit: Orbit,
) -> SatelliteConfiguration:
    return SatelliteConfiguration(
        id="9ea22c07-6977-48a7-8f68-dff758971d57",
        name=satellite_name,
        tags=["a", "b"],
        profile=Profile.nadir_pointing(
            orbit=orbit,
            orbital_frame_type=Orbit.FrameType.VVLH,
        ),
        components=[
            ComponentConfiguration(
                id="dc261118-dad7-476d-ab6c-fdfef12f20fd",
                name="Camera",
                type=Component.Type.Sensor,
                tags=["c", "d"],
                orientation=Quaternion.unit(),
                geometries=[
                    GeometryConfiguration(
                        name="FOV",
                        composite=Composite(
                            Pyramid(
                                base=Polygon(
                                    Polygon2d(
                                        [
                                            Point2d(-0.1, -1.0),
                                            Point2d(+0.1, -1.0),
                                            Point2d(+0.1, +1.0),
                                            Point2d(-0.1, +1.0),
                                        ]
                                    ),
                                    Point(0.0, 0.0, 1.0),
                                    (1.0, 0.0, 0.0),
                                    (0.0, 1.0, 0.0),
                                ),
                                apex=Point(0.0, 0.0, 0.0),
                            ),
                        ),
                    ),
                ],
            ),
        ],
    )


@pytest.fixture
def simulator(
    environment: Environment,
    satellite_configuration: SatelliteConfiguration,
) -> Simulator:
    simulator = Simulator.configure(
        configuration=SimulatorConfiguration(
            environment=environment,
            satellites=[satellite_configuration],
        ),
    )
    return simulator


@pytest.fixture
def instant() -> Instant:
    return Instant.date_time(datetime(2020, 1, 1, 0, 0, 0), Scale.UTC)


class TestSimulator:
    def test_is_defined(self, simulator: Simulator):
        assert simulator.is_defined()

    def test_access_environment(self, simulator: Simulator):
        assert simulator.access_environment() is not None

    def test_access_satellite_map(self, simulator: Simulator):
        assert simulator.access_satellite_map() is not None

    def test_access_satellite_with_name(
        self,
        simulator: Simulator,
        satellite_name: str,
    ):
        assert simulator.access_satellite_with_name(satellite_name) is not None

    def test_get_instant(self, simulator: Simulator):
        assert simulator.get_instant() is not None

    def test_set_instant(self, simulator: Simulator):
        instant: Instant = Instant.date_time(datetime(2022, 1, 1, 0, 0, 0), Scale.UTC)
        simulator.set_instant(instant)

        assert simulator.get_instant() == instant

    def test_step_forward(self, simulator: Simulator):
        initial_instant: Instant = simulator.get_instant()

        simulator.step_forward(Duration.seconds(10.0))

        assert simulator.get_instant() > initial_instant

    def test_add_satellite(
        self,
        environment: Environment,
        satellite_configuration: SatelliteConfiguration,
    ):
        simulator: Simulator = Simulator.configure(
            SimulatorConfiguration(environment, [])
        )
        satellite: Satellite = Satellite.configure(satellite_configuration)

        assert len(simulator.access_satellite_map()) == 0

        simulator.add_satellite(satellite)

        assert satellite in simulator.access_satellite_map().values()

    def test_undefined(self):
        simulator: Simulator = Simulator.undefined()

        assert not simulator.is_defined()

    def test_configure(self, environment: Environment):
        configuration: SimulatorConfiguration = SimulatorConfiguration(environment, [])
        simulator: Simulator = Simulator.configure(configuration)

        assert simulator.is_defined()

    def test_end_to_end(self, simulator: Simulator):
        camera: Component = simulator.access_satellite_with_name(
            "LoftSat-1"
        ).access_component_with_name("Camera")
        camera_geometry: Geometry = camera.access_geometry_with_name("FOV")
        earth = simulator.access_environment().access_celestial_object_with_name(
            "Earth"
        )

        assert camera_geometry.intersects(earth) is True

        assert camera_geometry.access_composite() is not None
        assert camera_geometry.access_frame() is not None
        assert camera_geometry.get_geometry_in(Frame.GCRF()) is not None
        assert camera_geometry.get_geometry_in(Frame.ITRF()) is not None

        assert (
            camera_geometry.intersection_with(earth)
            .access_composite()
            .get_object_count()
            == 2
        )
        assert (
            camera_geometry.intersection_with(earth)
            .access_composite()
            .access_object_at(0)
            .is_line_string()
            is True
        )
        assert (
            camera_geometry.intersection_with(earth)
            .access_composite()
            .access_object_at(1)
            .is_line_string()
            is True
        )
