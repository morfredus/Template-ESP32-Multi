#pragma once

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include "config.h"

// Forward declaration of tft object
extern Adafruit_ST7789 tft;

namespace TftDisplay {

// Constants for display rendering
constexpr uint8_t kTextMarginPx = 10;  // left/right padding for LCD text
constexpr uint8_t kBootBarMargin = 20;
constexpr uint8_t kLedcChannel = 5;
constexpr uint8_t kLedcResolution = 8;

/**
 * Initialize the backlight with PWM control.
 * If DISPLAY_BL_PIN is 255, skips initialization.
 */
void initBacklight();

/**
 * Initialize SPI and ST7789 display.
 * Sets up the display with proper rotation and performs test sequence.
 */
void setupDisplay();

/**
 * Draw boot screen with message and progress bar.
 * @param message Message to display
 * @param percent Progress bar percentage (0-100)
 */
void drawBootScreen(const char* message, uint8_t percent);

/**
 * Update the main information screen showing WiFi and system status.
 * @param force Force redraw even if nothing changed
 */
void updateMainScreen(bool force = false);

/**
 * Draw reboot confirmation prompt with progress bar.
 * @param percent Progress bar percentage (0-100)
 */
void drawRebootPrompt(uint8_t percent);

/**
 * Print text wrapped within given pixel width and line count.
 * @param text Text to print
 * @param textSize Adafruit text size (1-n)
 * @param x X coordinate
 * @param yStart Starting Y coordinate
 * @param maxWidthPx Maximum width in pixels
 * @param maxLines Maximum number of lines (default 2)
 * @return Y coordinate after last printed line
 */
uint16_t printWrap(const String& text, uint8_t textSize, uint16_t x, uint16_t yStart, uint16_t maxWidthPx, uint8_t maxLines = 2);

/**
 * Truncate text to fit within given pixel width.
 * Adds ellipsis if text is truncated.
 * @param text Text to fit
 * @param textSize Adafruit text size (1-n)
 * @param maxWidthPx Maximum width in pixels
 * @return Fitted text
 */
String fitText(const String& text, uint8_t textSize, uint16_t maxWidthPx);

/**
 * Print fitted text at specified position.
 * @param x X coordinate
 * @param y Y coordinate
 * @param text Text to print
 * @param textSize Adafruit text size (1-n)
 * @param maxWidthPx Maximum width in pixels
 */
void printFit(uint16_t x, uint16_t y, const String& text, uint8_t textSize, uint16_t maxWidthPx);

}  // namespace TftDisplay
