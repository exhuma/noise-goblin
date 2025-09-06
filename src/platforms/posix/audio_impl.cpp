#include <unistd.h>
#include <chrono>
#include <cstdio>
#include <string>
#include "../audio.hpp"
#include "../logging.hpp"

class PosixAudio : public IAudio {
  public:
    PosixAudio(ILogging &logger, IEventLoop &eventLoop)
        : IAudio(logger, eventLoop) {
    }

    void setup() override {
        logger.info("IAudio setup called");
        logger.debug("Audio Setup complete");
    }

    void play(std::string url) override {
        logger.info("PosixAudio play called with url: %s", url.c_str());
        playStartTime = std::chrono::steady_clock::now();
    }

    void tick() override {
        // simulate audio playing for 5 seconds. After time is elapsed, set
        // start-time back to unset
        if (playStartTime != kUnsetTime) {
            auto now = std::chrono::steady_clock::now();
            eventLoop.postEvent(SOUND_PLAY_STARTED);
            if (now - playStartTime >= std::chrono::seconds(5)) {
                playStartTime = kUnsetTime;
                eventLoop.postEvent(SOUND_PLAY_STOPPED);
            }
        }
    }

  private:
    static constexpr std::chrono::time_point<std::chrono::steady_clock>
        kUnsetTime = std::chrono::time_point<std::chrono::steady_clock>::min();
    std::chrono::time_point<std::chrono::steady_clock> playStartTime{
        kUnsetTime};
};
