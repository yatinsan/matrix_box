#pragma once

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

class APIManager {
public:
    APIManager();
    void begin(AsyncWebServer* server);
    
private:
    void handleStatus(AsyncWebServerRequest *request);
    void handleEffects(AsyncWebServerRequest *request);
    void handleBrightness(AsyncWebServerRequest *request);
    void handleConfig(AsyncWebServerRequest *request);
    void handleWifi(AsyncWebServerRequest *request);
    void handleReboot(AsyncWebServerRequest *request);
    void handleFactoryReset(AsyncWebServerRequest *request);
    
    // Helpers
    void sendJson(AsyncWebServerRequest *request, const JsonDocument& doc);
};

extern APIManager API;
