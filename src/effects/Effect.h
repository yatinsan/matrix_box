#pragma once

#include <Arduino.h>
#include "../core/Matrix.h"

class Effect {
public:
    virtual ~Effect() {}
    
    virtual void start() {}       // Called when effect becomes active
    virtual void run() = 0;       // Called every frame
    virtual void stop() {}        // Called when effect ends
    
    virtual const char* getName() const = 0;
    
    // Properties
    virtual bool supportsColor() const { return false; }
    virtual bool supportsSpeed() const { return true; }
    virtual bool supportsPalette() const { return false; }
};
