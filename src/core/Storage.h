#pragma once

#include <Arduino.h>
#include <Preferences.h>
#include <LittleFS.h>
#include "../Config.h"

struct SystemSettings {
    uint8_t brightness = DEFAULT_BRIGHTNESS;
    String wifiSSID = "";
    String wifiPass = "";
    String startupEffect = "Rainbow";
    bool mirrorX = false;
    bool mirrorY = false;
    uint8_t rotation = 0; // 0=0, 1=90, 2=180, 3=270
};

class StorageManager {
public:
    StorageManager();
    
    bool begin();
    
    // NVS Settings
    void loadSettings();
    void saveSettings();
    
    SystemSettings settings;
    
    // LittleFS Helpers
    bool fileExists(const char* path);
    String readFile(const char* path);
    bool writeFile(const char* path, const String& content);
    bool deleteFile(const char* path);
    
private:
    Preferences prefs;
    void applyDefaults();
};

extern StorageManager Storage;
