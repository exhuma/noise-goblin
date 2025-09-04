#include "../config.hpp"
#include "../logging.hpp"

struct PosixConfig : IConfig {
    explicit PosixConfig(ILogging &logger) : logger(logger) {
    }

    auto get(const char *key) -> std::string override;
    void set(const char *key, const char *value) override;
    auto tick() -> bool override;
    void clear() override;

  private:
    ILogging &logger;
};
