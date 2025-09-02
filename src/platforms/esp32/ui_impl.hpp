#include "../audio.hpp"
#include "../config.hpp"
#include "../logging.hpp"
#include "../ui.hpp"

struct Esp32Ui : IUserInterface {
    void setup(ILogging& logger) override;
    void tick(IAudio& audio, IConfig& config, ILogging& logger) override;
};
