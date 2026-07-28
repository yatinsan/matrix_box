#include "WebServer.h"
#include "API.h"
#include "WebSocket.h"
#include "WifiManager.h"

WebServerManager Web;

WebServerManager::WebServerManager() : server(80) {}

void WebServerManager::begin() {
    setupCaptivePortal();
    
    // API setup
    API.begin(&server);
    
    // WebSocket setup
    WS.begin(&server);
    
    setupStaticFiles();
    
    // Catch-all for 404 and SPA routing
    server.onNotFound([](AsyncWebServerRequest *request) {
        if (request->method() == HTTP_OPTIONS) {
            request->send(200);
        } else {
            // For Single Page Applications (SPA), return index.html on 404
            request->send(LittleFS, "/index.html", "text/html");
        }
    });
    
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "*");
    
    server.begin();
    Serial.println("Web server started on port 80");
}

AsyncWebServer* WebServerManager::getServer() {
    return &server;
}

void WebServerManager::setupCaptivePortal() {
    // If the host isn't our IP or matrix.local, redirect to our captive portal
    server.addHandler(new AsyncCallbackWebHandler([](AsyncWebServerRequest *request) {
        String host = request->host();
        String path = request->url();
        String apIP = Network.getIP();
        
        // Exclude mDNS and API requests from captive portal redirection
        if (host != apIP && host != "matrix.local" && !path.startsWith("/api") && !path.startsWith("/ws")) {
            request->redirect("http://" + apIP + "/");
            return true;
        }
        return false;
    }));
}

void WebServerManager::setupStaticFiles() {
    // Serve everything in LittleFS root to /
    server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html").setCacheControl("max-age=86400");
}
