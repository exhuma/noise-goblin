#include "../logging.hpp"
#include "../wifi.hpp"

struct Esp32Wifi : IWifi {
    bool is_wifi_connected(ILogging &logger) override;
    void setup(const char *ssid, const char *password, ILogging &logger) override;
    void tick(ILogging &logger) override;
};
