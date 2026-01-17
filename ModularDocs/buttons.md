# Module: buttons (Button handling)

> **Minimum valid version:** 0.4.4

---

> **Note:** All button and GPIO references in code now strictly match the names defined in board_config.h (immutable source of truth).

## Module role
This module manages the physical buttons on the board. It detects button presses, long presses, and allows the system to react to user input (like reboot, menu navigation, etc.).

## Why separate button handling?
- To keep input logic clean and independent from display or network code.
- To make it easy to add or change button functions without breaking the rest.
- To allow for easy expansion (more buttons, different actions).

## Where are the files?
- `include/buttons.h`: declares public functions and constants.
- `src/buttons.cpp`: contains the code for button handling.

## Main functions
- `void init()`: Initializes the button pins and state.
- `void handle()`: Checks button state, debounces, and triggers actions.
- `bool isPressed(uint8_t btn)`: Returns true if a button is pressed.
- `bool isLongPressed(uint8_t btn)`: Returns true if a button is held down.

## Important parameters
- Button pin numbers are set in `board_config.h`.
- Debounce and long-press timings are set in the code.

## Interactions with other modules
- Can trigger display changes (show menu, reboot prompt, etc.).
- Updates the global UI state (in `ui_state`).

## Best practices
- Always debounce button inputs to avoid false triggers.
- Use clear, documented actions for each button.
- Keep button logic simple and modular.

## For beginners
- If a button doesn't work, check the wiring and pin numbers in `board_config.h`.
- Try changing the action for a button to see how the system responds.

---

> Buttons are the main way users interact with the device. This module keeps it reliable and easy to extend!
