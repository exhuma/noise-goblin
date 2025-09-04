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

enum AppState { APP_UNINITIALISED, APP_RUNNING, APP_NO_NETWORK };

class Application {
  public:
    explicit Application(IConfig &config, IWifi &wifi, ILogging &logger,
                         IAudio &audio, IUserInterface &ui,
                         IEventLoop &eventLoop, ILibrary &library);
    void setup();
    void loop();
    AppState getState();

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
