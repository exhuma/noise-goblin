#include "../config.hpp"
#include "../logging.hpp"

struct Esp32Config : IConfig {
    explicit Esp32Config(ILogging &logger) : logger(logger) {
    }

    void get(const char *key, char *out_value, size_t out_size) override;
    void set(const char *key, const char *value) override;
    bool tick() override;
    void clear() override;

  private:
    ILogging &logger;
};
