/// Apache License 2.0

#ifndef __OpenSpaceToolkit_Simulation_Component__
#define __OpenSpaceToolkit_Simulation_Component__

#include <OpenSpaceToolkit/Simulation/Component/Geometry.hpp>
#include <OpenSpaceToolkit/Simulation/Component/State.hpp>
#include <OpenSpaceToolkit/Simulation/Entity.hpp>
#include <OpenSpaceToolkit/Simulation/Utility/ComponentHolder.hpp>

#include <OpenSpaceToolkit/Core/Container/Array.hpp>
#include <OpenSpaceToolkit/Core/Container/Map.hpp>
#include <OpenSpaceToolkit/Core/Type/Shared.hpp>
#include <OpenSpaceToolkit/Core/Type/String.hpp>
#include <OpenSpaceToolkit/Core/Type/Weak.hpp>

#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Transformation/Rotation/Quaternion.hpp>

namespace ostk
{
namespace simulation
{

using ostk::core::container::Array;
using ostk::core::container::Map;
using ostk::core::type::Shared;
using ostk::core::type::String;
using ostk::core::type::Unique;
using ostk::core::type::Weak;

using ostk::mathematics::geometry::d3::transformation::rotation::Quaternion;

using ostk::physics::coordinate::Frame;

using ostk::simulation::component::Geometry;
using ostk::simulation::component::GeometryConfiguration;
using ostk::simulation::component::State;
using ostk::simulation::utility::ComponentHolder;

#define DEFAULT_COMPONENT_TYPE Component::Type::Undefined
#define DEFAULT_TAGS Array<String>::Empty()
#define DEFAULT_ORIENTATION Quaternion::Unit()
#define DEFAULT_GEOMETRIES Array<GeometryConfiguration>::Empty()
#define DEFAULT_COMPONENTS Array<ComponentConfiguration>::Empty()

class Simulator;
struct ComponentConfiguration;

/// @brief A component within the simulation hierarchy.
/// @details A Component extends Entity with a type, tags, geometries, child components,
///          and a reference frame. Components form a tree structure where each component
///          can hold sub-components and geometries.
///
/// @code{.cpp}
///     const Component& sensor = satellite.accessComponentWithName("camera");
///     Component::Type type = sensor.getType();
///     Array<String> tags = sensor.getTags();
/// @endcode
class Component : public Entity, public ComponentHolder, public std::enable_shared_from_this<Component>
{
   public:
    /// @brief Component type enumeration.
    enum class Type
    {

        Undefined,
        Assembly,
        Controller,
        Sensor,
        Actuator,
        Other

    };

    /// @brief Construct a component.
    ///
    /// @code{.cpp}
    ///     Component component(
    ///         "id", "name", Component::Type::Sensor,
    ///         tagArray, geometryArray, componentArray,
    ///         parentSPtr, frameSPtr, simulatorSPtr
    ///     );
    /// @endcode
    ///
    /// @param [in] anId An identifier.
    /// @param [in] aName A name.
    /// @param [in] aType A component type.
    /// @param [in] aTagArray An array of tags.
    /// @param [in] aGeometryArray An array of geometries.
    /// @param [in] aComponentArray An array of child components.
    /// @param [in] aParentComponentSPtr A shared pointer to the parent component holder.
    /// @param [in] aFrameSPtr A shared pointer to the reference frame.
    /// @param [in] aSimulatorSPtr A shared pointer to the simulator.
    Component(
        const String& anId,
        const String& aName,
        const Component::Type& aType,
        const Array<String>& aTagArray,
        const Array<Shared<Geometry>>& aGeometryArray,
        const Array<Shared<Component>>& aComponentArray,
        const Shared<ComponentHolder>& aParentComponentSPtr,
        const Shared<const Frame>& aFrameSPtr,
        const Shared<const Simulator>& aSimulatorSPtr
    );

    /// @brief Copy constructor.
    ///
    /// @code{.cpp}
    ///     Component copy(component);
    /// @endcode
    ///
    /// @param [in] aComponent A component.
    Component(const Component& aComponent);

    /// @brief Destructor.
    virtual ~Component();

    /// @brief Clone the component.
    ///
    /// @code{.cpp}
    ///     Component* cloned = component.clone();
    /// @endcode
    ///
    /// @return A pointer to a cloned component.
    virtual Component* clone() const;

    /// @brief Output stream operator.
    ///
    /// @code{.cpp}
    ///     std::cout << component;
    /// @endcode
    ///
    /// @param [in] anOutputStream An output stream.
    /// @param [in] aComponent A component.
    /// @return A reference to the output stream.
    friend std::ostream& operator<<(std::ostream& anOutputStream, const Component& aComponent);

    /// @brief Check if the component is defined.
    ///
    /// @code{.cpp}
    ///     bool defined = component.isDefined();
    /// @endcode
    ///
    /// @return True if the component is defined.
    bool isDefined() const;

    /// @brief Access the reference frame.
    ///
    /// @code{.cpp}
    ///     const Shared<const Frame>& frame = component.accessFrame();
    /// @endcode
    ///
    /// @return A reference to the shared pointer to the frame.
    const Shared<const Frame>& accessFrame() const;

