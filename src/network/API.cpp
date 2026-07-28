#include "API.h"
#include <AsyncJson.h>
#include "../core/Storage.h"
#include "../core/Matrix.h"
#include "../network/WifiManager.h"

APIManager API;

APIManager::APIManager() {}

void APIManager::begin(AsyncWebServer* server) {
    server->on("/api/status", HTTP_GET, std::bind(&APIManager::handleStatus, this, std::placeholders::_1));
    server->on("/api/effects", HTTP_GET, std::bind(&APIManager::handleEffects, this, std::placeholders::_1));
    
    // POST handlers using AsyncCallbackJsonWebHandler
    AsyncCallbackJsonWebHandler* brightnessHandler = new AsyncCallbackJsonWebHandler("/api/brightness", 
        [](AsyncWebServerRequest *request, JsonVariant &json) {
            JsonObject jsonObj = json.as<JsonObject>();
            if (jsonObj.containsKey("brightness")) {
                uint8_t b = jsonObj["brightness"].as<uint8_t>();
                Matrix.setBrightness(b);
                Storage.settings.brightness = b;
                Storage.saveSettings();
                
                JsonDocument doc;
                doc["status"] = "success";
                doc["brightness"] = b;
                String response;
                serializeJson(doc, response);
                request->send(200, "application/json", response);
            } else {
                request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"Missing brightness\"}");
            }
        }
    );
    server->addHandler(brightnessHandler);

    AsyncCallbackJsonWebHandler* wifiHandler = new AsyncCallbackJsonWebHandler("/api/wifi", 
        [](AsyncWebServerRequest *request, JsonVariant &json) {
            JsonObject jsonObj = json.as<JsonObject>();
            if (jsonObj.containsKey("ssid") && jsonObj.containsKey("password")) {
                Storage.settings.wifiSSID = jsonObj["ssid"].as<String>();
                Storage.settings.wifiPass = jsonObj["password"].as<String>();
                Storage.saveSettings();
                
                request->send(200, "application/json", "{\"status\":\"success\",\"message\":\"WiFi credentials saved. Rebooting...\"}");
                delay(500);
                ESP.restart();
            } else {
                request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"Missing ssid or password\"}");
            }
        }
    );
    server->addHandler(wifiHandler);
}

void APIManager::handleStatus(AsyncWebServerRequest *request) {
    JsonDocument doc;
    doc["fps"] = TARGET_FPS; // TODO: Calculate actual FPS
    doc["brightness"] = Matrix.getBrightness();
    doc["heap"] = ESP.getFreeHeap();
    doc["psram"] = ESP.getFreePsram();
    doc["ip"] = Network.getIP();
    doc["mac"] = Network.getMacAddress();
    sendJson(request, doc);
}

void APIManager::handleEffects(AsyncWebServerRequest *request) {
    JsonDocument doc;
    JsonArray effects = doc["effects"].to<JsonArray>();
    // TODO: Pull from EffectManager
    effects.add("Rainbow");
    effects.add("Solid Color");
    
    sendJson(request, doc);
}

void APIManager::sendJson(AsyncWebServerRequest *request, const JsonDocument& doc) {
    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
}
