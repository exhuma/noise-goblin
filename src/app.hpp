#pragma once

/**
 * The main application code
 */

#include "enum.hpp"
#include "platforms/audio.hpp"
#include "platforms/config.hpp"
#include "platforms/config_ui.hpp"
#include "platforms/library.hpp"
#include "platforms/logging.hpp"
#include "platforms/ui.hpp"
#include "platforms/wifi.hpp"

/// @class Application
/// @brief Manages the main application lifecycle reacts to user-input, gives
/// user-feedback, and coordinates core subsystems.
///
/// @param config Reference to the configuration subsystem.
/// @param wifi Reference to the WiFi subsystem.
/// @param logger Reference to the logging subsystem.
/// @param audio Reference to the audio subsystem.
/// @param ui Reference to the user interface subsystem.
/// @param library Reference to the library subsystem.
/// @param configUi Reference to the configuration UI subsystem.
class Application {
  public:
    explicit Application(IConfig &config, IWifi &wifi, ILogging &logger,
                         IAudio &audio, IUserInterface &ui, ILibrary &library,
                         IConfigUi &configUi);
    /// @brief One-time startup initialisation code
    /// @details This implements the default Arduino setup() entry-point
    void setup();

    /// @brief Main application loop
    /// @details This implements the default Arduino loop() entry-point
    void loop();

  private:
    IConfig &config;
    ILogging &logger;
    IWifi &wifi;
    IAudio &audio;
    IUserInterface &ui;
    ILibrary &library;
    IConfigUi &configUi;
    AppState currentState = AppState::Startup;
    std::map<std::string, std::string> configBackup;
};
