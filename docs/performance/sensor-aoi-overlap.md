# Performance profile: sensor / area-of-interest overlap over 7 days

Scenario, measurements and conclusions for a 7-day, 20-second evaluation of
"does a nadir-pointing satellite sensor overlap a ground area of interest?".

Everything below is reproduced by
[`tutorials/python/sensor_aoi_overlap_profiling.py`](../../tutorials/python/sensor_aoi_overlap_profiling.py).

## Scenario

| | |
| --- | --- |
| Orbit | Sun-synchronous, 550 km altitude, 10:00 LTAN (22:00 LTDN) |
| Ephemeris | `Orbit.sun_synchronous` sampled on the 20 s grid, re-wrapped as `orbit.model.Tabulated` |
| Attitude | Nadir pointing — `Profile.local_orbital_frame_pointing(orbit, Orbit.FrameType.VVLH)` |
| Sensor | One `Component.Type.Sensor` holding a square `Pyramid` field of view, 30° full angle |
| Area of interest | Simplified outlines of the four main islands of Japan (4 polygons, 53 vertices), ITRF |
| Horizon | 7 days at 20 s → **30 241 evaluation steps** |

Result of the evaluation: **116 overlapping steps across 16 distinct passes**, the
first being `2024-01-01 00:09:20Z → 00:11:40Z` (160 s).

Measured on a 4-core Intel Xeon @ 2.10 GHz, Python 3.11.15, single threaded, with
`open-space-toolkit-simulation` 4.1.5 / `-astrodynamics` 17.15.0 / `-physics` 14.4.0 /
`-mathematics` 4.8.4.

## Headline numbers

| Phase | Wall clock |
| --- | --- |
| Setup (environment, SSO orbit, 30 249-state tabulation, profile, simulator, AOI) | **212 ms** |
| Evaluation, straightforward implementation | **22.97 s** (760 µs/step, 1 316 steps/s) |
| Evaluation, with a great-circle prefilter | **2.59 s** (86 µs/step, 8.4× faster) |

Setup is not the problem. Tabulating the orbit — 30 249 states sampled from the
analytical SSO model and handed to `Tabulated` — costs 192 ms once, and buys a
**1.4 µs** interpolated state lookup afterwards. Everything that matters happens
in the 30 241-step evaluation loop.

## Where the time goes

### In-loop stage timers (7 days, straightforward implementation)

```
  stage                                        total     calls     us/call    share
  ring_to_lonlat                            13.175 s     30241       435.7    58.6%
  intersection_with_earth                    6.282 s     30241       207.7    27.9%
  sensor_position                            2.688 s     30241        88.9    11.9%
  in_frame_itrf                            311.07 ms     30241        10.3     1.4%
  set_instant                               41.27 ms     30241         1.4     0.2%
  polygon_test                               2.15 ms       201        10.7     0.0%
```

### Incremental cost per stage (disjoint instants, cold frame cache)

```
stage                                          us/step    delta us
loop overhead only                                 0.0         0.0
+ simulator.set_instant                            0.2         0.2
+ component frame origin in ITRF                  71.6        71.4
+ geometry.intersection_with(Earth)              275.1       203.6
+ footprint.in_frame(ITRF)                       290.3        15.2
+ lon/lat conversion + polygon test              736.1       445.8
```

### cProfile, 7 days (30.8 s under the profiler vs 23.0 s without)

```
         28745706 function calls in 30.775 seconds

   ncalls  tottime  percall  cumtime  percall filename:lineno(function)
    30241    7.542    0.000   27.501    0.001 sensor_aoi_overlap_profiling.py:536(_footprint_ring)
    30241    6.522    0.000    6.522    0.000 {ostk.simulation.component.intersection_with}
  2449521    3.201    0.000    3.201    0.000 {ostk.physics.coordinate.spherical.cartesian}
    30241    2.946    0.000    2.946    0.000 {ostk.physics.coordinate.get_origin_in}
  4899042    2.322    0.000    2.322    0.000 {ostk.physics.unit.in_degrees}
  2419280    1.549    0.000    1.549    0.000 {ostk.mathematics.geometry.d3.object.access_point_at}
  2419280    1.278    0.000    1.278    0.000 {ostk.mathematics.geometry.d3.object.as_vector}
  2449521    1.226    0.000    1.226    0.000 {ostk.physics.coordinate.spherical.get_longitude}
  2449521    1.107    0.000    1.107    0.000 {ostk.physics.coordinate.spherical.get_latitude}
    60482    0.928    0.000    1.424    0.000 sensor_aoi_overlap_profiling.py:412(_unwrap_longitudes)
  7197358    0.521    0.000    0.521    0.000 {method 'append' of 'list' objects}
  2419280    0.281    0.000    0.281    0.000 {builtins.round}
```

**28.7 million Python calls for 30 241 steps — 950 calls per step.**

## Bottleneck 1 — converting the footprint to lon/lat (59% of the run)

