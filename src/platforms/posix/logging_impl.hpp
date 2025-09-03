#include "../logging.hpp"

struct PosixLogging : ILogging {
    void log(const char *msg) override;
    void logln(const char *msg) override;
    void setup() override;
};
