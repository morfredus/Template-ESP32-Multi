# Project Structure (EN)

## File Tree
```
00-Template-ESP32-Multi/
├── platformio.ini              # Project config, environments, version
├── .gitignore                  # Git ignore (includes secrets.h)
├── .copilot                    # Copilot permanent instructions
│
├── README.md                   # Quick overview (EN)
├── README_FR.md                # Quick overview (FR)
├── CHANGELOG.md                # Version history (EN)
├── CHANGELOG_FR.md             # Version history (FR)
│
├── docs/
│   ├── GETTING_STARTED.md      # Step-by-step first use (EN)
│   ├── GETTING_STARTED_FR.md   # Step-by-step first use (FR)
│   ├── PIN_MAPPING.md          # GPIO reference (EN)
│   ├── PIN_MAPPING_FR.md       # GPIO reference (FR)
│   ├── LCD_SYSTEM.md           # Display customization (EN)
│   ├── LCD_SYSTEM_FR.md        # Display customization (FR)
│   ├── WEB_UI.md               # Web server & API (EN)
│   ├── WEB_UI_FR.md            # Web server & API (FR)
│   ├── ADD_MODULES.md          # Sensor integration (EN)
│   ├── ADD_MODULES_FR.md       # Sensor integration (FR)
│   ├── USER_GUIDE.md           # General guide (EN)
│   ├── USER_GUIDE_FR.md        # General guide (FR)
│   └── [other guides]
│
├── include/
│   ├── config.h                # User-tunable settings
│   ├── board_config.h          # GPIO definitions (environment-specific)
│   ├── secrets.h               # WiFi credentials (user-created, ignored by git)
│   ├── secrets_example.h       # WiFi template (do not edit)
│   └── [other headers]
│
├── src/
│   ├── main.cpp                # Entry point (setup, loop)
│   └── [feature modules]       # Additional .cpp files for features
│
├── lib/
│   └── README                  # Local library placeholder
│
├── test/
│   └── README                  # Unit tests placeholder
│
├── .pio/                       # PlatformIO build output (git-ignored)
└── .vscode/                    # VS Code settings (git-ignored)
```

## Key Files Explained

### platformio.ini
Defines:
- **Environments** (`esp32devkit`, `esp32s3_n16r8`)
- **Project metadata** (`PROJECT_NAME`, `PROJECT_VERSION`)
- **Shared build flags** (optimization, compiler flags)
- **Libraries** (`lib_deps` for Adafruit, etc.)
- **Upload/monitor settings**

When adding new environments or libraries, update here.

### include/board_config.h
**DO NOT EDIT** unless adding new GPIO definitions. Contains:
- Button pin definitions (BOOT, USER1, USER2)
- Display pin definitions (SPI: SCK, MOSI, MISO, CS, DC, RST, BL)
- NeoPixel pin (S3 only, GPIO48)
- Environment-specific configurations

All pin references in code come from this file. Update documentation after changes.

### include/config.h
**User-editable**. Centralized settings:
- mDNS hostname (`mdnsHost`)
- LCD rotation, backlight level
- WiFi timeouts, connection attempts
- Boot bar visibility
- Web UI on/off
- Feature toggles

Edit here for project-specific customization without touching firmware code.

### include/secrets.h
**Git-ignored** (see `.gitignore`). User creates by duplicating `secrets_example.h`.
Contains WiFi networks (SSID/password pairs). Never commit this file.

### src/main.cpp
**Entry point**. Contains:
- Global object initialization (WiFiMulti, WebServer, display, NeoPixel)
- `setup()` — called once at boot (hardware init, WiFi connect, web server start)
- `loop()` — called repeatedly (~1000 times/sec)
- Helper functions (boot screen, status screen, button handling)

Keep this focused on framework setup. Put feature code in separate `.cpp` files.

### docs/
Complete bilingual documentation:
- **GETTING_STARTED** — First-time setup
- **PIN_MAPPING** — GPIO reference
- **LCD_SYSTEM** — Display customization
- **WEB_UI** — Server, API, networking
- **ADD_MODULES** — Adding sensors and features
- **USER_GUIDE** — General overview

All guides exist in English (`*.md`) and French (`*_FR.md`).

## Typical Workflow

1. **Duplicate this template** to start a new project
2. **Update platformio.ini**: Change `PROJECT_NAME`, `PROJECT_VERSION`, select default environment
3. **Create `include/secrets.h`** from `secrets_example.h`, list your WiFi networks
4. **Customize `include/config.h`**: mDNS hostname, display rotation, etc.
5. **Flash and test**: `pio run -t upload`, monitor at 115200 baud
6. **Add features**: Create `.cpp` files in `src/`, reference GPIO from `board_config.h`
7. **Update documentation**: Keep README and CHANGELOG in sync with changes

## Adding New Features

### Add a simple sensor
1. Add GPIO definition to `board_config.h`
2. Create `src/my_sensor.cpp` and `include/my_sensor.h`
3. Initialize in `setup()` 
4. Read/update in `loop()` or task functions
5. Document in [ADD_MODULES_FR.md](ADD_MODULES_FR.md)

### Add a new environment (e.g., ESP32-WROOM-32)
1. Update `platformio.ini` with new `[env:name]` section
2. Add GPIO definitions to `board_config.h` with matching `#if defined(ENV_NAME)`
3. Update [PIN_MAPPING_FR.md](PIN_MAPPING_FR.md) with new board reference

### Extend the web UI
1. Edit `buildRootPage()` in `src/main.cpp` to add HTML/CSS
2. Add new `server.on()` endpoints for API routes
3. Document changes in [WEB_UI_FR.md](WEB_UI_FR.md)

## Versioning

Follow **SEMVER** (Semantic Versioning):
- **MAJOR.MINOR.PATCH** (e.g., 1.2.3)
- MAJOR: breaking changes
- MINOR: new features (backward compatible)
- PATCH: bug fixes

Every version change:
1. Update `PROJECT_VERSION` in `platformio.ini`
2. Update `CHANGELOG.md` and `CHANGELOG_FR.md`
3. Commit with message: `chore: bump to v1.2.3`

## Common Tasks

### Change project name/version
Edit `platformio.ini`:
```ini
build_flags =
    -D PROJECT_VERSION='"1.0.0"'
    -D PROJECT_NAME='"My IoT Project"'
```

### Change default environment
Edit `platformio.ini`:
```ini
default_envs = esp32s3_n16r8  # or esp32devkit
```

### Add library dependency
Edit `platformio.ini` in `[env]`:
```ini
lib_deps =
    ... existing ...
    adafruit/Adafruit BME280@^2.2.2
```

### Test compilation
```bash
pio run -e esp32devkit
pio run -e esp32s3_n16r8
```

### Clean build
```bash
pio run -t clean
pio run
```

See [GETTING_STARTED_FR.md](GETTING_STARTED_FR.md) for first-time setup.
