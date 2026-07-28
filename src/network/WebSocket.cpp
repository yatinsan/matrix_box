#include "WebSocket.h"
#include "../core/Matrix.h"
#include "../network/WifiManager.h"

WebSocketManager WS;

WebSocketManager::WebSocketManager() : ws("/ws") {}

void WebSocketManager::begin(AsyncWebServer* server) {
    ws.onEvent(std::bind(&WebSocketManager::onEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6));
    server->addHandler(&ws);
}

void WebSocketManager::onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    switch (type) {
        case WS_EVT_CONNECT:
            Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
            break;
        case WS_EVT_DISCONNECT:
            Serial.printf("WebSocket client #%u disconnected\n", client->id());
            break;
        case WS_EVT_DATA:
            handleMessage(client, arg, data, len);
            break;
        case WS_EVT_PONG:
        case WS_EVT_ERROR:
            break;
    }
}

void WebSocketManager::handleMessage(AsyncWebSocketClient *client, void *arg, uint8_t *data, size_t len) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
    
    // Handle text messages (JSON)
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        data[len] = 0; // Null-terminate
        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, (char*)data);
        
        if (!error) {
            String type = doc["type"].as<String>();
            
            if (type == "drawPixel") {
                int x = doc["x"];
                int y = doc["y"];
                uint8_t r = doc["r"];
                uint8_t g = doc["g"];
                uint8_t b = doc["b"];
                Matrix.drawPixel(x, y, CRGB(r, g, b));
                Matrix.show();
            }
            // More websocket commands to come (gamepad, etc)
        }
    }
    
    // Handle binary messages (e.g. raw frame data)
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_BINARY) {
        // Example: receiving raw 289 RGB pixels (867 bytes)
        if (len == (17 * 17 * 3)) {
            for (int i = 0; i < 17 * 17; i++) {
                Matrix.drawPixel(i, CRGB(data[i*3], data[i*3+1], data[i*3+2]));
            }
        }
    }
}

void WebSocketManager::loop() {
    ws.cleanupClients();
    
    unsigned long currentMillis = millis();
    if (currentMillis - lastStatusUpdate > statusInterval) {
        lastStatusUpdate = currentMillis;
        // broadcastStatus(); // Optional: send periodic stats
    }
}

void WebSocketManager::broadcastStatus() {
    JsonDocument doc;
    doc["type"] = "status";
    doc["fps"] = TARGET_FPS;
    doc["heap"] = ESP.getFreeHeap();
    String msg;
    serializeJson(doc, msg);
    ws.textAll(msg);
}

void WebSocketManager::broadcastFrame(const uint8_t* frameData, size_t length) {
    ws.binaryAll((const char*)frameData, length);
}
