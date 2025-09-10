#include "../../const.hpp"
#include "../wifi.hpp"
#include "WiFi.h"

class NoiseWifi : public IWifi {
  public:
    NoiseWifi(ILogging &logger) : IWifi(logger) {
    }

    void setup() override {
        logger.debug("Resetting WiFi");
        WiFi.disconnect();
        WiFi.mode(WIFI_STA);
    }

    auto connect(const char *ssid, const char *password)
        -> std::vector<int> override {
        logger.info("Connecting to WiFi with SSID %s %s", ssid);
        newConnectionRequested = true;
        remainingRetries = 20;
        return {EVENT_WIFI_CONNECTING};
    }

    auto tick() -> std::vector<int> override {
        if (!newConnectionRequested) {
            return {};
        }

        const unsigned long retryDelay = 1000;
        const unsigned long currentTime = millis();
        static unsigned long lastAttemptTime = 0;

        if (currentTime - lastAttemptTime > retryDelay) {
            lastAttemptTime = currentTime;
            if (isConnected()) {
                logger.info("WiFi connected with IP: %s",
                            WiFi.localIP().toString().c_str());
                newConnectionRequested = false;
                return {EVENT_WIFI_CONNECTED};
            }
            if (newConnectionRequested) {
                logger.info("Retrying connection...");
                WiFi.reconnect();
                remainingRetries--;
            } else if (remainingRetries == 0) {
                logger.info("WiFi connection failed");
                return {EVENT_WIFI_FAILED};
            }
        }
        return {};
    }

    auto isConnected() -> bool override {
        return WiFi.status() == WL_CONNECTED;
    }

  private:
    bool newConnectionRequested = false;
    int remainingRetries = 0;
};
