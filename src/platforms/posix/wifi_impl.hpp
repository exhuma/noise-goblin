#include "../logging.hpp"
#include "../wifi.hpp"

struct PosixWifi : IWifi {
    explicit PosixWifi(ILogging& logger) : logger(logger), tickCount(0) {
    }

    void setup() override;
    void connect(const char* ssid, const char* password) override;
    void tick() override;
    bool isConnected() override;

  private:
    ILogging& logger;
    /// @brief How many times we have looped
    /// @details In this POSIX implementation, we use this to simulate spotty
    /// WiFi connectivity. The POSIX platform is only here for
    /// testing/simulating.
    int tickCount = 0;
};
