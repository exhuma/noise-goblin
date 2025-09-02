#pragma once

#include "../logging.hpp"

struct IWifi {
    virtual ~IWifi() = default;
    virtual bool is_wifi_connected(ILogging &logger) = 0;
    virtual void setup(const char *ssid, const char *password, ILogging &logger) = 0;
    virtual void tick(ILogging &logger) = 0;
};
