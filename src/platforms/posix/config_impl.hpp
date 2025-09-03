#include "../config.hpp"
#include "../logging.hpp"

struct PosixConfig : IConfig {
    explicit PosixConfig(ILogging &logger) : logger(logger) {
    }

    std::string get(const char *key) override;
    void set(const char *key, const char *value) override;
    bool tick() override;
    void clear() override;

  private:
    ILogging &logger;
};
