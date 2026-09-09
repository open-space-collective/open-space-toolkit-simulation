/// Apache License 2.0

#include <OpenSpaceToolkitSimulationPy/Utility/ArrayCasting.hpp>
#include <OpenSpaceToolkitSimulationPy/Utility/EigenSequenceCasting.hpp>
#include <OpenSpaceToolkitSimulationPy/Utility/ShiftToString.hpp>
#include <nanobind/eigen/dense.h>
#include <nanobind/nanobind.h>
#include <nanobind/ndarray.h>
#include <nanobind/stl/array.h>
#include <nanobind/stl/complex.h>
#include <nanobind/stl/map.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/set.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/tuple.h>
#include <nanobind/stl/unique_ptr.h>
#include <nanobind/stl/unordered_map.h>
#include <nanobind/stl/unordered_set.h>
#include <nanobind/stl/vector.h>

#include <OpenSpaceToolkitSimulationPy/Component.cpp>
#include <OpenSpaceToolkitSimulationPy/Entity.cpp>
#include <OpenSpaceToolkitSimulationPy/Satellite.cpp>
#include <OpenSpaceToolkitSimulationPy/Simulator.cpp>
#include <OpenSpaceToolkitSimulationPy/Utility/ComponentHolder.cpp>

NB_MODULE(OpenSpaceToolkitSimulationPy, m)
{
    // Add optional docstring for package OpenSpaceToolkitSimulationPy
    m.doc() = "Elementary space systems blocks for Simulation in Open Space Toolkit.";

    // Change attribute __name__ to make OpenSpaceToolkitSimulationPy invisible in import path
    m.attr("__name__") = "ostk.simulation";

// Package version information
#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif

    OpenSpaceToolkitSimulationPy_Utility_ComponentHolder(m);

    OpenSpaceToolkitSimulationPy_Simulator(m);
    OpenSpaceToolkitSimulationPy_Entity(m);
    OpenSpaceToolkitSimulationPy_Component(m);
    OpenSpaceToolkitSimulationPy_Satellite(m);
}
