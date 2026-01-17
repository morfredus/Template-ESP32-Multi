
# User Guide (EN)

> **Minimum version:** 0.4.3

> **Full technical documentation for each module is available in [ModularDocs/README.md](../ModularDocs/README.md). All docs are beginner-friendly and include a dedicated section for newcomers.**

## Duplicate the template
- Copy the repository folder, then rename it.
- Update `PROJECT_NAME` and `PROJECT_VERSION` in `platformio.ini` only.
- Keep `.pio` out of version control (already in .gitignore).

## Configure
- Secrets: copy `include/secrets_example.h` to `include/secrets.h` and list SSID/password pairs.
- User options: edit `include/config.h` (mDNS host, LCD rotation, WiFi timeouts, NeoPixel toggle).
- Pin map: review `include/board_config.h` for each environment before wiring.

## Environments
- `esp32devkit`: classic ESP32 DevKit, no NeoPixel by default, LCD over VSPI (SCK 18, MOSI 23, CS 5, DC 16, RST 17, BL 4), buttons on 0/32/33.
- `esp32s3_n16r8`: ESP32-S3 DevKitC-1 N16R8, NeoPixel on GPIO48, LCD SPI on 12/11/15 with CS 10, DC 17, RST 18, BL 16, buttons on 0/16/17.

## Add modules
- Place common code in `src/` or `lib/`. Keep headers in `include/`.
- Add new libraries via `lib_deps` in `[env]` to avoid duplication across environments.
- When adding sensors, extend `board_config.h` with dedicated pins and document them in `docs` and README.

## Modify pin mapping
- `include/board_config.h` is the single source of truth. Update pins per environment, keep BOOT on GPIO0 and NeoPixel on GPIO48 for S3.
- Avoid S3 strapping pins 45/46 and ESP32 strapping pins 0/2/4/5/12/15 for outputs unless required.
- After changes, sync the documentation and wiring diagrams.

## WiFi behavior
- Uses `WiFiMulti`: all networks from `secrets.h` are tried in order.
- LCD boot screen shows progress; on success, SSID and IP appear on the main screen.
- mDNS hostname comes from `config.h` and is registered on port 80.

## LCD system
- ST7789 over SPI with Adafruit GFX. No full-screen clear loops at runtime; updates are targeted to limit flicker.
- Boot bar is enabled by default; disable via `enableBootBar` if needed.
- Backlight uses LEDC PWM with two levels:
  - `bootBacklightLevel` (default 77/255, ~30%): reduced brightness during boot to minimize current draw and prevent bootloops on weak USB ports
  - `backlightLevel` (default 255/255, 100%): normal operation brightness, reached progressively after boot via 500ms fade
- Progressive power management prevents inrush current spikes during initialization.

## Web UI
- Served by the built-in `WebServer` on port 80.
- Access via IP or `http://<mdns>.local/` when mDNS is active.
- Cards display CPU type, speed, heap/PSRAM usage, SSID, and IP. JSON status is available at `/status`.

## Project structure
- `platformio.ini`: environments, shared build flags, dependencies, project metadata.
- `include/`: configuration (`config.h`, `board_config.h`, secrets) and shared headers.
- `src/`: firmware entry point (`main.cpp`) and features.
- `docs/`: bilingual user documentation.
- `test/`, `lib/`: placeholders for unit tests and local libraries.

## Best practices
- Keep project name/version only in `platformio.ini`; use macros in code.
- Prefer platform libraries first; add third-party deps sparingly in `[env]`.
- Test both environments after changes; watch GPIO safety notes above.
- Commit updates with matching README/CHANGELOG updates for every version bump.
