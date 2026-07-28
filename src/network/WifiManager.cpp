#include "WifiManager.h"

WifiManager Network;

WifiManager::WifiManager() {}

void WifiManager::begin() {
    WiFi.mode(WIFI_AP_STA);
    
    // Always start AP mode for fallback / captive portal
    startAP();
    
    // If we have saved credentials, try to connect to STA
    if (Storage.settings.wifiSSID.length() > 0) {
        connectToSTA();
    } else {
        Serial.println("No WiFi credentials saved. AP Mode only.");
        apModeOnly = true;
    }
    
    // Setup mDNS
    if (MDNS.begin("matrix")) {
        Serial.println("mDNS responder started: http://matrix.local");
    } else {
        Serial.println("Error setting up mDNS responder!");
    }
}

void WifiManager::startAP() {
    Serial.println("Starting AP Mode...");
    WiFi.softAP(AP_SSID, AP_PASSWORD);
    
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);
}

void WifiManager::connectToSTA() {
    Serial.print("Connecting to WiFi network: ");
    Serial.println(Storage.settings.wifiSSID);
    
    WiFi.begin(Storage.settings.wifiSSID.c_str(), Storage.settings.wifiPass.c_str());
    
    // We don't block here. We let the ESP connect asynchronously.
    // The loop() will check the status.
    apModeOnly = false;
    lastReconnectAttempt = millis();
}

void WifiManager::loop() {
    if (apModeOnly) return;
    
    unsigned long currentMillis = millis();
    
    // Check connection status every 10 seconds if disconnected
    if (WiFi.status() != WL_CONNECTED && (currentMillis - lastReconnectAttempt >= reconnectInterval)) {
        Serial.println("WiFi disconnected. Attempting to reconnect...");
        WiFi.disconnect();
        WiFi.begin(Storage.settings.wifiSSID.c_str(), Storage.settings.wifiPass.c_str());
        lastReconnectAttempt = currentMillis;
    }
}

bool WifiManager::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

String WifiManager::getIP() {
    if (isConnected()) {
        return WiFi.localIP().toString();
    } else {
        return WiFi.softAPIP().toString();
    }
}

String WifiManager::getMacAddress() {
    return WiFi.macAddress();
}