`Geometry.intersection_with(Earth)` returns the pyramid/ellipsoid intersection as
**two** `LineString`s of 40 points each: the near-side ring (the footprint) and the
far-side ring where the field-of-view rays leave the ellipsoid on the opposite limb.
The straightforward implementation converts all 80 points before deciding which ring
it wants:

```python
for point_index in range(line_string.get_point_count()):
    lla = LLA.cartesian(line_string.access_point_at(point_index).as_vector(), radius, flattening)
    longitudes.append(float(lla.get_longitude().in_degrees()))
    latitudes.append(float(lla.get_latitude().in_degrees()))
```

That is six binding crossings per point, 80 points per step, 2.4 million crossings over
the run: `access_point_at` + `as_vector` + `LLA.cartesian` + `get_longitude` +
`get_latitude` + two `in_degrees`. Summed over the profile: **10.7 s of binding time
plus 7.5 s of Python loop overhead — 18 s of the 30.8 s profiled run**.

None of it is numerical work. Each `in_degrees` call allocates a Python object to carry
one float across the boundary.

Three fixes, in increasing order of payoff:

1. **Pick the ring before converting it.** The near-side ring is the one whose first
   point is closest to the sensor, which is one subtraction and one dot product in
   Cartesian space. Halves the conversions.
2. **Convert the ring with NumPy.** The footprint points lie *on* the ellipsoid, so the
   geodetic conversion is closed form and vectorises:
   `lon = atan2(y, x)`, `lat = atan2(z, (1 − e²)·hypot(x, y))`. Agrees with
   `LLA.cartesian` to 3e-14° (the residual is float noise), and takes the stage from
   433 µs to 302 µs per step.
3. **Add a bulk accessor upstream.** Even with NumPy, the remaining cost is 40
   `access_point_at().as_vector()` round trips per step (~0.9 µs each). A
   `LineString.get_coordinates() -> numpy.ndarray[N, 3]` binding in
   `open-space-toolkit-mathematics` would collapse the whole stage to a single crossing.
   This is the single highest-leverage library change for workloads like this one.

## Bottleneck 2 — the pyramid / ellipsoid intersection (28% of the run)

`intersection_with(Earth)` costs 208 µs/step. `Pyramid::intersectionWith(Ellipsoid)`
discretises the lateral faces into rays and intersects each with the ellipsoid, and the
cost is exactly linear in the discretisation level:

| `discretization_level` | rays | `intersection_with` | `intersects` |
| --- | --- | --- | --- |
| 4 | 16 | 15.1 µs | 6.4 µs |
| 8 | 32 | 27.7 µs | 9.5 µs |
| 16 | 64 | 48.8 µs | 16.7 µs |
| **40 (default)** | **160** | **114.9 µs** | **32.6 µs** |
| 80 | 320 | 225.2 µs | 60.9 µs |

The default of 40 gives a 160-point footprint boundary. For an overlap *decision*
against a 53-vertex AOI, a level of 8 would be plenty — it is 4× cheaper and still
resolves the footprint to ~10 km. But **there is no way to ask for it**: the level is
defaulted inside `Object::intersects`/`Object::intersectionWith` dispatch in
`open-space-toolkit-mathematics`, and neither
`ostk::physics::environment::object::Geometry` nor
`ostk::simulation::component::Geometry` exposes it. Threading a discretisation
parameter through those two `Geometry` classes would make this stage tunable.

## Bottleneck 3 — the GCRF → ITRF transform (12%, and the floor)

Isolated, on cold instants:

| Operation | Cost |
| --- | --- |
| `orbit.get_state_at` (tabulated interpolation) | 1.4 µs |
| `profile.get_state_at` (position + attitude) | 13.9 µs |
| `component_frame.get_origin_in(GCRF)` | 24.4 µs |
| **`GCRF → ITRF` transform** | **52.6 µs** |
| `component_frame.get_origin_in(ITRF)` | 71.4 µs |

Anything expressed in ITRF pays ~53 µs per distinct instant for the Earth-orientation
chain. It is cached per instant by the frame manager — asking for it twice at the same
instant is free, which is why a naive ablation produces *negative* deltas if successive
stages reuse the same instants — but on a 30 241-instant walk each instant is computed
exactly once. Over the run that is **1.6 s that no amount of restructuring removes**,
short of not working in ITRF.

A one-time cost of ~114 ms is also paid on the first ITRF use, loading and indexing the
IERS finals-2000A data.

## Bottleneck 4 — nothing, for the parts people expect to be slow

Worth stating explicitly, because it redirects optimisation effort:

- `simulator.set_instant` is **1.4 µs**. It only stores the instant; the frames pull
  state lazily. Stepping the simulator is free.
- The tabulated orbit lookup is **1.4 µs**. Paying 192 ms once to tabulate 30 249 states
  was the right trade.
