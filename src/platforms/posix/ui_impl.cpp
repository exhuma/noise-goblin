#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include "../logging.hpp"
#include "../ui.hpp"

static void setNonCanonicalMode() {
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~(ICANON | ECHO);  // Disable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

static auto isKeyPressed() -> bool {
    struct timeval tv = {0, 0};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, nullptr, nullptr, &tv) > 0;
}

static void restoreTerminalMode() {
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag |= (ICANON | ECHO);  // Re-enable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

class PosixUserInterface : public IUserInterface {
  public:
    PosixUserInterface(ILogging &logger, IEventLoop &eventLoop)
        : IUserInterface(logger, eventLoop) {
    }

    void setup() override {
        logger.info("UserInterface setup complete");
        logger.debug("UI Setup complete");
    }

    void tick() override {
        switch (ledState) {
        default:
            logger.info("LED State: Unknown");
            break;
        case LedState::Normal:
            logger.info("LED State: Normal");
            break;
        case LedState::CaptivePortal:
            logger.info("LED State: Captive Portal");
            break;
        case LedState::Reset:
            logger.info("LED State: Reset");
            break;
        case LedState::Connecting:
            logger.info("LED State: Connecting");
            break;
        case LedState::Off:
            logger.info("LED State: Off");
            break;
        }
        setNonCanonicalMode();
        if (isKeyPressed()) {
            char c;
            read(STDIN_FILENO, &c, 1);
            if (c == 'r') {
                eventLoop.postEvent(EVENT_RESET_BUTTON_PRESSED);
            } else if (c == 'p') {
                eventLoop.postEvent(EVENT_PLAY_BUTTON_PRESSED);
            }
        }
        restoreTerminalMode();
    }

    void setState(LedState state) override {
        ledState = state;
    }
};
