#pragma once
#include "../enum.hpp"
#include "eventLoop.hpp"
#include "logging.hpp"

/// @brief The user-interface subsystem
/// @details Interface for user-interface functionality.
///
/// Provides an abstract interface for user-interface operations.
/// The responsibility of the User-Interface is to interact with the user.
/// Collect input, provide feedback.
struct IUserInterface {
    IUserInterface(ILogging &logger, IEventLoop &eventLoop)
        : logger(logger), eventLoop(eventLoop) {
    }

    /// @brief Virtual destructor for proper cleanup of derived classes.
    virtual ~IUserInterface() = default;

    /// @brief Initializes the user-interface subsystem.
    virtual void setup() = 0;

    /// @brief Updates the user-interface state.
    virtual void tick() = 0;

    /// @brief Sets the application state.
    virtual void setState(AppState state) = 0;

  protected:
    ILogging &logger;
    IEventLoop &eventLoop;
    AppState appState;
};
