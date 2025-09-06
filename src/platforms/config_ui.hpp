#pragma once

#include <map>
#include "logging.hpp"

struct IConfigUi {
  public:
    enum PortalState { PORTAL_OFF, PORTAL_RUNNING };

    IConfigUi(ILogging& logger) : logger(logger) {
    }

    virtual ~IConfigUi() = default;

    /// @brief Start the configuration UI.
    /// @param config The configuration object.
    /// @param logger The logging object.
    virtual void start() = 0;

    /// @brief Prompts the user for input for a specific config-value with a
    /// message and a default value.
    /// @param key The configuration key to prompt for.
    /// @param message The message to display to the user.
    /// @param default_value The default value to use if the user no input.
    /// @return The user's input or the default value if no input was given.
    virtual auto promptFor(const std::string& key, const std::string& message)
        -> std::string = 0;

    virtual void tick() = 0;
    virtual auto isRunning() -> bool = 0;
    virtual void setDefaults(
        const std::map<std::string, std::string>& defaults) = 0;

  protected:
    ILogging& logger;
};
