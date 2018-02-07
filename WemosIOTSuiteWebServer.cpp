#include "WemosIOTSuiteWebServer.h"

WemosIOTSuiteWebServer::WemosIOTSuiteWebServer()
{
    _webServer.port = DEFAULT_WEBSERVER_PORT;
    _webServer.ip = DEFAULT_WEBSERVER_IP;
    _webServer.netmask = DEFAULT_WEBSERVER_NETMASK;
    _webServer.ssid.name = DEFAULT_SSID_NAME;
    _webServer.ssid.password = DEFAULT_SSID_PASSWORD;
    _webServer.type = AP;
    _webServer.status = IDLE;
}

void WemosIOTSuiteWebServer::start()
{

    DEBUG_PRINTLN("Starting web server");

    WemosIOTSuiteWebServer::setupSTAMode();
    if (_webServer.status == AP_CONNECTION_TIMEOUT)
    {
        WemosIOTSuiteWebServer::setupAPMode();
    }
}

void WemosIOTSuiteWebServer::setupAPMode()
{

    DEBUG_PRINTLN("Configuring AP mode web server");

    _webServer.status = AP_CONNECTION_STARTING;

    WiFi.softAPConfig(_webServer.ip, _webServer.ip, _webServer.netmask);
    WiFi.softAP(_webServer.ssid.name, _webServer.ssid.password);
    
    delay(500);
    
    DEBUG_PRINT("AP IP address: ");
    DEBUG_CPRINTLN(WiFi.softAPIP());

    ESP8266WebServer *server = new ESP8266WebServer(_webServer.port);

    _webServer.status = AP_CONNECTION_CONNECTED;
    _webServer.type = AP;
    _webServer.server = server;

    DEBUG_PRINTLN("HTTP server started in AP mode");
}

void WemosIOTSuiteWebServer::setupSTAMode()
{

    DEBUG_PRINTLN("Configuring STA mode web server");

    _webServer.status = STA_CONNECTION_STARTING;

    // Configure as STA mode
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    WiFi.begin(_webServer.ssid.name, _webServer.ssid.password);

    // Wait for connection
    DEBUG_PRINT("Starting in STA mode");
    volatile long startTime = millis();
    while (WiFi.status() != WL_CONNECTED && _webServer.status != AP_CONNECTION_TIMEOUT)
    {
        delay(500);
        DEBUG_CPRINT(".");

        if (millis() - startTime > WEBSERVER_STA_TIMEOUT_MS)
        {
            _webServer.status = AP_CONNECTION_TIMEOUT;
        }
    }

    if (_webServer.status != AP_CONNECTION_TIMEOUT)
    {
        DEBUG_CPRINTLN();
        DEBUG_PRINT("Connected to ");
        DEBUG_PRINTLN(_webServer.ssid.name);
        DEBUG_PRINT("IP address: ");
        DEBUG_PRINTLN(WiFi.localIP());

        if (MDNS.begin("esp8266"))
        {
            DEBUG_PRINTLN("MDNS responder started");
        }

        ESP8266WebServer *server = new ESP8266WebServer(_webServer.port);

        // server.on("/", handleStaRoot);
        // server.on("/config", handleStaConfig);
        // server.on("/data", handleGetSensorData);
        // server.onNotFound(handleNotFound);

        server->begin();

        _webServer.status = STA_CONNECTION_CONNECTED;
        _webServer.type = STA;
        _webServer.server = server;

        DEBUG_PRINTLN("HTTP server started in STA mode");
    }
    else
    {
        DEBUG_CPRINTLN();
        DEBUG_PRINTLN("Timeout error starting in STA mode!");
    }
}

void WemosIOTSuiteWebServer::handleAPRoot()
{
}

void WemosIOTSuiteWebServer::handleSTARoot()
{
}
