# Module: neopixel (RGB LED)

## Module role
This module manages the NeoPixel RGB LED(s) on the board. It allows the system to display colors for status, feedback, or effects.

## Why separate NeoPixel control?
- To keep LED logic independent from other code.
- To allow easy changes to color effects or add more LEDs.
- To make the code reusable for other projects with NeoPixels.

## Where are the files?
- `include/neopixel.h`: declares public functions and constants.
- `src/neopixel.cpp`: contains the code for controlling the LED(s).

## Main functions
- `void init()`: Initializes the NeoPixel(s).
- `void setColor(uint32_t color)`: Sets the color of all LEDs.
- `uint32_t makeColor(uint8_t r, uint8_t g, uint8_t b)`: Helper to create a color value.
- `void setBrightness(uint8_t brightness)`: Sets LED brightness.
- `void clear()`: Turns off all LEDs.

## Important parameters
- Pin number and number of LEDs are set in `board_config.h`.
- Default brightness is set in the code.

## Interactions with other modules
- Used by main and network modules to show status (WiFi OK, error, etc.).
- Can be used for user feedback (reboot, notifications).

## Best practices
- Always initialize before use.
- Use clear color codes for each status.
- Avoid high brightness to save power.

## For beginners
- If the LED doesn't light up, check wiring and pin in `board_config.h`.
- Try changing the color in the code to see the effect.

---

> NeoPixels are a fun way to add feedback and style. This module keeps it simple and safe!
