#include "Twinkle.h"

TwinkleEffect::TwinkleEffect() {}

void TwinkleEffect::start() {}

void TwinkleEffect::run() {
    Matrix.fade(25); // Fade previous frame

    if (random8() < 90) {
        uint8_t rx = random8(MATRIX_WIDTH);
        uint8_t ry = random8(MATRIX_HEIGHT);
        uint8_t hue = random8();
        Matrix.drawPixel(rx, ry, CHSV(hue, random8(120, 255), 255));
    }

    Matrix.show();
}
