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
        // We only simulate state-changes for 100 iterations. This is just to
        // avoid running into overflows if the application runs for an extended
        // period of time. Not really necessary but it keeps things simple for
        // testing
        if (tickCount < 100) {
            tickCount++;
        }
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
