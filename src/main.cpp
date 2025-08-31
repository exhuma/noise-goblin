#include "core/core.h"
#include "platforms/audio.h"
#include "platforms/logging.h"
#include "platforms/wifi.h"

void setup() {
    setup_logging();
    setup_audio();
    connect_wifi(get_wifi_ssid(), get_wifi_password());
    play_audio("http://vis.media-ice.musicradio.com/CapitalMP3");
}

void loop() {
    audio_tick();
}

int main() {
    setup();
    while (1) {
        loop();
    }
    return 0;
}
