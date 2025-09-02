#include "../audio.hpp"
#include "../config.hpp"
#include "../logging.hpp"
#include "../ui.hpp"

struct PosixUi : IUserInterface {
    void setup(ILogging& logger) override;
    void tick(IAudio& audio, IConfig& config, ILogging& logger) override;
};
