#pragma once

#include "../Effect.h"

class RipplesEffect : public Effect {
public:
    RipplesEffect();
    
    void start() override;
    void run() override;
    
    const char* getName() const override { return "Color Ripples"; }
    
private:
    uint32_t stepCounter;
};
