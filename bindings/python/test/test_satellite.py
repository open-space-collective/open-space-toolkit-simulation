# Apache License 2.0

import pytest

from datetime import datetime

from ostk.physics import Environment
from ostk.physics.unit import Length
from ostk.physics.time import Instant
from ostk.physics.time import Scale
from ostk.physics.time import Time

from ostk.astrodynamics.trajectory import Orbit
from ostk.astrodynamics.flight import Profile

from ostk.simulation import Component
from ostk.simulation import ComponentConfiguration
from ostk.simulation import Satellite
from ostk.simulation import SatelliteConfiguration


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
def profile(orbit: Orbit) -> Profile:
    return Profile.local_orbital_frame_pointing(
        orbit=orbit,
        orbital_frame_type=Orbit.FrameType.VVLH,
    )


@pytest.fixture
def satellite_configuration(profile: Profile) -> SatelliteConfiguration:
    return SatelliteConfiguration(
        id="9ea22c07-6977-48a7-8f68-dff758971d57",
        name="LoftSat-1",
        profile=profile,
    )


@pytest.fixture
def satellite(satellite_configuration: SatelliteConfiguration) -> Satellite:
    return Satellite.configure(configuration=satellite_configuration)


@pytest.fixture
def frameless_satellite(profile: Profile) -> Satellite:
    return Satellite(
        id="9ea22c07-6977-48a7-8f68-dff758971d57",
        name="LoftSat-1",
        tags=["a"],
        geometries=[],
        components=[],
        frame=None,
        profile=profile,
        simulator=None,
    )


class TestSatellite:
    def test_configure(self, satellite: Satellite):
        assert satellite is not None
        assert satellite.get_name() == "LoftSat-1"
        assert satellite.get_frame() is not None

    @pytest.mark.parametrize(
        "id, name",
        [
            ("", "LoftSat-1"),
            ("9ea22c07-6977-48a7-8f68-dff758971d57", ""),
        ],
    )
    def test_constructor_with_empty_id_or_name(
        self,
        profile: Profile,
        id: str,
        name: str,
    ):
        with pytest.raises(RuntimeError):
            Satellite(
                id=id,
                name=name,
                tags=[],
                geometries=[],
                components=[],
                frame=None,
                profile=profile,
                simulator=None,
            )

    def test_has_no_undefined(self):
        assert not hasattr(Satellite, "undefined")
        assert not hasattr(Satellite, "is_defined")

    def test_without_frame(self, frameless_satellite: Satellite):
        assert frameless_satellite.get_name() == "LoftSat-1"
        assert frameless_satellite.get_type() == Component.Type.Assembly
        assert frameless_satellite.get_tags() == ["a"]
        assert frameless_satellite.get_profile() is not None

        with pytest.raises(RuntimeError):
            frameless_satellite.get_frame()

        with pytest.raises(RuntimeError):
            Component.configure(
                ComponentConfiguration(id="camera-id", name="Camera"),
                frameless_satellite,
            )

    def test_get_profile(self, satellite: Satellite):
        assert satellite.get_profile().is_defined()
