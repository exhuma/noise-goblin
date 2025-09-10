#include <iostream>
#include <string>
#include "../library.hpp"

class PosixLibrary : public ILibrary {
  public:
    PosixLibrary(ILogging &logger, IHttp &http, IConfig &config)
        : ILibrary(logger, http, config) {
    }

    auto getRandomSound() -> std::string override {
        if (soundByteNames.size() <= 0) {
            return "";
        }
        int index = rand() % soundByteNames.size();
        auto url = soundByteNames[index];
        auto baseUrl =
            config.get(LIBRARY_BASE_URL_KEY);  // TODO: We could cache this
        return baseUrl + "/" + url;
    }

    void tick() override {
        if (soundByteNames.size() == 0) {
            auto baseUrl = config.get(LIBRARY_BASE_URL_KEY);
            logger.info("Loading sounds from %s", baseUrl.c_str());
            auto response = http.getResourceNames(baseUrl);
            for (const auto &name : response) {
                logger.info("Resource found: %s", name.c_str());
            }
            soundByteNames = std::move(response);
        }
    }
};
