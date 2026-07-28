#pragma once

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

class WebSocketManager {
public:
    WebSocketManager();
    void begin(AsyncWebServer* server);
    void loop(); // Process client pings/messages if needed
    
    // Broadcast helpers
    void broadcastStatus();
    void broadcastFrame(const uint8_t* frameData, size_t length);
    
private:
    AsyncWebSocket ws;
    void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
    void handleMessage(AsyncWebSocketClient *client, void *arg, uint8_t *data, size_t len);
    
    unsigned long lastStatusUpdate = 0;
    const unsigned long statusInterval = 1000;
};

extern WebSocketManager WS;
