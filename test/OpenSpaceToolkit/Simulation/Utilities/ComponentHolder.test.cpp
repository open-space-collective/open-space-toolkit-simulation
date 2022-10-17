////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Simulation
/// @file           OpenSpaceToolkit/Simulation/Utilities/ComponentHolder.test.cpp
/// @author         Lucas Brémond <lucas@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <OpenSpaceToolkit/Simulation/Component.hpp>
#include <OpenSpaceToolkit/Simulation/Utilities/ComponentHolder.hpp>

#include <Global.test.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST (OpenSpaceToolkit_Simulation_Utilities_ComponentHolder, Constructor)
{

    using ostk::core::types::Unique ;
    using ostk::core::ctnr::Array ;

    using ostk::simulation::Component ;
    using ostk::simulation::utilities::ComponentHolder ;

    {

        EXPECT_NO_THROW(ComponentHolder componentHolder() ;) ;

    }

    {

        Array<Unique<Component>> components = Array<Unique<Component>>::Empty() ;

        components.emplace_back(Unique<Component>(Component("Component A", Component::Type::Sensor).clone())) ;
        components.emplace_back(Unique<Component>(Component("Component B", Component::Type::Actuator).clone())) ;

        EXPECT_NO_THROW(ComponentHolder componentHolder(components) ;) ;

    }

}

TEST (OpenSpaceToolkit_Simulation_Utilities_ComponentHolder, CopyConstructor)
{

    using ostk::core::types::Unique ;
    using ostk::core::ctnr::Array ;

    using ostk::simulation::Component ;
    using ostk::simulation::utilities::ComponentHolder ;

    {

        Array<Unique<Component>> components = Array<Unique<Component>>::Empty() ;

        components.emplace_back(Unique<Component>(Component("Component A", Component::Type::Sensor).clone())) ;
        components.emplace_back(Unique<Component>(Component("Component B", Component::Type::Actuator).clone())) ;

        const ComponentHolder componentHolder = { components } ;

        EXPECT_TRUE(componentHolder.hasComponentWithName("Component A")) ;
        EXPECT_TRUE(componentHolder.hasComponentWithName("Component B")) ;

        const ComponentHolder componentHolderCopy(componentHolder) ;

        EXPECT_TRUE(componentHolderCopy.hasComponentWithName("Component A")) ;
        EXPECT_TRUE(componentHolderCopy.hasComponentWithName("Component B")) ;

        EXPECT_NE(&(componentHolder.accessComponentWithName("Component A")), &(componentHolderCopy.accessComponentWithName("Component A"))) ;
        EXPECT_NE(&(componentHolder.accessComponentWithName("Component B")), &(componentHolderCopy.accessComponentWithName("Component B"))) ;

    }

}

TEST (OpenSpaceToolkit_Simulation_Utilities_ComponentHolder, HasComponentWithName)
{

    using ostk::core::types::String ;
    using ostk::core::types::Unique ;
    using ostk::core::ctnr::Array ;

    using ostk::simulation::Component ;
    using ostk::simulation::utilities::ComponentHolder ;

    {

        Array<Unique<Component>> components = Array<Unique<Component>>::Empty() ;

        components.emplace_back(Unique<Component>(Component("Component A", Component::Type::Sensor).clone())) ;
        components.emplace_back(Unique<Component>(Component("Component B", Component::Type::Actuator).clone())) ;

        const ComponentHolder componentHolder = { components } ;

        EXPECT_TRUE(componentHolder.hasComponentWithName("Component A")) ;
        EXPECT_TRUE(componentHolder.hasComponentWithName("Component B")) ;
        EXPECT_FALSE(componentHolder.hasComponentWithName("Component C")) ;

    }

    {

        Array<Unique<Component>> components = Array<Unique<Component>>::Empty() ;

        components.emplace_back(Unique<Component>(Component("Component A", Component::Type::Sensor).clone())) ;
        components.emplace_back(Unique<Component>(Component("Component B", Component::Type::Actuator).clone())) ;

        const ComponentHolder componentHolder = { components } ;

        EXPECT_ANY_THROW(componentHolder.hasComponentWithName(String::Empty())) ;

    }

}

