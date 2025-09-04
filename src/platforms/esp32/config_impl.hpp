#include "../config.hpp"
#include "../logging.hpp"

struct Esp32Config : IConfig {
    explicit Esp32Config(ILogging &logger) : logger(logger) {
    }

    auto get(const char *key) -> std::string override;
    void set(const char *key, const char *value) override;
    auto tick() -> bool override;
    void clear() override;

  private:
    ILogging &logger;
};
