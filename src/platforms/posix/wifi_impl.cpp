#include <chrono>
#include "../../const.hpp"
#include "../logging.hpp"
#include "../wifi.hpp"

class NoiseWifi : public IWifi {
  public:
    NoiseWifi(ILogging &logger) : IWifi{logger} {
        startupTime = std::chrono::system_clock::now();
    }

    void setup() override {
        logger.info("Resetting WiFi");
    }

    auto connect(const char *ssid, const char *password)
        -> std::vector<int> override {
        logger.info("Connecting to WiFi with SSID %s using password %s", ssid,
                    password);
        newConnectionRequested = true;
        return {EVENT_WIFI_CONNECTING};
    }

    auto tick() -> std::vector<int> override {
        if (newConnectionRequested && isConnected()) {
            newConnectionRequested = false;
            return {EVENT_WIFI_CONNECTED};
        }
        return {};
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
