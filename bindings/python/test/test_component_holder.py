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

from ostk.simulation import Simulator
from ostk.simulation import SimulatorConfiguration
from ostk.simulation import Satellite
from ostk.simulation import SatelliteConfiguration
from ostk.simulation import Component
from ostk.simulation import ComponentConfiguration


@pytest.fixture
def satellite_name() -> str:
    return "LoftSat-1"


@pytest.fixture
def simulator(satellite_name: str) -> Simulator:
    environment: Environment = Environment.default()

    orbit: Orbit = Orbit.sun_synchronous(
        epoch=Instant.date_time(datetime(2020, 1, 1, 0, 0, 0), Scale.UTC),
        altitude=Length.kilometers(500.0),
        local_time_at_descending_node=Time(14, 0, 0),
        celestial_object=environment.access_celestial_object_with_name("Earth"),
    )

    # LoftSat-1
    # └── Camera
    #     └── Lens
    #         └── Detector
    satellite_configuration = SatelliteConfiguration(
        id="9ea22c07-6977-48a7-8f68-dff758971d57",
        name=satellite_name,
        profile=Profile.local_orbital_frame_pointing(
            orbit=orbit,
            orbital_frame_type=Orbit.FrameType.VVLH,
        ),
        components=[
            ComponentConfiguration(
                id="camera",
                name="Camera",
                type=Component.Type.Assembly,
                components=[
                    ComponentConfiguration(
                        id="lens",
                        name="Lens",
                        type=Component.Type.Assembly,
                        components=[
                            ComponentConfiguration(
                                id="detector",
                                name="Detector",
                                type=Component.Type.Sensor,
                            ),
                        ],
                    ),
                ],
            ),
        ],
    )

    return Simulator.configure(
        configuration=SimulatorConfiguration(
            environment=environment,
            satellites=[satellite_configuration],
        ),
    )


@pytest.fixture
def satellite(simulator: Simulator, satellite_name: str) -> Satellite:
    return simulator.access_satellite_with_name(satellite_name)


class TestComponentHolder:
    def test_has_component_at_with_slash_delimiter(self, satellite: Satellite):
        assert satellite.has_component_at("Camera")
        assert satellite.has_component_at("Camera/Lens")
        assert satellite.has_component_at("Camera/Lens/Detector")
        assert satellite.has_component_at("/Camera/Lens/Detector/")

        assert not satellite.has_component_at("Camera/Detector")

    def test_has_component_at_with_dot_delimiter(self, satellite: Satellite):
        assert satellite.has_component_at("Camera.Lens")
        assert satellite.has_component_at("Camera.Lens.Detector")
        assert satellite.has_component_at(".Camera.Lens.Detector.")

        assert not satellite.has_component_at("Camera.Detector")

    def test_has_component_at_with_mixed_delimiters(self, satellite: Satellite):
        assert satellite.has_component_at("Camera/Lens.Detector")
        assert satellite.has_component_at("Camera.Lens/Detector")

    def test_access_component_at_with_slash_delimiter(self, satellite: Satellite):
        assert satellite.access_component_at("Camera").get_name() == "Camera"
        assert satellite.access_component_at("Camera/Lens").get_name() == "Lens"
        assert (
            satellite.access_component_at("Camera/Lens/Detector").get_name() == "Detector"
        )

        with pytest.raises(RuntimeError):
            satellite.access_component_at("Camera/Detector")

    def test_access_component_at_with_dot_delimiter(self, satellite: Satellite):
        assert satellite.access_component_at("Camera.Lens").get_name() == "Lens"
        assert (
            satellite.access_component_at("Camera.Lens.Detector").get_name() == "Detector"
        )

        with pytest.raises(RuntimeError):
            satellite.access_component_at("Camera.Detector")

    def test_access_component_at_with_mixed_delimiters(self, satellite: Satellite):
        assert (
            satellite.access_component_at("Camera/Lens.Detector").get_name() == "Detector"
        )
        assert (
            satellite.access_component_at("Camera.Lens/Detector").get_name() == "Detector"
        )
