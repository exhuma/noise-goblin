#include "app.hpp"
#include <string>
#include "const.hpp"
#include "enum.hpp"

Application::Application(IConfig &config, IWifi &wifi, ILogging &logger,
                         IAudio &audio, IUserInterface &ui, ILibrary &library,
                         IConfigUi &configUi)
    : config(config),
      wifi(wifi),
      logger(logger),
      audio(audio),
      ui(ui),
      configUi(configUi),
      library(library),
      currentState(RequestingConfig) {
}

void Application::setup() {
    logger.setup();
    audio.setup();
    ui.setup();
    wifi.setup();
    currentState = Startup;
    logger.info("----- Setup Done --------------");
}

void Application::loop() {
    std::map<std::string, std::string> storedValues;
    std::string ssid;
    std::string password;
    std::string library_base_url;
    std::vector<int> events;
    std::vector<int> wifiEvents;
    auto uiEvents =
        ui.tick(currentState);  // User-interface should always get a tick.
    events.insert(events.end(), uiEvents.begin(), uiEvents.end());

    switch (currentState) {
    case AppState::Startup:
        ssid = config.get(WIFI_SSID_KEY);
        password = config.get(WIFI_PASSWORD_KEY);
        library_base_url = config.get(LIBRARY_BASE_URL_KEY);
        if (ssid.empty() || password.empty() || library_base_url.empty()) {
            currentState = RequestingConfig;
            break;
        }
        wifiEvents = wifi.connect(ssid.c_str(), password.c_str());
        events.insert(events.end(), wifiEvents.begin(), wifiEvents.end());
        break;
    case AppState::RequestingConfig:
        configUi.start();
        configUi.tick();
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
        wifiEvents = wifi.connect(config.get(WIFI_SSID_KEY).c_str(),
                                  config.get(WIFI_PASSWORD_KEY).c_str());
        events.insert(events.end(), wifiEvents.begin(), wifiEvents.end());
        break;
    case AppState::Connecting:
        wifiEvents = wifi.tick();
        events.insert(events.end(), wifiEvents.begin(), wifiEvents.end());
        break;
    default:
    case AppState::Normal:
        library.tick();
        audio.tick();
        wifiEvents = wifi.tick();
        events.insert(events.end(), wifiEvents.begin(), wifiEvents.end());
        break;
    }

    // Handle Events
    for (const auto &event : events) {
        std::string url;
        switch (event) {
        case EVENT_RESET_BUTTON_PRESSED:
            logger.debug("Reset button pressed event received");
            configUi.setDefaults(config.getAll());  // keep a backup
            config.clear();
            currentState = AppState::RequestingConfig;
            break;
        case EVENT_PLAY_BUTTON_PRESSED:
            logger.debug("Play button pressed event received");
            url = library.getRandomSound();
            audio.play(url);
            break;
        case SOUND_PLAY_STARTED:
            currentState = AppState::PlayingSound;
            break;
        case SOUND_PLAY_STOPPED:
        case EVENT_WIFI_CONNECTED:
            currentState = AppState::Normal;
            break;
        case EVENT_WIFI_CONNECTING:
            currentState = AppState::Connecting;
            break;
        default:
            logger.error("Unknown event received: %d", event);
            break;
        }
    }
}