    /// @brief Access a geometry by name.
    ///
    /// @code{.cpp}
    ///     const Geometry& fov = component.accessGeometryWithName("fov");
    /// @endcode
    ///
    /// @param [in] aName A geometry name.
    /// @return A reference to the geometry.
    const Geometry& accessGeometryWithName(const String& aName) const;

    /// @brief Access the simulator.
    ///
    /// @code{.cpp}
    ///     const Simulator& simulator = component.accessSimulator();
    /// @endcode
    ///
    /// @return A reference to the simulator.
    const Simulator& accessSimulator() const;

    /// @brief Get the component type.
    ///
    /// @code{.cpp}
    ///     Component::Type type = component.getType();
    /// @endcode
    ///
    /// @return The component type.
    Component::Type getType() const;

    /// @brief Get the tags.
    ///
    /// @code{.cpp}
    ///     Array<String> tags = component.getTags();
    /// @endcode
    ///
    /// @return An array of tags.
    Array<String> getTags() const;

    /// @brief Get the geometries.
    ///
    /// @code{.cpp}
    ///     Array<Shared<Geometry>> geometries = component.getGeometries();
    /// @endcode
    ///
    /// @return An array of shared pointers to the geometries.
    Array<Shared<Geometry>> getGeometries() const;

    /// @brief Set the parent component.
    ///
    /// @code{.cpp}
    ///     component.setParent(parentComponentSPtr);
    /// @endcode
    ///
    /// @param [in] aComponentSPtr A shared pointer to the parent component.
    void setParent(const Shared<Component>& aComponentSPtr);

    /// @brief Add a geometry to the component.
    ///
    /// @code{.cpp}
    ///     component.addGeometry(geometrySPtr);
    /// @endcode
    ///
    /// @param [in] aGeometrySPtr A shared pointer to a geometry.
    void addGeometry(const Shared<Geometry>& aGeometrySPtr);

    /// @brief Add a child component.
    ///
    /// @code{.cpp}
    ///     component.addComponent(childComponentSPtr);
    /// @endcode
    ///
    /// @param [in] aComponentSPtr A shared pointer to a component.
    void addComponent(const Shared<Component>& aComponentSPtr);

    /// @brief Construct an undefined component.
    ///
    /// @code{.cpp}
    ///     Component component = Component::Undefined();
    /// @endcode
    ///
    /// @return An undefined component.
    static Component Undefined();

    /// @brief Configure a component from a configuration.
    ///
    /// @code{.cpp}
    ///     Shared<Component> component = Component::Configure(componentConfiguration, parentComponentSPtr);
    /// @endcode
    ///
    /// @param [in] aComponentConfiguration A component configuration.
    /// @param [in] aParentComponentSPtr A shared pointer to the parent component.
    /// @return A shared pointer to the configured component.
    static Shared<Component> Configure(
        const ComponentConfiguration& aComponentConfiguration, const Shared<Component>& aParentComponentSPtr
    );

    /// @brief Convert a component type to a string.
    ///
    /// @code{.cpp}
    ///     String typeString = Component::StringFromType(Component::Type::Sensor); // "Sensor"
    /// @endcode
    ///
    /// @param [in] aType A component type.
    /// @return A string representation of the type.
    static String StringFromType(const Component::Type& aType);

    /// @brief Generate a reference frame for a component.
    ///
    /// @code{.cpp}
    ///     Shared<const Frame> frame = Component::GenerateFrame("sensor", quaternion, parentFrameSPtr);
    /// @endcode
    ///
    /// @param [in] aName A name for the frame.
    /// @param [in] aQuaternion An orientation quaternion relative to the parent frame.
    /// @param [in] aParentFrameSPtr A shared pointer to the parent frame.
    /// @return A shared pointer to the generated frame.
    static Shared<const Frame> GenerateFrame(
        const String& aName, const Quaternion& aQuaternion, const Shared<const Frame>& aParentFrameSPtr
    );

   protected:
    /// @brief Print the component to an output stream.
    ///
    /// @code{.cpp}
    ///     component.print(std::cout, true);
    /// @endcode
    ///
    /// @param [in] anOutputStream An output stream.
    /// @param [in] displayDecorators If true, display decorators.
    virtual void print(std::ostream& anOutputStream, bool displayDecorators = true) const;

   private:
    Component::Type type_;
    Array<String> tags_;
    Array<Shared<Geometry>> geometries_;  // Array of Geometries defined in Component Frame

    Weak<const ComponentHolder> parentWPtr_;
    Shared<const Frame> frameSPtr_;
    Shared<const Simulator> simulatorSPtr_;
};

/// @brief Configuration for constructing a Component.
struct ComponentConfiguration
{
    const String id;                                                            ///< The identifier.
    const String name;                                                          ///< The name.
    const Component::Type type = DEFAULT_COMPONENT_TYPE;                        ///< The component type.
    const Array<String> tags = DEFAULT_TAGS;                                     ///< The tags.
    const Quaternion orientation = DEFAULT_ORIENTATION;                          ///< The orientation quaternion.
    const Array<GeometryConfiguration> geometries = DEFAULT_GEOMETRIES;          ///< The geometry configurations.
    const Array<ComponentConfiguration> components = DEFAULT_COMPONENTS;          ///< The child component configurations.
};

}  // namespace simulation
}  // namespace ostk

#endif
