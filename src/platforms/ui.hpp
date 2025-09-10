#pragma once
#include <vector>
#include "../enum.hpp"
#include "logging.hpp"

/// @brief The user-interface subsystem
/// @details Interface for user-interface functionality.
///
/// Provides an abstract interface for user-interface operations.
/// The responsibility of the User-Interface is to interact with the user.
/// Collect input, provide feedback.
struct IUserInterface {
    IUserInterface(ILogging &logger) : logger(logger) {
    }

    /// @brief Virtual destructor for proper cleanup of derived classes.
    virtual ~IUserInterface() = default;

    /// @brief Initializes the user-interface subsystem.
    virtual void setup() = 0;

    /// @brief Updates the user-interface state.
    virtual auto tick(AppState state) -> std::vector<int> = 0;

  protected:
    ILogging &logger;
};
