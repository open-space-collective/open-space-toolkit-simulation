# Apache License 2.0

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


class TestSatellite:
    def test_configure(self):
        environment: Environment = Environment.default()

        orbit = Orbit.sun_synchronous(
            epoch=Instant.date_time(datetime(2020, 1, 1, 0, 0, 0), Scale.UTC),
            altitude=Length.kilometers(500.0),
            local_time_at_descending_node=Time(14, 0, 0),
            celestial_object=environment.access_celestial_object_with_name("Earth"),
        )

        satellite = Satellite.configure(
            configuration=SatelliteConfiguration(
                id="9ea22c07-6977-48a7-8f68-dff758971d57",
                name="LoftSat-1",
                profile=Profile.nadir_pointing(
                    orbit=orbit,
                    orbital_frame_type=Orbit.FrameType.VVLH,
                ),
            ),
        )

        assert satellite.is_defined()
