#include "buttons.h"
#include "board_config.h"
#include "config.h"
#include "tft_display.h"
#include "neopixel.h"
#include "ui_state.h"
#include <Arduino.h>

namespace Buttons {

void init() {
  pinMode(BUTTON_BOOT_PIN, INPUT_PULLUP);
  pinMode(BUTTON_USER1_PIN, INPUT_PULLUP);
  pinMode(BUTTON_USER2_PIN, INPUT_PULLUP);
}

void handle() {
  UiState::State& ui = UiState::getState();
  const bool bootPressed = digitalRead(BUTTON_BOOT_PIN) == LOW;
  
  if (bootPressed && !ui.rebootPrompt) {
    ui.rebootPrompt = true;
    ui.rebootHoldStart = millis();
    TftDisplay::drawRebootPrompt(0);
  }

  if (ui.rebootPrompt) {
    const uint32_t elapsed = millis() - ui.rebootHoldStart;
    const uint32_t requiredMs = static_cast<uint32_t>(kTemplateConfig.rebootHoldSeconds) * 1000UL;
    uint8_t percent = elapsed >= requiredMs ? 100 : static_cast<uint8_t>((elapsed * 100) / requiredMs);
    TftDisplay::drawRebootPrompt(percent);
    
    if (!bootPressed) {
      ui.rebootPrompt = false;
      TftDisplay::updateMainScreen(true);
      return;
    }
    
    if (percent >= 100) {
      #if defined(ENV_ESP32S3_N16R8)
      NeoPixel::setColor(NeoPixel::makeColor(0, 0, 255));  // Blue LED
      #endif
      ESP.restart();
    }
  }
}

}  // namespace Buttons
