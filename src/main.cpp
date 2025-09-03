#include <cstdio>
#include <cstring>
#include "audio_loop.hpp"
#include "config_loop.hpp"
#include "logging.hpp"
#include "ui_loop.hpp"
#include "wifi_loop.hpp"

#if defined(BUILD_ESP32)
    #include "platforms/esp32/audio_impl.hpp"
    #include "platforms/esp32/config_impl.hpp"
    #include "platforms/esp32/logging_impl.hpp"
    #include "platforms/esp32/ui_impl.hpp"
    #include "platforms/esp32/wifi_impl.hpp"
#else
    #include "platforms/posix/audio_impl.hpp"
    #include "platforms/posix/config_impl.hpp"
    #include "platforms/posix/logging_impl.hpp"
    #include "platforms/posix/ui_impl.hpp"
    #include "platforms/posix/wifi_impl.hpp"
#endif

enum AppState { APP_REQUESTING_CONFIG, APP_RUNNING };
AppState appState = APP_REQUESTING_CONFIG;

void setup() {
#if defined(BUILD_ESP32)
    Esp32Logging logging_impl;              // stack-allocated, caller owns lifetime
    Esp32Wifi wifi_impl(logging_impl);      // stack-allocated, caller owns lifetime
    Esp32Audio audio_impl(logging_impl);    // stack-allocated, caller owns lifetime
    Esp32Config config_impl(logging_impl);  // stack-allocated, caller owns lifetime
    Esp32Ui ui_impl(audio_impl, config_impl,
                    logging_impl);  // stack-allocated, caller owns lifetime
#else
    PosixLogging logging_impl;                               // stack-allocated for host
    PosixWifi wifi_impl(logging_impl);                       // stack-allocated for host
    PosixAudio audio_impl(logging_impl);                     // stack-allocated for host
    PosixConfig config_impl(logging_impl);                   // stack-allocated for host
    PosixUi ui_impl(audio_impl, config_impl, logging_impl);  // stack-allocated for host
#endif
    AudioLoop player(audio_impl);
    UiLoop ui_loop(ui_impl);
    WifiLoop wifi_loop(wifi_impl);
    Logging logger(logging_impl);
    logger.setup();
    ConfigLoop config_loop(config_impl);
    player.setup();
    ui_loop.setup();
    char ssid[256];
    char password[256];
    config_loop.get("wifi_ssid", ssid, sizeof(ssid));
    config_loop.get("wifi_password", password, sizeof(password));
    if (strcmp(ssid, "") == 0 || strcmp(password, "") == 0) {
        appState = APP_REQUESTING_CONFIG;
        return;
    } else {
        appState = APP_RUNNING;
    }
    wifi_loop.setup(ssid, password);
}

void loop() {
#if defined(BUILD_ESP32)
    Esp32Logging logging_impl;              // stack-allocated, caller owns lifetime
    Esp32Wifi wifi_impl(logging_impl);      // stack-allocated, caller owns lifetime
    Esp32Audio audio_impl(logging_impl);    // stack-allocated, caller owns lifetime
    Esp32Config config_impl(logging_impl);  // stack-allocated, caller owns lifetime
    Esp32Ui ui_impl(audio_impl, config_impl,
                    logging_impl);  // stack-allocated, caller owns lifetime
#else
    PosixLogging logging_impl;                               // stack-allocated for host
    PosixWifi wifi_impl(logging_impl);                       // stack-allocated for host
    PosixAudio audio_impl(logging_impl);                     // stack-allocated for host
    PosixConfig config_impl(logging_impl);                   // stack-allocated for host
    PosixUi ui_impl(audio_impl, config_impl, logging_impl);  // stack-allocated for host
#endif
    ConfigLoop config_loop(config_impl);
    AudioLoop player(audio_impl);
    UiLoop ui_loop(ui_impl);
    WifiLoop wifi_loop(wifi_impl);
    Logging logger(logging_impl);
    switch (appState) {
    case APP_REQUESTING_CONFIG:
    default:
        if (config_impl.tick()) {
            appState = APP_RUNNING;
        }
        break;
    case APP_RUNNING:
        player.tick();
        ui_loop.tick();
        wifi_loop.tick();
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
