#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <functional>
#include "../http.hpp"
#include "WiFi.h"

static void collectResources(WiFiClient* client,
                             std::function<void(const std::string&)> callback) {
    std::string current;
    int depth = 0;
    char c;
    char buffer[1024];

    while (client->available()) {
        size_t bytesRead = client->readBytes(buffer, sizeof(buffer) - 1);
        buffer[bytesRead] = '\0';  // Null-terminate the string
        for (size_t i = 0; i < bytesRead; i++) {
            c = buffer[i];
            if (c == '{') {
                depth++;
            }
            if (depth > 1) {
                current.push_back(c);
            }
            if (c == '}') {
                depth--;
                if (depth == 1) {
                    callback(current);
                    current.clear();
                }
            }
        }
    }
}

class Esp32Http : public IHttp {
  public:
    Esp32Http(ILogging& logger) : IHttp(logger) {
    }

    auto getResourceNames(std::string url)
        -> std::vector<std::string> override {
        std::vector<std::string> resourceNames;

        // Create HTTP client
        HTTPClient http;
        http.setUserAgent("noise-goblin/0.1");
        http.addHeader("Accept", "application/json");
        http.begin(url.c_str());
        int httpCode = http.GET();

        if (httpCode == HTTP_CODE_OK) {
            auto client = http.getStreamPtr();
            collectResources(client, [&](const std::string& jsonString) {
                try {
                    DynamicJsonDocument doc(1024);
                    DeserializationError error =
                        deserializeJson(doc, jsonString);
                    if (!error) {
                        if (doc.containsKey("resource_str")) {
                            resourceNames.push_back(
                                doc["resource_str"].as<std::string>());
                        }
                    } else {
                        logger.error("Failed to parse JSON response");
                        logger.error("Deserialization error: %s",
                                     error.c_str());
                    }
                } catch (const std::exception& e) {
                    logger.error("JSON parse error: %s", e.what());
                }
            });

            // Parse JSON

        } else {
            logger.error("HTTP GET failed, error: %s",
                         http.errorToString(httpCode).c_str());
        }

        http.end();
        return resourceNames;
    }
};
