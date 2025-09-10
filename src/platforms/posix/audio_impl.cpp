#include <unistd.h>
#include <chrono>
#include <cstdio>
#include <string>
#include "../../const.hpp"
#include "../audio.hpp"
#include "../logging.hpp"

class NoiseAudio : public IAudio {
  public:
    NoiseAudio(ILogging &logger) : IAudio(logger) {
    }

    void setup() override {
        logger.info("IAudio setup called");
        logger.debug("Audio Setup complete");
    }

    void play(std::string url) override {
        logger.info("PosixAudio play called with url: %s", url.c_str());
        playStartTime = std::chrono::steady_clock::now();
    }

    auto tick() -> std::vector<int> override {
        std::vector<int> events = {};
        // simulate audio playing for 5 seconds. After time is elapsed, set
        // start-time back to unset
        if (playStartTime != kUnsetTime) {
            auto now = std::chrono::steady_clock::now();
            events.push_back(SOUND_PLAY_STARTED);
            if (now - playStartTime >= std::chrono::seconds(5)) {
                playStartTime = kUnsetTime;
                events.push_back(SOUND_PLAY_STOPPED);
            }
        }
        return events;
    }

  private:
    static constexpr std::chrono::time_point<std::chrono::steady_clock>
        kUnsetTime = std::chrono::time_point<std::chrono::steady_clock>::min();
    std::chrono::time_point<std::chrono::steady_clock> playStartTime{
        kUnsetTime};
};
