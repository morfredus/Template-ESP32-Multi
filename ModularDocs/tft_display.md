# Module: tft_display (LCD Display)

> **Minimum valid version:** 0.4.4

---

> **Note:** Display constants (DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SPI_FREQ) are defined for both ESP32 Classic and ESP32-S3 in board_config.h.

## Module role
This module manages everything related to the LCD display (drawing, backlight, messages, etc.). It allows you to show information to the user, display system status, and improve the visual experience.

## Why separate display logic?
- To be able to change the screen easily without touching the rest of the code.
- To avoid mixing business logic (WiFi, buttons, etc.) with display code.
- To make the code more readable and reusable.

## Where are the files?
- `include/tft_display.h`: declares public functions and constants.
- `src/tft_display.cpp`: contains the code that drives the display.

## Main functions

### 1. `void setupDisplay()`
Initializes the LCD display:
- Configures SPI communication
- Sets the display rotation
- Turns on the backlight (in low-power mode at boot)
- Shows a boot screen

### 2. `void drawBootScreen(const char* message, uint8_t percent)`
Displays a boot screen with a message and a progress bar (e.g., "WiFi", "Ready", etc.).

### 3. `void updateMainScreen(bool force = false)`
Displays the main screen with WiFi info, IP, mDNS, etc. Refreshes the display if needed.

### 4. `void fadeBacklightToNormal(uint16_t durationMs = 500)`
Gradually increases the backlight brightness after boot to avoid current spikes.

### 5. `void initBacklight()`
Initializes the backlight in low-power mode at startup.

### 6. Utility functions
- `printFit`, `printWrap`, `fitText`: to display text that fits the screen width.

## Important parameters
- `backlightLevel`: normal display brightness (0-255)
- `bootBacklightLevel`: startup brightness (0-255)
- `lcdRotation`: display orientation
- `enableBootBar`: show or hide the boot progress bar

All these parameters are defined in `config.h`.

## Interactions with other modules
- Uses WiFi info (from the `network` module) to display network status.
- Uses the global user interface state (from `ui_state`).
- Can be called by the main module (`main.cpp`) to display messages at each boot step.

## Best practices
- Always use the module's functions to display or modify the screen.
- Never manipulate the `tft` object directly outside this module.
- Adjust brightness according to the environment to save energy.

## For beginners
- The display is often the first thing you see: a bug here is immediately visible!
- Feel free to change messages or background color to experiment.
- If the screen stays black, check the wiring and SPI settings in `board_config.h`.

---

> This module is a good example of separation of concerns: it does one thing, and does it well!
