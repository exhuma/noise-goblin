#include "http_impl.hpp"
#include <vector>
#include "curl/curl.h"
#include "nlohmann/json.hpp"

auto PosixHttp::getResourceNames(std::string url) -> std::vector<std::string> {
    logger.info("Fetching document from URL: %s", url.c_str());
    CURL* curl = curl_easy_init();
    std::vector<std::string> resourceNames;
    if (curl) {
        std::string response;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(
            curl, CURLOPT_WRITEFUNCTION,
            +[](char* ptr, size_t size, size_t nmemb,
                void* userdata) -> size_t {
                auto& resp = *static_cast<std::string*>(userdata);
                resp.append(ptr, size * nmemb);
                return size * nmemb;
            });
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res == CURLE_OK) {
            // Parse JSON response
            try {
                // Assuming nlohmann/json is available
                auto json = nlohmann::json::parse(response);
                if (json.contains("jingles") && json["jingles"].is_array()) {
                    for (const auto& item : json["jingles"]) {
                        if (item.contains("resource_str") &&
                            item["resource_str"].is_string()) {
                            resourceNames.push_back(
                                item["resource_str"].get<std::string>());
                        }
                    }
                }
            } catch (const std::exception& e) {
                logger.error("JSON parse error: %s", e.what());
            }
        } else {
            logger.error("CURL error: %s", curl_easy_strerror(res));
        }
    }
    return resourceNames;
}
