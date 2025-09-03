#pragma once

struct IWifi {
    virtual ~IWifi() = default;
    virtual void setup() = 0;
    virtual void connect(const char *ssid, const char *password) = 0;
    virtual void tick() = 0;
    virtual bool isConnected() = 0;
};
