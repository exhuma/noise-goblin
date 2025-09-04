#pragma once
#include <string>

/// @brief The Audio subsystem
/// @details Interface for audio platform functionality.
///
/// Provides an abstract interface for audio operations.
struct IAudio {
    /// @brief Virtual destructor for proper cleanup of derived classes.
    virtual ~IAudio() = default;

    /// @brief Initializes the audio hardware or platform.
    virtual void setup() = 0;

    /// @brief Plays audio from the specified URL.
    /// @param url The URL of the audio stream or file.
    virtual void play(std::string url) = 0;

    /// @brief Performs periodic audio-related tasks or updates.
    virtual void tick() = 0;
};
