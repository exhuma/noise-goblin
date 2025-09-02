#pragma once

#include "platforms/config.hpp"

class ConfigLoop {
    IConfig &config_;

  public:
    explicit ConfigLoop(IConfig &config) : config_(config) {
    }
    void get(const char *key, char *out_value, size_t out_size, ILogging &logger) {
        config_.get(key, out_value, out_size, logger);
    }
    void set(const char *key, const char *value, ILogging &logger) {
        config_.set(key, value, logger);
    }
    void tick(ILogging &logger) {
        config_.tick(logger);
    }
    void clear(ILogging &logger) {
        config_.clear(logger);
    }
};
