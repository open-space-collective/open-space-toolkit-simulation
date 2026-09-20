#!/usr/bin/env python3
# Apache License 2.0

"""
Sensor / area-of-interest overlap: scenario + performance profile.

Scenario
--------

- One satellite on a sun-synchronous orbit at 550 km altitude with a 10:00 LTAN.
- The orbit is sampled on a fixed 20 s grid and re-wrapped as a tabulated orbit,
  so that the propagation cost is paid once, up front, instead of at every step.
- A nadir-pointing attitude profile (VVLH local orbital frame).
- A single `Sensor` component holding a square pyramidal field of view.
- A ground area of interest (the Japanese archipelago) defined in ITRF.

The evaluation walks the 7-day grid and, at every step, answers the question
"does the sensor field of view overlap the area of interest?".

Run it
------

    python sensor_aoi_overlap_profiling.py --duration-days 7 --step 20

Useful flags:

    --strategy full|prefiltered   Overlap evaluation strategy (default: full).
    --profile                     Run the evaluation under cProfile.
    --stage-timers                Report a per-stage wall-clock breakdown.
    --ablation                    Measure the incremental cost of each stage.
"""

from __future__ import annotations

import argparse
import cProfile
import io
import math
import os
import pstats
import sys
import time
from dataclasses import dataclass, field
from typing import Callable, Sequence

# --------------------------------------------------------------------------------------
# Offline data configuration
#
# OSTk fetches ephemerides, EOP and gravity coefficients from the network on first use.
# Point OSTK_PHYSICS_DATA_LOCAL_REPOSITORY at a populated data directory and set
# OSTK_OFFLINE=1 to pin every manager to manual mode instead.
# --------------------------------------------------------------------------------------


def configure_data_managers() -> None:
    if os.environ.get("OSTK_OFFLINE", "0") != "1":
        return

    from ostk.physics.coordinate.frame.provider.iers import Manager as IersManager
    from ostk.physics.environment.atmospheric.earth import Manager as AtmosphericManager
    from ostk.physics.environment.ephemeris.spice import Manager as SpiceManager
    from ostk.physics.environment.gravitational.earth import (
        Manager as GravitationalManager,
    )
    from ostk.physics.environment.magnetic.earth import Manager as MagneticManager

    for manager_class in (
        GravitationalManager,
        IersManager,
        SpiceManager,
        AtmosphericManager,
        MagneticManager,
    ):
        manager_class.get().set_mode(manager_class.Mode.Manual)


configure_data_managers()

from ostk.astrodynamics.flight import Profile  # noqa: E402
from ostk.astrodynamics.trajectory import Orbit  # noqa: E402
from ostk.astrodynamics.trajectory.orbit.model import Tabulated  # noqa: E402
from ostk.mathematics.geometry.d2.object import Point as Point2d  # noqa: E402
from ostk.mathematics.geometry.d2.object import Polygon as Polygon2d  # noqa: E402
from ostk.mathematics.geometry.d3.object import (
    Composite,
    Point,
    Polygon,
    Pyramid,
)  # noqa: E402
from ostk.mathematics.geometry.d3.transformation.rotation import Quaternion  # noqa: E402
from ostk.physics import Environment  # noqa: E402
from ostk.physics.coordinate import Frame  # noqa: E402
from ostk.physics.coordinate.spherical import LLA  # noqa: E402
from ostk.physics.time import (
    DateTime,
    Duration,
    Instant,
    Interval,
    Scale,
    Time,
)  # noqa: E402
from ostk.physics.unit import Length  # noqa: E402
from ostk.simulation import Component, ComponentConfiguration, Satellite  # noqa: E402
from ostk.simulation import (
    SatelliteConfiguration,
    Simulator,
    SimulatorConfiguration,
)  # noqa: E402
from ostk.simulation.component import GeometryConfiguration  # noqa: E402

import numpy as np  # noqa: E402

# --------------------------------------------------------------------------------------
# Scenario definition
# --------------------------------------------------------------------------------------


@dataclass
class ScenarioConfiguration:
    epoch_utc: tuple = (2024, 1, 1, 0, 0, 0)
    altitude_km: float = 550.0
    # Local time at *ascending* node. `Orbit.sun_synchronous` is parameterised by the
    # descending node, which for a sun-synchronous orbit is exactly 12 h away.
    ltan_hours: int = 10
    ltan_minutes: int = 0
    duration_days: float = 7.0
    step_seconds: float = 20.0
    # Full cross-track / along-track field of view of the square pyramidal sensor.
    fov_degrees: float = 30.0
    satellite_name: str = "Sat-1"
    sensor_name: str = "Camera"
    geometry_name: str = "FOV"

    @property
    def epoch(self) -> Instant:
        return Instant.date_time(DateTime(*self.epoch_utc), Scale.UTC)

    @property
    def local_time_at_descending_node(self) -> Time:
        hours = (self.ltan_hours + 12) % 24
        return Time(hours, self.ltan_minutes, 0)

    @property
    def step(self) -> Duration:
        return Duration.seconds(self.step_seconds)

    @property
    def duration(self) -> Duration:
        return Duration.days(self.duration_days)