TEST (OpenSpaceToolkit_Simulation_Utilities_ComponentHolder, HasComponentAt)
{

    using ostk::core::types::String ;
    using ostk::core::types::Unique ;
    using ostk::core::ctnr::Array ;

    using ostk::simulation::Component ;
    using ostk::simulation::utilities::ComponentHolder ;

    {

        ComponentHolder rootComponentHolder = {} ;

        rootComponentHolder.addComponent(Component("A", Component::Type::Other)) ;
        rootComponentHolder.addComponent(Component("B", Component::Type::Other)) ;

        Component subComponent = { "C", Component::Type::Other } ;

        subComponent.addComponent(Component("C1", Component::Type::Other)) ;
        subComponent.addComponent(Component("C2", Component::Type::Other)) ;

        rootComponentHolder.addComponent(subComponent) ;

        EXPECT_TRUE(rootComponentHolder.hasComponentAt("A")) ;
        EXPECT_TRUE(rootComponentHolder.hasComponentAt("B")) ;
        EXPECT_TRUE(rootComponentHolder.hasComponentAt("C")) ;
        EXPECT_TRUE(rootComponentHolder.hasComponentAt("C/C1")) ;
        EXPECT_TRUE(rootComponentHolder.hasComponentAt("C/C2")) ;

        EXPECT_FALSE(rootComponentHolder.hasComponentAt("D")) ;
        EXPECT_FALSE(rootComponentHolder.hasComponentAt("C/C3")) ;

    }

    {

        ComponentHolder rootComponentHolder = {} ;

        EXPECT_ANY_THROW(rootComponentHolder.hasComponentWithName(String::Empty())) ;

    }

}

TEST (OpenSpaceToolkit_Simulation_Utilities_ComponentHolder, AccessComponentWithName)
{

    using ostk::core::types::String ;
    using ostk::core::types::Unique ;
    using ostk::core::ctnr::Array ;

    using ostk::simulation::Component ;
    using ostk::simulation::utilities::ComponentHolder ;

    {

        Array<Unique<Component>> components = Array<Unique<Component>>::Empty() ;

        components.emplace_back(Unique<Component>(Component("Component A", Component::Type::Sensor).clone())) ;
        components.emplace_back(Unique<Component>(Component("Component B", Component::Type::Actuator).clone())) ;

        const ComponentHolder componentHolder = { components } ;

        EXPECT_EQ("Component A", componentHolder.accessComponentWithName("Component A").getName()) ;
        EXPECT_EQ("Component B", componentHolder.accessComponentWithName("Component B").getName()) ;

        EXPECT_ANY_THROW(componentHolder.accessComponentWithName("Component C")) ;

    }

    {

        Array<Unique<Component>> components = Array<Unique<Component>>::Empty() ;

        components.emplace_back(Unique<Component>(Component("Component A", Component::Type::Sensor).clone())) ;
        components.emplace_back(Unique<Component>(Component("Component B", Component::Type::Actuator).clone())) ;

        const ComponentHolder componentHolder = { components } ;

        EXPECT_ANY_THROW(componentHolder.accessComponentWithName(String::Empty())) ;

    }

}

TEST (OpenSpaceToolkit_Simulation_Utilities_ComponentHolder, AccessComponentsWithTag)
{

    using ostk::core::types::String ;
    using ostk::core::types::Unique ;
    using ostk::core::ctnr::Array ;

    using ostk::simulation::Component ;
    using ostk::simulation::utilities::ComponentHolder ;

    {

        Array<Unique<Component>> components = Array<Unique<Component>>::Empty() ;

        components.emplace_back(Unique<Component>(Component("Component A", Component::Type::Sensor, {"ADCS"}).clone())) ;
        components.emplace_back(Unique<Component>(Component("Component B", Component::Type::Actuator, {"ADCS"}).clone())) ;

        const ComponentHolder componentHolder = { components } ;

        EXPECT_EQ(2, componentHolder.accessComponentsWithTag("ADCS").getSize()) ;
        EXPECT_EQ(0, componentHolder.accessComponentsWithTag("COM").getSize()) ;

        EXPECT_ANY_THROW(componentHolder.accessComponentsWithTag("")) ;

    }

}

