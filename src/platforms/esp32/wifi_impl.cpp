#include "../eventLoop.hpp"
#include "../wifi.hpp"
#include "WiFi.h"

class Esp32Wifi : public IWifi {
  public:
    Esp32Wifi(ILogging &logger, IEventLoop &eventLoop)
        : IWifi(logger, eventLoop) {
    }

    void setup() override {
        logger.debug("Resetting WiFi");
        WiFi.disconnect();
        WiFi.mode(WIFI_STA);
    }

    void connect(const char *ssid, const char *password) override {
        logger.info("Connecting to WiFi with SSID %s %s", ssid);
        newConnectionRequested = true;
        remainingRetries = 20;
        eventLoop.postEvent(EVENT_WIFI_CONNECTING);
    }

    void tick() override {
        if (!newConnectionRequested) {
            return;
        }

        const unsigned long retryDelay = 1000;
        const unsigned long currentTime = millis();
        static unsigned long lastAttemptTime = 0;

        if (currentTime - lastAttemptTime > retryDelay) {
            lastAttemptTime = currentTime;
            if (isConnected()) {
                logger.info("WiFi connected with IP: %s",
                            WiFi.localIP().toString().c_str());
                eventLoop.postEvent(EVENT_WIFI_CONNECTED);
                newConnectionRequested = false;
            } else if (newConnectionRequested) {
                logger.info("Retrying connection...");
                WiFi.reconnect();
                remainingRetries--;
            } else if (remainingRetries == 0) {
                logger.info("WiFi connection failed");
                eventLoop.postEvent(EVENT_WIFI_FAILED);
            }
        }
    }

    auto isConnected() -> bool override {
        return WiFi.status() == WL_CONNECTED;
    }

  private:
    bool newConnectionRequested = false;
    int remainingRetries = 0;
};
