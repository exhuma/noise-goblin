#pragma once

#include <cstddef>
#include <logging.hpp>

struct IConfig {
    virtual ~IConfig() = default;
    virtual void get(const char *key, char *out_value, size_t out_size, ILogging &logger) = 0;
    virtual void set(const char *key, const char *value, ILogging &logger) = 0;
    virtual bool tick(ILogging &logger) = 0;
    virtual void clear(ILogging &logger) = 0;
};
