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
                                                                                const   Array<Satellite>&           aSatelliteArray                             )
                                :   environment_(anEnvironment),
                                    satelliteMap_()
{

    for (const auto& satellite : aSatelliteArray)
    {
        satelliteMap_.insert({ satellite.getName(), Shared<Satellite>(satellite.clone()) }) ;
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
    return environment_.isDefined() ;
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

    return satelliteMap_.find(aSatelliteName) != satelliteMap_.end() ;

}

const Environment&              Simulator::accessEnvironment                ( ) const
{

    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Simulator") ;
    }

    return environment_ ;


}

Satellite&                      Simulator::accessSatelliteWithName          (   const   String&                     aSatelliteName                              ) const
{

    if (aSatelliteName.isEmpty())
    {
        throw ostk::core::error::runtime::Undefined("Satellite name") ;
    }

    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Simulator") ;
    }

    const auto satelliteIt = satelliteMap_.find(aSatelliteName) ;

    if (satelliteIt == satelliteMap_.end())
    {
        throw ostk::core::error::RuntimeError("No Satellite found with name [{}].", aSatelliteName) ;
    }

    return *(satelliteIt->second) ;

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

    environment_.setInstant(anInstant) ;

}

void                            Simulator::step                             (   const   Duration&                   aDuration                                   )
{

    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Simulator") ;
    }

    this->setInstant(environment_.getInstant() + aDuration) ;

}

Simulator                       Simulator::Undefined                        ( )
{
    return { Environment::Undefined(), {} } ;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
