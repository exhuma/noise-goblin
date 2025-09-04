#include <vector>
#include "../http.hpp"
#include "../logging.hpp"

struct Esp32Http : IHttp {
    explicit Esp32Http(ILogging& logger) : logger(logger) {
    }

    auto getResourceNames(std::string url) -> std::vector<std::string> override;

  private:
    ILogging& logger;
};
