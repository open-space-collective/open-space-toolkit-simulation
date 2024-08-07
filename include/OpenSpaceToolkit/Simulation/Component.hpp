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

/// @brief                      Component

class Component : public Entity, public ComponentHolder, public std::enable_shared_from_this<Component>
{
   public:
    enum class Type
    {

        Undefined,
        Assembly,
        Controller,
        Sensor,
        Actuator,
        Other

    };

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

    Component(const Component& aComponent);

    virtual ~Component();

    virtual Component* clone() const;

    friend std::ostream& operator<<(std::ostream& anOutputStream, const Component& aComponent);

    bool isDefined() const;

    const Shared<const Frame>& accessFrame() const;

    const Geometry& accessGeometryWithName(const String& aName) const;

    const Simulator& accessSimulator() const;

    Component::Type getType() const;

    Array<String> getTags() const;

    Array<Shared<Geometry>> getGeometries() const;

    void setParent(const Shared<Component>& aComponentSPtr);

    void addGeometry(const Shared<Geometry>& aGeometrySPtr);

    void addComponent(const Shared<Component>& aComponentSPtr);

    static Component Undefined();

    static Shared<Component> Configure(
        const ComponentConfiguration& aComponentConfiguration, const Shared<Component>& aParentComponentSPtr
    );

    static String StringFromType(const Component::Type& aType);

    static Shared<const Frame> GenerateFrame(
        const String& aName, const Quaternion& aQuaternion, const Shared<const Frame>& aParentFrameSPtr
    );

   protected:
    /// @brief              Print component
    ///
    /// @param              [in] anOutputStream An output stream
    /// @param              [in] (optional) displayDecorators If true, display decorators

    virtual void print(std::ostream& anOutputStream, bool displayDecorators = true) const;

   private:
    Component::Type type_;
    Array<String> tags_;
    Array<Shared<Geometry>> geometries_;  // Array of Geometries defined in Component Frame

    Weak<const ComponentHolder> parentWPtr_;
    Shared<const Frame> frameSPtr_;
    Shared<const Simulator> simulatorSPtr_;
};

struct ComponentConfiguration
{
    const String id;
    const String name;
    const Component::Type type = DEFAULT_COMPONENT_TYPE;
    const Array<String> tags = DEFAULT_TAGS;
    const Quaternion orientation = DEFAULT_ORIENTATION;
    const Array<GeometryConfiguration> geometries = DEFAULT_GEOMETRIES;
    const Array<ComponentConfiguration> components = DEFAULT_COMPONENTS;
};

}  // namespace simulation
}  // namespace ostk

#endif
