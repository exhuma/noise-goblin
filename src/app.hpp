#pragma once

/**
 * The main application code
 */

#include "platforms/config.hpp"
#include "platforms/logging.hpp"
#include "platforms/wifi.hpp"

enum AppState { APP_UNINITIALISED, APP_RUNNING, APP_NO_NETWORK };

// Global state defined in app.cpp
extern AppState appState;

class Application {
  public:
    explicit Application(IConfig &config, IWifi &wifi, ILogging &logger);
    void setup();
    void loop();
    AppState getState();
    void handleEvent(int event);

  private:
    IConfig &config;
    ILogging &logger;
    IWifi &wifi;
};
