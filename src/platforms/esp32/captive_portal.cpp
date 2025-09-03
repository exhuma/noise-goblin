/**
 * This file defines functionality to run a captive portal on the ESP32 board.
 *
 * The captive portal provides a UI to enter a WiFi SSID and Password.
 */
#include <DNSServer.h>
#include <WebServer.h>
#include <WiFi.h>
#include "../config.hpp"
#include "../logging.hpp"

const byte DNS_PORT = 53;
DNSServer dnsServer;
WebServer server(80);

// default AP name and password
const char* apSSID = "ESP32-Setup";
const char* apPassword = "12345678";

enum PortalState { PORTAL_OFF, PORTAL_RUNNING };
PortalState PORTAL_STATE = PORTAL_OFF;

void handleRoot() {
    String html =
        "<!DOCTYPE html><html><head>"
        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
        "<style>"
        "body { font-family: Arial, sans-serif; margin: 0; padding: 20px; }"
        "h2 { color: #333; }"
        "form { max-width: 300px; margin: auto; }"
        "input[type=\"text\"], input[type=\"password\"] { "
        "  width: 100%; padding: 10px; margin: 10px 0; box-sizing: border-box; }"
        "input[type=\"submit\"] { "
        "  background-color: #4CAF50; color: white; padding: 10px; border: none; "
        "  width: 100%; cursor: pointer; }"
        "input[type=\"submit\"]:hover { background-color: #45a049; }"
        "</style>"
        "</head><body>"
        "<h2>WiFi Setup</h2>"
        "<form action=\"/connect\" method=\"POST\">"
        "SSID: <input type=\"text\" name=\"ssid\" placeholder=\"Enter SSID\"><br>"
        "Password: <input type=\"password\" name=\"password\" placeholder=\"Enter Password\"><br>"
        "<input type=\"submit\" value=\"Connect\">"
        "</form>"
        "</body></html>";
    server.send(200, "text/html", html);
}

void handleConnect(IConfig& config, ILogging& logger) {
    String ssid = server.arg("ssid");
    String pass = server.arg("password");

    logger.logln(("Trying to connect to SSID: " + ssid).c_str());
    WiFi.begin(ssid.c_str(), pass.c_str());

    // Attempt connection for a few seconds
    int retries = 0;
    while (WiFi.status() != WL_CONNECTED && retries < 20) {
        delay(500);
        logger.logln(".");
        retries++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        logger.logln("Connected!");
        logger.logln(("IP address: " + WiFi.localIP().toString()).c_str());
        config.set("wifi_ssid", ssid.c_str());
        config.set("wifi_password", pass.c_str());
        server.send(200, "text/html",
                    "<html><body><h3>Connection successful. Restarting...</h3></body></html>");
        delay(2000);
        ESP.restart();
    } else {
        logger.logln("Failed to connect.");
        server.send(200, "text/html", "<html><body><h3>Connection failed</h3></body></html>");
    }
}

void start_captive_portal(IConfig& config, ILogging& logger) {
    if (PORTAL_STATE == PORTAL_RUNNING) {
        return;
    }
    // Start Access Point
    WiFi.mode(WIFI_AP);
    WiFi.softAP(apSSID, apPassword);

    IPAddress myIP = WiFi.softAPIP();
    logger.logln("Captive portal started");
    logger.logln(("Connect your phone to WiFi: " + String(apSSID)).c_str());
    logger.logln(("Password: " + String(apPassword)).c_str());
    logger.logln(("Then open: http://" + myIP.toString()).c_str());

    // DNS server to redirect all queries to our ESP
    dnsServer.start(DNS_PORT, "*", myIP);

    // Setup web handlers
    // Serve the main page for all GET requests
    server.onNotFound([&logger]() {
        if (server.method() == HTTP_GET) {
            logger.logln(("Redirecting " + server.uri() + " to root page").c_str());
            handleRoot();
        } else {
            logger.logln(("404 - Not Found: " + server.uri()).c_str());
            server.send(404, "text/plain", "Not found");
        }
    });

    // Only handle POST on /connect explicitly
    server.on("/connect", HTTP_POST, [&config, &logger]() { handleConnect(config, logger); });
    server.begin();
    PORTAL_STATE = PORTAL_RUNNING;
}

void process_request(ILogging& logger) {
    dnsServer.processNextRequest();
    server.handleClient();
}
