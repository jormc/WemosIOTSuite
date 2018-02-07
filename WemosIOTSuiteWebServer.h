#ifndef WEMOS_IOTSUITE_WEBSERVER_H
#define WEMOS_IOTSUITE_WEBSERVER_H

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>

extern "C" {
  #include "user_interface.h"
}

// Define where debug output will be printed.
#define DEBUG_PRINTER Serial

// Uncomment to debug to Serial
#define WEMOS_IOTSUITE_DEBUG

// Setup debug printing macros.
#ifdef WEMOS_IOTSUITE_DEBUG
    #define DEBUG_PRINT(...)      { DEBUG_PRINTER.print("[WebServer] ");DEBUG_PRINTER.print(__VA_ARGS__); }
    #define DEBUG_PRINTLN(...)    { DEBUG_PRINTER.print("[WebServer] ");DEBUG_PRINTER.println(__VA_ARGS__); }
    #define DEBUG_CPRINT(...)     { DEBUG_PRINTER.print(__VA_ARGS__); }
    #define DEBUG_CPRINTLN(...)   { DEBUG_PRINTER.println(__VA_ARGS__); }
#else
    #define DEBUG_PRINT(...)      {}
    #define DEBUG_PRINTLN(...)    {}
    #define DEBUG_CPRINT(...)     {}
    #define DEBUG_CPRINTLN(...)   {}
#endif

#define DEFAULT_WEBSERVER_PORT      80
#define DEFAULT_WEBSERVER_IP        IPAddress(192,168,4,1)
#define DEFAULT_WEBSERVER_NETMASK   IPAddress(255,255,255,0)
#define DEFAULT_SSID_NAME           "WEMOS"
#define DEFAULT_SSID_PASSWORD       "1234567890"
#define WEBSERVER_STA_TIMEOUT_MS    15000
#define WEBSERVER_AP_TIMEOUT_MS     15000

typedef enum {
    IDLE = 0,
    STARTING = 1,
    ERROR = 2,
    STA_CONNECTION_TIMEOUT = 100,
    STA_CONNECTION_STARTING = 101,
    STA_CONNECTION_CONNECTED = 102,
    AP_CONNECTION_TIMEOUT = 200,
    AP_CONNECTION_STARTING = 201,
    AP_CONNECTION_CONNECTED = 202
} webserver_status_t;

typedef enum {
    AP = 0,
    STA = 1
} webserver_type_t;

typedef struct {
    char* name;
    char* password;
} ssid_t;

typedef struct {
    int port;
    IPAddress ip;
    IPAddress netmask;
    ssid_t ssid;
    webserver_type_t type;
    webserver_status_t status;
    ESP8266WebServer* server;
} webserver_t;

class WemosIOTSuiteWebServer {

    public:
        WemosIOTSuiteWebServer();
        void start();

    private:


        void setupAPMode();
        void setupSTAMode();

        void handleAPRoot();
        void handleSTARoot();

        webserver_t _webServer;

};

#endif