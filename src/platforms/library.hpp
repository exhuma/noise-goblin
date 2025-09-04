#pragma once
#include <string>

/// @brief The sound-library subsystem
/// @details Interface for sound library functionality.
///
/// Provides an abstract interface for sound library operations.
struct ILibrary {
    /// @brief Virtual destructor for proper cleanup of derived classes.
    virtual ~ILibrary() = default;

    /// @brief Sets up the sound library.
    virtual void setup() = 0;

    /// @brief Retrieves a random sound from the library.
    /// @return A string representing the random sound.
    virtual auto getRandomSound() -> std::string = 0;
};
