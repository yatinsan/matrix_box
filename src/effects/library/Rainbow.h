#pragma once

#include "../Effect.h"

class RainbowEffect : public Effect {
public:
    RainbowEffect();
    
    void start() override;
    void run() override;
    
    const char* getName() const override { return "Rainbow"; }
    bool supportsSpeed() const override { return true; }
    
private:
    uint16_t hue;
    uint8_t speed;
};
