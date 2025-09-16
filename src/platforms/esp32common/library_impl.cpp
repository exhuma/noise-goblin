#include <string>
#include "../library.hpp"

class NoiseLibrary : public ILibrary {
  public:
    NoiseLibrary(ILogging &logger, IHttp &http, IConfig &config)
        : ILibrary(logger, http, config) {
    }

    auto getRandomSound() -> std::string override {
        std::string serverUrl = config.get(LIBRARY_BASE_URL_KEY);
        if (serverUrl.empty()) {
            logger.error("Server URL is not configured");
            return "";
        }
        if (serverUrl.back() == '/') {
            serverUrl.pop_back();  // Remove trailing slash if present
        }
        std::string_view urlPath = "/randomjingle";
        std::string fullUrl = serverUrl + std::string(urlPath);
        return fullUrl;
    }

    void tick() override {
    }
};
