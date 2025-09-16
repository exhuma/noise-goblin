#include <string>
#include "../audio.hpp"
#include "Audio.h"

static Audio esp32_audio;

class NoiseAudio : public IAudio {
  public:
    NoiseAudio(ILogging &logger) : IAudio(logger) {
    }

    void setup() override {
        esp32_audio.setPinout(8,  // I2S_BCLK
                              7,  // I2S_LRC
                              9   // I2S_DOUT
        );
        esp32_audio.setVolume(15);
        logger.debug("Audio Setup complete");
    }

    void play(std::string url) override {
        if (url.empty()) {
            logger.error("No URL provided to play. Ignoring.");
            return;
        }
        logger.debug("Playing %s", url.c_str());
        currentUrl = url;
    }

    auto tick() -> std::vector<int> override {
        if (!currentUrl.empty()) {
            esp32_audio.stopSong();
            const bool success = esp32_audio.connecttohost(currentUrl.c_str());
            currentUrl.clear();
            if (!success) {
                logger.error("Failed to connect to host");
            }
        }
        esp32_audio.loop();
        return {};
    }

  private:
    std::string currentUrl;
};
