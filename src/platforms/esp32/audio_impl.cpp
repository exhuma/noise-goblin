#include "audio_impl.hpp"
#include "Audio.h"
#include "logging_impl.hpp"

Audio audio;

void Esp32Audio::setup(ILogging& logger) {
    audio.setPinout(17,  // I2S_BCLK
                    16,  // I2S_LRC
                    15   // I2S_DOUT
    );
    audio.setVolume(20);
}

void Esp32Audio::play(const char* url, ILogging& logger) {
    logger.logln("Connecting to audio stream");
    if (!audio.connecttohost(url)) {
        logger.logln("Failed to connect to host");
    }
}

void Esp32Audio::tick(ILogging& logger) {
    audio.loop();
}
