/// Apache License 2.0

#include <OpenSpaceToolkitSimulationPy/Utility/ArrayCasting.hpp>
#include <OpenSpaceToolkitSimulationPy/Utility/ShiftToString.hpp>
#include <pybind11/eigen.h>
#include <pybind11/numpy.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <OpenSpaceToolkitSimulationPy/Component.cpp>
#include <OpenSpaceToolkitSimulationPy/Entity.cpp>
#include <OpenSpaceToolkitSimulationPy/Satellite.cpp>
#include <OpenSpaceToolkitSimulationPy/Simulator.cpp>
#include <OpenSpaceToolkitSimulationPy/Utility/ComponentHolder.cpp>

PYBIND11_MODULE(OpenSpaceToolkitSimulationPy, m)
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
