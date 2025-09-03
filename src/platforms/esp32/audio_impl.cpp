#include "audio_impl.hpp"
#include <mutex>
#include <string>
#include "Audio.h"

Audio esp32_audio;

void Esp32Audio::setup() {
    esp32_audio.setPinout(17,  // I2S_BCLK
                          16,  // I2S_LRC
                          15   // I2S_DOUT
    );
    esp32_audio.setVolume(20);
    logger.debug("Audio Setup complete");
}

void Esp32Audio::play(std::string url) {
    esp32_audio.stopSong();
    logger.debug("Playing %s", url.c_str());
    {
        currentUrl = url;
        logger.debug("Set current URL to: %s -> %s", url.c_str(),
                     currentUrl.c_str());
    }
}

void Esp32Audio::tick() {
    if (!currentUrl.empty()) {
        const bool success = esp32_audio.connecttohost(currentUrl.c_str());
        currentUrl.clear();
        if (!success) {
            logger.error("Failed to connect to host");
        }
    }
    esp32_audio.loop();
}