- The actual overlap answer — `Polygon2d.intersects` against 4 polygons — is **10.7 µs**
  and runs 201 times in the whole 7-day run. It is 0.0% of the profile. The geometry
  question being asked is not what costs anything; getting the data into a shape where
  it can be asked is.

## What actually fixes it: reject early

The evaluation is a rare-event search — 116 hits out of 30 241 steps, 0.4%. A
great-circle test between the sub-satellite point and a bounding cap around the AOI
(AOI angular radius 9.36° + footprint angular radius 1.88° + 0.5° margin) rejects
**99.1% of steps** before any geometry is touched:

```
  strategy              wall clock     us/step   speed-up   matches full
  full                    21.690 s       717.2       1.0x            yes
  prefiltered              2.586 s        85.5       8.4x            yes
  tuned                    2.658 s        87.9       8.2x            yes
```

All three agree on all 30 241 steps.

Note that `tuned` — prefilter *plus* single-ring NumPy conversion — is **not** faster
than `prefiltered`. Once 99.1% of steps are rejected, the expensive path runs 261 times
and its cost is invisible; what remains is 30 241 × ~80 µs of sub-satellite position in
ITRF. **After the prefilter, the profile is entirely Bottleneck 3.** Optimising the
footprint conversion is only worth doing if you have not prefiltered.

Further headroom, in order:

1. **Coarse-to-fine time stepping.** A 550 km orbit crosses a 10°-radius cap in roughly
   5 minutes. Prefiltering on a 60 s grid and refining to 20 s only inside candidate
   windows cuts the ITRF transforms by ~3×, taking the run toward ~0.9 s.
2. **Batch the ground track.** One vectorised pass computing all sub-satellite points,
   then a NumPy cap test over the whole array, replaces 30 241 Python-level
   `get_origin_in` calls with 30 241 C++ transforms and one array operation.
3. **Reuse the process.** The 114 ms IERS initialisation and 192 ms tabulation are
   per-process, not per-query. For repeated AOI queries against the same ephemeris they
   amortise to nothing.

## Two library issues found while profiling

### `Geometry.access_composite()` segfaults on a temporary

```python
composite = geometry.intersection_with(earth).access_composite()   # segfault on use
```

`access_composite` is bound with `return_value_policy::reference` and no `keep_alive`,
so the returned `Composite` is a bare reference into a parent that Python is free to
collect. The parent must be held in a live variable:

```python
geometry = sensor_geometry.intersection_with(earth)   # keep this alive
composite = geometry.access_composite()               # fine
```

The crash goes through the `open-space-toolkit-physics` binding
(`bindings/python/src/OpenSpaceToolkitPhysicsPy/Environment/Object/Geometry.cpp`), since
`intersection_with` returns an `environment.object.Geometry` **by value**. This repository
binds `component::Geometry::accessComposite` the same way
(`bindings/python/src/OpenSpaceToolkitSimulationPy/Component/Geometry.cpp:185-188`); the
consequences there are milder only because the parent is normally kept alive by the
`Component` that owns it. Both should use `return_value_policy::reference_internal`
(or add `py::keep_alive<0, 1>()`).

### `Pyramid::intersects` is a lateral-face test, not a containment test

`Pyramid::intersects(Ellipsoid)` walks the rays of the lateral *faces* and returns true
if any of them hits. An object entirely inside the field-of-view cone — which is exactly
the case for a small AOI under a wide sensor — is reported as **not** intersecting:

```python
pyramid.intersects(Ellipsoid(Point(0.0, 0.0, 3.0), 4.0, 4.0, 4.0))   # True  (crosses a face)
pyramid.intersects(Ellipsoid(Point(0.0, 0.0, 3.0), 1.0, 1.0, 1.0))   # False (fully inside)
```

This works for the Earth (always larger than the field of view) but silently gives false
negatives for a modelled AOI. It is the reason this scenario computes the footprint
against the Earth ellipsoid and tests the overlap in lon/lat, rather than modelling the
AOI as geometry and calling `intersects` on it. Worth either fixing or documenting.

Separately, `Composite.intersects(other_composite)` from Python resolves to the
`Object` overload (a `Composite` *is* an `Object`, and pybind11 tries that overload
first), which then dispatches pairs the kernel does not implement and raises
`{Object :: intersects} is not yet implemented`. Passing the bare object instead of
wrapping it in a `Composite` takes the supported path.

## Reproducing

```bash
python tutorials/python/sensor_aoi_overlap_profiling.py --duration-days 7            # baseline
python tutorials/python/sensor_aoi_overlap_profiling.py --duration-days 7 --stage-timers
python tutorials/python/sensor_aoi_overlap_profiling.py --duration-days 7 --profile
python tutorials/python/sensor_aoi_overlap_profiling.py --duration-days 7 --strategy tuned --verify --ablation
```

Set `OSTK_PHYSICS_DATA_LOCAL_REPOSITORY` to a populated data directory and `OSTK_OFFLINE=1`
to pin every data manager to manual mode and run without network access.