def build_sun_synchronous_orbit(
    environment: Environment, configuration: ScenarioConfiguration
) -> Orbit:
    """Analytical sun-synchronous orbit used as the source of the tabulated ephemeris."""

    return Orbit.sun_synchronous(
        epoch=configuration.epoch,
        altitude=Length.kilometers(configuration.altitude_km),
        local_time_at_descending_node=configuration.local_time_at_descending_node,
        celestial_object=environment.access_celestial_object_with_name("Earth"),
    )


def generate_grid(configuration: ScenarioConfiguration) -> list:
    interval = Interval.closed(
        configuration.epoch, configuration.epoch + configuration.duration
    )

    return interval.generate_grid(configuration.step)


def generate_tabulation_grid(
    configuration: ScenarioConfiguration, margin_steps: int = 4
) -> list:
    """Grid used to build the tabulated orbit.

    It extends a few steps beyond the evaluation horizon on both sides so that every
    evaluated instant sits strictly inside the interpolation range.
    """

    margin = Duration.seconds(configuration.step_seconds * margin_steps)
    interval = Interval.closed(
        configuration.epoch - margin,
        configuration.epoch + configuration.duration + margin,
    )

    return interval.generate_grid(configuration.step)


def tabulate_orbit(orbit: Orbit, grid: Sequence, environment: Environment) -> Orbit:
    """Sample the analytical orbit on `grid` and wrap the states in a tabulated model."""

    states = orbit.get_states_at(grid)

    return Orbit(
        Tabulated(states, 1),
        environment.access_celestial_object_with_name("Earth"),
    )


def build_field_of_view(fov_degrees: float) -> Composite:
    """Square pyramidal field of view, boresight along the component +Z axis."""

    half_extent = math.tan(math.radians(fov_degrees / 2.0))

    base = Polygon(
        Polygon2d(
            [
                Point2d(-half_extent, -half_extent),
                Point2d(+half_extent, -half_extent),
                Point2d(+half_extent, +half_extent),
                Point2d(-half_extent, +half_extent),
            ]
        ),
        Point(0.0, 0.0, 1.0),
        (1.0, 0.0, 0.0),
        (0.0, 1.0, 0.0),
    )

    return Composite(Pyramid(base=base, apex=Point(0.0, 0.0, 0.0)))


def build_simulator(
    environment: Environment,
    profile: Profile,
    configuration: ScenarioConfiguration,
) -> Simulator:
    satellite_configuration = SatelliteConfiguration(
        id="a1b2c3d4-0000-4000-8000-000000000001",
        name=configuration.satellite_name,
        tags=["sso", "550km"],
        profile=profile,
        components=[
            ComponentConfiguration(
                id="a1b2c3d4-0000-4000-8000-000000000002",
                name=configuration.sensor_name,
                type=Component.Type.Sensor,
                tags=["imager"],
                orientation=Quaternion.unit(),
                geometries=[
                    GeometryConfiguration(
                        name=configuration.geometry_name,
                        composite=build_field_of_view(configuration.fov_degrees),
                    ),
                ],
            ),
        ],
    )

    return Simulator.configure(
        SimulatorConfiguration(
            environment=environment, satellites=[satellite_configuration]
        )
    )


# --------------------------------------------------------------------------------------
# Area of interest (ITRF)
# --------------------------------------------------------------------------------------

# Simplified outlines of the four main islands of Japan, as (longitude, latitude) pairs in
# degrees. Coarse on purpose: the point of this scenario is the overlap machinery, not
# coastline fidelity.
JAPAN_OUTLINES: dict[str, list[tuple[float, float]]] = {
    "Hokkaido": [
        (140.5, 42.6),
        (141.2, 41.4),
        (141.9, 42.6),
        (143.0, 42.0),
        (144.4, 42.9),
        (145.4, 43.4),
        (145.3, 44.3),
        (144.2, 44.1),
        (143.0, 44.3),
        (141.6, 45.4),
        (141.6, 43.4),
        (140.3, 43.3),
    ],
    "Honshu": [
        (140.3, 41.5),
        (141.5, 41.2),
        (141.0, 40.5),
        (141.9, 39.9),
        (141.0, 38.3),
        (140.9, 37.8),
        (140.8, 36.9),
        (140.6, 36.3),
        (140.9, 35.7),
        (139.8, 35.0),
        (138.9, 34.6),
        (137.0, 34.6),
        (136.9, 34.2),
        (136.0, 33.5),
        (135.1, 33.6),
        (135.0, 34.3),
        (133.0, 34.4),
        (131.0, 33.9),
        (130.9, 34.4),
        (132.5, 35.5),
        (135.2, 35.7),
        (136.7, 37.4),
        (138.3, 37.2),
        (139.6, 38.2),
        (139.8, 39.9),
        (140.0, 40.6),
    ],
    "Shikoku": [
        (132.0, 33.5),
        (133.3, 34.3),
        (134.7, 34.2),
        (134.6, 33.5),
        (133.7, 33.2),
        (132.8, 32.9),
    ],
    "Kyushu": [
        (130.9, 33.9),
        (131.9, 33.6),
        (131.7, 32.8),
        (131.4, 31.6),
        (130.6, 31.0),
        (130.2, 31.4),
        (129.8, 32.6),
        (130.2, 33.3),
        (130.4, 33.8),
    ],
}


