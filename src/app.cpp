/**
 * The main application code
 */

#include "app.hpp"
#include <string>
#include "platforms/eventLoop.hpp"

AppState appState = APP_UNINITIALISED;

Application::Application(IConfig &config, IWifi &wifi, ILogging &logger)
    : config(config), wifi(wifi), logger(logger) {
}

void Application::setup() {
}

void Application::loop() {
}

AppState Application::getState() {
    std::string ssid = config.get(WIFI_SSID_KEY);
    std::string password = config.get(WIFI_PASSWORD_KEY);

    if (ssid.empty() || password.empty()) {
        return APP_UNINITIALISED;
    } else if (!wifi.isConnected()) {
        return APP_NO_NETWORK;
    } else {
        return APP_RUNNING;
    }
}
