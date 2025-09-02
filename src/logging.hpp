#pragma once

#include "platforms/logging.hpp"

class Logging {
    ILogging& logging_;

  public:
    explicit Logging(ILogging& logging) : logging_(logging) {
    }
    void setup() {
        logging_.setup();
    }
    void log(const char* msg) {
        logging_.log(msg);
    }
    void logln(const char* msg) {
        logging_.logln(msg);
    }
};
