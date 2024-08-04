/// Apache License 2.0

#ifndef __OpenSpaceToolkit_Simulation_Satellite__
#define __OpenSpaceToolkit_Simulation_Satellite__

#include <OpenSpaceToolkit/Simulation/Component.hpp>
#include <OpenSpaceToolkit/Simulation/Entity.hpp>

#include <OpenSpaceToolkit/Core/Container/Array.hpp>
#include <OpenSpaceToolkit/Core/Container/Map.hpp>
#include <OpenSpaceToolkit/Core/Type/Shared.hpp>
#include <OpenSpaceToolkit/Core/Type/String.hpp>

#include <OpenSpaceToolkit/Physics/Coordinate/Frame.hpp>

#include <OpenSpaceToolkit/Astrodynamics/Flight/Profile.hpp>

namespace ostk
{
namespace simulation
{

using ostk::core::container::Array;
using ostk::core::container::Map;
using ostk::core::type::Shared;
using ostk::core::type::String;

using ostk::physics::coordinate::Frame;

using ostk::astrodynamics::flight::Profile;

using ostk::simulation::Component;
using ostk::simulation::ComponentConfiguration;

#define DEFAULT_COMPONENTS Array<ComponentConfiguration>::Empty()
#define DEFAULT_TAGS Array<String>::Empty()
#define DEFAULT_GEOMETRIES Array<GeometryConfiguration>::Empty()

class Simulator;
struct SatelliteConfiguration;

/// @brief                      Satellite

class Satellite : public Component
{
   public:
    Satellite(
        const String& anId,
        const String& aName,
        const Array<String>& aTagArray,
        const Array<Shared<Geometry>>& aGeometryArray,
        const Array<Shared<Component>>& aComponentArray,
        const Shared<const Frame>& aFrameSPtr,
        const Shared<const Profile>& aProfileSPtr,
        const Shared<const Simulator>& aSimulatorSPtr
    );

    Satellite(const Satellite& aSatellite);

    ~Satellite();

    Satellite* clone() const;

    friend std::ostream& operator<<(std::ostream& anOutputStream, const Satellite& aSatellite);

    bool isDefined() const;

    const Shared<const Profile> accessProfile() const;

    /// @brief              Print satellite
    ///
    /// @param              [in] anOutputStream An output stream
    /// @param              [in] (optional) displayDecorators If true, display decorators

    void print(std::ostream& anOutputStream, bool displayDecorators = true) const;

    static Satellite Undefined();

    static Shared<Satellite> Configure(
        const SatelliteConfiguration& aSatelliteConfiguration, const Shared<const Simulator>& aSimulatorSPtr
    );

    static Shared<const Frame> GenerateFrame(const String& aName, const Shared<const Profile>& aProfile);

   private:
    Shared<const Profile> profileSPtr_;
};

struct SatelliteConfiguration
{
    const String id;
    const String name;
    const Profile profile;
    const Array<ComponentConfiguration> components = DEFAULT_COMPONENTS;
    const Array<String> tags = DEFAULT_TAGS;
    const Array<GeometryConfiguration> geometries = DEFAULT_GEOMETRIES;
};

}  // namespace simulation
}  // namespace ostk

#endif
