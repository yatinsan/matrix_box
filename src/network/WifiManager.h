#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <DNSServer.h>
#include "../Config.h"
#include "../core/Storage.h"

class WifiManager {
public:
    WifiManager();
    
    void begin();
    void loop(); // Handles auto-reconnect if needed
    
    bool isConnected();
    String getIP();
    String getMacAddress();
    
private:
    void startAP();
    void connectToSTA();
    
    DNSServer dnsServer;
    unsigned long lastReconnectAttempt = 0;
    const unsigned long reconnectInterval = 10000; // 10 seconds
    bool apModeOnly = false;
};

extern WifiManager WifiMgr;
