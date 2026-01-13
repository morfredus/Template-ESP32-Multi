#pragma once

#include <Arduino.h>

namespace NeoPixel {

/**
 * Initialize NeoPixel LED strip (if available).
 * Only compiled for ENV_ESP32S3_N16R8.
 */
void init();

/**
 * Set all pixels to a specific color and show.
 * Color format: 0x00RRGGBB
 * @param color RGB color value
 */
void setColor(uint32_t color);

/**
 * Get a color value from RGB components.
 * @param r Red component (0-255)
 * @param g Green component (0-255)
 * @param b Blue component (0-255)
 * @return 32-bit color value
 */
uint32_t makeColor(uint8_t r, uint8_t g, uint8_t b);

/**
 * Set brightness level for all pixels.
 * @param brightness Brightness value (0-255)
 */
void setBrightness(uint8_t brightness);

/**
 * Turn off all pixels.
 */
void clear();

}  // namespace NeoPixel
