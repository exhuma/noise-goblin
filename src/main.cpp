#include <cstring>
#include "config.h"
#include "core/core.h"
#include "platforms/audio.h"
#include "platforms/config.h"
#include "platforms/logging.h"
#include "platforms/wifi.h"

void setup() {
    setup_logging();
    setup_audio();
    const char *ssid = get_config_value("wifi_ssid");
    const char *password = get_config_value("wifi_password");
    if (!ssid || strcmp(ssid, "") == 0) {
        ssid = WIFI_SSID;
        set_config_value("wifi_ssid", ssid);
    }
    if (!password || strcmp(password, "") == 0) {
        password = WIFI_PASSWORD;
        set_config_value("wifi_password", password);
    }
    connect_wifi(ssid, password);
    play_audio("http://vis.media-ice.musicradio.com/CapitalMP3");
}

void loop() {
    audio_tick();
}

// Entry-point for POSIX platforms (ignored by ESP32)
int main() {
    setup();
    while (1) {
        loop();
    }
    return 0;
}
