#include <mutex>
#include <string>
#include "../audio.hpp"
#include "Audio.h"

static Audio esp32_audio;

class Esp32Audio : public IAudio {
  public:
    Esp32Audio(ILogging &logger) : IAudio(logger) {
    }
    void setup() override {
        esp32_audio.setPinout(17,  // I2S_BCLK
                              16,  // I2S_LRC
                              15   // I2S_DOUT
        );
        esp32_audio.setVolume(20);
        logger.debug("Audio Setup complete");
    }

    void play(std::string url) override {
        logger.debug("Playing %s", url.c_str());
        {
            std::lock_guard<std::mutex> lock(urlMutex);
            currentUrl = url;
        }
    }

    void tick() override {
        if (!currentUrl.empty()) {
            esp32_audio.stopSong();
            const bool success = esp32_audio.connecttohost(currentUrl.c_str());
            {
                std::lock_guard<std::mutex> lock(urlMutex);
                currentUrl.clear();
            }
            if (!success) {
                logger.error("Failed to connect to host");
            }
        }
        esp32_audio.loop();
    }

  private:
    std::mutex urlMutex;
    std::string currentUrl;
};
