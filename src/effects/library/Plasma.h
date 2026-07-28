#pragma once

#include "../Effect.h"

class PlasmaEffect : public Effect {
public:
    PlasmaEffect();
    
    void start() override;
    void run() override;
    
    const char* getName() const override { return "Perlin Plasma"; }
    
private:
    uint32_t stepCounter;
};
