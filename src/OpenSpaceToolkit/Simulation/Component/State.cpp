/// Apache License 2.0

#include <OpenSpaceToolkit/Simulation/Component/State.hpp>

#include <OpenSpaceToolkit/Core/Utility.hpp>

namespace ostk
{
namespace simulation
{
namespace component
{

State::State(const State::Status& aStatus, const bool& isVerbose)
    : status_(aStatus),
      verbose_(isVerbose)
{
}

State::State(const State& aState)
    : status_(aState.status_),
      verbose_(aState.verbose_)
{
}

State::~State() {}

State* State::clone() const
{
    return new State(*this);
}

State& State::operator=(const State& aState)
{
    if (this != &aState)
    {
        status_ = aState.status_;
        verbose_ = aState.verbose_;
    }

    return *this;
}

bool State::operator==(const State& aState) const
{
    return (status_ == aState.status_) && (verbose_ == aState.verbose_);
}

State::Status State::getStatus() const
{
    return status_;
}

}  // namespace component
}  // namespace simulation
}  // namespace ostk
