#pragma once

#include "../Effect.h"

class SolidColorEffect : public Effect {
public:
    SolidColorEffect();
    
    void start() override;
    void run() override;
    
    const char* getName() const override { return "Solid Color"; }
    bool supportsColor() const override { return true; }
    bool supportsSpeed() const override { return false; }
    
    void setColor(CRGB newColor) { color = newColor; }
    
private:
    CRGB color;
};
