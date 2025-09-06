#include <stdio.h>
#include <unistd.h>
#include <string>
#include "../audio.hpp"
#include "../logging.hpp"

class PosixAudio : public IAudio {
  public:
    PosixAudio(ILogging &logger) : IAudio(logger) {
    }

    void setup() override {
        logger.info("IAudio setup called");
        logger.debug("Audio Setup complete");
    }

    void play(std::string url) override {
        logger.info("PosixAudio play called with url: %s", url.c_str());
        sleep(5);
    }

    void tick() override {
        logger.debug("PosixAudio tick called");
    }
};
