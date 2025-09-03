#pragma once

#include <cstddef>

struct IConfig {
    virtual ~IConfig() = default;
    virtual void get(const char *key, char *out_value, size_t out_size) = 0;
    virtual void set(const char *key, const char *value) = 0;
    virtual bool tick() = 0;
    virtual void clear() = 0;
};
