////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Simulation
/// @file           OpenSpaceToolkit/Simulation/Utilities/ComponentHolder.hpp
/// @author         Lucas Brémond <lucas@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __OpenSpaceToolkit_Simulation_Utilties_ComponentHolder__
#define __OpenSpaceToolkit_Simulation_Utilties_ComponentHolder__

#include <OpenSpaceToolkit/Core/Containers/Map.hpp>
#include <OpenSpaceToolkit/Core/Containers/Array.hpp>
#include <OpenSpaceToolkit/Core/Containers/Pair.hpp>
#include <OpenSpaceToolkit/Core/Types/Shared.hpp>
#include <OpenSpaceToolkit/Core/Types/Unique.hpp>
#include <OpenSpaceToolkit/Core/Types/String.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace ostk
{
namespace simulation
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Component ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace utilities
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using ostk::core::types::String ;
using ostk::core::types::Unique ;
using ostk::core::types::Shared ;
using ostk::core::ctnr::Pair ;
using ostk::core::ctnr::Array ;
using ostk::core::ctnr::Map ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief                      Generic Component holder

class ComponentHolder
{

    public:

                                ComponentHolder                             ( ) ;

                                ComponentHolder                             (   const   Array<Unique<Component>>&   aComponentArray                             ) ;

                                ComponentHolder                             (   const   ComponentHolder&            aComponentHolder                            ) ;

                                ~ComponentHolder                            ( ) ;

        ComponentHolder&        operator =                                  (   const   ComponentHolder&            aComponentHolder                            ) ;

        bool                    hasComponentWithName                        (   const   String&                     aComponentName                              ) const ;

        bool                    hasComponentAt                              (   const   String&                     aComponentPath                              ) const ;

        Component&              accessComponentWithName                     (   const   String&                     aComponentName                              ) const ;

        Component&              accessComponentAt                           (   const   String&                     aComponentPath                              ) const ;

        void                    addComponent                                (   const   Component&                  aComponent                                  ) ;

    private:

        Map<String, Shared<Component>> componentMap_ ;

} ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Pair<String, String>            splitComponentPath                          (   const    String&                    aComponentPath                              ) ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
