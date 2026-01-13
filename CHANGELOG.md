# Changelog

## [0.2.0] - 2026-01-13
- Rewrite board_config.h with comprehensive beginner-friendly comments
- Add educational documentation for GPIO declarations including:
  - Required voltages for each peripheral
  - Resistor and capacitor values with power ratings
  - Common module pin label aliases (MISO/SDA, CLK/SCLK, etc.)
  - Wiring diagrams in ASCII art
  - Safety rules and electrical concepts
- Update PIN_MAPPING.md with detailed component requirements
- Update PIN_MAPPING_FR.md with detailed component requirements
## [0.1.2] - 2026-01-13
- Enable PSRAM on ESP32-S3 N16R8 via build flags and OPI PSRAM setting

## [0.1.1] - 2026-01-13
- Force PSRAM initialization on ESP32-S3 N16R8 and log status at boot

## [0.1.0] - 2026-01-12
- Add multi-environment template for ESP32 DevKit and ESP32-S3 DevKitC-1
- Implement WiFiMulti onboarding with LCD boot progress
- Add ST7789 display status screen using project metadata from platformio.ini
- Provide optional NeoPixel feedback on ESP32-S3 (GPIO48)
- Ship web UI with system stats and JSON status endpoint
- Centralize settings in config.h and pin mapping in board_config.h
- Add bilingual documentation and secrets example
