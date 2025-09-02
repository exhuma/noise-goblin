#include "../logging.hpp"

struct Esp32Logging : ILogging {
    void log(const char *msg) override;
    void logln(const char *msg) override;
    void setup() override;
};
