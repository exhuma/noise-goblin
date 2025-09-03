#pragma once

#include <cstddef>
#include <string>

struct IConfig {
    virtual ~IConfig() = default;
    virtual std::string get(const char *key) = 0;
    virtual void set(const char *key, const char *value) = 0;
    virtual bool tick() = 0;
    virtual void clear() = 0;
};
