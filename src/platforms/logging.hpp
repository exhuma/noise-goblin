#pragma once

struct ILogging {
    virtual ~ILogging() = default;
    virtual void log(const char *msg) = 0;
    virtual void logln(const char *msg) = 0;
    virtual void setup() = 0;
};
