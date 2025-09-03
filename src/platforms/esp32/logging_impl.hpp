#include "../logging.hpp"

struct Esp32Logging : ILogging {
    void setup() override;

    void debug(const char *fmt, ...) override;
    void info(const char *fmt, ...) override;
    void error(const char *fmt, ...) override;
};
