/**
 * The main application code
 */

#include "app.hpp"
#include <string>
#include "platforms/eventLoop.hpp"

Application::Application(IConfig &config, IWifi &wifi, ILogging &logger,
                         IAudio &audio, IUserInterface &ui,
                         IEventLoop &eventLoop, ILibrary &library)
    : config(config),
      wifi(wifi),
      logger(logger),
      audio(audio),
      ui(ui),
      eventLoop(eventLoop),
      library(library),
      currentState(APP_UNINITIALISED) {
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
            ui.setState(IUserInterface::LedState::Reset);
            break;
        case EVENT_PLAY_BUTTON_PRESSED:
            logger.debug("Play button pressed event received");
            randomSound = library.getRandomSound();
            url = "https://base-url/" + randomSound + ".mp3";
            audio.play(url);
            break;
        default:
            logger.error("Unknown event received: %d", event);
            break;
        }
    });
    currentState = getState();
    logger.info("----- Setup Done --------------");
}

void Application::loop() {
    currentState = getState();  // TODO replace with an event-handling system
                                // using the ESP32 event loop
    ui.tick();
    switch (currentState) {
    case APP_UNINITIALISED:
    default:
        ui.setState(IUserInterface::LedState::Off);
        config.tick();
        break;
    case APP_NO_NETWORK:
        ui.setState(IUserInterface::LedState::Connecting);
        wifi.tick();
        wifi.connect(config.get(WIFI_SSID_KEY).c_str(),
                     config.get(WIFI_PASSWORD_KEY).c_str());
        break;
    case APP_RUNNING:
        ui.setState(IUserInterface::LedState::Normal);
        audio.tick();
        wifi.tick();
        break;
    }
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
