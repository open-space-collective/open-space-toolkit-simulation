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

/// @brief A satellite in the simulation.
/// @details A Satellite is a specialized Component that has an associated flight Profile
///          and a dedicated reference frame derived from that profile.
///
/// @code{.cpp}
///     const Satellite& satellite = simulator.accessSatelliteWithName("sat-1");
///     const Shared<const Profile> profile = satellite.accessProfile();
/// @endcode
class Satellite : public Component
{
   public:
    /// @brief Construct a satellite.
    ///
    /// @code{.cpp}
    ///     Satellite satellite(
    ///         "id", "sat-1", tagArray, geometryArray,
    ///         componentArray, frameSPtr, profileSPtr, simulatorSPtr
    ///     );
    /// @endcode
    ///
    /// @param [in] anId An identifier.
    /// @param [in] aName A name.
    /// @param [in] aTagArray An array of tags.
    /// @param [in] aGeometryArray An array of geometries.
    /// @param [in] aComponentArray An array of child components.
    /// @param [in] aFrameSPtr A shared pointer to the reference frame.
    /// @param [in] aProfileSPtr A shared pointer to the flight profile.
    /// @param [in] aSimulatorSPtr A shared pointer to the simulator.
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

    /// @brief Copy constructor.
    ///
    /// @code{.cpp}
    ///     Satellite copy(satellite);
    /// @endcode
    ///
    /// @param [in] aSatellite A satellite.
    Satellite(const Satellite& aSatellite);

    /// @brief Destructor.
    ~Satellite();

    /// @brief Clone the satellite.
    ///
    /// @code{.cpp}
    ///     Satellite* cloned = satellite.clone();
    /// @endcode
    ///
    /// @return A pointer to a cloned satellite.
    Satellite* clone() const;

    /// @brief Output stream operator.
    ///
    /// @code{.cpp}
    ///     std::cout << satellite;
    /// @endcode
    ///
    /// @param [in] anOutputStream An output stream.
    /// @param [in] aSatellite A satellite.
    /// @return A reference to the output stream.
    friend std::ostream& operator<<(std::ostream& anOutputStream, const Satellite& aSatellite);

    /// @brief Check if the satellite is defined.
    ///
    /// @code{.cpp}
    ///     bool defined = satellite.isDefined();
    /// @endcode
    ///
    /// @return True if the satellite is defined.
    bool isDefined() const;

    /// @brief Access the flight profile.
    ///
    /// @code{.cpp}
    ///     const Shared<const Profile> profile = satellite.accessProfile();
    /// @endcode
    ///
    /// @return A shared pointer to the flight profile.
    const Shared<const Profile> accessProfile() const;

    /// @brief Print the satellite to an output stream.
    ///
    /// @code{.cpp}
    ///     satellite.print(std::cout, true);
    /// @endcode
    ///
    /// @param [in] anOutputStream An output stream.
    /// @param [in] displayDecorators If true, display decorators.
    void print(std::ostream& anOutputStream, bool displayDecorators = true) const;

    /// @brief Construct an undefined satellite.
    ///
    /// @code{.cpp}
    ///     Satellite satellite = Satellite::Undefined();
    /// @endcode
    ///
    /// @return An undefined satellite.
    static Satellite Undefined();

    /// @brief Configure a satellite from a configuration.
    ///
    /// @code{.cpp}
    ///     Shared<Satellite> satellite = Satellite::Configure(satelliteConfiguration, simulatorSPtr);
    /// @endcode
    ///
    /// @param [in] aSatelliteConfiguration A satellite configuration.
    /// @param [in] aSimulatorSPtr A shared pointer to the simulator.
    /// @return A shared pointer to the configured satellite.
    static Shared<Satellite> Configure(
        const SatelliteConfiguration& aSatelliteConfiguration, const Shared<const Simulator>& aSimulatorSPtr
    );

    /// @brief Generate a reference frame for the satellite from a flight profile.
    ///
    /// @code{.cpp}
    ///     Shared<const Frame> frame = Satellite::GenerateFrame("sat-1", profileSPtr);
    /// @endcode
    ///
    /// @param [in] aName A name for the frame.
    /// @param [in] aProfile A shared pointer to the flight profile.
    /// @return A shared pointer to the generated frame.
    static Shared<const Frame> GenerateFrame(const String& aName, const Shared<const Profile>& aProfile);

   private:
    Shared<const Profile> profileSPtr_;
};

/// @brief Configuration for constructing a Satellite.
struct SatelliteConfiguration
{
    const String id;                                                      ///< The identifier.
    const String name;                                                    ///< The name.
    const Profile profile;                                                ///< The flight profile.
    const Array<ComponentConfiguration> components = DEFAULT_COMPONENTS;  ///< The child component configurations.
    const Array<String> tags = DEFAULT_TAGS;                              ///< The tags.
    const Array<GeometryConfiguration> geometries = DEFAULT_GEOMETRIES;   ///< The geometry configurations.
};

}  // namespace simulation
}  // namespace ostk

#endif
