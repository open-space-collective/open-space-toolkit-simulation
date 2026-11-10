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
from ostk.physics.coordinate import Frame
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
        celestial_object=environment.get_celestial_object_with_name("Earth"),
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
                    profile=Profile.constant_local_orbital_frame_pointing(
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
    return simulator.get_satellite_with_name("LoftSat-1").get_component_with_name(
        "Camera"
    )


class TestGeometry:
    def test_has_no_undefined(self):
        assert not hasattr(Geometry, "undefined")
        assert not hasattr(Geometry, "is_defined")

    def test_constructor(self, camera: Component, field_of_view: Composite):
        geometry: Geometry = Geometry("Other", field_of_view, camera)

        assert geometry.get_name() == "Other"
        assert geometry.get_component().get_id() == camera.get_id()

    def test_constructor_errors(self, camera: Component, field_of_view: Composite):
        with pytest.raises(RuntimeError):
            Geometry("", field_of_view, camera)

        with pytest.raises(TypeError):
            Geometry("FOV", field_of_view, None)

        frameless_component: Component = Component(
            id="frameless-id",
            name="Frameless",
            type=Component.Type.Sensor,
            tags=[],
            geometries=[],
            components=[],
            parent=None,
            frame=None,
            simulator=None,
        )

        with pytest.raises(RuntimeError):
            Geometry("FOV", field_of_view, frameless_component)

    def test_get_composite(self, camera: Component):
        geometry: Geometry = camera.get_geometry_with_name("FOV")

        assert geometry.get_composite().get_object_count() == 1

    def test_get_composite_outlives_temporary_parent(self, camera: Component):
        composite = camera.get_geometry_with_name("FOV").get_composite()

        gc.collect()

        # Churn the heap, so that freed storage is likely to have been reused.
        ballast = [bytearray(4096) for _ in range(2000)]
        del ballast

        assert composite.get_object_count() == 1
        assert composite.get_object_at(0).is_pyramid()
