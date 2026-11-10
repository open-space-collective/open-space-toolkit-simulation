# Apache License 2.0

from datetime import datetime

import pytest

from ostk.core.type import String

from ostk.mathematics.geometry.d3.object import Composite
from ostk.mathematics.geometry.d3.object import Point
from ostk.mathematics.geometry.d3.transformation.rotation import Quaternion

from ostk.physics import Environment
from ostk.physics.coordinate import Frame
from ostk.physics.environment.object import Geometry as ObjectGeometry
from ostk.physics.time import Duration
from ostk.physics.time import Instant
from ostk.physics.time import Scale
from ostk.physics.time import Time
from ostk.physics.unit import Length

from ostk.astrodynamics.flight import Profile
from ostk.astrodynamics.trajectory import Orbit
from ostk.astrodynamics.trajectory import State

from ostk.simulation import Component
from ostk.simulation import ComponentConfiguration
from ostk.simulation import Satellite
from ostk.simulation import Simulator
from ostk.simulation.component import Geometry
from ostk.simulation.component import GeometryConfiguration

# These tests pass objects of the upstream OSTk modules into this module and back, so they
# exercise nanobind's cross-module type resolution (every OSTk module must use the same
# nanobind ABI).


@pytest.fixture
def instant() -> Instant:
    return Instant.date_time(datetime(2020, 1, 1, 0, 0, 0), Scale.UTC)


@pytest.fixture
def environment() -> Environment:
    return Environment.default()


@pytest.fixture
def simulator(environment: Environment, instant: Instant) -> Simulator:
    simulator = Simulator(environment=environment, satellites=[])
    simulator.set_instant(instant)

    return simulator


@pytest.fixture
def component(simulator: Simulator) -> Component:
    return Component(
        id=String("component-id"),
        name=String("Component"),
        type=Component.Type.Assembly,
        tags=[String("tag"), "other-tag"],
        geometries=[],
        components=[],
        parent=None,
        frame=Frame.GCRF(),
        simulator=simulator,
    )


@pytest.fixture
def orbit(environment: Environment, instant: Instant) -> Orbit:
    return Orbit.sun_synchronous(
        epoch=instant,
        altitude=Length.kilometers(500.0),
        local_time_at_descending_node=Time(14, 0, 0),
        celestial_object=environment.get_celestial_object_with_name("Earth"),
    )


@pytest.fixture
def profile(orbit: Orbit) -> Profile:
    # The profile refers to the orbit, so the orbit is a fixture of its own to outlive it.
    return Profile.constant_local_orbital_frame_pointing(
        orbit=orbit,
        orbital_frame_type=Orbit.FrameType.VVLH,
    )


class TestCrossModule:
    def test_core_string(self, component: Component):
        assert isinstance(component.get_id(), String)
        assert component.get_id() == "component-id"
        assert component.get_name() == String("Component")
        assert [str(tag) for tag in component.get_tags()] == ["tag", "other-tag"]

        configuration = ComponentConfiguration(id=String("child-id"), name="Child")
        child: Component = Component.configure(configuration, component)

        assert child.get_id() == "child-id"

    def test_physics_environment_and_instant(
        self,
        simulator: Simulator,
        instant: Instant,
    ):
        assert isinstance(simulator.get_environment(), Environment)
        assert isinstance(simulator.get_instant(), Instant)
        assert simulator.get_instant() == instant

        simulator.step_forward(Duration.seconds(10.0))

        assert simulator.get_instant() == instant + Duration.seconds(10.0)

    def test_physics_frame(self, component: Component):
        assert isinstance(component.get_frame(), Frame)
        assert component.get_frame() == Frame.GCRF()

    def test_mathematics_quaternion_default(self, component: Component):
        for configuration in (
            ComponentConfiguration(id="a", name="A"),
            ComponentConfiguration(id="b", name="B", orientation=Quaternion.unit()),
        ):
            child: Component = Component.configure(configuration, component)

            assert isinstance(child.get_frame(), Frame)

    def test_mathematics_composite_and_physics_geometry(self, component: Component):
        composite = Composite(Point(0.0, 0.0, 1.0))
        geometry: Geometry = Geometry.configure(
            GeometryConfiguration(name="Point", composite=composite),
            component,
        )

        assert isinstance(geometry.get_composite(), Composite)
        assert geometry.get_composite() == composite
        assert geometry.get_frame() == Frame.GCRF()

        geometry_in_itrf = geometry.get_geometry_in(Frame.ITRF())

        assert isinstance(geometry_in_itrf, ObjectGeometry)
        assert geometry_in_itrf.get_frame() == Frame.ITRF()

    def test_astrodynamics_profile(self, profile: Profile, instant: Instant):
        satellite = Satellite(
            id="satellite-id",
            name="Satellite",
            tags=[],
            geometries=[],
            components=[],
            frame=None,
            profile=profile,
            simulator=None,
        )

        assert isinstance(satellite.get_profile(), Profile)
        assert isinstance(satellite.get_profile().get_state_at(instant), State)
        assert satellite.get_profile().get_state_at(instant) == profile.get_state_at(
            instant
        )
