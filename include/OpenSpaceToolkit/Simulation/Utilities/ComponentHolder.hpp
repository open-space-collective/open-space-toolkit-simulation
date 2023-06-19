/// Apache License 2.0

#ifndef __OpenSpaceToolkit_Simulation_Utilties_ComponentHolder__
#define __OpenSpaceToolkit_Simulation_Utilties_ComponentHolder__

#include <OpenSpaceToolkit/Core/Containers/Map.hpp>
#include <OpenSpaceToolkit/Core/Containers/Array.hpp>
#include <OpenSpaceToolkit/Core/Containers/Pair.hpp>
#include <OpenSpaceToolkit/Core/Types/Shared.hpp>
#include <OpenSpaceToolkit/Core/Types/Unique.hpp>
#include <OpenSpaceToolkit/Core/Types/String.hpp>

namespace ostk
{
namespace simulation
{

class Component ;
class ComponentConfiguration ;

namespace utilities
{

using ostk::core::types::String ;
using ostk::core::types::Unique ;
using ostk::core::types::Shared ;
using ostk::core::ctnr::Pair ;
using ostk::core::ctnr::Array ;
using ostk::core::ctnr::Map ;

/// @brief                      Generic Component holder

class ComponentHolder
{

    public:

                                ComponentHolder                             (   const   Array<Shared<Component>>&   aComponentArray                             =   Array<Shared<Component>>::Empty() ) ;

                                ComponentHolder                             (   const   ComponentHolder&            aComponentHolder                            ) ;

                                ~ComponentHolder                            ( ) ;

        bool                    hasComponentWithId                          (   const   String&                     aComponentId                                ) const ;

        bool                    hasComponentWithName                        (   const   String&                     aComponentName                              ) const ;

        bool                    hasComponentAt                              (   const   String&                     aComponentPath                              ) const ;

        Array<Shared<Component>> accessComponents                           ( ) const ;

        const Component&        accessComponentWithId                       (   const   String&                     aComponentId                                ) const ;

        const Component&        accessComponentWithName                     (   const   String&                     aComponentName                              ) const ;

        Array<Shared<const Component>> accessComponentsWithTag              (   const   String&                     aComponentTag                               ) const ;

        const Component&        accessComponentAt                           (   const   String&                     aComponentPath                              ) const ;

        void                    addComponent                                (   const   Shared<Component>&          aComponentSPtr                              ) ;

    private:

        Map<String, Shared<Component>> componentMap_ ;

} ;

Pair<String, String>            splitComponentPath                          (   const    String&                    aComponentPath                              ) ;

}
}
}

#endif
