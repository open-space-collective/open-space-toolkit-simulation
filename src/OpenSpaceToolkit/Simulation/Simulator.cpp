////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Simulation
/// @file           OpenSpaceToolkit/Simulation/Simulator.cpp
/// @author         Remy Derollez <remy@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <OpenSpaceToolkit/Simulation/Simulator.hpp>

#include <OpenSpaceToolkit/Core/Error.hpp>
#include <OpenSpaceToolkit/Core/Utilities.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace ostk
{
namespace simulation
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                                Simulator::Simulator                        (   const   Environment&                anEnvironment,
                                                                                const   Array<Shared<Satellite>>&   aSatelliteArray                             )
                                :   environment_(anEnvironment),
                                    satelliteMap_()
{

    for (const auto& satelliteSPtr : aSatelliteArray)
    {
        this->satelliteMap_.insert({ satelliteSPtr->getName(), satelliteSPtr }) ;
    }

}

std::ostream&                   operator <<                                 (           std::ostream&               anOutputStream,
                                                                                const   Simulator&                  aSimulator                                  )
{

    aSimulator.print(anOutputStream, true) ;

    return anOutputStream ;

}

bool                            Simulator::isDefined                        ( ) const
{
    return this->environment_.isDefined() ;
}

bool                            Simulator::hasSatelliteWithName             (   const   String&                     aSatelliteName                              ) const
{

    if (aSatelliteName.isEmpty())
    {
        throw ostk::core::error::runtime::Undefined("Satellite name") ;
    }

    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Simulator") ;
    }

    return this->satelliteMap_.find(aSatelliteName) != this->satelliteMap_.end() ;

}

const Environment&              Simulator::accessEnvironment                ( ) const
{

    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Simulator") ;
    }

    return this->environment_ ;


}

const Satellite&                Simulator::accessSatelliteWithName          (   const   String&                     aSatelliteName                              ) const
{

    if (aSatelliteName.isEmpty())
    {
        throw ostk::core::error::runtime::Undefined("Satellite name") ;
    }

    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Simulator") ;
    }

    const auto satelliteIt = this->satelliteMap_.find(aSatelliteName) ;

    if (satelliteIt == this->satelliteMap_.end())
    {
        throw ostk::core::error::RuntimeError("No Satellite found with name [{}].", aSatelliteName) ;
    }

    return *(satelliteIt->second) ;

}

Instant                         Simulator::getInstant                       ( ) const
{

    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Simulator") ;
    }

    return this->environment_.getInstant() ;

}

void                            Simulator::print                            (           std::ostream&               anOutputStream,
                                                                                        bool                        displayDecorators                           ) const
{

    displayDecorators ? ostk::core::utils::Print::Header(anOutputStream, "Simulator") : void () ;

    // environment_.print(anOutputStream, true) ;

    displayDecorators ? ostk::core::utils::Print::Footer(anOutputStream) : void () ;

}

void                            Simulator::setInstant                       (   const   Instant&                    anInstant                                   )
{

    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Simulator") ;
    }

    this->environment_.setInstant(anInstant) ;

}

void                            Simulator::stepForward                      (   const   Duration&                   aDuration                                   )
{

    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Simulator") ;
    }

    this->setInstant(this->environment_.getInstant() + aDuration) ;

}

void                            Simulator::addSatellite                     (   const   Shared<Satellite>&          aSatelliteSPtr                              )
{

    if ((!aSatelliteSPtr) || (!aSatelliteSPtr->isDefined()))
    {
        throw ostk::core::error::runtime::Undefined("Satellite") ;
    }

    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Simulator") ;
    }

    this->satelliteMap_.insert({ aSatelliteSPtr->getName(), aSatelliteSPtr }) ;

}

Simulator                       Simulator::Undefined                        ( )
{
    return { Environment::Undefined(), {} } ;
}

Shared<Simulator>               Simulator::Configure                        (   const   SimulatorConfiguration&     aSimulatorConfiguration                     )
{

    const Shared<Simulator> simulatorSPtr = std::make_shared<Simulator>
    (
        aSimulatorConfiguration.environment,
        Array<Shared<Satellite>>::Empty()
    ) ;

    for (const auto& satelliteConfiguration : aSimulatorConfiguration.satellites)
    {
        simulatorSPtr->addSatellite(Satellite::Configure(satelliteConfiguration, simulatorSPtr)) ;
    }

    return simulatorSPtr ;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
