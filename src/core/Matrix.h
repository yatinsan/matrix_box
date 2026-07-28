#pragma once

#define FASTLED_ESP32_FLASH_LOCK 1
#include <FastLED.h>
#include "../Config.h"

enum class MatrixLayout {
    PROGRESSIVE,
    SERPENTINE
};

enum class MatrixRotation {
    ROT_0,
    ROT_90,
    ROT_180,
    ROT_270
};

class MatrixEngine {
public:
    MatrixEngine();
    
    void begin();
    void show();
    void clear();
    void fill(CRGB color);
    void setBrightness(uint8_t brightness);
    uint8_t getBrightness() const;
    
    // Pixel plotting
    void drawPixel(int16_t x, int16_t y, CRGB color);
    void drawPixel(uint16_t index, CRGB color);
    CRGB getPixel(int16_t x, int16_t y);
    
    // Config
    void setMirrorX(bool mirror);
    void setMirrorY(bool mirror);
    void setRotation(MatrixRotation rotation);
    void setLayout(MatrixLayout layout);

    // Helpers
    uint16_t XY(int16_t x, int16_t y);
    CRGB* getLeds();

    // Primitives
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB color);
    void drawRectangle(int16_t x, int16_t y, int16_t w, int16_t h, CRGB color);
    void drawCircle(int16_t x0, int16_t y0, int16_t r, CRGB color);
    void blur(uint8_t amount);
    void fade(uint8_t amount);

private:
    CRGB leds[NUM_LEDS];
    bool _mirrorX = false;
    bool _mirrorY = false;
    MatrixRotation _rotation = MatrixRotation::ROT_0;
    MatrixLayout _layout = MatrixLayout::SERPENTINE; // Common for WS2812B panels
    uint8_t _brightness = DEFAULT_BRIGHTNESS;
};

extern MatrixEngine Matrix;
