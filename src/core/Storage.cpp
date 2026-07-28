#include "Storage.h"

StorageManager Storage;

StorageManager::StorageManager() {}

bool StorageManager::begin() {
    bool fsOk = LittleFS.begin(true); // formatOnFail = true
    if (!fsOk) {
        Serial.println("LittleFS Mount Failed");
        // Could be first boot, format is automatic with true param
    } else {
        Serial.println("LittleFS Mounted Successfully");
    }
    
    // Load NVS preferences
    loadSettings();
    return fsOk;
}

void StorageManager::loadSettings() {
    prefs.begin("matrixos", false);
    
    settings.brightness = prefs.getUChar("brightness", DEFAULT_BRIGHTNESS);
    settings.wifiSSID = prefs.getString("wifiSSID", "");
    settings.wifiPass = prefs.getString("wifiPass", "");
    settings.startupEffect = prefs.getString("startEffect", "Rainbow");
    settings.mirrorX = prefs.getBool("mirrorX", false);
    settings.mirrorY = prefs.getBool("mirrorY", false);
    settings.rotation = prefs.getUChar("rotation", 0);
    
    prefs.end();
}

void StorageManager::saveSettings() {
    prefs.begin("matrixos", false);
    
    prefs.putUChar("brightness", settings.brightness);
    prefs.putString("wifiSSID", settings.wifiSSID);
    prefs.putString("wifiPass", settings.wifiPass);
    prefs.putString("startEffect", settings.startupEffect);
    prefs.putBool("mirrorX", settings.mirrorX);
    prefs.putBool("mirrorY", settings.mirrorY);
    prefs.putUChar("rotation", settings.rotation);
    
    prefs.end();
}

void StorageManager::applyDefaults() {
    settings.brightness = DEFAULT_BRIGHTNESS;
    settings.wifiSSID = "";
    settings.wifiPass = "";
    settings.startupEffect = "Rainbow";
    settings.mirrorX = false;
    settings.mirrorY = false;
    settings.rotation = 0;
}

bool StorageManager::fileExists(const char* path) {
    return LittleFS.exists(path);
}

String StorageManager::readFile(const char* path) {
    if (!LittleFS.exists(path)) return String();
    
    File file = LittleFS.open(path, "r");
    if (!file) return String();
    
    String content = file.readString();
    file.close();
    return content;
}

bool StorageManager::writeFile(const char* path, const String& content) {
    File file = LittleFS.open(path, "w");
    if (!file) return false;
    
    file.print(content);
    file.close();
    return true;
}

bool StorageManager::deleteFile(const char* path) {
    return LittleFS.remove(path);
}
