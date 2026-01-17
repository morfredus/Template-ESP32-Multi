#pragma once
#include <Arduino.h>

// Centralized user-facing settings for the template.
struct TemplateConfig {
    const char* mdnsHost;
    uint8_t lcdRotation;
    uint8_t backlightLevel;   // 0-255 (PWM duty) - normal operation
    uint8_t bootBacklightLevel; // 0-255 (PWM duty) - during boot to reduce current draw
    uint32_t wifiConnectTimeoutMs;
    uint32_t wifiRetryDelayMs;
    uint8_t maxWifiAttempts;
    uint8_t rebootHoldSeconds;
    bool enableBootBar;
    bool enableWebUi;
    bool enableNeoPixel;
};

constexpr TemplateConfig kTemplateConfig{
    .mdnsHost = "esp32-template",
    .lcdRotation = 2,               // Landscape by default
    .backlightLevel = 255,          // Full brightness (normal operation)
    .bootBacklightLevel = 77,       // ~30% brightness during boot (reduces USB current draw)
    .wifiConnectTimeoutMs = 12000,  // Per attempt
    .wifiRetryDelayMs = 500,        // Between WiFiMulti polls
    .maxWifiAttempts = 10,
    .rebootHoldSeconds = 3,
    .enableBootBar = true,
    .enableWebUi = true,
    .enableNeoPixel = true
};

inline const char* projectName() { return PROJECT_NAME; }
inline const char* projectVersion() { return PROJECT_VERSION; }
