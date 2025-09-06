/**
 * This file defines functionality to run a configuration UI on the ESP32 board.
 *
 * The configuration UI provides a UI to enter a WiFi SSID and Password.
 */
#include "../config_ui.hpp"
#include <DNSServer.h>
#include <WebServer.h>
#include <WiFi.h>
#include <map>
#include <string>
#include "../../const.hpp"

// TODO: These HTML files should be moved to a template-file
static const std::string ROOT_PAGE =
    "<!DOCTYPE html><html><head>"
    "<meta name=\"viewport\" content=\"width=device-width, "
    "initial-scale=1\">"
    "<style>"
    "body { font-family: Arial, sans-serif; margin: 0; padding: 20px; }"
    "h2 { color: #333; }"
    "form { max-width: 300px; margin: auto; }"
    "input[type=\"text\"], input[type=\"password\"] { "
    "  width: 100%; padding: 10px; margin: 10px 0; box-sizing: "
    "border-box; "
    "}"
    "input[type=\"submit\"] { "
    "  background-color: #4CAF50; color: white; padding: 10px; border: "
    "none; "
    "  width: 100%; cursor: pointer; }"
    "input[type=\"submit\"]:hover { background-color: #45a049; }"
    "</style>"
    "</head><body>"
    "<h2>Initial Setup</h2>"
    "<form action=\"/save\" method=\"POST\">"
    "Wifi SSID: <input type=\"text\" name=\"ssid\" placeholder=\"Enter "
    "SSID\" value=\"{" +
    std::string(WIFI_SSID_KEY) +
    "}\"><br>"
    "Wifi Password: <input type=\"text\" name=\"password\" "
    "placeholder=\"Enter Password\" value=\"{" +
    std::string(WIFI_PASSWORD_KEY) +
    "}\"><br>"
    "Library URL: <input type=\"text\" name=\"library_url\" "
    "placeholder=\"https://example.com/library\" value=\"{" +
    std::string(LIBRARY_BASE_URL_KEY) +
    "}\"><br>"
    "<input type=\"submit\" value=\"Save\">"
    "</form>"
    "</body></html>";

static const String SUCCESS_PAGE =
    "<!DOCTYPE html><html><head>"
    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
    "</head><body><h3>Connection successful. Restarting...</h3></body></html>";

static const String FAILURE_PAGE =
    "<!DOCTYPE html><html><head>"
    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
    "</head><body><h3>Connection failed</h3></body></html>";

static auto replace(std::string str, const char* from,
                    const std::map<std::string, std::string>& defaults)
    -> std::string {
    size_t start_pos = 0;
    std::string wrapped_key = "{" + std::string(from) + "}";
    std::string to = defaults.count(from) ? defaults.at(from) : "";
    while ((start_pos = str.find(wrapped_key, start_pos)) !=
           std::string::npos) {
        str.replace(start_pos, wrapped_key.length(), to);
        start_pos += to.length();
    }
    return str;
}

class Esp32ConfigUi : public IConfigUi {
  public:
    Esp32ConfigUi(ILogging& logger) : IConfigUi(logger) {
    }

    void start() override {
        if (PORTAL_STATE == PORTAL_RUNNING) {
            return;
        }
        // Start Access Point
        WiFi.mode(WIFI_AP);
        WiFi.softAP(apSSID, apPassword);

        IPAddress myIP = WiFi.softAPIP();
        logger.info("Configuration UI started");
        logger.info("Connect your phone to WiFi: %s", apSSID);
        logger.info("Password: %s", apPassword);
        logger.info("Then open: http://%s", myIP.toString().c_str());

        // DNS server to redirect all queries to our ESP
        dnsServer.start(DNS_PORT, "*", myIP);

        // Setup web handlers
        // Serve the main page for all GET requests
        webServer.onNotFound([&]() {
            if (webServer.method() == HTTP_GET) {
                logger.debug("Redirecting %s to root page", webServer.uri());
                logger.debug("Redirecting %s to root page", webServer.uri().c_str());
                handleRoot();
            } else {
                logger.info("404 - Not Found: %s", webServer.uri().c_str());
                webServer.send(404, "text/plain", "Not found");
            }
        });

        // Only handle POST on /connect explicitly
        webServer.on("/connect", HTTP_POST, [&]() { handleSave(); });
        webServer.begin();
        PORTAL_STATE = PORTAL_RUNNING;
    }

    auto promptFor(const std::string& key, const std::string& message)
        -> std::string override {
        return userResponses.count(key) ? userResponses[key] : "";
    }

    auto isRunning() -> bool override {
        return PORTAL_STATE == PORTAL_RUNNING;
    }

    void setDefaults(
        const std::map<std::string, std::string>& defaults) override {
        this->defaults = defaults;
    }

    void tick() override {
        dnsServer.processNextRequest();
        webServer.handleClient();
    }

  private:
    std::map<std::string, std::string> defaults = {};
    std::map<std::string, std::string> userResponses;
    const uint8_t DNS_PORT = 53;
    DNSServer dnsServer;
    WebServer webServer;
    PortalState PORTAL_STATE = PORTAL_OFF;
    // default AP name and password
    const char* apSSID = "ESP32-Setup";
    const char* apPassword = "12345678";

    void handleRoot() {
        std::string page = ROOT_PAGE;
        page = replace(page, WIFI_SSID_KEY, defaults);
        page = replace(page, WIFI_PASSWORD_KEY, defaults);
        page = replace(page, LIBRARY_BASE_URL_KEY, defaults);
        webServer.send(200, "text/html", String(page.c_str()));
    }

    void handleSave() {
        // TODO: This should preferrably be done asynchronously
        String ssid = webServer.arg("ssid");
        String pass = webServer.arg("password");
        String libraryUrl = webServer.arg("library_url");
        bool isConnected = canConnect(ssid, pass);
        if (isConnected) {
            logger.info("Connected!");
            logger.info("IP address: %s", WiFi.localIP());
            userResponses[WIFI_SSID_KEY] = ssid.c_str();
            userResponses[WIFI_PASSWORD_KEY] = pass.c_str();
            userResponses[LIBRARY_BASE_URL_KEY] = libraryUrl.c_str();
            webServer.send(200, "text/html", SUCCESS_PAGE);
            defaults.clear();
        } else {
            logger.error("Failed to connect.");
            webServer.send(200, "text/html", FAILURE_PAGE);
        }
    }

    auto canConnect(const String& ssid, const String& pass) -> bool {
        logger.info("Trying to connect to SSID: %s", ssid);
        WiFi.begin(ssid.c_str(), pass.c_str());

        // Attempt connection for a few seconds
        int retries = 0;
        while (WiFi.status() != WL_CONNECTED && retries < 20) {
            delay(500);
            logger.debug(".");
            retries++;
        }
        if (WiFi.status() == WL_CONNECTED) {
            return true;
        }
        return false;
    }
};
