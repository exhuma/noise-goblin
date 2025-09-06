#include <iostream>
#include <istream>
#include <string>
#include <vector>

#include "../../util.hpp"
#include "../http.hpp"
#include "curl/curl.h"
#include "nlohmann/json.hpp"

class PosixHttp : public IHttp {
  public:
    PosixHttp(ILogging &logger) : IHttp(logger) {
    }
    auto getResourceNames(std::string url)
        -> std::vector<std::string> override {
        logger.info("Fetching document from URL: %s", url.c_str());
        CURL *curl = curl_easy_init();
        std::vector<std::string> resourceNames;
        if (curl) {
            std::string response;
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(
                curl, CURLOPT_WRITEFUNCTION,
                +[](char *ptr, size_t size, size_t nmemb,
                    void *userdata) -> size_t {
                    auto &resp = *static_cast<std::string *>(userdata);
                    resp.append(ptr, size * nmemb);
                    return size * nmemb;
                });
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

            CURLcode res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);

            if (res == CURLE_OK) {
                std::istringstream iss(response);
                try {
                    collectResources(iss, [&resourceNames](
                                              const std::string &jsonString) {
                        try {
                            auto jsonObj = nlohmann::json::parse(jsonString);
                            if (jsonObj.contains("resource_str") &&
                                jsonObj["resource_str"].is_string()) {
                                resourceNames.push_back(
                                    jsonObj["resource_str"].get<std::string>());
                            }
                        } catch (const std::exception &e) {
                            std::cerr << "JSON parse error: " << e.what()
                                      << "\n";
                        }
                    });
                } catch (const std::exception &e) {
                    logger.error("JSON parse error: %s", e.what());
                }
            } else {
                logger.error("CURL error: %s", curl_easy_strerror(res));
            }
        }
        return resourceNames;
    }
};
