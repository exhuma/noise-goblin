#include "../audio.hpp"
#include "../config.hpp"
#include "../logging.hpp"
#include "../ui.hpp"

struct PosixUi : IUserInterface {
    explicit PosixUi(IAudio& audio, IConfig& config, ILogging& logger)
        : logger(logger), audio(audio), config(config) {
    }
    void setup() override;
    void tick() override;

  private:
    ILogging& logger;
    IAudio& audio;
    IConfig& config;
};
