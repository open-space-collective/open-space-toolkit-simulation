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

                                ComponentHolder::ComponentHolder            ( )
                                :   componentMap_()
{

}

                                ComponentHolder::ComponentHolder            (   const   Array<Unique<Component>>&   aComponentArray                             )
                                :   componentMap_()
{

    for (const auto& componentUPtr : aComponentArray)
    {

        if (componentUPtr == nullptr)
        {
            throw ostk::core::error::runtime::Undefined("Component is undefined.") ;
        }

        componentMap_.insert({ componentUPtr->getName(), Shared<Component>(componentUPtr->clone()) }) ;

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

ComponentHolder&                ComponentHolder::operator =                 (   const   ComponentHolder&            aComponentHolder                            )
{

    if (this != &aComponentHolder)
    {

        componentMap_.clear() ;

        for (const auto& componentMapIt : aComponentHolder.componentMap_)
        {
            componentMap_.insert({ componentMapIt.first, Shared<Component>(componentMapIt.second->clone()) }) ;
        }

    }

    return *this ;

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

void                            ComponentHolder::addComponent               (   const   Component&                  aComponent                                  )
{

    if (!aComponent.isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Component") ;
    }

    componentMap_.insert({ aComponent.getName(), Shared<Component>(aComponent.clone()) }) ;

}

Component&                      ComponentHolder::accessComponentWithName    (   const   String&                     aComponentName                              ) const
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

Array<Component>                ComponentHolder::accessComponentsWithTag    (   const   String&                     aComponentTag                               ) const
{

    if (aComponentTag.isEmpty())
    {
        throw ostk::core::error::runtime::Undefined("Component tag") ;
    }

    Array<Component> arrayComponents = Array<Component>::Empty() ;

    for (const auto& componentMapIt : this->componentMap_)
    {

        Array<String> componentTags = componentMapIt.second->getTags() ;

        bool tagPresent = std::find(std::begin(componentTags), std::end(componentTags), aComponentTag) != std::end(componentTags) ;

        if (tagPresent == true)
        {
            arrayComponents.add(*(componentMapIt.second)) ;
        }

    }

    return arrayComponents ;

}

Component&                      ComponentHolder::accessComponentAt          (   const   String&                     aComponentPath                              ) const
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

    Component& Comp = accessComponentWithName(componentName) ;

    return subComponentPath.isEmpty() ? Comp : Comp.accessComponentAt(subComponentPath) ;

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
