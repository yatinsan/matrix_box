#include "Plasma.h"

PlasmaEffect::PlasmaEffect() : stepCounter(0) {}

void PlasmaEffect::start() {
    stepCounter = 0;
}

void PlasmaEffect::run() {
    stepCounter += 2;
    float centerX = (MATRIX_WIDTH - 1) / 2.0f;
    float centerY = (MATRIX_HEIGHT - 1) / 2.0f;

    for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
        for (uint8_t y = 0; y < MATRIX_HEIGHT; y++) {
            float dx = x - centerX;
            float dy = y - centerY;
            float angle = atan2(dy, dx);
            float dist = sqrt(dx * dx + dy * dy);

            uint8_t angleByte = uint8_t((angle + PI) * (255.0f / (2.0f * PI)));
            uint8_t val = sin8(angleByte * 3 + uint8_t(dist * 15) - stepCounter * 4);
            uint8_t hue = angleByte + stepCounter;

            Matrix.drawPixel(x, y, CHSV(hue, 240, val));
        }
    }
    Matrix.show();
}
