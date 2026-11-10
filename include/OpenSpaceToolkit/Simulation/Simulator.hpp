/// Apache License 2.0

#ifndef __OpenSpaceToolkit_Simulation_Simulator__
#define __OpenSpaceToolkit_Simulation_Simulator__

#include <OpenSpaceToolkit/Simulation/Satellite.hpp>

#include <OpenSpaceToolkit/Core/Container/Array.hpp>
#include <OpenSpaceToolkit/Core/Container/Map.hpp>
#include <OpenSpaceToolkit/Core/Type/Shared.hpp>
#include <OpenSpaceToolkit/Core/Type/String.hpp>

#include <OpenSpaceToolkit/Physics/Environment.hpp>
#include <OpenSpaceToolkit/Physics/Time/Duration.hpp>
#include <OpenSpaceToolkit/Physics/Time/Instant.hpp>

namespace ostk
{
namespace simulation
{

#define DEFAULT_SATELLITES Array<SatelliteConfiguration>::Empty()

using ostk::core::container::Array;
using ostk::core::container::Map;
using ostk::core::type::Shared;
using ostk::core::type::String;

using ostk::physics::Environment;
using ostk::physics::time::Duration;
using ostk::physics::time::Instant;

using ostk::simulation::Satellite;

struct SimulatorConfiguration;

/// @brief The top-level simulation manager.
/// @details The Simulator holds an Environment and a collection of Satellites, providing
///          methods to advance the simulation in time and manage satellites.
///
/// @code{.cpp}
///     Simulator simulator(environment, satelliteArray);
///     simulator.setInstant(instant);
///     simulator.stepForward(Duration::Minutes(5.0));
///     const Satellite& sat = simulator.getSatelliteWithName("sat-1");
/// @endcode
class Simulator
{
   public:
    /// @brief Construct a simulator.
    ///
    /// @code{.cpp}
    ///     Simulator simulator(environment, satelliteArray);
    /// @endcode
    ///
    /// @param [in] anEnvironment An environment.
    /// @param [in] aSatelliteArray An array of satellites.
    Simulator(const Environment& anEnvironment, const Array<Shared<Satellite>>& aSatelliteArray);

    /// @brief Output stream operator.
    ///
    /// @code{.cpp}
    ///     std::cout << simulator;
    /// @endcode
    ///
    /// @param [in] anOutputStream An output stream.
    /// @param [in] aSimulator A simulator.
    /// @return A reference to the output stream.
    friend std::ostream& operator<<(std::ostream& anOutputStream, const Simulator& aSimulator);

    /// @brief Check if a satellite with the given name exists.
    ///
    /// @code{.cpp}
    ///     bool exists = simulator.hasSatelliteWithName("sat-1");
    /// @endcode
    ///
    /// @param [in] aSatelliteName A satellite name.
    /// @return True if a satellite with the name exists.
    bool hasSatelliteWithName(const String& aSatelliteName) const;

    /// @brief Get the environment.
    ///
    /// @code{.cpp}
    ///     const Environment& environment = simulator.getEnvironment();
    /// @endcode
    ///
    /// @return A const reference to the environment.
    const Environment& getEnvironment() const;

    /// @brief Get the satellite map.
    ///
    /// @code{.cpp}
    ///     const Map<String, Shared<Satellite>>& satellites = simulator.getSatelliteMap();
    /// @endcode
    ///
    /// @return A const reference to the map of satellite names to satellites.
    const Map<String, Shared<Satellite>>& getSatelliteMap() const;

    /// @brief Get a satellite by name.
    ///
    /// @code{.cpp}
    ///     const Satellite& satellite = simulator.getSatelliteWithName("sat-1");
    /// @endcode
    ///
    /// @param [in] aSatelliteName A satellite name.
    /// @return A const reference to the satellite.
    const Satellite& getSatelliteWithName(const String& aSatelliteName) const;

    /// @brief Get the current simulation instant.
    ///
    /// @code{.cpp}
    ///     const Instant& instant = simulator.getInstant();
    /// @endcode
    ///
    /// @return A const reference to the current instant.
    const Instant& getInstant() const;

    /// @brief Print the simulator to an output stream.
    ///
    /// @code{.cpp}
    ///     simulator.print(std::cout, true);
    /// @endcode
    ///
    /// @param [in] anOutputStream An output stream.
    /// @param [in] displayDecorators If true, display decorators.
    void print(std::ostream& anOutputStream, bool displayDecorators = true) const;

    /// @brief Set the simulation instant.
    ///
    /// @code{.cpp}
    ///     simulator.setInstant(instant);
    /// @endcode
    ///
    /// @param [in] anInstant An instant.
    void setInstant(const Instant& anInstant);

    /// @brief Advance the simulation by a duration.
    ///
    /// @code{.cpp}
    ///     simulator.stepForward(Duration::Minutes(5.0));
    /// @endcode
    ///
    /// @param [in] aDuration A duration.
    void stepForward(const Duration& aDuration);

    /// @brief Add a satellite to the simulation.
    ///
    /// @code{.cpp}
    ///     simulator.addSatellite(satelliteSPtr);
    /// @endcode
    ///
    /// @param [in] aSatelliteSPtr A shared pointer to a satellite.
    void addSatellite(const Shared<Satellite>& aSatelliteSPtr);

    /// @brief Remove a satellite by name.
    ///
    /// @code{.cpp}
    ///     simulator.removeSatelliteWithName("sat-1");
    /// @endcode
    ///
    /// @param [in] aSatelliteName A satellite name.
    void removeSatelliteWithName(const String& aSatelliteName);

    /// @brief Remove all satellites from the simulation.
    ///
    /// @code{.cpp}
    ///     simulator.clearSatellites();
    /// @endcode
    void clearSatellites();

    /// @brief Configure a simulator from a configuration.
    ///
    /// @code{.cpp}
    ///     Shared<Simulator> simulator = Simulator::Configure(simulatorConfiguration);
    /// @endcode
    ///
    /// @param [in] aSimulatorConfiguration A simulator configuration.
    /// @return A shared pointer to the configured simulator.
    static Shared<Simulator> Configure(const SimulatorConfiguration& aSimulatorConfiguration);

   private:
    Environment environment_;
    Map<String, Shared<Satellite>> satelliteMap_;
};

/// @brief Configuration for constructing a Simulator.
struct SimulatorConfiguration
{
    const Environment environment;                                        ///< The environment.
    const Array<SatelliteConfiguration> satellites = DEFAULT_SATELLITES;  ///< The satellite configurations.
};

}  // namespace simulation
}  // namespace ostk

#endif
