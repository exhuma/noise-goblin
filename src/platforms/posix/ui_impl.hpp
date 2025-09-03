#include "../audio.hpp"
#include "../config.hpp"
#include "../eventLoop.hpp"
#include "../logging.hpp"
#include "../ui.hpp"

struct PosixUi : IUserInterface {
    explicit PosixUi(IAudio& audio, IConfig& config, ILogging& logger,
                     IEventLoop& eventLoop)
        : logger(logger),
          audio(audio),
          config(config),
          eventLoop(eventLoop),
          isNormal(true) {
    }
    void setup() override;
    void tick() override;
    void displayConfigCleared() override;
    void displayCaptivePortal() override;
    void displayNormal() override;

  private:
    ILogging& logger;
    IAudio& audio;
    IConfig& config;
    IEventLoop& eventLoop;
    bool isNormal;
};
