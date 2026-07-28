#include "MatrixRain.h"

MatrixRainEffect::MatrixRainEffect() {}

void MatrixRainEffect::start() {
    for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
        rainHead[x] = random8(MATRIX_HEIGHT);
        rainSpeed[x] = random8(1, 4);
        rainCounter[x] = 0;
    }
}

void MatrixRainEffect::run() {
    Matrix.fade(75); // Fade trail

    for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
        rainCounter[x]++;
        if (rainCounter[x] >= rainSpeed[x]) {
            rainCounter[x] = 0;
            rainHead[x]++;

            if (rainHead[x] >= MATRIX_HEIGHT + 4) {
                rainHead[x] = -random8(1, 5);
                rainSpeed[x] = random8(1, 4);
            }
        }

        int8_t headY = rainHead[x];
        if (headY >= 0 && headY < MATRIX_HEIGHT) {
            Matrix.drawPixel(x, headY, CRGB(180, 255, 180));
        }
        if (headY - 1 >= 0 && headY - 1 < MATRIX_HEIGHT) {
            Matrix.drawPixel(x, headY - 1, CRGB(0, 255, 60));
        }
        if (headY - 2 >= 0 && headY - 2 < MATRIX_HEIGHT) {
            Matrix.drawPixel(x, headY - 2, CRGB(0, 160, 30));
        }
    }

    Matrix.show();
}
