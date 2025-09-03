#pragma once

#include "platforms/config.hpp"
#include "platforms/logging.hpp"

class ConfigLoop {
    IConfig &config_;

  public:
    explicit ConfigLoop(IConfig &config) : config_(config) {
    }
    void get(const char *key, char *out_value, size_t out_size) {
        config_.get(key, out_value, out_size);
    }
    void set(const char *key, const char *value) {
        config_.set(key, value);
    }
    void tick() {
        config_.tick();
    }
    void clear() {
        config_.clear();
    }
};
