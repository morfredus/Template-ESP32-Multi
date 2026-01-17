# Module: main (Program entry point)

## Module role
The `main.cpp` file is the heart of the project. It orchestrates the initialization of all modules, manages the main loop, and coordinates interactions between the different parts of the system.

## Why a main file?
- To centralize system startup.
- To keep an overview of the initialization order.
- To separate orchestration logic from module details.

## Where is the file?
- `src/main.cpp`

## Main functions

### 1. `void setup()`
Function called once at startup. It:
- Initializes serial communication (for debugging)
- Initializes each module (PSRAM, buttons, display, NeoPixel, network, etc.)
- Displays boot and progress screens
- Starts WiFi connection and network services
- Manages progressive backlight increase

### 2. `void loop()`
Function called in a loop. It:
- Handles network and web requests
- Handles user input (buttons)
- Refreshes the display periodically
- Updates the NeoPixel LED status according to connection

## Important parameters
- Uses global parameters defined in `config.h` and global state in `ui_state.h`

## Interactions with other modules
- Calls initialization and display functions of each module
- Updates the global user interface state
- Coordinates display, network, buttons, etc.

## Best practices
- Keep the `setup()` function clear and sequential
- Do not overload the `loop()`; delegate as much as possible to modules
- Use serial logs for debugging

## For beginners
- This is the starting point of the program: everything begins here!
- Read the initialization order to understand the global operation
- Feel free to add logs to follow the boot process

---

> The main file should remain simple: it orchestrates, it doesn't do everything!
