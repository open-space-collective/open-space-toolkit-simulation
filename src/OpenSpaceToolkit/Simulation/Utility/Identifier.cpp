/// Apache License 2.0

#include <OpenSpaceToolkit/Simulation/Utility/Identifier.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace ostk
{
namespace simulation
{
namespace utility
{

String generateId()
{
    boost::uuids::random_generator generator;
    return boost::uuids::to_string(generator());
}

}  // namespace utility
}  // namespace simulation
}  // namespace ostk
