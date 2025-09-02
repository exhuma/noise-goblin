#pragma once

#include "audio_impl.hpp"
#include <stdio.h>
#include <unistd.h>
#include "../logging.hpp"

void PosixAudio::setup(ILogging &logger) {
    logger.logln("PosixAudio setup called");
}

void PosixAudio::play(const char *url, ILogging &logger) {
    logger.log("PosixAudio play called with url: ");
    logger.logln(url);
}

void PosixAudio::tick(ILogging &logger) {
    logger.logln("PosixAudio tick called");
    sleep(1);
}
