#include <cstdio>
#include <cstring>
#include "config.h"
#include "platforms/audio.h"
#include "platforms/config.h"
#include "platforms/logging.h"
#include "platforms/wifi.h"

enum AppState { APP_REQUESTING_CONFIG, APP_RUNNING };
AppState appState = APP_REQUESTING_CONFIG;

void setup() {
    setup_logging();
    setup_audio();
    const char *ssid = get_config_value("wifi_ssid");
    const char *password = get_config_value("wifi_password");
    if (!ssid || strcmp(ssid, "") == 0 || !password || strcmp(password, "") == 0) {
        appState = APP_REQUESTING_CONFIG;
        return;
    } else {
        appState = APP_RUNNING;
    }
    connect_wifi(ssid, password);
    play_audio("http://vis.media-ice.musicradio.com/CapitalMP3");
}

void loop() {
    switch (appState) {
    case APP_REQUESTING_CONFIG:
    default:
        if (config_tick()) {
            appState = APP_RUNNING;
        }
        break;
    case APP_RUNNING:
        audio_tick();
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
