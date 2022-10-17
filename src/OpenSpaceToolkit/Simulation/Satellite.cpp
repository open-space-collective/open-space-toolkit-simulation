////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Simulation
/// @file           OpenSpaceToolkit/Simulation/Satellite.cpp
/// @author         Remy Derollez <remy@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <OpenSpaceToolkit/Simulation/Utilities/Identifier.hpp>
#include <OpenSpaceToolkit/Simulation/Satellite.hpp>

#include <OpenSpaceToolkit/Physics/Coordinate/Frame/Providers/Dynamic.hpp>
#include <OpenSpaceToolkit/Physics/Coordinate/Frame/Provider.hpp>

#include <OpenSpaceToolkit/Core/Error.hpp>
#include <OpenSpaceToolkit/Core/Utilities.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace ostk
{
namespace simulation
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using ostk::physics::coord::Transform ;
using ostk::physics::coord::frame::Provider ;
using DynamicProvider = ostk::physics::coord::frame::provider::Dynamic ;

using namespace ostk::simulation::utilities ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                                Satellite::Satellite                        (   const   String&                     aName,
                                                                                const   Profile&                    aProfile,
                                                                                const   Array<Component>&           aComponentArray                             )
                                :   Entity(aName),
                                    ComponentHolder(aComponentArray),
                                    frameSPtr_(nullptr)
{

    if (aProfile.isDefined())
    {
        frameSPtr_ = Satellite::GenerateFrame(this->getId(), aProfile) ;
    }


}

                                Satellite::Satellite                        (   const   String&                     anId,
                                                                                const   String&                     aName,
                                                                                const   Profile&                    aProfile,
                                                                                const   Array<Component>&           aComponentArray                             )
                                :   Entity(anId, aName),
                                    ComponentHolder(aComponentArray),
                                    frameSPtr_(nullptr)
{

    if (aProfile.isDefined())
    {
        frameSPtr_ = Satellite::GenerateFrame(this->getId(), aProfile) ;
    }

}

                                Satellite::~Satellite                       ( )
{

}

Satellite*                      Satellite::clone                            ( ) const
{
    return new Satellite(*this) ;
}

std::ostream&                   operator <<                                 (           std::ostream&               anOutputStream,
                                                                                const   Satellite&                  aSatellite                                  )
{

    aSatellite.print(anOutputStream, true) ;

    return anOutputStream ;

}

bool                            Satellite::isDefined                        ( ) const
{
    return Entity::isDefined() ;
}

const Shared<const Frame>&      Satellite::accessFrame                      ( ) const
{

    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Satellite") ;
    }

    return this->frameSPtr_ ;

}

void                            Satellite::print                            (           std::ostream&               anOutputStream,
                                                                                        bool                        displayDecorators                           ) const
{

    displayDecorators ? ostk::core::utils::Print::Header(anOutputStream, "Satellite") : void () ;

    Entity::print(anOutputStream, false) ;

    displayDecorators ? ostk::core::utils::Print::Footer(anOutputStream) : void () ;

}

Satellite                       Satellite::Undefined                        ( )
{
    return { String::Empty(), Profile::Undefined() } ;
}

Shared<const Frame>             Satellite::GenerateFrame                    (   const   String&                     aName,
                                                                                const   Profile&                    aProfile                                    )
{

    using ostk::physics::time::Instant ;

    if (Frame::Exists(aName))
    {
        Frame::Destruct(aName) ;
    }

    const Shared<const DynamicProvider> transformProviderSPtr = std::make_shared<const DynamicProvider>
    (
        // TBI: Use `shared_from_this` instead?
        [aProfile] (const Instant& anInstant) -> Transform
        {

            const auto& state = aProfile.getStateAt(anInstant).inFrame(Frame::GCRF()) ;

            return Transform::Passive
            (
                anInstant,
                -state.getPosition(),
                -state.getVelocity(), // TBM: Random expression, didn't test at all
                state.getAttitude(),
                -state.getAngularVelocity() // TBM: Random expression, didn't test at all
            ) ;

        }
    ) ;

    return Frame::Construct(aName, false, Frame::GCRF(), transformProviderSPtr) ;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
