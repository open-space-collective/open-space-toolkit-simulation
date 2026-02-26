/// Apache License 2.0

#ifndef __OpenSpaceToolkit_Simulation_Component_Geometry__
#define __OpenSpaceToolkit_Simulation_Component_Geometry__

#include <OpenSpaceToolkit/Core/Type/String.hpp>

#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Object.hpp>
#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Object/Composite.hpp>

#include <OpenSpaceToolkit/Physics/Environment/Object/Celestial.hpp>
#include <OpenSpaceToolkit/Physics/Environment/Object/Geometry.hpp>

namespace ostk
{
namespace simulation
{

class Component;

namespace component
{

using ostk::core::type::Shared;
using ostk::core::type::String;
using ostk::mathematics::geometry::d3::Object;
using ostk::mathematics::geometry::d3::object::Composite;

using ostk::physics::coordinate::Frame;
using ObjectGeometry = ostk::physics::environment::object::Geometry;
using ostk::physics::environment::object::Celestial;

struct GeometryConfiguration;

/// @brief A geometry associated with a component.
/// @details Represents a 3D geometric shape (Composite) attached to a Component, enabling
///          spatial queries such as intersection and containment checks against other geometries
///          and celestial objects.
///
/// @code{.cpp}
///     Geometry geometry("fov", composite, componentSPtr);
///     bool defined = geometry.isDefined();
///     bool doesIntersect = geometry.intersects(otherGeometry);
/// @endcode
class Geometry
{
   public:
    /// @brief Construct a geometry.
    ///
    /// @code{.cpp}
    ///     Geometry geometry("fov", composite, componentSPtr);
    /// @endcode
    ///
    /// @param [in] aName A name.
    /// @param [in] aComposite A composite 3D object.
    /// @param [in] aComponentSPtr A shared pointer to the owning component.
    Geometry(const String& aName, const Composite& aComposite, const Shared<const Component>& aComponentSPtr);

    /// @brief Equal to operator.
    ///
    /// @code{.cpp}
    ///     bool isEqual = (firstGeometry == secondGeometry);
    /// @endcode
    ///
    /// @param [in] aGeometry A geometry.
    /// @return True if geometries are equal.
    bool operator==(const Geometry& aGeometry) const;

    /// @brief Not equal to operator.
    ///
    /// @code{.cpp}
    ///     bool isNotEqual = (firstGeometry != secondGeometry);
    /// @endcode
    ///
    /// @param [in] aGeometry A geometry.
    /// @return True if geometries are not equal.
    bool operator!=(const Geometry& aGeometry) const;

    /// @brief Output stream operator.
    ///
    /// @code{.cpp}
    ///     std::cout << geometry;
    /// @endcode
    ///
    /// @param [in] anOutputStream An output stream.
    /// @param [in] aGeometry A geometry.
    /// @return A reference to the output stream.
    friend std::ostream& operator<<(std::ostream& anOutputStream, const Geometry& aGeometry);

    /// @brief Check if the geometry is defined.
    ///
    /// @code{.cpp}
    ///     bool defined = geometry.isDefined();
    /// @endcode
    ///
    /// @return True if the geometry is defined.
    bool isDefined() const;

    /// @brief Access the owning component.
    ///
    /// @code{.cpp}
    ///     const Component& component = geometry.accessComponent();
    /// @endcode
    ///
    /// @return A reference to the owning component.
    const Component& accessComponent() const;

    /// @brief Get the name of the geometry.
    ///
    /// @code{.cpp}
    ///     String name = geometry.getName();
    /// @endcode
    ///
    /// @return The name.
    String getName() const;

    /// @brief Print the geometry to an output stream.
    ///
    /// @code{.cpp}
    ///     geometry.print(std::cout, true);
    /// @endcode
    ///
    /// @param [in] anOutputStream An output stream.
    /// @param [in] displayDecorators If true, display decorators.
    void print(std::ostream& anOutputStream, bool displayDecorators = true) const;

    /// @brief Check if the geometry intersects another geometry.
    ///
    /// @code{.cpp}
    ///     bool doesIntersect = geometry.intersects(otherGeometry);
    /// @endcode
    ///
    /// @param [in] aGeometry A geometry.
    /// @return True if the geometries intersect.
    bool intersects(const ObjectGeometry& aGeometry) const;

