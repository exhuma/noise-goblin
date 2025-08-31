/**
 * This file defines functionality to run a captive portal on the ESP32 board.
 *
 * The captive portal provides a UI to enter a WiFi SSID and Password.
 */
#include <DNSServer.h>
#include <WebServer.h>
#include <WiFi.h>
#include "../config.h"
#include "../logging.h"

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
        "<!DOCTYPE html><html><body>"
        "<h2>WiFi Setup</h2>"
        "<form action=\"/connect\" method=\"POST\">"
        "SSID: <input type=\"text\" name=\"ssid\"><br>"
        "Password: <input type=\"password\" name=\"password\"><br>"
        "<input type=\"submit\" value=\"Connect\">"
        "</form>"
        "</body></html>";
    server.send(200, "text/html", html);
}

void handleConnect() {
    String ssid = server.arg("ssid");
    String pass = server.arg("password");

    logln(("Trying to connect to SSID: " + ssid).c_str());
    WiFi.begin(ssid.c_str(), pass.c_str());

    // Attempt connection for a few seconds
    int retries = 0;
    while (WiFi.status() != WL_CONNECTED && retries < 20) {
        delay(500);
        logln(".");
        retries++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        logln("Connected!");
        logln(("IP address: " + WiFi.localIP().toString()).c_str());
        set_config_value("wifi_ssid", ssid.c_str());
        set_config_value("wifi_password", pass.c_str());
        server.send(200, "text/html", "<html><body><h3>Connection successful</h3></body></html>");
    } else {
        logln("Failed to connect.");
        server.send(200, "text/html", "<html><body><h3>Connection failed</h3></body></html>");
    }
}

void start_captive_portal() {
    if (PORTAL_STATE == PORTAL_RUNNING) {
        return;
    }
    // Start Access Point
    WiFi.mode(WIFI_AP);
    WiFi.softAP(apSSID, apPassword);

    IPAddress myIP = WiFi.softAPIP();
    logln("Captive portal started");
    logln(("Connect your phone to WiFi: " + String(apSSID)).c_str());
    logln(("Password: " + String(apPassword)).c_str());
    logln(("Then open: http://" + myIP.toString()).c_str());

    // DNS server to redirect all queries to our ESP
    dnsServer.start(DNS_PORT, "*", myIP);

    // Setup web handlers
    // Serve the main page for all GET requests
    server.onNotFound([]() {
        if (server.method() == HTTP_GET) {
            logln(("Redirecting " + server.uri() + " to root page").c_str());
            handleRoot();
        } else {
            logln(("404 - Not Found: " + server.uri()).c_str());
            server.send(404, "text/plain", "Not found");
        }
    });

    // Only handle POST on /connect explicitly
    server.on("/connect", HTTP_POST, handleConnect);
    server.begin();
    PORTAL_STATE = PORTAL_RUNNING;
}

void process_request() {
    dnsServer.processNextRequest();
    server.handleClient();
}
