#include "audio_impl.hpp"
#include <stdio.h>
#include <unistd.h>
#include <string>

void PosixAudio::setup() {
    logger.info("PosixAudio setup called");
    logger.debug("Audio Setup complete");
}

void PosixAudio::play(std::string url) {
    logger.info("PosixAudio play called with url: %s", url.c_str());
}

void PosixAudio::tick() {
    logger.info("PosixAudio tick called");
    sleep(1);
}
