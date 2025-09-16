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
#if defined(BUILD_ESP32S3) || defined(BUILD_ESP32) || defined(BUILD_XIAOESP32)
    #include "platforms/esp32common/config_impl.cpp"
    #include "platforms/esp32common/config_ui.cpp"
    #include "platforms/esp32common/http_impl.cpp"
    #include "platforms/esp32common/library_impl.cpp"
    #include "platforms/esp32common/logging_impl.cpp"
    #include "platforms/esp32common/wifi_impl.cpp"
#elif defined(BUILD_POSIX)
    #include "platforms/posix/audio_impl.cpp"
    #include "platforms/posix/config_impl.cpp"
    #include "platforms/posix/config_ui.cpp"
    #include "platforms/posix/http_impl.cpp"
    #include "platforms/posix/library_impl.cpp"
    #include "platforms/posix/logging_impl.cpp"
    #include "platforms/posix/ui_impl.cpp"
    #include "platforms/posix/wifi_impl.cpp"
#endif

#if defined(BUILD_ESP32S3)
    #include "platforms/esp32s3/audio_impl.cpp"
    #include "platforms/esp32s3/ui_impl.cpp"
#elif defined(BUILD_ESP32)
    #include "platforms/esp32/audio_impl.cpp"
    #include "platforms/esp32/ui_impl.cpp"
#elif defined(BUILD_XIAOESP32)
    #include "platforms/xiaoesp32/audio_impl.cpp"
    #include "platforms/xiaoesp32/ui_impl.cpp"
#endif

NoiseLogging logging;
NoiseConfigUi configUi(logging);
NoiseWifi wifi(logging);
NoiseAudio audio(logging);
NoiseConfig config(logging, configUi);
NoiseHttp http(logging);
NoiseLibrary library(logging, http, config);
NoiseUi ui(logging);

Application app(config, wifi, logging, audio, ui, library, configUi);

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
        usleep(50 * 1000);  // Sleep for 50 milliseconds
    }
    return 0;
}
