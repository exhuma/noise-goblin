#include "../logging.hpp"
#include "../wifi.hpp"

struct Esp32Wifi : IWifi {
    explicit Esp32Wifi(ILogging& logger) : logger(logger) {
    }
    bool is_wifi_connected() override;
    void setup(const char* ssid, const char* password) override;
    void tick() override;

  private:
    ILogging& logger;
};
