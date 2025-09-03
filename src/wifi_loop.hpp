#pragma once

#include "platforms/wifi.hpp"

class WifiLoop {
    IWifi& wifi_;

  public:
    explicit WifiLoop(IWifi& wifi) : wifi_(wifi) {
    }
    void setup(char* ssid, char* password) {
        wifi_.setup(ssid, password);
    }
    void tick() {
        wifi_.tick();
    }
};
