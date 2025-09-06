#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "../http.hpp"

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
        http.begin(url.c_str());
        int httpCode = http.GET();

        if (httpCode == HTTP_CODE_OK) {
            String payload = http.getString();

            // Parse JSON
            DynamicJsonDocument doc(200 * 1024);
            DeserializationError error = deserializeJson(doc, payload);

            if (!error) {
                if (doc.containsKey("jingles") &&
                    doc["jingles"].is<JsonArray>()) {
                    for (const auto& item : doc["jingles"].as<JsonArray>()) {
                        if (item.containsKey("resource_str")) {
                            resourceNames.push_back(
                                item["resource_str"].as<std::string>());
                        }
                    }
                }
            }
        }

        http.end();
        return resourceNames;
    }
};
