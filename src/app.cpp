#include "app.hpp"
#include <string>
#include "const.hpp"
#include "platforms/eventLoop.hpp"

Application::Application(IConfig &config, IWifi &wifi, ILogging &logger,
                         IAudio &audio, IUserInterface &ui,
                         IEventLoop &eventLoop, ILibrary &library,
                         IConfigUi &configUi)
    : config(config),
      wifi(wifi),
      logger(logger),
      audio(audio),
      ui(ui),
      configUi(configUi),
      eventLoop(eventLoop),
      library(library),
      currentState(RequestingConfig) {
}

void Application::setup() {
    logger.setup();
    audio.setup();
    ui.setup();
    wifi.setup();
    eventLoop.setup();
    eventLoop.setEventCallback([&](int event) {
        std::string randomSound;
        std::string url;
        switch (event) {
        case EVENT_RESET_BUTTON_PRESSED:
            logger.debug("Reset button pressed event received");
            config.clear();
            break;
        case EVENT_PLAY_BUTTON_PRESSED:
            logger.debug("Play button pressed event received");
            url = library.getRandomSound();
            audio.play(url);
            break;
        default:
            logger.error("Unknown event received: %d", event);
            break;
        }
    });
    currentState = computeState();
    logger.info("----- Setup Done --------------");
}

void Application::loop() {
    std::map<std::string, std::string> storedValues;
    currentState = computeState();  // TODO replace with an event-handling
                                    // system using the ESP32 event loop
    ui.tick();
    ui.setState(currentState);
    switch (currentState) {
    case RequestingConfig:
        storedValues = config.getAll();
        if (storedValues[WIFI_SSID_KEY].empty()) {
            config.set(WIFI_SSID_KEY,
                       configUi.promptFor(WIFI_SSID_KEY, "WiFi SSID").c_str());
            return;  // liberate the loop
        }
        if (storedValues[WIFI_PASSWORD_KEY].empty()) {
            config.set(
                WIFI_PASSWORD_KEY,
                configUi.promptFor(WIFI_PASSWORD_KEY, "Enter WiFi Password")
                    .c_str());
            return;  // liberate the loop
        }
        if (storedValues[LIBRARY_BASE_URL_KEY].empty()) {
            config.set(
                LIBRARY_BASE_URL_KEY,
                configUi
                    .promptFor(LIBRARY_BASE_URL_KEY, "Enter Library Base URL")
                    .c_str());
            return;  // liberate the loop
        }
        break;
    case NoNetwork:
        wifi.tick();
        wifi.connect(config.get(WIFI_SSID_KEY).c_str(),
                     config.get(WIFI_PASSWORD_KEY).c_str());
        break;
    default:
    case Normal:
        library.tick();
        audio.tick();
        wifi.tick();
        break;
    }
}

auto Application::computeState() -> AppState {
    std::string ssid = config.get(WIFI_SSID_KEY);
    std::string password = config.get(WIFI_PASSWORD_KEY);
    std::string library_base_url = config.get(LIBRARY_BASE_URL_KEY);

    if (ssid.empty() || password.empty() || library_base_url.empty()) {
        return RequestingConfig;
    } else if (!wifi.isConnected()) {
        return NoNetwork;
    } else {
        return Normal;
    }
}