@dataclass
class AreaOfInterest:
    """Ground area of interest, held as lon/lat polygons plus a bounding cap."""

    name: str
    polygons: list = field(default_factory=list)
    center_longitude_deg: float = 0.0
    center_latitude_deg: float = 0.0
    angular_radius_deg: float = 0.0
    min_longitude_deg: float = -180.0
    max_longitude_deg: float = 180.0
    min_latitude_deg: float = -90.0
    max_latitude_deg: float = 90.0

    @classmethod
    def from_outlines(cls, name: str, outlines: dict) -> "AreaOfInterest":
        polygons = [
            Polygon2d(
                [
                    Point2d(float(longitude), float(latitude))
                    for longitude, latitude in outline
                ]
            )
            for outline in outlines.values()
        ]

        vertices = [vertex for outline in outlines.values() for vertex in outline]
        longitudes = [vertex[0] for vertex in vertices]
        latitudes = [vertex[1] for vertex in vertices]

        center_longitude = sum(longitudes) / len(longitudes)
        center_latitude = sum(latitudes) / len(latitudes)
        angular_radius = max(
            _angular_separation_deg(
                center_longitude, center_latitude, longitude, latitude
            )
            for longitude, latitude in vertices
        )

        return cls(
            name=name,
            polygons=polygons,
            center_longitude_deg=center_longitude,
            center_latitude_deg=center_latitude,
            angular_radius_deg=angular_radius,
            min_longitude_deg=min(longitudes),
            max_longitude_deg=max(longitudes),
            min_latitude_deg=min(latitudes),
            max_latitude_deg=max(latitudes),
        )

    def bounding_box_rejects(
        self,
        min_longitude: float,
        max_longitude: float,
        min_latitude: float,
        max_latitude: float,
    ) -> bool:
        return (
            max_longitude < self.min_longitude_deg
            or min_longitude > self.max_longitude_deg
            or max_latitude < self.min_latitude_deg
            or min_latitude > self.max_latitude_deg
        )

    def intersects(self, footprint: Polygon2d) -> bool:
        return any(polygon.intersects(footprint) for polygon in self.polygons)


def _angular_separation_deg(
    longitude_a: float, latitude_a: float, longitude_b: float, latitude_b: float
) -> float:
    lon_a, lat_a = math.radians(longitude_a), math.radians(latitude_a)
    lon_b, lat_b = math.radians(longitude_b), math.radians(latitude_b)

    cosine = math.sin(lat_a) * math.sin(lat_b) + math.cos(lat_a) * math.cos(
        lat_b
    ) * math.cos(lon_a - lon_b)

    return math.degrees(math.acos(max(-1.0, min(1.0, cosine))))


def _unwrap_longitudes(longitudes: list, reference_longitude: float) -> list:
    """Make a ring of longitudes continuous, then move it next to `reference_longitude`.

    `Polygon2d` lives in the plane: a footprint that straddles the antimeridian comes out
    of the lon/lat conversion as a band stretching from -180 to +180, which spuriously
    overlaps everything in between. Unwrapping the ring and re-centring it on the area of
    interest removes that failure mode.
    """

    unwrapped = [longitudes[0]]
    for longitude in longitudes[1:]:
        previous = unwrapped[-1]
        unwrapped.append(longitude - 360.0 * round((longitude - previous) / 360.0))

    center = sum(unwrapped) / len(unwrapped)
    shift = 360.0 * round((reference_longitude - center) / 360.0)

    if shift:
        unwrapped = [longitude + shift for longitude in unwrapped]

    return unwrapped


# --------------------------------------------------------------------------------------
# Overlap evaluation
# --------------------------------------------------------------------------------------


