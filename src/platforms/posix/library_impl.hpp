#include <vector>
#include "../config.hpp"
#include "../http.hpp"
#include "../library.hpp"
#include "../logging.hpp"

struct PosixLibrary : ILibrary {
    explicit PosixLibrary(ILogging& logger, IHttp& http, IConfig& config)
        : logger(logger), http(http), config(config) {
    }

    auto getRandomSound() -> std::string override;
    void tick() override;

  private:
    ILogging& logger;
    IHttp& http;
    IConfig& config;
    std::vector<std::string> soundByteNames;
};
