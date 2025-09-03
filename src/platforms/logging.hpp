#pragma once

#include <cstdarg>
#include <string>

struct ILogging {
    virtual ~ILogging() = default;

    virtual void setup() = 0;

    virtual void debug(const char *fmt, ...) = 0;
    virtual void info(const char *fmt, ...) = 0;
    virtual void error(const char *fmt, ...) = 0;
};
