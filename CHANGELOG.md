

# Changelog

## [0.4.3] - 2026-01-17
### Documentation
- Full migration to ModularDocs/ (bilingual, modular, beginner-friendly)
- All project docs updated to reference ModularDocs
- Tech/ folder removed
- Version bump in platformio.ini

> **All technical documentation is now modular, bilingual, and beginner-friendly. See [ModularDocs/README.md](ModularDocs/README.md) for details.**

## [0.4.2] - 2026-01-17
### Added
- Progressive power management during boot sequence to prevent bootloop on weak USB ports
- New configuration parameter `bootBacklightLevel` to reduce display backlight during boot
- Function `fadeBacklightToNormal()` to gradually increase backlight after boot completion

### Changed
- Display backlight now starts at ~30% during boot (77/255) instead of 100%
- Backlight gradually fades to full brightness over 500ms after boot complete
- NeoPixel LED now starts off (black) during boot instead of soft blue
- Reduced current draw during initialization phase to improve stability on low-power USB ports

### Fixed
- Bootloop issue on computers with insufficient USB port power delivery
- Inrush current spikes during simultaneous peripheral initialization

## [0.4.1] - 2026-01-17
### Changed
- Version bump for release after documentation corrections on pin mapping
- Synced dev/maint into main for publication

### Documentation
- No content changes since 0.4.0; documents remain valid from 0.4.0

## [0.4.0] - 2026-01-17
### Changed
- Updated pin mapping for both development environments for optimal performance
- ESP32 DevKit: No changes (verified stable configuration)
- ESP32-S3 N16R8: Updated display interface pins
  - Display DC: GPIO 17 (from GPIO 9)
  - Display RST: GPIO 18 (from GPIO 8)
  - Display BL: GPIO 16 (from GPIO 7)
  - Display CS: GPIO 10, MOSI: GPIO 11, SCK: GPIO 12, MISO: GPIO 15
- All GPIO assignments optimized to avoid PSRAM conflicts on N16R8

### Documentation
- Updated PIN_MAPPING.md with corrected ESP32-S3 table and wiring diagram
- Updated PIN_MAPPING_FR.md with corrected ESP32-S3 table and wiring diagram
- Updated USER_GUIDE.md with accurate GPIO references for ESP32-S3
- Updated USER_GUIDE_FR.md with accurate GPIO references for ESP32-S3
- Updated MODULAR_ARCHITECTURE.md version number to 0.4.0
- Updated MODULAR_ARCHITECTURE_FR.md version number to 0.4.0
- All documentation now reflects actual board_config.h GPIO definitions

## [0.3.4] - 2026-01-13
### Fixed
- mDNS status incorrectly displayed as "disabled" on LCD despite service running
- Add dynamic mDNS state checking instead of relying on boot-time cached value
- Implement `Network::isMdnsRunning()` to query actual mDNS service state

### Added
- New function `Network::isMdnsRunning()` - queries real mDNS state at runtime
- Dynamic status update in display loop - mDNS status now reflects actual service state
- Add network.h include to tft_display.cpp for isMdnsRunning() call

### Changed
- TFT display mDNS status now checks `Network::isMdnsRunning()` instead of cached `g_uiState.mdnsOk`
- Hostname display updates dynamically as network conditions change

### Technical
- Root cause: Status was set only at boot, never updated in main display loop
- Solution: Query actual mDNS state on each display update cycle
- Behavior: Display shows hostname when mDNS is running, "disabled" otherwise
### Fixed
- mDNS service initialization failing due to insufficient WiFi stabilization time
- Add WiFi connection verification before starting mDNS
- Add 100ms delay to ensure ESP32 WiFi stack is fully initialized
- Improve error logging for mDNS initialization failures

### Changed
- Enhanced `Network::initMdns()` with stability checks and debug output
- Serial output now displays mDNS hostname when successfully initialized

### Verified
- mDNS service now reliably starts after WiFi connection
- Device discoverable as `[hostname].local` on network
- No regression in other network services (OTA, WebServer)
### Fixed
- Linking error caused by multiple definitions of `WIFI_NETWORKS` in secrets.h
- Create security wrapper `secrets_wrapper.h/.cpp` to isolate secrets.h inclusion
- Adapt network.cpp to use wrapper API (getWifiSsid, getWifiPassword, getWifiNetworksCount)
- Remove unused secrets.h include from main.cpp

### Added
- New file `include/secrets_wrapper.h` - Secure credential access interface
- New file `src/secrets_wrapper.cpp` - Wrapper implementation (only file including secrets.h)
- Complete patch documentation in `docs/PATCH_001_SECRETS_WRAPPER.md` and `docs/PATCH_001_SECRETS_WRAPPER_FR.md`

### Security
- Credentials remain centralized only in secrets.h
- Index validation to prevent buffer overflow
- Architecture ensuring only one compilation unit includes secrets.h

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
