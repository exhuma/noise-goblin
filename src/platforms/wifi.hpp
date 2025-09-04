#pragma once

/// @brief The WiFi subsystem
/// @details Interface for WiFi platform functionality.
///
/// Provides an abstract interface for WiFi operations.
struct IWifi {
    /// @brief Virtual destructor for proper cleanup of derived classes.
    virtual ~IWifi() = default;

    /// @brief Initializes the WiFi hardware or platform.
    virtual void setup() = 0;

    /// @brief Connects to a WiFi network using the provided SSID and password.
    /// @param ssid The SSID (network name) to connect to.
    /// @param password The password for the WiFi network.
    virtual void connect(const char *ssid, const char *password) = 0;

    /// @brief Performs periodic WiFi-related tasks or updates.
    virtual void tick() = 0;

    /// @brief Checks if the device is currently connected to a WiFi network.
    /// @return True if connected, false otherwise.
    virtual auto isConnected() -> bool = 0;
};
