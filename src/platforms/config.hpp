#pragma once

#include <cstddef>
#include <string>

/// @brief WiFi SSID configuration key
inline const char *WIFI_SSID_KEY = "wifi_ssid";

/// @brief WiFi password configuration key
inline const char *WIFI_PASSWORD_KEY = "wifi_password";

/// @brief The Configuration subsystem
/// @details Interface for configuration management functionality.
///
/// Provides an abstract interface for configuration operations.
struct IConfig {
    /// @brief Virtual destructor for proper cleanup of derived classes.
    virtual ~IConfig() = default;

    /// @brief Retrieves the value associated with the specified configuration
    /// key.
    /// @param key The configuration key to retrieve the value for.
    /// @return The value associated with the specified key.
    virtual std::string get(const char *key) = 0;

    /// @brief Sets the value for the specified configuration key.
    /// @param key The configuration key to set the value for.
    /// @param value The value to associate with the specified key.
    virtual void set(const char *key, const char *value) = 0;

    /// @brief Performs periodic configuration-related tasks or updates.
    /// @return True if we have configuration data available, False otherwise.
    virtual bool tick() = 0;  // TODO: should return void. Use a better method
                              // to retrieve the current state.

    /// @brief Clears all configuration settings.
    virtual void clear() = 0;
};
