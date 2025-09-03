#include "../logging.hpp"

struct PosixLogging : ILogging {
    void setup() override;

    void debug(const char *fmt, ...) override;
    void info(const char *fmt, ...) override;
    void error(const char *fmt, ...) override;
};
