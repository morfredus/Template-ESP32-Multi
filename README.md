<p align="center">
  <img src="https://github.com/user-attachments/assets/99f9b4e0-36b0-408a-a19b-68bbb6272378" alt="Template ESP32 Multi" width="800">
</p>

# ESP32 Multi-Environment Template

Beginner-friendly PlatformIO template for ESP32 DevKit and ESP32-S3 DevKitC. It centralizes configuration, WiFi onboarding, LCD feedback, buttons, optional NeoPixel, and a built-in web UI.

## Environments
- `esp32devkit` (ESP32 DevKit)
- `esp32s3_n16r8` (ESP32-S3 DevKitC-1 N16R8)

## Quick start
1. Copy this folder to create a new project.
2. Duplicate `include/secrets_example.h` to `include/secrets.h`, then list your WiFi networks.
3. Adjust user settings in `include/config.h` (mDNS name, screen rotation, timers).
4. Verify pin mapping in `include/board_config.h` matches your hardware.
5. Build and flash with `pio run -t upload` (choose `esp32devkit` or `esp32s3_n16r8`).
6. Open the serial monitor at 115200 baud.

## What’s inside
- WiFiMulti onboarding with LCD boot bar and IP display
- LCD status screen (project name/version from platformio.ini)
- mDNS responder for easy access by host name
- Simple web UI showing CPU, memory, WiFi, and IP
- Button handling with long-press reboot on BOOT (GPIO0)
- Optional NeoPixel feedback on ESP32-S3 (GPIO48)
- Centralized config and pin mapping

## Configuration files
- `include/config.h` — user-tunable options (mDNS host, timeouts, display defaults)
- `include/board_config.h` — pin map per environment
- `include/secrets.h` — WiFi credentials (kept out of VCS); see `include/secrets_example.h`

## Display & Web UI
- LCD uses Adafruit ST7789 + GFX over hardware SPI. Keep the pin map and rotation consistent.
- Web UI available at `http://<device-ip>/` and `http://<mdns>.local/` when mDNS is on.

## Buttons
- BOOT (GPIO0) long press shows a reboot bar; release early to cancel.
- Two extra user buttons are exposed per board in `board_config.h`.

## NeoPixel (ESP32-S3 only)
- GPIO48 is reserved for the NeoPixel strip (default one LED). Colors indicate WiFi status.

## Project metadata
Project name and version live in `platformio.ini` build flags (`PROJECT_NAME`, `PROJECT_VERSION`). Never hard-code them in source files.

## License
Template code is provided for reuse inside your projects. Add your license when duplicating.
