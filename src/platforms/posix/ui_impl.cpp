#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include "../../enum.hpp"
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

static auto rpad(const std::string &str, std::size_t width) -> std::string {
    if (str.size() < width) {
        return str + std::string(width - str.size(), ' ');
    }
    return str;
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
        // Braille spinner characters encoded as UTF-8
        static const std::array<const char *, 10> spinner = {
            u8"\u280B", u8"\u2819", u8"\u2839", u8"\u2838", u8"\u283C",
            u8"\u2834", u8"\u2826", u8"\u2827", u8"\u2807", u8"\u280F"};
        static const std::size_t spinnerSize =
            sizeof(spinner) / sizeof(spinner[0]);
        static std::size_t spinnerIndex = 0;
        std::cout << "\r" << spinner[spinnerIndex % spinnerSize]
                  << " App State: " << rpad(appStateToString(appState), 50)
                  << std::flush;

        spinnerIndex = (spinnerIndex + 1) % spinner.size();

        // Handle key-presses
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

    void setState(AppState state) override {
        appState = state;
    }
};
