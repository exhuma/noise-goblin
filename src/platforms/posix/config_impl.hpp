#include "../config.hpp"

struct PosixConfig : IConfig {
    void get(const char *key, char *out_value, size_t out_size, ILogging &logger) override;
    void set(const char *key, const char *value, ILogging &logger) override;
    bool tick(ILogging &logger) override;
    void clear(ILogging &logger) override;
};
