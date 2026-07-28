#include "API.h"
#include <AsyncJson.h>
#include "../core/Storage.h"
#include "../core/Matrix.h"
#include "../network/WifiManager.h"

#include "../effects/EffectManager.h"

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

    AsyncCallbackJsonWebHandler* effectHandler = new AsyncCallbackJsonWebHandler("/api/effect", 
        [](AsyncWebServerRequest *request, JsonVariant &json) {
            JsonObject jsonObj = json.as<JsonObject>();
            if (jsonObj.containsKey("name")) {
                String effectName = jsonObj["name"].as<String>();
                if (FX.setEffect(effectName)) {
                    request->send(200, "application/json", "{\"status\":\"success\"}");
                } else {
                    request->send(404, "application/json", "{\"status\":\"error\",\"message\":\"Effect not found\"}");
                }
            } else {
                request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"Missing name\"}");
            }
        }
    );
    server->addHandler(effectHandler);

    AsyncCallbackJsonWebHandler* wifiHandler = new AsyncCallbackJsonWebHandler("/api/wifi", 
        [](AsyncWebServerRequest *request, JsonVariant &json) {
            JsonObject jsonObj = json.as<JsonObject>();
            if (jsonObj.containsKey("ssid") && jsonObj.containsKey("password")) {
                Storage.settings.wifiSSID = jsonObj["ssid"].as<String>();
                Storage.settings.wifiPass = jsonObj["password"].as<String>();
                Storage.saveSettings();
                
                Serial.printf("[API] WiFi credentials saved for SSID: '%s'. Rebooting in 1 second...\n", Storage.settings.wifiSSID.c_str());
                request->send(200, "application/json", "{\"status\":\"success\",\"message\":\"WiFi credentials saved. Rebooting...\"}");
                
                // Reboot asynchronously after 1000ms to allow HTTP response to send completely
                xTaskCreate([](void*){
                    vTaskDelay(pdMS_TO_TICKS(1000));
                    ESP.restart();
                }, "reboot_task", 2048, NULL, 1, NULL);
            } else {
                request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"Missing ssid or password\"}");
            }
        }
    );
    server->addHandler(wifiHandler);
}

void APIManager::handleStatus(AsyncWebServerRequest *request) {
    JsonDocument doc;
    doc["fps"] = TARGET_FPS;
    doc["brightness"] = Matrix.getBrightness();
    doc["heap"] = ESP.getFreeHeap();
    doc["psram"] = ESP.getFreePsram();
    doc["ip"] = WifiMgr.getIP();
    doc["mac"] = WifiMgr.getMacAddress();
    Effect* cur = FX.getCurrentEffect();
    doc["currentEffect"] = cur ? cur->getName() : "None";
    sendJson(request, doc);
}

void APIManager::handleEffects(AsyncWebServerRequest *request) {
    JsonDocument doc;
    JsonArray effects = doc["effects"].to<JsonArray>();
    std::vector<String> list = FX.getEffectList();
    for (const auto& name : list) {
        effects.add(name);
    }
    sendJson(request, doc);
}

void APIManager::sendJson(AsyncWebServerRequest *request, const JsonDocument& doc) {
    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
}