    /// @brief Check if the geometry intersects a celestial object.
    ///
    /// @code{.cpp}
    ///     bool doesIntersect = geometry.intersects(celestialObject);
    /// @endcode
    ///
    /// @param [in] aCelestialObject A celestial object.
    /// @return True if the geometry intersects the celestial object.
    bool intersects(const Celestial& aCelestialObject) const;

    /// @brief Check if the geometry contains another geometry.
    ///
    /// @code{.cpp}
    ///     bool doesContain = geometry.contains(otherGeometry);
    /// @endcode
    ///
    /// @param [in] aGeometry A geometry.
    /// @return True if this geometry contains the other geometry.
    bool contains(const ObjectGeometry& aGeometry) const;

    /// @brief Check if the geometry contains a celestial object.
    ///
    /// @code{.cpp}
    ///     bool doesContain = geometry.contains(celestialObject);
    /// @endcode
    ///
    /// @param [in] aCelestialObject A celestial object.
    /// @return True if the geometry contains the celestial object.
    bool contains(const Celestial& aCelestialObject) const;

    /// @brief Access the composite 3D object.
    ///
    /// @code{.cpp}
    ///     const Composite& composite = geometry.accessComposite();
    /// @endcode
    ///
    /// @return A reference to the composite.
    const Composite& accessComposite() const;

    /// @brief Access the reference frame.
    ///
    /// @code{.cpp}
    ///     Shared<const Frame> frame = geometry.accessFrame();
    /// @endcode
    ///
    /// @return A shared pointer to the frame.
    Shared<const Frame> accessFrame() const;

    /// @brief Get the geometry expressed in a given frame.
    ///
    /// @code{.cpp}
    ///     ObjectGeometry geom = geometry.getGeometryIn(frameSPtr);
    /// @endcode
    ///
    /// @param [in] aFrameSPtr A shared pointer to the target frame.
    /// @return The geometry in the target frame.
    ObjectGeometry getGeometryIn(const Shared<const Frame>& aFrameSPtr) const;

    /// @brief Compute the intersection with another geometry.
    ///
    /// @code{.cpp}
    ///     ObjectGeometry intersection = geometry.intersectionWith(otherGeometry);
    /// @endcode
    ///
    /// @param [in] aGeometry A geometry.
    /// @return The intersection geometry.
    ObjectGeometry intersectionWith(const ObjectGeometry& aGeometry) const;

    /// @brief Compute the intersection with a celestial object.
    ///
    /// @code{.cpp}
    ///     ObjectGeometry intersection = geometry.intersectionWith(celestialObject);
    /// @endcode
    ///
    /// @param [in] aCelestialObject A celestial object.
    /// @return The intersection geometry.
    ObjectGeometry intersectionWith(const Celestial& aCelestialObject) const;

    /// @brief Construct an undefined geometry.
    ///
    /// @code{.cpp}
    ///     Geometry geometry = Geometry::Undefined();
    /// @endcode
    ///
    /// @return An undefined geometry.
    static Geometry Undefined();

    /// @brief Configure a geometry from a configuration.
    ///
    /// @code{.cpp}
    ///     Shared<Geometry> geometry = Geometry::Configure(geometryConfiguration, componentSPtr);
    /// @endcode
    ///
    /// @param [in] aGeometryConfiguration A geometry configuration.
    /// @param [in] aComponentSPtr A shared pointer to the owning component.
    /// @return A shared pointer to the configured geometry.
    static Shared<Geometry> Configure(
        const GeometryConfiguration& aGeometryConfiguration, const Shared<const Component>& aComponentSPtr
    );

   private:
    String name_;
    ObjectGeometry geometry_;
    Shared<const Component> componentPtr_;
};

/// @brief Configuration for constructing a Geometry.
struct GeometryConfiguration
{
    const String name;          ///< The geometry name.
    const Composite composite;  ///< The 3D composite object.
};

}  // namespace component
}  // namespace simulation
}  // namespace ostk

#endif
