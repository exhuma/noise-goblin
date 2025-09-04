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
    /// @brief Flag indicating whether we are currently trying to connect to
    /// WiFi.
    bool isConnecting = false;
    /// @brief Number of remaining connection retries.
    int remainingRetries = 10;
};
