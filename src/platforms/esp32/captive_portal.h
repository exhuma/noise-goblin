/// @brief Start the captive portal.
/// @param config The configuration object.
/// @param logger The logging object.
void start_captive_portal(IConfig &config, ILogging &logger);

/// @brief Process an incoming HTTP request.
/// @param logger The logging object.
void process_request(ILogging &logger);
