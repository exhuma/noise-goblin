/**
 * Main entry point.
 *
 * This file deals with dependency injection and wiring all components together.
 *
 * Core logic is implemented in `app.cpp`
 */
#include <cstdio>
#include <cstring>
#include <string>
#include "app.hpp"

// Import and instantiate modules for the currently selected build target
#if defined(BUILD_ESP32)
    #include "platforms/esp32/audio_impl.hpp"
    #include "platforms/esp32/config_impl.hpp"
    #include "platforms/esp32/event_loop_impl.hpp"
    #include "platforms/esp32/library_impl.hpp"
    #include "platforms/esp32/logging_impl.hpp"
    #include "platforms/esp32/ui_impl.hpp"
    #include "platforms/esp32/wifi_impl.hpp"
Esp32EventLoop eventLoop;
Esp32Logging logging;
Esp32Wifi wifi(logging);
Esp32Audio audio(logging);
Esp32Config config(logging);
Esp32Ui ui(audio, config, logging, eventLoop);
Esp32Library library(logging);
#else
    #include "platforms/posix/audio_impl.hpp"
    #include "platforms/posix/config_impl.hpp"
    #include "platforms/posix/event_loop_impl.hpp"
    #include "platforms/posix/library_impl.hpp"
    #include "platforms/posix/logging_impl.hpp"
    #include "platforms/posix/ui_impl.hpp"
    #include "platforms/posix/wifi_impl.hpp"
PosixEventLoop eventLoop;
PosixLogging logging;
PosixWifi wifi(logging);
PosixAudio audio(logging);
PosixConfig config(logging);
PosixUi ui(audio, config, logging, eventLoop);
PosixLibrary library(logging);
#endif

Application app(config, wifi, logging, audio, ui, eventLoop, library);

// The Arduino entry-point for "run-once" initialisations.
void setup() {
    app.setup();
}

// The Arduino entry-point for the core application loop
void loop() {
    app.loop();
}

// Entry-point for POSIX platforms (ignored by ESP32)
auto main() -> int {
    setup();
    while (true) {
        loop();
    }
    return 0;
}
