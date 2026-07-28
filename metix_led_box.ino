#include <Arduino.h>
#include "src/Config.h"
#include "src/core/Storage.h"
#include "src/network/WifiManager.h"
#include "src/network/WebServer.h"
#include "src/network/WebSocket.h"
#include "src/core/Matrix.h"
#include "src/effects/EffectManager.h"
#include "src/effects/library/Rainbow.h"
#include "src/effects/library/SolidColor.h"

// Task scheduling variable
unsigned long lastFrameTime = 0;
const unsigned long frameInterval = 1000 / TARGET_FPS;

RainbowEffect fxRainbow;
SolidColorEffect fxSolid;

void setup() {
    Serial.begin(115200);
    delay(1000); // Wait for serial monitor to open
    
    Serial.println("\n--- MatrixOS Booting ---");
    
    Serial.println("Initializing Storage...");
    Storage.begin();
    
    Serial.println("Initializing Network...");
    Network.begin();

    Serial.println("Initializing Matrix...");
    Matrix.begin();
    
    Serial.println("Initializing Web Server...");
    Web.begin();
    
    // Test pixel to show we are alive
    Matrix.drawPixel(0, 0, CRGB::Red);
    Matrix.show();
    delay(500);
    Matrix.clear();
    Matrix.show();
    
    Serial.println("Initializing Effects...");
    FX.addEffect(&fxRainbow);
    FX.addEffect(&fxSolid);
    FX.begin();
    
    Serial.println("Setup Complete.");
}

void loop() {
    unsigned long currentMillis = millis();

    // 60 FPS loop
    if (currentMillis - lastFrameTime >= frameInterval) {
        lastFrameTime = currentMillis;
        
        FX.run(); // Updates effect and calls Matrix.show() internally
    }
    
    Network.loop();
    WS.loop();
    // TODO: Update OTA (non-blocking)
}
