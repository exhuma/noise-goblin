#include <cstddef>

void get_config_value(const char *key, char *out_value, size_t out_size);
const void *set_config_value(const char *key, const char *value);
bool config_tick();
