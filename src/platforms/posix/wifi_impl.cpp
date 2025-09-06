#include <chrono>
#include "../logging.hpp"
#include "../wifi.hpp"

class PosixWifi : public IWifi {
  public:
    PosixWifi(ILogging &logger, IEventLoop &eventLoop)
        : IWifi{logger, eventLoop} {
        startupTime = std::chrono::system_clock::now();
    }

    void setup() override {
        logger.info("Resetting WiFi");
    }

    void connect(const char *ssid, const char *password) override {
        logger.info("Connecting to WiFi with SSID %s using password %s", ssid,
                    password);
        newConnectionRequested = true;
        eventLoop.postEvent(EVENT_WIFI_CONNECTING);
    }

    void tick() override {
        if (newConnectionRequested && isConnected()) {
            eventLoop.postEvent(EVENT_WIFI_CONNECTED);
            newConnectionRequested = false;
        }
    }

    auto isConnected() -> bool override {
        // We are connected if the application is running for 5 seconds
        return (std::chrono::system_clock::now() - startupTime) >
               std::chrono::seconds(5);
    }

  private:
    std::chrono::system_clock::time_point startupTime;
    bool newConnectionRequested = false;
};
