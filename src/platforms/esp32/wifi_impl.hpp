#include "../logging.hpp"
#include "../wifi.hpp"

struct Esp32Wifi : IWifi {
    explicit Esp32Wifi(ILogging& logger) : logger(logger) {
    }
    void setup() override;
    void connect(const char* ssid, const char* password) override;
    void tick() override;
    bool isConnected() override;

  private:
    ILogging& logger;
    bool isConnecting = false;
    int remainingRetries = 10;
};
