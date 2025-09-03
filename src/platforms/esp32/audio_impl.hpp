#pragma once

#include <string>
#include "../audio.hpp"
#include "../logging.hpp"

#include <mutex>

struct Esp32Audio : IAudio {
    explicit Esp32Audio(ILogging& logger) : logger(logger), currentUrl("") {
    }
    void setup() override;
    void play(std::string url) override;
    void tick() override;

  private:
    ILogging& logger;
    std::string currentUrl;
    std::mutex urlMutex;
};
