/**
 * Main entry point.
 *
 * This file deals with dependency injection and wiring all components together.
 *
 * Core logic is implemented in `app.cpp`
 */
#include <cstdio>
#include <cstring>
#include <map>
#include <string>
#include "app.hpp"

// Import and instantiate modules for the currently selected build target
#if defined(BUILD_ESP32)
    #include "platforms/esp32/audio_impl.cpp"
    #include "platforms/esp32/config_impl.cpp"
    #include "platforms/esp32/config_ui.cpp"
    #include "platforms/esp32/event_loop_impl.cpp"
    #include "platforms/esp32/http_impl.cpp"
    #include "platforms/esp32/library_impl.cpp"
    #include "platforms/esp32/logging_impl.cpp"
    #include "platforms/esp32/ui_impl.cpp"
    #include "platforms/esp32/wifi_impl.cpp"
Esp32EventLoop eventLoop;
Esp32Logging logging;
Esp32ConfigUi configUi(logging);
Esp32Wifi wifi(logging);
Esp32Audio audio(logging, eventLoop);
Esp32Config config(logging, configUi);
Esp32Ui ui(logging, eventLoop);
Esp32Http http(logging);
Esp32Library library(logging, http, config);
#else
    #include "platforms/posix/audio_impl.cpp"
    #include "platforms/posix/config_impl.cpp"
    #include "platforms/posix/config_ui.cpp"
    #include "platforms/posix/event_loop_impl.cpp"
    #include "platforms/posix/http_impl.cpp"
    #include "platforms/posix/library_impl.cpp"
    #include "platforms/posix/logging_impl.cpp"
    #include "platforms/posix/ui_impl.cpp"
    #include "platforms/posix/wifi_impl.cpp"
PosixEventLoop eventLoop;
PosixLogging logging;
PosixConfigUi configUi(logging);
PosixWifi wifi(logging);
PosixAudio audio(logging, eventLoop);
PosixConfig config(logging, configUi);
PosixUserInterface ui(logging, eventLoop);
PosixHttp http(logging);
PosixLibrary library(logging, http, config);
#endif

Application app(config, wifi, logging, audio, ui, eventLoop, library, configUi);

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
