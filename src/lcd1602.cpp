
#include <LiquidCrystal_PCF8574.h>
#include "lcd1602.h"

namespace {
  LiquidCrystal_PCF8574 lcd(0x27);
  bool lcdInitialized = false;
}

namespace Lcd1602 {

void begin(uint8_t addr, uint8_t cols, uint8_t rows) {
  if (!lcdInitialized) {
    lcd = LiquidCrystal_PCF8574(addr);
    lcd.begin(cols, rows);
    lcd.setBacklight(255);
    lcd.clear();
    lcdInitialized = true;
  }
}

void clear() {
  if (lcdInitialized) lcd.clear();
}

void print(const char* text, uint8_t col, uint8_t row) {
  if (!lcdInitialized) return;
  lcd.setCursor(col, row);
  lcd.print(text);
}


void setBacklight(bool on) {
  if (lcdInitialized) lcd.setBacklight(on ? 255 : 0);
}

void scrollText(const char* text, uint8_t row, uint16_t delayMs) {
  if (!lcdInitialized) return;
  const uint8_t width = 16;
  String msg(text);
  if (msg.length() <= width) {
    print(text, 0, row);
    delay(2 * delayMs);
    return;
  }
  msg = msg + "   "; // espace après le texte
  for (uint8_t i = 0; i <= msg.length() - width; ++i) {
    lcd.setCursor(0, row);
    lcd.print(msg.substring(i, i + width));
    delay(delayMs);
  }
}

} // namespace Lcd1602
