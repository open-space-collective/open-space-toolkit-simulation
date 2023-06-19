/// Apache License 2.0

#include <OpenSpaceToolkit/Simulation/Utilities/Identifier.hpp>

#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid.hpp>

namespace ostk
{
namespace simulation
{
namespace utilities
{

String                          generateId                                  ( )
{
    boost::uuids::random_generator generator ;
    return boost::uuids::to_string(generator()) ;
}

}
}
}
