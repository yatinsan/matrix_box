#pragma once

#include "../Effect.h"

class TwinkleEffect : public Effect {
public:
    TwinkleEffect();
    
    void start() override;
    void run() override;
    
    const char* getName() const override { return "Star Twinkle"; }
};
