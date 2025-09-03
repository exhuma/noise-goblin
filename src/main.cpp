#include <cstdio>
#include <cstring>
#include <string>
#include "app.hpp"

#if defined(BUILD_ESP32)
    #include "platforms/esp32/audio_impl.hpp"
    #include "platforms/esp32/config_impl.hpp"
    #include "platforms/esp32/event_loop_impl.hpp"
    #include "platforms/esp32/library_impl.hpp"
    #include "platforms/esp32/logging_impl.hpp"
    #include "platforms/esp32/ui_impl.hpp"
    #include "platforms/esp32/wifi_impl.hpp"
Esp32EventLoop eventLoop;
Esp32Logging logging;
Esp32Wifi wifi(logging);
Esp32Audio audio(logging);
Esp32Config config(logging);
Esp32Ui ui(audio, config, logging, eventLoop);
Esp32Library library(logging);
#else
    #include "platforms/posix/audio_impl.hpp"
    #include "platforms/posix/config_impl.hpp"
    #include "platforms/posix/event_loop_impl.hpp"
    #include "platforms/posix/library_impl.hpp"
    #include "platforms/posix/logging_impl.hpp"
    #include "platforms/posix/ui_impl.hpp"
    #include "platforms/posix/wifi_impl.hpp"
PosixEventLoop eventLoop;
PosixLogging logging;
PosixWifi wifi(logging);
PosixAudio audio(logging);
PosixConfig config(logging);
PosixUi ui(audio, config, logging, eventLoop);
PosixLibrary library(logging);
#endif

// Instantiate the application with the active config implementation
Application app(config, wifi, logging);
void setup() {
    logging.setup();
    audio.setup();
    ui.setup();
    wifi.setup();
    eventLoop.setup();
    appState = app.getState();
    eventLoop.setEventCallback([&](int event) {
        std::string randomSound;
        std::string url;
        switch (event) {
        case EVENT_RESET_BUTTON_PRESSED:
            logging.debug("Reset button prersed event received");
            config.clear();
            ui.displayConfigCleared();
            break;
        case EVENT_PLAY_BUTTON_PRESSED:
            logging.debug("Play button pressed event received");
            randomSound = library.getRandomSound();
            url = "https://base-url/" + randomSound + ".mp3";
            audio.play(url);
            break;
        default:
            logging.error("Unknown event received: %d", event);
            break;
        }
    });
    logging.info("----- Setup Done --------------");
}

void loop() {
    appState = app.getState();  // TODO replace with an event-handling system
                                // using the ESP32 event loop
    ui.tick();
    switch (appState) {
    case APP_UNINITIALISED:
    default:
        logging.debug("App state: APP_UNINITIALISED");
        ui.displayCaptivePortal();
        config.tick();
        break;
    case APP_NO_NETWORK:
        logging.debug("App state: APP_NO_NETWORK");
        wifi.tick();
        wifi.connect(config.get(WIFI_SSID_KEY).c_str(),
                     config.get(WIFI_PASSWORD_KEY).c_str());
        break;
    case APP_RUNNING:
        logging.debug("App state: APP_RUNNING");
        ui.displayNormal();
        audio.tick();
        wifi.tick();
        break;
    }
}

// Entry-point for POSIX platforms (ignored by ESP32)
int main() {
    setup();
    while (1) {
        loop();
    }
    return 0;
}
