#pragma once

/// @brief The user-interface subsystem
/// @details Interface for user-interface functionality.
///
/// Provides an abstract interface for user-interface operations.
/// The responsibility of the User-Interface is to interact with the user.
/// Collect input, provide feedback.
struct IUserInterface {
    /// @brief Enumeration for LED states.
    enum class LedState { Off, CaptivePortal, Reset, Normal, Connecting };

    /// @brief Virtual destructor for proper cleanup of derived classes.
    virtual ~IUserInterface() = default;

    /// @brief Initializes the user-interface subsystem.
    virtual void setup() = 0;

    /// @brief Updates the user-interface state.
    virtual void tick() = 0;

    /// @brief Sets the LED state.
    virtual void setState(LedState state) = 0;
};
