#include "library_impl.hpp"
#include <string>

auto PosixLibrary::getRandomSound() -> std::string {
    logger.info("Getting random sound");
    int index = rand() % soundByteNames.size();
    auto url = soundByteNames[index];
    auto baseUrl =
        config.get(LIBRARY_BASE_URL_KEY);  // TODO: We could cache this
    return baseUrl + "/" + url;
}

void PosixLibrary::tick() {
    if (soundByteNames.size() == 0) {
        auto baseUrl = config.get(LIBRARY_BASE_URL_KEY);
        logger.info("Loading sounds from %s", baseUrl.c_str());
        auto response = http.getResourceNames(baseUrl);
        for (const auto& name : response) {
            logger.info("Resource found: %s", name.c_str());
        }
        soundByteNames = std::move(response);
    }
    logger.info("Library tick: %zu sounds available", soundByteNames.size());
}
