#include "Ripples.h"

RipplesEffect::RipplesEffect() : stepCounter(0) {}

void RipplesEffect::start() {
    stepCounter = 0;
}

void RipplesEffect::run() {
    stepCounter += 3;
    float centerX = (MATRIX_WIDTH - 1) / 2.0f;
    float centerY = (MATRIX_HEIGHT - 1) / 2.0f;

    for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
        for (uint8_t y = 0; y < MATRIX_HEIGHT; y++) {
            float dx = x - centerX;
            float dy = y - centerY;
            float dist = sqrt(dx * dx + dy * dy);

            uint8_t wave = sin8(uint8_t(dist * 28 - stepCounter * 4));
            uint8_t hue = uint8_t(dist * 12 + stepCounter);
            Matrix.drawPixel(x, y, CHSV(hue, 220, wave));
        }
    }
    Matrix.show();
}
