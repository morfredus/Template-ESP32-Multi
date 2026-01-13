# Changelog

## [0.3.1] - 2026-01-13
- Fix mDNS initialization to only run when WiFi connection is successful
- Skip network services (mDNS, OTA, web server) when WiFi fails to connect
- Add consistent logging message when network services are disabled

## [0.3.0] - 2026-01-13
- Complete code refactoring for modularity and maintainability
- Separate codebase into functional modules:
  - `tft_display.h/cpp`: LCD ST7789 display and rendering functions
  - `web_ui.h/cpp`: HTML page builders and JSON response generation
  - `web_styles.h/cpp`: CSS styles and JavaScript for web UI
  - `web_server.h/cpp`: HTTP server setup and route handling
  - `network.h/cpp`: WiFi, mDNS, and OTA connectivity
  - `neopixel.h/cpp`: NeoPixel LED strip management
  - `buttons.h/cpp`: Button input and reboot logic
  - `ui_state.h/cpp`: Global UI state management
  - `psram.h/cpp`: PSRAM initialization
- Simplify main.cpp to use modular components
- Improve code organization and reduce main.cpp from 474 to ~70 lines
- All modules use namespaces for better organization
- All modules are independent and can be tested separately
- Enhance documentation with function headers and parameter descriptions

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