class OverlapEvaluator:
    """Evaluates sensor / AOI overlap at a given instant.

    The pipeline per step is:

      1. move the simulator to the instant;
      2. intersect the field of view with the Earth ellipsoid (in GCRF);
      3. express the resulting footprint in ITRF;
      4. pick the near-side ring and convert it to lon/lat;
      5. test the ring against the AOI polygons.
    """

    def __init__(
        self,
        simulator: Simulator,
        configuration: ScenarioConfiguration,
        area_of_interest: AreaOfInterest,
        stage_timers: bool = False,
    ) -> None:
        self.simulator = simulator
        self.configuration = configuration
        self.area_of_interest = area_of_interest

        self.environment = simulator.access_environment()
        self.earth = self.environment.access_celestial_object_with_name("Earth")
        self.equatorial_radius = self.earth.get_equatorial_radius()
        self.flattening = self.earth.get_flattening()
        self.eccentricity_squared = float(self.flattening) * (
            2.0 - float(self.flattening)
        )

        satellite: Satellite = simulator.access_satellite_with_name(
            configuration.satellite_name
        )
        self.component: Component = satellite.access_component_with_name(
            configuration.sensor_name
        )
        self.sensor_geometry = self.component.access_geometry_with_name(
            configuration.geometry_name
        )
        self.component_frame = self.component.access_frame()

        self.itrf = Frame.ITRF()

        self.stage_timers = stage_timers
        self.stage_seconds: dict[str, float] = {
            "set_instant": 0.0,
            "sensor_position": 0.0,
            "intersection_with_earth": 0.0,
            "in_frame_itrf": 0.0,
            "ring_to_lonlat": 0.0,
            "polygon_test": 0.0,
        }
        self.stage_calls: dict[str, int] = {key: 0 for key in self.stage_seconds}

        self.prefilter_evaluations = 0
        self.prefilter_rejections = 0
        self._cached_footprint_radius_deg: float | None = None

    # -- helpers ------------------------------------------------------------------------

    def _record(self, stage: str, started_at: float) -> None:
        self.stage_seconds[stage] += time.perf_counter() - started_at
        self.stage_calls[stage] += 1

    def _sensor_lla(self, instant: Instant) -> LLA:
        started_at = time.perf_counter() if self.stage_timers else 0.0
        position = self.component_frame.get_origin_in(self.itrf, instant)
        lla = LLA.cartesian(
            position.get_coordinates(), self.equatorial_radius, self.flattening
        )
        if self.stage_timers:
            self._record("sensor_position", started_at)

        return lla

    def _footprint_composite(self, instant: Instant):
        """Field of view / Earth intersection, expressed in ITRF.

        Returns `(geometry, composite)`. The geometry MUST be kept alive by the caller:
        `Geometry.access_composite()` hands out a bare reference into it.
        """

        started_at = time.perf_counter() if self.stage_timers else 0.0
        intersection = self.sensor_geometry.intersection_with(self.earth)
        if self.stage_timers:
            self._record("intersection_with_earth", started_at)

        started_at = time.perf_counter() if self.stage_timers else 0.0
        intersection = intersection.in_frame(self.itrf, instant)
        composite = intersection.access_composite()
        if self.stage_timers:
            self._record("in_frame_itrf", started_at)

        return intersection, composite

    def _footprint_ring(self, instant: Instant, sensor_lla: LLA) -> "Polygon2d | None":
        """Near-side footprint, converted ring by ring through the `LLA` bindings.

        This is the straightforward implementation: every point of every ring goes
        through `LLA.cartesian` before the near-side ring is picked.
        """

        geometry, composite = self._footprint_composite(instant)

        started_at = time.perf_counter() if self.stage_timers else 0.0

        sensor_longitude = float(sensor_lla.get_longitude().in_degrees())
        sensor_latitude = float(sensor_lla.get_latitude().in_degrees())

        best_vertices = None
        best_separation = math.inf

        for index in range(composite.get_object_count()):
            geometric_object = composite.access_object_at(index)
            if not geometric_object.is_line_string():
                continue

            line_string = geometric_object.as_line_string()
            point_count = line_string.get_point_count()
            if point_count < 3:
                continue

            longitudes: list[float] = []
            latitudes: list[float] = []
            for point_index in range(point_count):
                lla = LLA.cartesian(
                    line_string.access_point_at(point_index).as_vector(),
                    self.equatorial_radius,
                    self.flattening,
                )
                longitudes.append(float(lla.get_longitude().in_degrees()))
                latitudes.append(float(lla.get_latitude().in_degrees()))

            longitudes = _unwrap_longitudes(longitudes, sensor_longitude)

            # The pyramid / ellipsoid intersection yields both the near-side ring (the
            # actual footprint) and the far-side ring on the opposite limb. Keep the one
            # centred closest to the sub-satellite point.
            mean_longitude = sum(longitudes) / len(longitudes)
            mean_latitude = sum(latitudes) / len(latitudes)
            separation = _angular_separation_deg(
                sensor_longitude, sensor_latitude, mean_longitude, mean_latitude
            )

            if separation < best_separation:
                best_separation = separation
                best_vertices = (longitudes, latitudes)

        if self.stage_timers:
            self._record("ring_to_lonlat", started_at)

        del geometry

        if best_vertices is None:
            return None

        return self._polygon_from_ring(*best_vertices)

    def _footprint_ring_fast(
        self, instant: Instant, sensor_position_itrf
    ) -> "Polygon2d | None":
        """Near-side footprint, picked in Cartesian space and converted with NumPy.

        Two changes with respect to `_footprint_ring`:

          * the near-side ring is identified from one point per ring, in Cartesian space,
            so only one ring is ever converted to lon/lat;
          * that ring is converted with the closed-form geodetic formula for points lying
            *on* the ellipsoid, vectorised over the ring, instead of one `LLA.cartesian`
            binding call per point.
        """

        geometry, composite = self._footprint_composite(instant)

        started_at = time.perf_counter() if self.stage_timers else 0.0

        best_line_string = None
        best_distance = math.inf

        for index in range(composite.get_object_count()):
            geometric_object = composite.access_object_at(index)
            if not geometric_object.is_line_string():
                continue

            line_string = geometric_object.as_line_string()
            if line_string.get_point_count() < 3:
                continue

            delta = line_string.access_point_at(0).as_vector() - sensor_position_itrf
            distance = float(delta.dot(delta))

            if distance < best_distance:
                best_distance = distance
                best_line_string = line_string

        longitudes = latitudes = None

        if best_line_string is not None:
            point_count = best_line_string.get_point_count()
            coordinates = np.empty((point_count, 3))
            for point_index in range(point_count):
                coordinates[point_index] = best_line_string.access_point_at(
                    point_index
                ).as_vector()

            longitudes = np.degrees(
                np.arctan2(coordinates[:, 1], coordinates[:, 0])
            ).tolist()
            latitudes = np.degrees(
                np.arctan2(
                    coordinates[:, 2],
                    (1.0 - self.eccentricity_squared)
                    * np.hypot(coordinates[:, 0], coordinates[:, 1]),
                )
            ).tolist()
            longitudes = _unwrap_longitudes(
                longitudes, self.area_of_interest.center_longitude_deg
            )

        if self.stage_timers:
            self._record("ring_to_lonlat", started_at)

        del geometry

        if longitudes is None:
            return None

        return self._polygon_from_ring(longitudes, latitudes)

    def _polygon_from_ring(self, longitudes: list, latitudes: list) -> "Polygon2d | None":
        """Build the lon/lat polygon, after a bounding-box rejection."""

        if self.area_of_interest.bounding_box_rejects(
            min(longitudes), max(longitudes), min(latitudes), max(latitudes)
        ):
            return None

        vertices = list(zip(longitudes, latitudes))

        # Drop the repeated closing vertex, if any.
        if len(vertices) > 3 and vertices[0] == vertices[-1]:
            vertices = vertices[:-1]

        if len(vertices) < 3:
            return None

        return Polygon2d(
            [Point2d(longitude, latitude) for longitude, latitude in vertices]
        )

    # -- strategies ---------------------------------------------------------------------

    def evaluate_full(self, instant: Instant) -> bool:
        """Straightforward implementation: full pipeline at every step."""

        started_at = time.perf_counter() if self.stage_timers else 0.0
        self.simulator.set_instant(instant)
        if self.stage_timers:
            self._record("set_instant", started_at)

        sensor_lla = self._sensor_lla(instant)

        footprint = self._footprint_ring(instant, sensor_lla)
        if footprint is None:
            return False

        started_at = time.perf_counter() if self.stage_timers else 0.0
        overlaps = self.area_of_interest.intersects(footprint)
        if self.stage_timers:
            self._record("polygon_test", started_at)

        return overlaps

    def evaluate_prefiltered(self, instant: Instant) -> bool:
        """Same answer as `evaluate_full`, with a cheap great-circle rejection first."""

        started_at = time.perf_counter() if self.stage_timers else 0.0
        self.simulator.set_instant(instant)
        if self.stage_timers:
            self._record("set_instant", started_at)

        sensor_lla = self._sensor_lla(instant)

        if self._rejected_by_prefilter(sensor_lla):
            return False

        footprint = self._footprint_ring(instant, sensor_lla)
        if footprint is None:
            return False

        started_at = time.perf_counter() if self.stage_timers else 0.0
        overlaps = self.area_of_interest.intersects(footprint)
        if self.stage_timers:
            self._record("polygon_test", started_at)

        return overlaps

    def evaluate_tuned(self, instant: Instant) -> bool:
        """Prefilter, plus a footprint conversion that avoids the per-point bindings."""

        started_at = time.perf_counter() if self.stage_timers else 0.0
        self.simulator.set_instant(instant)
        if self.stage_timers:
            self._record("set_instant", started_at)

        started_at = time.perf_counter() if self.stage_timers else 0.0
        sensor_position = self.component_frame.get_origin_in(
            self.itrf, instant
        ).get_coordinates()
        sensor_lla = LLA.cartesian(
            sensor_position, self.equatorial_radius, self.flattening
        )
        if self.stage_timers:
            self._record("sensor_position", started_at)

        if self._rejected_by_prefilter(sensor_lla):
            return False

        footprint = self._footprint_ring_fast(instant, sensor_position)
        if footprint is None:
            return False

        started_at = time.perf_counter() if self.stage_timers else 0.0
        overlaps = self.area_of_interest.intersects(footprint)
        if self.stage_timers:
            self._record("polygon_test", started_at)

        return overlaps

    def _rejected_by_prefilter(self, sensor_lla: LLA) -> bool:
        separation = _angular_separation_deg(
            float(sensor_lla.get_longitude().in_degrees()),
            float(sensor_lla.get_latitude().in_degrees()),
            self.area_of_interest.center_longitude_deg,
            self.area_of_interest.center_latitude_deg,
        )
        self.prefilter_evaluations += 1

        rejected = separation > (
            self.area_of_interest.angular_radius_deg
            + self._footprint_radius_deg()
            + self.PREFILTER_MARGIN_DEG
        )
        if rejected:
            self.prefilter_rejections += 1

        return rejected

    PREFILTER_MARGIN_DEG = 0.5

    def _footprint_radius_deg(self) -> float:
        """Conservative angular radius of the footprint, seen from the Earth centre."""

        if self._cached_footprint_radius_deg is not None:
            return self._cached_footprint_radius_deg

        altitude_m = self.configuration.altitude_km * 1e3
        earth_radius_m = float(self.equatorial_radius.in_meters())
        half_fov = math.radians(self.configuration.fov_degrees / 2.0)
        # Diagonal half-angle of a square field of view.
        half_diagonal = math.atan(math.sqrt(2.0) * math.tan(half_fov))

        sine = (earth_radius_m + altitude_m) * math.sin(half_diagonal) / earth_radius_m
        if sine >= 1.0:
            return 90.0

        nadir_angle = math.asin(sine)

        self._cached_footprint_radius_deg = math.degrees(nadir_angle - half_diagonal)

        return self._cached_footprint_radius_deg


