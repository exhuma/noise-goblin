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
    /// @brief The current audio URL being played.
    std::string currentUrl;
    /// @brief Mutex for synchronizing access to the current URL.
    std::mutex urlMutex;
};
