/// Apache License 2.0

#include <OpenSpaceToolkit/Simulation/Utilities/Identifier.hpp>
#include <OpenSpaceToolkit/Simulation/Satellite.hpp>

#include <OpenSpaceToolkit/Physics/Coordinate/Frame/Providers/Dynamic.hpp>
#include <OpenSpaceToolkit/Physics/Coordinate/Frame/Provider.hpp>

#include <OpenSpaceToolkit/Core/Error.hpp>
#include <OpenSpaceToolkit/Core/Utilities.hpp>

namespace ostk
{
namespace simulation
{

using ostk::physics::coord::Transform ;
using ostk::physics::coord::frame::Provider ;
using DynamicProvider = ostk::physics::coord::frame::provider::Dynamic ;

using namespace ostk::simulation::utilities ;

                                Satellite::Satellite                        (   const   String&                     anId,
                                                                                const   String&                     aName,
                                                                                const   Array<String>&              aTagArray,
                                                                                const   Array<Shared<Geometry>>&    aGeometryArray,
                                                                                const   Array<Shared<Component>>&   aComponentArray,
                                                                                const   Shared<const Frame>&        aFrameSPtr,
                                                                                const   Shared<Profile>&            aProfileSPtr,
                                                                                const   Shared<const Simulator>&    aSimulatorSPtr                              )
                                :   Component
                                    (
                                        anId,
                                        aName,
                                        Component::Type::Assembly,
                                        aTagArray,
                                        aGeometryArray,
                                        aComponentArray,
                                        nullptr,
                                        aFrameSPtr,
                                        aSimulatorSPtr
                                    ),
                                    profileSPtr_(aProfileSPtr)
{

}

                                Satellite::Satellite                        (   const   Satellite&                  aSatellite                                  )
                                :   Component(aSatellite),
                                    profileSPtr_(aSatellite.profileSPtr_)
{

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

void                            Satellite::print                            (           std::ostream&               anOutputStream,
                                                                                        bool                        displayDecorators                           ) const
{

    displayDecorators ? ostk::core::utils::Print::Header(anOutputStream, "Satellite") : void () ;

    Entity::print(anOutputStream, false) ;

    displayDecorators ? ostk::core::utils::Print::Footer(anOutputStream) : void () ;

}

Satellite                       Satellite::Undefined                        ( )
{

    return
    {
        String::Empty(),
        String::Empty(),
        Array<String>::Empty(),
        Array<Shared<Geometry>>::Empty(),
        Array<Shared<Component>>::Empty(),
        nullptr,
        nullptr,
        nullptr
    } ;

}

Shared<Satellite>               Satellite::Configure                        (   const   SatelliteConfiguration&     aSatelliteConfiguration,
                                                                                const   Shared<const Simulator>&    aSimulatorSPtr                              )
{

    const Shared<Profile> profileSPtr = std::make_shared<Profile>(aSatelliteConfiguration.profile) ;

    const Shared<Satellite> satelliteSPtr = std::make_shared<Satellite>
    (
        aSatelliteConfiguration.id,
        aSatelliteConfiguration.name,
        aSatelliteConfiguration.tags,
        Array<Shared<Geometry>>::Empty(),
        Array<Shared<Component>>::Empty(),
        Satellite::GenerateFrame
        (
            String::Format("Satellite [{}]", aSatelliteConfiguration.id),
            profileSPtr
        ),
        profileSPtr,
        aSimulatorSPtr
    ) ;

    for (const auto& geometryConfiguration : aSatelliteConfiguration.geometries)
    {
        satelliteSPtr->addGeometry(Geometry::Configure(geometryConfiguration, satelliteSPtr)) ;
    }

    for (const auto& componentConfiguration : aSatelliteConfiguration.components)
    {
        satelliteSPtr->addComponent(Component::Configure(componentConfiguration, satelliteSPtr)) ;
    }

    return satelliteSPtr ;

}

Shared<const Frame>             Satellite::GenerateFrame                    (   const   String&                     aName,
                                                                                const   Shared<const Profile>&      aProfileSPtr                                )
{

    using ostk::physics::time::Instant ;

    if (Frame::Exists(aName))
    {
        Frame::Destruct(aName) ;
    }

    const Weak<const Profile> profileWPtr = aProfileSPtr ;

    const Shared<const DynamicProvider> transformProviderSPtr = std::make_shared<const DynamicProvider>
    (
        [profileWPtr] (const Instant& anInstant) -> Transform
        {

            if (Shared<const Profile> profileSPtr = profileWPtr.lock())
            {

                const auto& state = profileSPtr->getStateAt(anInstant).inFrame(Frame::GCRF()) ;

                return Transform::Passive
                (
                    anInstant,
                    -state.getPosition().getCoordinates(),
                    -state.getVelocity().getCoordinates(), // TBM: Random expression, didn't test this at all
                    state.getAttitude(),
                    -state.getAngularVelocity() // TBM: Random expression, didn't test this at all
                ) ;

            }
            else
            {
                throw ostk::core::error::RuntimeError("Cannot get pointer to Profile.") ;
            }

        }
    ) ;

    return Frame::Construct(aName, false, Frame::GCRF(), transformProviderSPtr) ;

}

}
}
