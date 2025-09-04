#include <vector>
#include "../http.hpp"
#include "../logging.hpp"

struct PosixHttp : IHttp {
    explicit PosixHttp(ILogging& logger) : logger(logger) {
    }

    auto getResourceNames(std::string url) -> std::vector<std::string> override;

  private:
    ILogging& logger;
};
