////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Simulation
/// @file           bindings/python/src/OpenSpaceToolkitSimulationPy.cxx
/// @author         Lucas Brémond <lucas@loftorbital.com>, Remy Derollez <remy@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

#include <OpenSpaceToolkitSimulationPy/Utilities/ShiftToString.hpp>

#include <OpenSpaceToolkitSimulationPy/Satellite.cpp>
#include <OpenSpaceToolkitSimulationPy/Component.cpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

PYBIND11_MODULE (OpenSpaceToolkitSimulationPy, m)
{

    // Add optional docstring for package OpenSpaceToolkitSimulationPy
    m.doc() = "Elementary space systems blocks for Simulation in OpenSpaceToolkit." ;

    // Add __path__ attribute to python package
    m.attr("__path__") = "ostk.simulation" ;

    // Change attribute __name__ to make OpenSpaceToolkitSimulationPy invisible in import path
    m.attr("__name__") = "ostk.simulation" ;

    // Package version information
    #ifdef VERSION_INFO
        m.attr("__version__") = VERSION_INFO ;
    #else
        m.attr("__version__") = "dev" ;
    #endif

    // Add python submodules to OpenSpaceToolkitSimulationPy
    OpenSpaceToolkitSimulationPy_Satellite(m) ;
    OpenSpaceToolkitSimulationPy_Component(m) ;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
