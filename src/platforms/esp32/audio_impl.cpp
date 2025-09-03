#include "audio_impl.hpp"
#include "Audio.h"

Audio esp32_audio;

void Esp32Audio::setup() {
    esp32_audio.setPinout(17,  // I2S_BCLK
                          16,  // I2S_LRC
                          15   // I2S_DOUT
    );
    esp32_audio.setVolume(20);
}

void Esp32Audio::play(const char* url) {
    logger.logln("Connecting to audio stream");
    if (!esp32_audio.connecttohost(url)) {
        logger.logln("Failed to connect to host");
    }
}

void Esp32Audio::tick() {
    esp32_audio.loop();
}
