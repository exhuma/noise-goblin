#include "../logging.hpp"
#include "../wifi.hpp"

class PosixWifi : public IWifi {
  public:
    PosixWifi(ILogging &logger) : IWifi{logger} {
    }
    void setup() override {
        logger.info("Resetting WiFi");
    }

    void connect(const char *ssid, const char *password) override {
        logger.info("Connecting to WiFi with SSID %s using password %s", ssid,
                    password);
    }

    void tick() override {
        logger.info("IWifi tick called %d times", tickCount);
        tickCount++;
    }

    auto isConnected() -> bool override {
        // Simulate a connection that interrupts for a few seconds.
        if (tickCount <= 5) {
            return false;
        }
        if (tickCount <= 10) {
            return true;
        }
        if (tickCount <= 15) {
            return false;
        }
        return true;
    }

  private:
    int tickCount = 0;
};