# --------------------------------------------------------------------------------------
# Reporting helpers
# --------------------------------------------------------------------------------------


@dataclass
class PassRecord:
    start: Instant
    end: Instant
    step_count: int


def collapse_to_passes(grid: Sequence, flags: Sequence[bool]) -> list[PassRecord]:
    passes: list[PassRecord] = []
    start_index: int | None = None

    for index, flag in enumerate(flags):
        if flag and start_index is None:
            start_index = index
        elif not flag and start_index is not None:
            passes.append(
                PassRecord(grid[start_index], grid[index - 1], index - start_index)
            )
            start_index = None

    if start_index is not None:
        passes.append(PassRecord(grid[start_index], grid[-1], len(flags) - start_index))

    return passes


def format_duration(seconds: float) -> str:
    if seconds < 1e-3:
        return f"{seconds * 1e6:.1f} us"
    if seconds < 1.0:
        return f"{seconds * 1e3:.2f} ms"

    return f"{seconds:.3f} s"


def run_ablation(evaluator: OverlapEvaluator, grid: Sequence, sample_count: int) -> None:
    """Measure the incremental cost of each stage of the pipeline.

    Every stage is measured over its own, disjoint slice of instants. That matters: OSTk
    caches frame transforms per instant, so re-measuring a longer pipeline over instants
    a shorter one already visited reads back a warm cache and produces negative deltas.
    """

    earth = evaluator.earth
    itrf = evaluator.itrf
    geometry = evaluator.sensor_geometry
    simulator = evaluator.simulator
    component_frame = evaluator.component_frame

    def origin(instant: Instant):
        return component_frame.get_origin_in(itrf, instant)

    stages: list[tuple[str, Callable[[Instant], object]]] = [
        ("loop overhead only", lambda instant: None),
        ("+ simulator.set_instant", lambda instant: simulator.set_instant(instant)),
        (
            "+ component frame origin in ITRF",
            lambda instant: (simulator.set_instant(instant), origin(instant)),
        ),
        (
            "+ geometry.intersection_with(Earth)",
            lambda instant: (
                simulator.set_instant(instant),
                origin(instant),
                geometry.intersection_with(earth),
            ),
        ),
        (
            "+ footprint.in_frame(ITRF)",
            lambda instant: (
                simulator.set_instant(instant),
                origin(instant),
                geometry.intersection_with(earth).in_frame(itrf, instant),
            ),
        ),
        ("+ lon/lat conversion + polygon test", evaluator.evaluate_full),
    ]

    # Pay the one-time IERS / ITRF initialisation before measuring anything.
    started_at = time.perf_counter()
    origin(grid[-1])
    initialisation_seconds = time.perf_counter() - started_at

    per_stage = max(1, min(sample_count, len(grid) // len(stages)))

    print()
    print(f"Stage ablation (incremental, {per_stage} steps per stage, disjoint instants)")
    print(
        f"  one-time ITRF / IERS initialisation: {format_duration(initialisation_seconds)}"
    )
    print("-" * 78)
    print(f"{'stage':<42}{'us/step':>12}{'delta us':>12}")

    previous = 0.0
    for index, (label, action) in enumerate(stages):
        sample = grid[index * per_stage : (index + 1) * per_stage]
        started_at = time.perf_counter()
        for instant in sample:
            action(instant)
        micros = (time.perf_counter() - started_at) / len(sample) * 1e6
        print(f"{label:<42}{micros:>12.1f}{micros - previous:>12.1f}")
        previous = micros


# --------------------------------------------------------------------------------------
# Entry point
# --------------------------------------------------------------------------------------


def parse_arguments(argv: Sequence[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parser.add_argument("--duration-days", type=float, default=7.0)
    parser.add_argument(
        "--step", type=float, default=20.0, help="Evaluation time step, in seconds."
    )
    parser.add_argument("--altitude-km", type=float, default=550.0)
    parser.add_argument("--fov-degrees", type=float, default=30.0)
    parser.add_argument(
        "--strategy",
        choices=("full", "prefiltered", "tuned"),
        default="full",
        help=(
            "full: complete pipeline at every step. "
            "prefiltered: full pipeline behind a great-circle rejection test. "
            "tuned: prefiltered, plus a footprint conversion that avoids per-point bindings."
        ),
    )
    parser.add_argument(
        "--verify",
        action="store_true",
        help="Cross-check every strategy against 'full' over the same grid.",
    )
    parser.add_argument(
        "--profile", action="store_true", help="Run the evaluation under cProfile."
    )
    parser.add_argument("--profile-rows", type=int, default=25)
    parser.add_argument("--stage-timers", action="store_true")
    parser.add_argument("--ablation", action="store_true")
    parser.add_argument("--ablation-samples", type=int, default=500)

    return parser.parse_args(argv)


def main(argv: Sequence[str] | None = None) -> int:
    arguments = parse_arguments(argv)

    configuration = ScenarioConfiguration(
        duration_days=arguments.duration_days,
        step_seconds=arguments.step,
        altitude_km=arguments.altitude_km,
        fov_degrees=arguments.fov_degrees,
    )

    timings: dict[str, float] = {}

    started_at = time.perf_counter()
    environment = Environment.default()
    timings["environment"] = time.perf_counter() - started_at

    started_at = time.perf_counter()
    analytical_orbit = build_sun_synchronous_orbit(environment, configuration)
    timings["sun-synchronous orbit"] = time.perf_counter() - started_at

    started_at = time.perf_counter()
    grid = generate_grid(configuration)
    tabulation_grid = generate_tabulation_grid(configuration)
    timings["time grid"] = time.perf_counter() - started_at

    started_at = time.perf_counter()
    orbit = tabulate_orbit(analytical_orbit, tabulation_grid, environment)
    timings["orbit tabulation"] = time.perf_counter() - started_at

    started_at = time.perf_counter()
    profile = Profile.local_orbital_frame_pointing(orbit, Orbit.FrameType.VVLH)
    timings["nadir pointing profile"] = time.perf_counter() - started_at

    started_at = time.perf_counter()
    simulator = build_simulator(environment, profile, configuration)
    timings["simulator"] = time.perf_counter() - started_at

    started_at = time.perf_counter()
    area_of_interest = AreaOfInterest.from_outlines("Japan", JAPAN_OUTLINES)
    timings["area of interest"] = time.perf_counter() - started_at

    evaluator = OverlapEvaluator(
        simulator=simulator,
        configuration=configuration,
        area_of_interest=area_of_interest,
        stage_timers=arguments.stage_timers,
    )

    print("Scenario")
    print("-" * 78)
    print(f"  epoch                    {configuration.epoch.to_string(Scale.UTC)}")
    print(
        f"  altitude                 {configuration.altitude_km:.1f} km (sun-synchronous)"
    )
    print(
        f"  LTAN                     {configuration.ltan_hours:02d}:{configuration.ltan_minutes:02d} "
        f"(LTDN {configuration.local_time_at_descending_node.to_string()})"
    )
    print(f"  attitude                 nadir pointing (VVLH)")
    print(
        f"  sensor field of view     {configuration.fov_degrees:.1f} deg square pyramid"
    )
    print(
        f"  area of interest         {area_of_interest.name} "
        f"({len(area_of_interest.polygons)} polygons, "
        f"{sum(p.get_vertex_count() for p in area_of_interest.polygons)} vertices)"
    )
    print(
        f"  horizon                  {configuration.duration_days:g} days @ {configuration.step_seconds:g} s"
    )
    print(f"  evaluation steps         {len(grid)}")
    print(f"  tabulated states         {len(tabulation_grid)}")
    print(f"  strategy                 {arguments.strategy}")

    print()
    print("Setup")
    print("-" * 78)
    for label, seconds in timings.items():
        print(f"  {label:<24} {format_duration(seconds):>12}")
    print(f"  {'total':<24} {format_duration(sum(timings.values())):>12}")

    strategies = {
        "full": evaluator.evaluate_full,
        "prefiltered": evaluator.evaluate_prefiltered,
        "tuned": evaluator.evaluate_tuned,
    }
    evaluate = strategies[arguments.strategy]

    def run_evaluation() -> list[bool]:
        return [evaluate(instant) for instant in grid]

    if arguments.profile:
        profiler = cProfile.Profile()
        profiler.enable()
        started_at = time.perf_counter()
        flags = run_evaluation()
        elapsed = time.perf_counter() - started_at
        profiler.disable()
    else:
        profiler = None
        started_at = time.perf_counter()
        flags = run_evaluation()
        elapsed = time.perf_counter() - started_at

    passes = collapse_to_passes(grid, flags)

    print()
    print("Evaluation")
    print("-" * 78)
    print(f"  wall clock               {format_duration(elapsed):>12}")
    print(f"  per step                 {format_duration(elapsed / len(grid)):>12}")
    print(f"  throughput               {len(grid) / elapsed:>9.0f} steps/s")
    print(f"  overlapping steps        {sum(flags):>12}")
    print(f"  distinct passes          {len(passes):>12}")

    if evaluator.prefilter_evaluations:
        rejected = evaluator.prefilter_rejections
        print(
            f"  prefilter rejections     {rejected:>12}"
            f"  ({rejected / evaluator.prefilter_evaluations * 100.0:.1f}% of steps)"
        )

    if passes:
        print()
        print("  first passes over the area of interest")
        for record in passes[:5]:
            print(
                f"    {record.start.to_string(Scale.UTC)} -> {record.end.to_string(Scale.UTC)} "
                f"({record.step_count * configuration.step_seconds:.0f} s)"
            )

    if arguments.stage_timers:
        print()
        print("Stage breakdown (in-loop timers)")
        print("-" * 78)
        print(f"{'  stage':<40}{'total':>12}{'calls':>10}{'us/call':>12}{'share':>9}")
        total = sum(evaluator.stage_seconds.values())
        for stage, seconds in sorted(
            evaluator.stage_seconds.items(), key=lambda item: -item[1]
        ):
            calls = evaluator.stage_calls[stage]
            per_call = seconds / calls * 1e6 if calls else 0.0
            share = seconds / total * 100.0 if total else 0.0
            print(
                f"  {stage:<38}{format_duration(seconds):>12}{calls:>10}{per_call:>12.1f}{share:>8.1f}%"
            )
        print(f"  {'measured total':<38}{format_duration(total):>12}")
        print(f"  {'unaccounted':<38}{format_duration(elapsed - total):>12}")

    if profiler is not None:
        stream = io.StringIO()
        statistics = pstats.Stats(profiler, stream=stream).sort_stats("tottime")
        statistics.print_stats(arguments.profile_rows)
        print()
        print("cProfile (sorted by total time)")
        print("-" * 78)
        print(stream.getvalue())

    if arguments.ablation:
        run_ablation(evaluator, grid, min(arguments.ablation_samples, len(grid)))

    if arguments.verify:
        print()
        print("Strategy cross-check")
        print("-" * 78)
        reference = flags if arguments.strategy == "full" else None
        results: dict[str, list[bool]] = {}
        timings_by_strategy: dict[str, float] = {arguments.strategy: elapsed}
        results[arguments.strategy] = flags

        for name, function in strategies.items():
            if name in results:
                continue
            started_at = time.perf_counter()
            results[name] = [function(instant) for instant in grid]
            timings_by_strategy[name] = time.perf_counter() - started_at

        reference = results["full"]
        print(
            f"{'  strategy':<20}{'wall clock':>14}{'us/step':>12}{'speed-up':>11}{'matches full':>15}"
        )
        baseline = timings_by_strategy["full"]
        for name in ("full", "prefiltered", "tuned"):
            seconds = timings_by_strategy[name]
            mismatches = sum(1 for a, b in zip(reference, results[name]) if a != b)
            verdict = "yes" if mismatches == 0 else f"no ({mismatches} steps)"
            print(
                f"  {name:<18}{format_duration(seconds):>14}"
                f"{seconds / len(grid) * 1e6:>12.1f}{baseline / seconds:>10.1f}x{verdict:>15}"
            )

    return 0


if __name__ == "__main__":
    sys.exit(main())
