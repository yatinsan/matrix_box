#pragma once

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include "../Config.h"

class WebServerManager {
public:
    WebServerManager();
    
    void begin();
    AsyncWebServer* getServer();
    
private:
    AsyncWebServer server;
    void setupRoutes();
    void setupCaptivePortal();
    void setupStaticFiles();
};

extern WebServerManager Web;
