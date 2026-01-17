# Module: psram (PSRAM memory)

## Module role
This module manages the external PSRAM (Pseudo-Static RAM) available on some ESP32 boards. It allows the system to use more memory for buffers, images, or advanced features.

## Why separate PSRAM management?
- To keep memory initialization and checks isolated from other logic.
- To allow easy adaptation if the board changes (with or without PSRAM).
- To make the code robust and portable.

## Where are the files?
- `include/psram.h`: declares public functions.
- `src/psram.cpp`: contains the code for PSRAM detection and use.

## Main functions
- `void init()`: Initializes PSRAM if available.
- `bool isAvailable()`: Returns true if PSRAM is detected and usable.
- `size_t getSize()`: Returns the size of available PSRAM.

## Important parameters
- No user parameters; detection is automatic.

## Interactions with other modules
- Used by display or web modules for large buffers.
- Can be checked in main to adapt features if PSRAM is missing.

## Best practices
- Always check if PSRAM is available before using large buffers.
- Use PSRAM for memory-hungry features only.

## For beginners
- Not all boards have PSRAM! Check your board specs.
- If you get memory errors, check if PSRAM is detected at boot.

---

> PSRAM gives your ESP32 superpowers for big projects. This module makes it easy to use safely!
