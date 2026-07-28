#include "SolidColor.h"

SolidColorEffect::SolidColorEffect() : color(CRGB::Red) {}

void SolidColorEffect::start() {
    // Optionally load color from storage here
}

void SolidColorEffect::run() {
    Matrix.fill(color);
    Matrix.show();
}
