////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Simulation
/// @file           OpenSpaceToolkit/Simulation/ComponentHolder.cpp
/// @author         Lucas Brémond <lucas@loftorbital.com>, Remy Derollez <remy@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <OpenSpaceToolkit/Simulation/Component.hpp>
#include <OpenSpaceToolkit/Simulation/Utilities/ComponentHolder.hpp>

#include <OpenSpaceToolkit/Core/Types/Index.hpp>
#include <OpenSpaceToolkit/Core/Error.hpp>
#include <OpenSpaceToolkit/Core/Utilities.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace ostk
{
namespace simulation
{
namespace utilities
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                                ComponentHolder::ComponentHolder            (   const   Array<Shared<Component>>&   aComponentArray                             )
                                :   componentMap_()
{

    for (const auto& componentSPtr : aComponentArray)
    {
        componentMap_.insert({ componentSPtr->getName(), componentSPtr }) ;
    }

}

                                ComponentHolder::ComponentHolder            (   const   ComponentHolder&            aComponentHolder                            )
                                :   componentMap_()
{

    for (const auto& componentMapIt : aComponentHolder.componentMap_)
    {
        componentMap_.insert({ componentMapIt.first, Shared<Component>(componentMapIt.second->clone()) }) ;
    }

}

                                ComponentHolder::~ComponentHolder           ( )
{

}

bool                            ComponentHolder::hasComponentWithId         (   const   String&                     aComponentId                                ) const
{

    if (aComponentId.isEmpty())
    {
        throw ostk::core::error::runtime::Undefined("Component ID") ;
    }

    for (const auto& componentMapIt : this->componentMap_)
    {
        if (componentMapIt.second->getId() == aComponentId)
        {
            return true ;
        }
    }

    return false ;

}

bool                            ComponentHolder::hasComponentWithName       (   const   String&                     aComponentName                              ) const
{

    if (aComponentName.isEmpty())
    {
        throw ostk::core::error::runtime::Undefined("Component name") ;
    }

    return componentMap_.find(aComponentName) != componentMap_.end() ;

}

bool                            ComponentHolder::hasComponentAt             (   const   String&                     aComponentPath                              ) const
{

    if (aComponentPath.isEmpty())
    {
        throw ostk::core::error::runtime::Undefined("Component path") ;
    }

    const auto [ componentName, subComponentPath ] = splitComponentPath(aComponentPath) ;

    if (!this->hasComponentWithName(componentName))
    {
        return false ;
    }

    return subComponentPath.isEmpty() ? true : this->accessComponentWithName(componentName).hasComponentAt(subComponentPath) ;

}

void                            ComponentHolder::addComponent               (   const   Shared<Component>&          aComponentSPtr                              )
{

    if ((!aComponentSPtr) || (!aComponentSPtr->isDefined()))
    {
        throw ostk::core::error::runtime::Undefined("Component") ;
    }

    componentMap_.insert({ aComponentSPtr->getName(), aComponentSPtr }) ;

}

Array<Shared<Component>>        ComponentHolder::accessComponents           ( ) const
{

    // TBM: Return iterator instead, to avoid computing the Array.

    Array<Shared<Component>> components = Array<Shared<Component>>::Empty() ;

    for (const auto& componentMapIt : this->componentMap_)
    {
        components.add(componentMapIt.second) ;
    }

    return components ;

}

const Component&                ComponentHolder::accessComponentWithId      (   const   String&                     aComponentId                                ) const
{

    if (aComponentId.isEmpty())
    {
        throw ostk::core::error::runtime::Undefined("Component ID") ;
    }

    for (const auto& componentMapIt : this->componentMap_)
    {
        if (componentMapIt.second->getId() == aComponentId)
        {
            return *(componentMapIt.second) ;
        }
    }

    throw ostk::core::error::RuntimeError("No Component found with ID [{}].", aComponentId) ;

}

const Component&                ComponentHolder::accessComponentWithName    (   const   String&                     aComponentName                              ) const
{

    if (aComponentName.isEmpty())
    {
        throw ostk::core::error::runtime::Undefined("Component name") ;
    }

    const auto componentIt = componentMap_.find(aComponentName) ;

    if (componentIt == componentMap_.end())
    {
        throw ostk::core::error::RuntimeError("No Component found with name [{}].", aComponentName) ;
    }

    return *(componentIt->second) ;

}

Array<Shared<const Component>>  ComponentHolder::accessComponentsWithTag    (   const   String&                     aComponentTag                               ) const
{

    if (aComponentTag.isEmpty())
    {
        throw ostk::core::error::runtime::Undefined("Component tag") ;
    }

    Array<Shared<const Component>> components = Array<Shared<const Component>>::Empty() ;

    for (const auto& componentMapIt : this->componentMap_)
    {

        Array<String> componentTags = componentMapIt.second->getTags() ;

        bool tagPresent = std::find(std::begin(componentTags), std::end(componentTags), aComponentTag) != std::end(componentTags) ;

        if (tagPresent == true)
        {
            components.add(componentMapIt.second) ;
        }

    }

    return components ;

}

const Component&                ComponentHolder::accessComponentAt          (   const   String&                     aComponentPath                              ) const
{

    if (aComponentPath.isEmpty())
    {
        throw ostk::core::error::runtime::Undefined("Component path") ;
    }

    if (!this->hasComponentAt(aComponentPath))
    {
        throw ostk::core::error::RuntimeError("No Component found with path [{}].", aComponentPath) ;
    }

    const auto [ componentName, subComponentPath ] = splitComponentPath(aComponentPath) ;

    const Component& component = accessComponentWithName(componentName) ;

    return subComponentPath.isEmpty() ? component : component.accessComponentAt(subComponentPath) ;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Pair<String, String>            splitComponentPath                          (   const    String&                    aComponentPath                              )
{

    using ostk::core::types::Index ;

    if (aComponentPath.getFirst() == '/')
    {
        return splitComponentPath(aComponentPath.getTail(aComponentPath.getLength() - 1)) ;
    }

    const Index tokenPosition = aComponentPath.find('/') ;

    if (tokenPosition == std::string::npos)
    {
        return { aComponentPath, "" } ;
    }

    if ((aComponentPath.getLength() - tokenPosition) == 1)
    {
        return { aComponentPath.getHead(tokenPosition), "" } ;
    }

    return { aComponentPath.getHead(tokenPosition), aComponentPath.getSubstring(tokenPosition + 1, aComponentPath.getLength() - tokenPosition - 1) } ;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
