/// Apache License 2.0

#include <OpenSpaceToolkit/Simulation/Utility/ComponentHolder.hpp>
#include <OpenSpaceToolkitSimulationPy/Utility/ComponentHolderMixin.hpp>

inline void OpenSpaceToolkitSimulationPy_Utility_ComponentHolder(nanobind::module_& aModule)
{
    using namespace nanobind;

    using ostk::simulation::utility::ComponentHolder;

    class_<ComponentHolder> componentHolderClass(
        aModule,
        "ComponentHolder",
        R"doc(
            Mixin class providing component management functionality for hierarchical component structures.

            ComponentHolder enables storage and retrieval of child components, supporting both flat
            and hierarchical component organization with path-based access.
        )doc"
    );

    OpenSpaceToolkitSimulationPy_Utility_ComponentHolder_AddMethods(componentHolderClass);
}
