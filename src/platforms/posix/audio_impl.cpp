#include "audio_impl.hpp"
#include <stdio.h>
#include <unistd.h>

void PosixAudio::setup() {
    logger.info("PosixAudio setup called");
}

void PosixAudio::play(const char *url) {
    logger.info("PosixAudio play called with url: ", url);
}

void PosixAudio::tick() {
    logger.info("PosixAudio tick called");
    sleep(1);
}
