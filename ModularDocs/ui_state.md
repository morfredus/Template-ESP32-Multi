# Module: ui_state (User Interface State)

## Module role
This module manages the state of the user interface (UI) for the ESP32 project. It keeps track of what is displayed, user interactions, and UI transitions.

## Why separate UI state?
- To centralize all UI-related variables and logic.
- To make it easy to update the display or web UI based on state changes.
- To keep UI logic independent from hardware and backend code.

## Where are the files?
- `include/ui_state.h`: declares state variables and functions.
- `src/ui_state.cpp`: contains the logic for managing and updating UI state.

## Main functions
- `void setState(UIState newState)`: Changes the current UI state.
- `UIState getState()`: Returns the current UI state.
- `void update()`: Applies state changes to the display or web UI.

## Important parameters
- UI states are defined as enums (e.g., BOOT, HOME, ERROR, etc.).
- State transitions are managed in code for consistency.

## Interactions with other modules
- Used by tft_display and web_ui to know what to show.
- Can be updated by buttons, network, or other events.

## Best practices
- Keep state transitions simple and predictable.
- Document all possible UI states.
- Use enums for clarity and type safety.

## For beginners
- The UI state controls what the user sees on the screen or web page.
- If the display or web UI seems stuck, check the current state and transitions.

---

> The UI state module is the brain of your interface. It keeps everything in sync and easy to debug!
