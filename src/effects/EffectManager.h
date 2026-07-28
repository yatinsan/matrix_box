#pragma once

#include <Arduino.h>
#include <vector>
#include "Effect.h"

class EffectManager {
public:
    EffectManager();
    
    void begin();
    void run(); // Called every frame from loop()
    
    // Registry
    void addEffect(Effect* effect);
    
    // Control
    bool setEffect(const String& name);
    bool nextEffect();
    bool prevEffect();
    
    // State
    Effect* getCurrentEffect();
    std::vector<String> getEffectList();

private:
    std::vector<Effect*> effects;
    int currentEffectIndex = -1;
};

extern EffectManager FX;
