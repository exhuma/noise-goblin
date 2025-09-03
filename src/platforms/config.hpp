#pragma once

#include <cstddef>
#include <string>

inline const char *WIFI_SSID_KEY = "wifi_ssid";
inline const char *WIFI_PASSWORD_KEY = "wifi_password";

struct IConfig {
    virtual ~IConfig() = default;
    virtual std::string get(const char *key) = 0;
    virtual void set(const char *key, const char *value) = 0;
    virtual bool tick() = 0;
    virtual void clear() = 0;
};
