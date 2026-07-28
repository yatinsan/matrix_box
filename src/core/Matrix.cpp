#include "Matrix.h"

MatrixEngine Matrix;

MatrixEngine::MatrixEngine() {
}

void MatrixEngine::begin() {
    FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS, MAX_MILLIAMPS);
    FastLED.setBrightness(_brightness);
    clear();
    show();
}

void MatrixEngine::show() {
    FastLED.show();
}

void MatrixEngine::clear() {
    FastLED.clear();
}

void MatrixEngine::fill(CRGB color) {
    fill_solid(leds, NUM_LEDS, color);
}

void MatrixEngine::setBrightness(uint8_t brightness) {
    _brightness = brightness;
    FastLED.setBrightness(_brightness);
}

uint8_t MatrixEngine::getBrightness() const {
    return _brightness;
}

CRGB* MatrixEngine::getLeds() {
    return leds;
}

uint16_t MatrixEngine::XY(int16_t x, int16_t y) {
    if (x < 0 || x >= MATRIX_WIDTH || y < 0 || y >= MATRIX_HEIGHT) {
        return NUM_LEDS; // Out of bounds mapping (requires careful bounds checking before using return)
    }

    // Apply rotation
    int16_t tempX = x;
    int16_t tempY = y;

    switch (_rotation) {
        case MatrixRotation::ROT_90:
            tempX = y;
            tempY = MATRIX_WIDTH - 1 - x;
            break;
        case MatrixRotation::ROT_180:
            tempX = MATRIX_WIDTH - 1 - x;
            tempY = MATRIX_HEIGHT - 1 - y;
            break;
        case MatrixRotation::ROT_270:
            tempX = MATRIX_HEIGHT - 1 - y;
            tempY = x;
            break;
        case MatrixRotation::ROT_0:
        default:
            break;
    }

    // Apply mirroring
    if (_mirrorX) tempX = MATRIX_WIDTH - 1 - tempX;
    if (_mirrorY) tempY = MATRIX_HEIGHT - 1 - tempY;

    uint16_t i;
    if (_layout == MatrixLayout::SERPENTINE) {
        if (tempY & 0x01) {
            // Odd rows run backwards
            uint8_t reverseX = (MATRIX_WIDTH - 1) - tempX;
            i = (tempY * MATRIX_WIDTH) + reverseX;
        } else {
            // Even rows run forwards
            i = (tempY * MATRIX_WIDTH) + tempX;
        }
    } else {
        // PROGRESSIVE
        i = (tempY * MATRIX_WIDTH) + tempX;
    }
    
    return i;
}

void MatrixEngine::drawPixel(int16_t x, int16_t y, CRGB color) {
    uint16_t i = XY(x, y);
    if (i < NUM_LEDS) {
        leds[i] = color;
    }
}

void MatrixEngine::drawPixel(uint16_t index, CRGB color) {
    if (index < NUM_LEDS) {
        leds[index] = color;
    }
}

CRGB MatrixEngine::getPixel(int16_t x, int16_t y) {
    uint16_t i = XY(x, y);
    if (i < NUM_LEDS) {
        return leds[i];
    }
    return CRGB::Black;
}

void MatrixEngine::setMirrorX(bool mirror) { _mirrorX = mirror; }
void MatrixEngine::setMirrorY(bool mirror) { _mirrorY = mirror; }
void MatrixEngine::setRotation(MatrixRotation rotation) { _rotation = rotation; }
void MatrixEngine::setLayout(MatrixLayout layout) { _layout = layout; }

// Bresenham's line algorithm
void MatrixEngine::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB color) {
    int16_t dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int16_t dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int16_t err = dx + dy, e2;

    for (;;) {
        drawPixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void MatrixEngine::drawRectangle(int16_t x, int16_t y, int16_t w, int16_t h, CRGB color) {
    drawLine(x, y, x + w - 1, y, color);
    drawLine(x, y + h - 1, x + w - 1, y + h - 1, color);
    drawLine(x, y, x, y + h - 1, color);
    drawLine(x + w - 1, y, x + w - 1, y + h - 1, color);
}

void MatrixEngine::drawCircle(int16_t x0, int16_t y0, int16_t r, CRGB color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    drawPixel(x0, y0 + r, color);
    drawPixel(x0, y0 - r, color);
    drawPixel(x0 + r, y0, color);
    drawPixel(x0 - r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        drawPixel(x0 + x, y0 + y, color);
        drawPixel(x0 - x, y0 + y, color);
        drawPixel(x0 + x, y0 - y, color);
        drawPixel(x0 - x, y0 - y, color);
        drawPixel(x0 + y, y0 + x, color);
        drawPixel(x0 - y, y0 + x, color);
        drawPixel(x0 + y, y0 - x, color);
        drawPixel(x0 - y, y0 - x, color);
    }
}

void MatrixEngine::blur(uint8_t amount) {
    blur2d(leds, MATRIX_WIDTH, MATRIX_HEIGHT, amount);
}

void MatrixEngine::fade(uint8_t amount) {
    fadeToBlackBy(leds, NUM_LEDS, amount);
}
