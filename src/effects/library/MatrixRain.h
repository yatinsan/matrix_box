#pragma once

#include "../Effect.h"

class MatrixRainEffect : public Effect {
public:
    MatrixRainEffect();
    
    void start() override;
    void run() override;
    
    const char* getName() const override { return "Matrix Rain"; }
    
private:
    int8_t rainHead[MATRIX_WIDTH];
    uint8_t rainSpeed[MATRIX_WIDTH];
    uint8_t rainCounter[MATRIX_WIDTH];
};
