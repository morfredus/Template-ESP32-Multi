#include "tft_display.h"
#include "config.h"
#include "board_config.h"
#include "ui_state.h"
#include <Arduino.h>
#include <WiFi.h>
#include <SPI.h>

// Global TFT object - initialized in main.cpp
Adafruit_ST7789 tft(DISPLAY_CS_PIN, DISPLAY_DC_PIN, DISPLAY_RST_PIN);

namespace TftDisplay {

void initBacklight() {
  if (DISPLAY_BL_PIN == 255) {
    return;
  }
  ledcSetup(kLedcChannel, 5000, kLedcResolution);
  ledcAttachPin(DISPLAY_BL_PIN, kLedcChannel);
  ledcWrite(kLedcChannel, kTemplateConfig.backlightLevel);
}

void setupDisplay() {
  Serial.println("[DISPLAY] Starting SPI with low frequency...");
  // Use low frequency at startup (20 MHz instead of 27 MHz)
  SPI.begin(DISPLAY_SCK_PIN, DISPLAY_MISO_PIN, DISPLAY_MOSI_PIN);
  SPI.setFrequency(20000000);  // 20 MHz
  
  Serial.println("[DISPLAY] Initializing ST7789 1.54\"...");
  delay(100);
  tft.init(DISPLAY_WIDTH, DISPLAY_HEIGHT);
  delay(100);
  
  Serial.println("[DISPLAY] Setting rotation...");
  tft.setRotation(kTemplateConfig.lcdRotation);
  delay(100);
  
  Serial.println("[DISPLAY] Initializing backlight...");
  initBacklight();
  
  // Simple test with delay
  Serial.println("[DISPLAY] Test: filling RED...");
  tft.fillScreen(ST77XX_RED);
  delay(1500);
  
  Serial.println("[DISPLAY] Drawing boot screen...");
  drawBootScreen("Boot", 5);
  Serial.println("[DISPLAY] Setup complete!");
}

uint16_t printWrap(const String& text, uint8_t textSize, uint16_t x, uint16_t yStart, uint16_t maxWidthPx, uint8_t maxLines) {
  const uint16_t charWidth = 6 * textSize;
  const uint16_t charHeight = 8 * textSize;
  const uint16_t maxChars = maxWidthPx / charWidth;
  if (maxChars == 0 || maxLines == 0) return yStart;

  String remaining = text;
  uint16_t y = yStart;
  for (uint8_t line = 0; line < maxLines && remaining.length() > 0; ++line) {
    String slice;
    if (remaining.length() <= maxChars) {
      slice = remaining;
      remaining = "";
    } else {
      slice = remaining.substring(0, maxChars);
      remaining = remaining.substring(maxChars);
      // If this is the last allowed line and there is still text, add ellipsis
      if (line + 1 == maxLines) {
        if (slice.length() >= 3) {
          slice = slice.substring(0, slice.length() - 3) + "...";
        }
        remaining = "";
      }
    }
    tft.setCursor(x, y);
    tft.print(slice);
    y += charHeight;
  }
  return y;
}

String fitText(const String& text, uint8_t textSize, uint16_t maxWidthPx) {
  const uint16_t charWidth = 6 * textSize;  // default Adafruit_GFX font width
  const uint16_t maxChars = maxWidthPx / charWidth;
  if (maxChars == 0) return "";
  if (text.length() <= maxChars) return text;
  if (maxChars <= 3) return text.substring(0, maxChars);  // not enough room for ellipsis
  return text.substring(0, maxChars - 3) + "...";  // ASCII ellipsis to avoid unsupported glyphs
}

void printFit(uint16_t x, uint16_t y, const String& text, uint8_t textSize, uint16_t maxWidthPx) {
  tft.setTextSize(textSize);
  tft.setCursor(x, y);
  tft.print(fitText(text, textSize, maxWidthPx));
}

void drawBootScreen(const char* message, uint8_t percent) {
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  const uint16_t lineWidth = DISPLAY_WIDTH - (2 * kTextMarginPx);
  printFit(kTextMarginPx, 20, projectName(), 2, lineWidth);
  tft.setTextSize(2);
  tft.setCursor(kTextMarginPx, 50);
  tft.print("v");
  tft.print(projectVersion());
  printFit(kTextMarginPx, 90, message, 2, lineWidth);
  if (kTemplateConfig.enableBootBar) {
    uint16_t barWidth = DISPLAY_WIDTH - (2 * kBootBarMargin);
    uint16_t filled = (percent * barWidth) / 100;
    uint16_t y = DISPLAY_HEIGHT - 60;
    tft.drawRect(kBootBarMargin, y, barWidth, 20, ST77XX_WHITE);
    tft.fillRect(kBootBarMargin + 2, y + 2, filled > 4 ? filled - 4 : 0, 16, ST77XX_GREEN);
  }
}

void updateMainScreen(bool force) {
  const bool wifiNow = WiFi.isConnected();
  const String ssidNow = wifiNow ? WiFi.SSID() : "Offline";
  const IPAddress ipNow = wifiNow ? WiFi.localIP() : IPAddress();
  
  // Access UI state
  UiState::State& ui = UiState::getState();
  
  if (!force && wifiNow == ui.wifiConnected && ssidNow == ui.currentSsid && ipNow == ui.currentIp) {
    return;
  }
  ui.wifiConnected = wifiNow;
  ui.currentSsid = ssidNow;
  ui.currentIp = ipNow;

  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  const uint16_t lineWidth = DISPLAY_WIDTH - (2 * kTextMarginPx);

  // Header
  printFit(kTextMarginPx, 20, projectName(), 2, lineWidth);
  tft.setTextSize(2);
  tft.setCursor(kTextMarginPx, 40);
  tft.print("v");
  tft.print(projectVersion());

  // Details (smaller font, wrapped)
  uint16_t y = 80;
  tft.setTextSize(2);
  tft.setCursor(kTextMarginPx, y);
  tft.print("SSID:");
  y += 20;
  y = printWrap(ssidNow, 1, kTextMarginPx, y, lineWidth);
  y += 20;

  tft.setCursor(kTextMarginPx, y);
  tft.print("IP:");
  y += 20;
  y = printWrap(wifiNow ? ipNow.toString() : String("0.0.0.0"), 1, kTextMarginPx, y, lineWidth);
  y += 20;

  tft.setCursor(kTextMarginPx, y);
  tft.print("mDNS:");
  y += 20;
  y = printWrap(ui.mdnsOk ? String(kTemplateConfig.mdnsHost) : String("disabled"), 1, kTextMarginPx, y, lineWidth);
}

void drawRebootPrompt(uint8_t percent) {
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(2);
  tft.setCursor(10, 40);
  tft.print("Reboot ?");
  uint16_t barWidth = DISPLAY_WIDTH - (2 * kBootBarMargin);
  uint16_t filled = (percent * barWidth) / 100;
  uint16_t y = DISPLAY_HEIGHT / 2;
  tft.drawRect(kBootBarMargin, y, barWidth, 20, ST77XX_WHITE);
  tft.fillRect(kBootBarMargin + 2, y + 2, filled > 4 ? filled - 4 : 0, 16, ST77XX_ORANGE);
}

}  // namespace TftDisplay
