#pragma once

#include <cstddef>
#include <map>
#include <string>
#include "../const.hpp"
#include "config_ui.hpp"
#include "logging.hpp"

/// @brief The Configuration subsystem
/// @details Interface for configuration management functionality.
///
/// Provides an abstract interface for configuration operations.
struct IConfig {
  public:
    IConfig(ILogging &logger, IConfigUi &configUi)
        : logger(logger), configUi(configUi) {
    }

    /// @brief Virtual destructor for proper cleanup of derived classes.
    virtual ~IConfig() = default;

    /// @brief Retrieves the value associated with the specified configuration
    /// key.
    /// @param key The configuration key to retrieve the value for.
    /// @return The value associated with the specified key.
    virtual auto get(const char *key) -> std::string = 0;

    /// @brief Sets the value for the specified configuration key.
    /// @param key The configuration key to set the value for.
    /// @param value The value to associate with the specified key.
    virtual void set(const char *key, const char *value) = 0;

    /// @brief Retrieves the current configuration values.
    /// @return A map containing the current configuration key-value pairs.
    virtual auto getAll() -> std::map<std::string, std::string> = 0;

    /// @brief Clears all configuration settings.
    virtual void clear() = 0;

    /// @brief Main loop for the configuration subsystem.
    virtual void tick() = 0;

  protected:
    ILogging &logger;
    IConfigUi &configUi;
};
