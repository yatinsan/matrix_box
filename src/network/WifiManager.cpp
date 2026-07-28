#include "WifiManager.h"

WifiManager WifiMgr;

WifiManager::WifiManager() {}

void WifiManager::begin() {
    WiFi.mode(WIFI_AP_STA);
    
    // Always start AP mode for fallback / captive portal
    startAP();
    
    // If we have saved credentials, try to connect to STA
    if (Storage.settings.wifiSSID.length() > 0) {
        Serial.printf("Saved WiFi credentials found for SSID: '%s'. Attempting STA connection...\n", Storage.settings.wifiSSID.c_str());
        connectToSTA();
    } else {
        Serial.println("No WiFi credentials saved. Running in AP Mode only.");
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
    IPAddress local_ip(192, 168, 4, 1);
    IPAddress gateway(192, 168, 4, 1);
    IPAddress subnet(255, 255, 255, 0);
    
    WiFi.softAPConfig(local_ip, gateway, subnet);
    WiFi.softAP(AP_SSID, AP_PASSWORD);
    
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);
    
    // Redirect all DNS requests to AP IP (Captive Portal)
    dnsServer.start(53, "*", IP);
    Serial.println("DNS Server started for Captive Portal (port 53)");
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
    dnsServer.processNextRequest();

    if (apModeOnly) return;
    
    unsigned long currentMillis = millis();
    static bool wasConnected = false;

    if (WiFi.status() == WL_CONNECTED) {
        if (!wasConnected) {
            wasConnected = true;
            Serial.println("\n==========================================");
            Serial.print("🎉 WiFi Connected to: ");
            Serial.println(Storage.settings.wifiSSID);
            Serial.print("🌐 Local IP Address: ");
            Serial.println(WiFi.localIP());
            Serial.println("🔗 Local URL: http://matrix.local");
            Serial.println("==========================================\n");
        }
    } else {
        if (wasConnected) {
            wasConnected = false;
            Serial.println("⚠️ Lost WiFi connection.");
        }
        
        // Reconnect attempt every 10 seconds
        if (currentMillis - lastReconnectAttempt >= reconnectInterval) {
            Serial.println("Retrying WiFi connection...");
            WiFi.disconnect();
            WiFi.begin(Storage.settings.wifiSSID.c_str(), Storage.settings.wifiPass.c_str());
            lastReconnectAttempt = currentMillis;
        }
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
