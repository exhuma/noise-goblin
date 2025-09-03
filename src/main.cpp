#include <cstdio>
#include <cstring>

#if defined(BUILD_ESP32)
    #include "platforms/esp32/audio_impl.hpp"
    #include "platforms/esp32/config_impl.hpp"
    #include "platforms/esp32/logging_impl.hpp"
    #include "platforms/esp32/ui_impl.hpp"
    #include "platforms/esp32/wifi_impl.hpp"
Esp32Logging logging;
Esp32Wifi wifi(logging);
Esp32Audio audio(logging);
Esp32Config config(logging);
Esp32Ui ui(audio, config, logging);
#else
    #include "platforms/posix/audio_impl.hpp"
    #include "platforms/posix/config_impl.hpp"
    #include "platforms/posix/logging_impl.hpp"
    #include "platforms/posix/ui_impl.hpp"
    #include "platforms/posix/wifi_impl.hpp"
PosixLogging logging;
PosixWifi wifi(logging);
PosixAudio audio(logging);
PosixConfig config(logging);
PosixUi ui(audio, config, logging);
#endif

enum AppState { APP_REQUESTING_CONFIG, APP_RUNNING };
AppState appState = APP_REQUESTING_CONFIG;

void setup() {
    logging.setup();
    audio.setup();
    ui.setup();
    char ssid[256];
    char password[256];
    config.get("wifi_ssid", ssid, sizeof(ssid));
    config.get("wifi_password", password, sizeof(password));
    if (strcmp(ssid, "") == 0 || strcmp(password, "") == 0) {
        appState = APP_REQUESTING_CONFIG;
        return;
    } else {
        appState = APP_RUNNING;
    }
    wifi.setup(ssid, password);
}

void loop() {
    switch (appState) {
    case APP_REQUESTING_CONFIG:
    default:
        if (config.tick()) {
            appState = APP_RUNNING;
        }
        break;
    case APP_RUNNING:
        audio.tick();
        ui.tick();
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
