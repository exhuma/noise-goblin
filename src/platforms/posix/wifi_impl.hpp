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
    int tickCount = 0;
};
