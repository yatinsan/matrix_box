#include "Rainbow.h"

RainbowEffect::RainbowEffect() : hue(0), speed(5) {}

void RainbowEffect::start() {
    hue = 0;
}

void RainbowEffect::run() {
    fill_rainbow(Matrix.getLeds(), NUM_LEDS, hue, 255 / NUM_LEDS);
    Matrix.show();
    hue += speed;
}
