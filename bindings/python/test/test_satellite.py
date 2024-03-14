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
def satellite_configuration(orbit: Orbit) -> SatelliteConfiguration:
    return SatelliteConfiguration(
        id="9ea22c07-6977-48a7-8f68-dff758971d57",
        name="LoftSat-1",
        profile=Profile.nadir_pointing(
            orbit=orbit,
            orbital_frame_type=Orbit.FrameType.VVLH,
        ),
    )


@pytest.fixture
def satellite(satellite_configuration: SatelliteConfiguration) -> Satellite:
    return Satellite.configure(configuration=satellite_configuration)


class TestSatellite:
    def test_configure(self, satellite: Satellite):
        assert satellite is not None

    def test_is_defined(self, satellite: Satellite):
        assert satellite.is_defined()

    def test_access_profile(self, satellite: Satellite):
        assert satellite.access_profile().is_defined()
