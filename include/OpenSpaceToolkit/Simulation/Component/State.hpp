/// Apache License 2.0

#ifndef __OpenSpaceToolkit_Simulation_Component_State__
#define __OpenSpaceToolkit_Simulation_Component_State__

namespace ostk
{
namespace simulation
{
namespace component
{

#define DEFAULT_STATUS State::Status::Disabled
#define DEFAULT_VERBOSE false

/// @brief Represents the state of a component.
/// @details A State holds a Status (Undefined, Disabled, Idle, Busy, Error) and a verbosity flag.
///
/// @code{.cpp}
///     State state(State::Status::Idle, false);
///     State::Status status = state.getStatus();
/// @endcode
class State
{
   public:
    /// @brief Component status enumeration.
    enum class Status
    {

        Undefined,
        Disabled,
        Idle,
        Busy,
        Error

    };

    /// @brief Construct a state.
    ///
    /// @code{.cpp}
    ///     State state(State::Status::Idle, true);
    /// @endcode
    ///
    /// @param [in] aStatus A status (default: Disabled).
    /// @param [in] isVerbose If true, enable verbose mode (default: false).
    State(const State::Status& aStatus = DEFAULT_STATUS, const bool& isVerbose = DEFAULT_VERBOSE);

    /// @brief Copy constructor.
    ///
    /// @code{.cpp}
    ///     State original(State::Status::Idle);
    ///     State copy(original);
    /// @endcode
    ///
    /// @param [in] aState A state.
    State(const State& aState);

    /// @brief Destructor.
    virtual ~State();

    /// @brief Clone the state.
    ///
    /// @code{.cpp}
    ///     State state(State::Status::Idle);
    ///     State* cloned = state.clone();
    /// @endcode
    ///
    /// @return A pointer to a cloned state.
    virtual State* clone() const;

    /// @brief Copy assignment operator.
    ///
    /// @code{.cpp}
    ///     State first(State::Status::Idle);
    ///     State second = first;
    /// @endcode
    ///
    /// @param [in] aState A state.
    /// @return Reference to the assigned state.
    State& operator=(const State& aState);

    /// @brief Equal to operator.
    ///
    /// @code{.cpp}
    ///     State firstState(State::Status::Idle);
    ///     State secondState(State::Status::Idle);
    ///     bool isEqual = (firstState == secondState); // true
    /// @endcode
    ///
    /// @param [in] aState A state.
    /// @return True if states are equal.
    bool operator==(const State& aState) const;

    /// @brief Check if the state is defined.
    ///
    /// @code{.cpp}
    ///     State state(State::Status::Idle);
    ///     bool defined = state.isDefined(); // true
    /// @endcode
    ///
    /// @return True if the state is defined.
    bool isDefined() const;

    /// @brief Get the status.
    ///
    /// @code{.cpp}
    ///     State state(State::Status::Idle);
    ///     State::Status status = state.getStatus(); // State::Status::Idle
    /// @endcode
    ///
    /// @return The status.
    State::Status getStatus() const;

    /// @brief Construct an undefined state.
    ///
    /// @code{.cpp}
    ///     State state = State::Undefined();
    ///     bool defined = state.isDefined(); // false
    /// @endcode
    ///
    /// @return An undefined state.
    static State Undefined();

   private:
    State::Status status_;
    bool verbose_;
};

}  // namespace component
}  // namespace simulation
}  // namespace ostk

#endif
