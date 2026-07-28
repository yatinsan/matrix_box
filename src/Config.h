#pragma once

#include <Arduino.h>

// Matrix Configuration
#define MATRIX_WIDTH 17
#define MATRIX_HEIGHT 17
#define NUM_LEDS (MATRIX_WIDTH * MATRIX_HEIGHT)
#define LED_PIN 4  // Can be changed later or configured via web
#define COLOR_ORDER GRB
#define CHIPSET WS2812B
#define MAX_BRIGHTNESS 255
#define DEFAULT_BRIGHTNESS 64

// Power Configuration
#define VOLTS 5
#define MAX_MILLIAMPS 4000 // 4 Amps limit

// WiFi Configuration (Fallback AP)
#define AP_SSID "MatrixOS-17x17"
#define AP_PASSWORD "" // Open network for captive portal

// Buttons
#define BUTTON_PIN 0 // BOOT button on most ESP32-S3 boards

// System
#define TARGET_FPS 60
