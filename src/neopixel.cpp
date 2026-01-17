#include "neopixel.h"
#include "config.h"
#include "board_config.h"

#if defined(ENV_ESP32S3_N16R8)
#include <Adafruit_NeoPixel.h>

static Adafruit_NeoPixel pixel(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

namespace NeoPixel {

void init() {
  pixel.begin();
  pixel.clear();
  pixel.setBrightness(NEOPIXEL_BRIGHTNESS);
  setColor(makeColor(0, 0, 0));  // Off during boot to save power
}

void setColor(uint32_t color) {
  for (uint16_t i = 0; i < NEOPIXEL_COUNT; ++i) {
    pixel.setPixelColor(i, color);
  }
  pixel.show();
}

uint32_t makeColor(uint8_t r, uint8_t g, uint8_t b) {
  return pixel.Color(r, g, b);
}

void setBrightness(uint8_t brightness) {
  pixel.setBrightness(brightness);
  pixel.show();
}

void clear() {
  pixel.clear();
  pixel.show();
}

}  // namespace NeoPixel

#else

// Dummy implementations for boards without NeoPixel
namespace NeoPixel {

void init() {
  // NeoPixel not available on this board
}

void setColor(uint32_t color) {
  // NeoPixel not available on this board
}

uint32_t makeColor(uint8_t r, uint8_t g, uint8_t b) {
  return 0;
}

void setBrightness(uint8_t brightness) {
  // NeoPixel not available on this board
}

void clear() {
  // NeoPixel not available on this board
}

}  // namespace NeoPixel

#endif
