#include "../wifi.hpp"
#include "WiFi.h"

class Esp32Wifi : public IWifi {
  public:
    Esp32Wifi(ILogging &logger) : IWifi(logger) {
    }

    void setup() override {
        logger.debug("Resetting WiFi");
        WiFi.disconnect();
        WiFi.mode(WIFI_STA);
    }

    void connect(const char *ssid, const char *password) override {
        if (isConnecting || isConnected()) {
            logger.info("Already connected or connecting to WiFi");
            return;
        }
        logger.info("Connecting to WiFi: ", ssid);
        WiFi.begin(ssid, password);
        isConnecting = true;
        remainingRetries = 10;
    }

    void tick() override {
        if (isConnecting && WiFi.status() != WL_CONNECTED) {
            if (remainingRetries > 0) {
                logger.info("Wifi Connecting...");
                delay(1000);  // TODO replace with nonblocking RTC wait
                remainingRetries--;
            } else if (remainingRetries == 0) {
                logger.error("Wifi Connection Failed");
                isConnecting = false;
            }
            logger.info("Connected with IP: %s",
                        WiFi.localIP().toString().c_str());
            return;
        }
    }

    auto isConnected() -> bool override {
        return WiFi.status() == WL_CONNECTED;
    }

  private:
    bool isConnecting = false;
    int remainingRetries = 0;
};
