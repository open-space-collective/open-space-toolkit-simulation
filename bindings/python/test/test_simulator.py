################################################################################################################################################################

# @project        Open Space Toolkit ▸ Simulation
# @file           bindings/python/test/test_simulator.py
# @author         Lucas Brémond <lucas@loftorbital.com>
# @license        Apache License 2.0

################################################################################################################################################################

from datetime import datetime

import numpy as np

from ostk.mathematics.geometry.d2.objects import Point as Point2d
from ostk.mathematics.geometry.d2.objects import Polygon as Polygon2d
from ostk.mathematics.geometry.d3.objects import Point
from ostk.mathematics.geometry.d3.objects import Polygon
from ostk.mathematics.geometry.d3.objects import Pyramid
from ostk.mathematics.geometry.d3.transformations.rotations import Quaternion

from ostk.physics import Environment
from ostk.physics.units import Length
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
from ostk.simulation.component import GeometryConfiguration

################################################################################################################################################################

class TestSimulator:

    def test_end_to_end (self):

        environment = Environment.default()

        orbit = Orbit.sun_synchronous(
            epoch = Instant.date_time(datetime(2020, 1, 1, 0, 0, 0), Scale.UTC),
            altitude = Length.kilometers(500.0),
            local_time_at_descending_node = Time(14, 0, 0),
            celestial_object = environment.access_celestial_object_with_name('Earth'),
        )

        simulator = Simulator.configure(
            SimulatorConfiguration(
                environment = environment,
                satellites = [
                    SatelliteConfiguration(
                        id = '9ea22c07-6977-48a7-8f68-dff758971d57',
                        name = 'LoftSat-1',
                        tags = ['a', 'b'],
                        profile = Profile.nadir_pointing(
                            orbit = orbit,
                            orbital_frame_type = Orbit.FrameType.VVLH,
                        ),
                        components = [
                            ComponentConfiguration(
                                id = 'dc261118-dad7-476d-ab6c-fdfef12f20fd',
                                name = 'Camera',
                                type = Component.Type.Sensor,
                                tags = ['c', 'd'],
                                orientation = Quaternion.unit(),
                                geometries = [
                                    GeometryConfiguration(
                                        name = 'FOV',
                                        object = Pyramid(
                                            Polygon(
                                                Polygon2d([Point2d(-0.1, -1.0), Point2d(+0.1, -1.0), Point2d(+0.1, +1.0), Point2d(-0.1, +1.0)]),
                                                Point(0.0, 0.0, 1.0),
                                                (1.0, 0.0, 0.0),
                                                (0.0, 1.0, 0.0),
                                            ),
                                            Point(0.0, 0.0, 0.0),
                                        ),
                                    ),
                                ],
                                components = [],
                            ),
                        ],
                        geometries = [],
                    ),
                ],
            )
        )

        instant = Instant.date_time(datetime(2020, 1, 1, 0, 0, 0), Scale.UTC)
        print('instant = ', instant)

        simulator.set_instant(instant)

        camera = simulator.access_satellite_with_name('LoftSat-1').access_component_with_name('Camera')
        print('camera = ', camera)

        camera_geometry = camera.access_geometry_with_name('FOV')
        earth_geometry = environment.access_celestial_object_with_name('Earth').access_geometry()

        assert camera_geometry.intersects(earth_geometry) is True

        # EXPECT_EQ(2, camera_geometry.intersectionWith(earth_geometry).access_composite().getObjectCount()) ;
        # EXPECT_TRUE(camera_geometry.intersectionWith(earth_geometry).accessComposite().accessObjectAt(0).is<LineString>()) ;
        # EXPECT_TRUE(camera_geometry.intersectionWith(earth_geometry).accessComposite().accessObjectAt(1).is<LineString>()) ;

################################################################################################################################################################