TEST (OpenSpaceToolkit_Simulation_Utilities_ComponentHolder, AccessComponentAt)
{

    using ostk::core::types::String ;
    using ostk::core::types::Unique ;
    using ostk::core::ctnr::Array ;

    using ostk::simulation::Component ;
    using ostk::simulation::utilities::ComponentHolder ;

    {

        Array<Unique<Component>> components = Array<Unique<Component>>::Empty() ;

        components.emplace_back(Unique<Component>(Component("Component A", Component::Type::Sensor).clone())) ;
        components.emplace_back(Unique<Component>(Component("Component B", Component::Type::Actuator).clone())) ;

        const ComponentHolder componentHolder = { components } ;

        EXPECT_EQ("Component A", componentHolder.accessComponentAt("Component A").getName()) ;
        EXPECT_EQ("Component B", componentHolder.accessComponentAt("Component B").getName()) ;

        EXPECT_ANY_THROW(componentHolder.accessComponentAt("Component C")) ;

    }

    {

        ComponentHolder rootComponentHolder = {} ;

        rootComponentHolder.addComponent(Component("Component A", Component::Type::Sensor)) ;
        rootComponentHolder.addComponent(Component("Component B", Component::Type::Actuator)) ;

        Component subComponent = { "Component C", Component::Type::Other } ;
        Component subSubComponent = { "Component C1", Component::Type::Other } ;
        Component subSubSubComponent = { "Component C2", Component::Type::Other } ;

        subSubSubComponent.addComponent(Component("Component C3", Component::Type::Sensor)) ;
        subSubComponent.addComponent(subSubSubComponent) ;
        subComponent.addComponent(subSubComponent) ;

        rootComponentHolder.addComponent(subComponent) ;

        EXPECT_EQ("Component A", rootComponentHolder.accessComponentAt("Component A").getName()) ;
        EXPECT_EQ("Component B", rootComponentHolder.accessComponentAt("Component B").getName()) ;
        EXPECT_EQ("Component C", rootComponentHolder.accessComponentAt("Component C").getName()) ;

        EXPECT_EQ("Component C1", rootComponentHolder.accessComponentAt("Component C/Component C1").getName()) ;
        EXPECT_EQ("Component C1", rootComponentHolder.accessComponentAt("/Component C/Component C1").getName()) ;
        EXPECT_EQ("Component C1", rootComponentHolder.accessComponentAt("//Component C/Component C1").getName()) ;
        EXPECT_EQ("Component C2", rootComponentHolder.accessComponentAt("Component C/Component C1/Component C2").getName()) ;
        EXPECT_EQ("Component C3", rootComponentHolder.accessComponentAt("Component C/Component C1/Component C2/Component C3").getName()) ;
        EXPECT_EQ("Component C3", rootComponentHolder.accessComponentAt("Component C//Component C1//Component C2/Component C3").getName()) ;

        EXPECT_ANY_THROW(rootComponentHolder.accessComponentAt("Component D")) ;
        EXPECT_ANY_THROW(rootComponentHolder.accessComponentAt("Component C/Component A")) ;
        EXPECT_ANY_THROW(rootComponentHolder.accessComponentAt("Component C/Component B")) ;
        EXPECT_ANY_THROW(rootComponentHolder.accessComponentAt("Component C/Component C1/Component C3")) ;

    }

}

TEST (OpenSpaceToolkit_Simulation_Utilities_ComponentHolder, AddComponent)
{

    using ostk::core::types::Unique ;
    using ostk::core::ctnr::Array ;

    using ostk::simulation::Component ;
    using ostk::simulation::utilities::ComponentHolder ;

    {

        Array<Unique<Component>> components = Array<Unique<Component>>::Empty() ;

        components.emplace_back(Unique<Component>(Component("Component A", Component::Type::Sensor).clone())) ;
        components.emplace_back(Unique<Component>(Component("Component B", Component::Type::Actuator).clone())) ;

        ComponentHolder componentHolder = { components } ;

        EXPECT_TRUE(componentHolder.hasComponentWithName("Component A")) ;
        EXPECT_TRUE(componentHolder.hasComponentWithName("Component B")) ;
        EXPECT_FALSE(componentHolder.hasComponentWithName("Component C")) ;

        componentHolder.addComponent(Component("Component C", Component::Type::Sensor)) ;

        EXPECT_TRUE(componentHolder.hasComponentWithName("Component A")) ;
        EXPECT_TRUE(componentHolder.hasComponentWithName("Component B")) ;
        EXPECT_TRUE(componentHolder.hasComponentWithName("Component C")) ;

    }

    {

        ComponentHolder componentHolder = {} ;

        EXPECT_ANY_THROW(componentHolder.addComponent(Component::Undefined())) ;

    }

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST (OpenSpaceToolkit_Simulation_Utilities_ComponentHolder, SplitComponentPath)
{

    using ostk::core::types::String ;
    using ostk::core::ctnr::Pair ;

    using ostk::simulation::utilities::splitComponentPath ;

    {

        const Pair<String, String> reference = { "Component", "" } ;

        EXPECT_EQ(reference, splitComponentPath("Component")) ;
        EXPECT_EQ(reference, splitComponentPath("/Component")) ;
        EXPECT_EQ(reference, splitComponentPath("//Component")) ;
        EXPECT_EQ(reference, splitComponentPath("Component/")) ;

    }

    {

        const Pair<String, String> reference = { "A", "B" } ;

        EXPECT_EQ(reference, splitComponentPath("A/B")) ;

    }

    {

        const Pair<String, String> reference = { "A", "B/C" } ;

        EXPECT_EQ(reference, splitComponentPath("A/B/C")) ;
        EXPECT_EQ(reference, splitComponentPath("/A/B/C")) ;
        EXPECT_EQ(reference, splitComponentPath("//A/B/C")) ;

    }

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
