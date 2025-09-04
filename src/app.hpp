#pragma once

/**
 * The main application code
 */

#include "platforms/audio.hpp"
#include "platforms/config.hpp"
#include "platforms/eventLoop.hpp"
#include "platforms/library.hpp"
#include "platforms/logging.hpp"
#include "platforms/ui.hpp"
#include "platforms/wifi.hpp"

/// @brief Possible application states
/// @details These are used during the main-loop to display useful information
/// to the user and/or request user-input.
enum AppState { APP_UNINITIALISED, APP_RUNNING, APP_NO_NETWORK };

/// @class Application
/// @brief Manages the main application lifecycle reacts to user-input, gives
/// user-feedback, and coordinates core subsystems.
///
/// @param config Reference to the configuration subsystem.
/// @param wifi Reference to the WiFi subsystem.
/// @param logger Reference to the logging subsystem.
/// @param audio Reference to the audio subsystem.
/// @param ui Reference to the user interface subsystem.
/// @param eventLoop Reference to the event loop subsystem.
/// @param library Reference to the library subsystem.
class Application {
  public:
    explicit Application(IConfig &config, IWifi &wifi, ILogging &logger,
                         IAudio &audio, IUserInterface &ui,
                         IEventLoop &eventLoop, ILibrary &library);
    /// @brief One-time startup initialisation code
    /// @details This implements the default Arduino setup() entry-point
    void setup();

    /// @brief Main application loop
    /// @details This implements the default Arduino loop() entry-point
    void loop();

    /// @brief Determine the current state of the system.
    /// @details This looks at various subsystems and determines the current
    /// application state. Depending on state, the system changes behaviour.
    /// @return The current application state.
    auto computeState() -> AppState;

  private:
    IConfig &config;
    ILogging &logger;
    IWifi &wifi;
    IAudio &audio;
    IUserInterface &ui;
    IEventLoop &eventLoop;
    ILibrary &library;
    AppState currentState;
};
