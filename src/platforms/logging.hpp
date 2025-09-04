#pragma once

#include <cstdarg>
#include <string>

/// @brief The logging subsystem
/// @details Interface for logging functionality.
///
/// Provides an abstract interface for logging operations.
struct ILogging {
    /// @brief Virtual destructor for proper cleanup of derived classes.
    virtual ~ILogging() = default;

    /// @brief Initialises the logging subsystem.
    virtual void setup() = 0;

    /// @brief Logs a debug message.
    virtual void debug(const char *fmt, ...) = 0;

    /// @brief Logs an info message.
    virtual void info(const char *fmt, ...) = 0;

    /// @brief Logs an error message.
    virtual void error(const char *fmt, ...) = 0;
};
