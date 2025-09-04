#include "ui_impl.hpp"
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cstdlib>
#include <string>

void setNonCanonicalMode() {
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~(ICANON | ECHO);  // Disable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

auto isKeyPressed() -> bool {
    struct timeval tv = {0, 0};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, nullptr, nullptr, &tv) > 0;
}

void restoreTerminalMode() {
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag |= (ICANON | ECHO);  // Re-enable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

void PosixUi::setup() {
    logger.info("Posix UI setup complete");
    logger.debug("UI Setup complete");
}

void PosixUi::tick() {
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

void PosixUi::setState(LedState state) {
    ledState = state;
}
