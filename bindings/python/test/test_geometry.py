# Apache License 2.0

import gc

from datetime import datetime

import pytest

from ostk.mathematics.geometry.d2.object import Point as Point2d
from ostk.mathematics.geometry.d2.object import Polygon as Polygon2d
from ostk.mathematics.geometry.d3.object import Composite
from ostk.mathematics.geometry.d3.object import Point
from ostk.mathematics.geometry.d3.object import Polygon
from ostk.mathematics.geometry.d3.object import Pyramid
from ostk.mathematics.geometry.d3.transformation.rotation import Quaternion

from ostk.physics import Environment
from ostk.physics.time import Instant
from ostk.physics.time import Scale
from ostk.physics.time import Time
from ostk.physics.unit import Length

from ostk.astrodynamics.flight import Profile
from ostk.astrodynamics.trajectory import Orbit

from ostk.simulation import Component
from ostk.simulation import ComponentConfiguration
from ostk.simulation import SatelliteConfiguration
from ostk.simulation import Simulator
from ostk.simulation import SimulatorConfiguration
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
def field_of_view() -> Composite:
    return Composite(
        Pyramid(
            base=Polygon(
                Polygon2d(
                    [
                        Point2d(-0.1, -0.1),
                        Point2d(+0.1, -0.1),
                        Point2d(+0.1, +0.1),
                        Point2d(-0.1, +0.1),
                    ]
                ),
                Point(0.0, 0.0, 1.0),
                (1.0, 0.0, 0.0),
                (0.0, 1.0, 0.0),
            ),
            apex=Point(0.0, 0.0, 0.0),
        ),
    )


@pytest.fixture
def simulator(
    environment: Environment, orbit: Orbit, field_of_view: Composite
) -> Simulator:
    return Simulator.configure(
        SimulatorConfiguration(
            environment=environment,
            satellites=[
                SatelliteConfiguration(
                    id="9ea22c07-6977-48a7-8f68-dff758971d57",
                    name="LoftSat-1",
                    profile=Profile.local_orbital_frame_pointing(
                        orbit=orbit,
                        orbital_frame_type=Orbit.FrameType.VVLH,
                    ),
                    components=[
                        ComponentConfiguration(
                            id="dc261118-dad7-476d-ab6c-fdfef12f20fd",
                            name="Camera",
                            type=Component.Type.Sensor,
                            orientation=Quaternion.unit(),
                            geometries=[
                                GeometryConfiguration(
                                    name="FOV", composite=field_of_view
                                ),
                            ],
                        ),
                    ],
                ),
            ],
        ),
    )


@pytest.fixture
def camera(simulator: Simulator) -> Component:
    return simulator.access_satellite_with_name("LoftSat-1").access_component_with_name(
        "Camera"
    )


class TestGeometry:
    def test_access_composite(self, camera: Component):
        geometry: Geometry = camera.access_geometry_with_name("FOV")

        assert geometry.access_composite().get_object_count() == 1

    def test_access_composite_outlives_temporary_parent(self, camera: Component):
        """`access_composite` must keep its parent geometry alive.

        `access_geometry_with_name` hands back a *copy* of the geometry, so binding
        `access_composite` with a bare `return_value_policy::reference` leaves the
        returned composite pointing into an object Python is free to collect as soon as
        the expression ends. A regression shows up here as garbage data or a segfault.
        """

        composite = camera.access_geometry_with_name("FOV").access_composite()

        gc.collect()

        # Churn the heap, so that freed storage is likely to have been reused.
        ballast = [bytearray(4096) for _ in range(2000)]
        del ballast

        assert composite.get_object_count() == 1
        assert composite.access_object_at(0).is_pyramid()
