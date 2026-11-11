# Apache License 2.0

"""Calls that the bindings accept, and that the generated type stubs must accept too.

The bindings convert some Python values implicitly, for example a `str` where a `String` is
declared. Each test makes such a call. `test_stubs_accept_these_calls` type-checks this file
with mypy against the generated stubs, so a stub that rejects one of these calls fails.
"""

from pathlib import Path

import numpy as np
import pytest

import ostk.simulation
from ostk.mathematics.geometry.d3.transformation.rotation import Quaternion
from ostk.physics.coordinate import Frame
from ostk.physics.coordinate import Position
from ostk.astrodynamics import Trajectory
from ostk.astrodynamics.flight import Profile
from ostk.simulation import SatelliteConfiguration


def test_string_parameters_accept_str() -> None:
    profile = Profile.constant_inertial_pointing(
        Trajectory.position(
            Position.meters(np.array([7000.0e3, 0.0, 0.0]), Frame.ITRF())
        ),
        Quaternion.unit(),
    )
    tags: list[str] = ["a", "b"]

    satellite_configuration = SatelliteConfiguration(
        id="9ea22c07-6977-48a7-8f68-dff758971d57",
        name="LoftSat-1",
        profile=profile,
        tags=tags,
    )

    assert isinstance(satellite_configuration, SatelliteConfiguration)


def test_stubs_accept_these_calls(
    tmp_path: Path, monkeypatch: pytest.MonkeyPatch
) -> None:
    mypy_api = pytest.importorskip("mypy.api")

    # In the CMake build tree, the package is a directory next to `test`, not an installed
    # distribution: mypy only finds it through MYPYPATH.
    package_root: Path = Path(ostk.simulation.__file__).resolve().parents[2]

    if package_root.name != "site-packages":
        monkeypatch.setenv("MYPYPATH", str(package_root))

    # Report the errors in the calls of this file only: the stubs themselves are not checked,
    # as for an installed package.
    config: Path = tmp_path / "mypy.ini"
    config.write_text("[mypy]\n[mypy-ostk.*]\nignore_errors = True\n")

    stdout, stderr, exit_status = mypy_api.run(
        [
            __file__,
            "--config-file",
            str(config),
            "--cache-dir",
            str(tmp_path / "mypy_cache"),
        ]
    )

    assert exit_status == 0, stdout + stderr
