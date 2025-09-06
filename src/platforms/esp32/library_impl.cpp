#include <string>
#include "../library.hpp"

class Esp32Library : public ILibrary {
  public:
    Esp32Library(ILogging &logger, IHttp &http, IConfig &config)
        : ILibrary(logger, http, config) {
    }

    auto getRandomSound() -> std::string override {
        logger.info("Getting random sound");
        srand(static_cast<unsigned int>(time(nullptr)));
        int index = rand() % 100;
        auto url = soundByteNames[index];
        auto baseUrl =
            config.get(LIBRARY_BASE_URL_KEY);  // TODO: We could cache this
        return baseUrl + "/" + url;
    }


    void tick() override {
        if (soundByteNames.size() == 0) {
            auto baseUrl = config.get(LIBRARY_BASE_URL_KEY);
            if (baseUrl.empty()) {
                logger.debug("No library base URL configured");
                return;
            }
            logger.info("Loading sounds from %s", baseUrl.c_str());
            auto response = http.getResourceNames(baseUrl);
            logger.info("Loaded %d resources", response.size());
            soundByteNames = std::move(response);
        }
    }
};
