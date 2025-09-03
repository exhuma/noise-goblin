#pragma once

struct IWifi {
    virtual ~IWifi() = default;
    virtual bool is_wifi_connected() = 0;
    virtual void setup(const char *ssid, const char *password) = 0;
    virtual void tick() = 0;
};
