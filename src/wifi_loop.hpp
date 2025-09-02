#pragma once

#include "platforms/wifi.hpp"

class WifiLoop {
    IWifi& wifi_;

  public:
    explicit WifiLoop(IWifi& wifi) : wifi_(wifi) {
    }
    void setup(char* ssid, char* password, ILogging& logger) {
        wifi_.setup(ssid, password, logger);
    }
    void tick(ILogging& logger) {
        wifi_.tick(logger);
    }
};
