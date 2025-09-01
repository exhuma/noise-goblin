#include "Audio.h"
#include "../audio.h"
#include "../logging.h"

Audio audio;

void setup_audio() {
    audio.setPinout(17,  // I2S_BCLK
                    16,  // I2S_LRC
                    15   // I2S_DOUT
    );
    audio.setVolume(20);
}

void play_audio(const char *url) {
    logln("Connecting to audio stream");
    if (!audio.connecttohost(url)) {
        logln("Failed to connect to host");
    }
}

void audio_tick() {
    audio.loop();
}
