# Dependency Graph - Module Dependencies

This document shows the dependencies between modules to help understand the architecture.

## Module Dependencies Diagram

```
┌─────────────────────────────────────────────────────────────┐
│                        main.cpp                             │
│                    (Orchestrator)                           │
└──────────────────┬──────────────────────────────────────────┘
                   │
    ┌──────────────┼──────────────────────┬────────────────┐
    ▼              ▼                      ▼                ▼
┌─────────┐   ┌─────────┐         ┌──────────────┐   ┌─────────┐
│ Psram   │   │Buttons  │         │ NeoPixel     │   │TftDisplay│
│  (1)    │   │  (2)    │         │   (3)        │   │   (4)    │
└─────────┘   └─────────┘         └──────────────┘   └────┬─────┘
                                                          │
                   ┌───────────────────────────────────────┘
                   │
                   ▼
         ┌─────────────────────┐
         │   UiState (5)       │
         │ (Shared State)      │
         └─────────────────────┘
                   │
    ┌──────────────┼──────────────┐
    ▼              ▼              ▼
┌─────────┐   ┌─────────┐   ┌──────────────┐
│ Network │   │WebServer│   │  WebUI (7)   │
│  (6)    │   │  (8)    │   └──────────────┘
└────┬────┘   └────┬────┘          │
     │             │               ▼
     │             │        ┌──────────────────┐
     │             │        │ WebStyles (9)    │
     │             │        │ (CSS + JS)       │
     │             │        └──────────────────┘
     │             │
     └─────┬───────┘
           │
           ▼
    ┌─────────────────┐
    │   app.h (10)    │
    │ (Global exports)│
    └─────────────────┘
```

## Module Details by Layer

### Hardware/System Layer (No dependencies)

```
psram.h
├─ Only depends on: Arduino, esp32-hal-psram
├─ Used by: main.cpp
└─ Provides: PSRAM initialization
```

```
buttons.h
├─ Depends on: board_config.h, config.h
├─ Used by: main.cpp, Buttons handler
└─ Provides: Button input handling
```

```
neopixel.h
├─ Depends on: config.h, Adafruit_NeoPixel
├─ Used by: main.cpp, Network (LED status)
└─ Provides: LED control functions
```

### Display Layer

```
tft_display.h
├─ Depends on: config.h, UiState::State
├─ Used by: main.cpp, Network (boot feedback)
└─ Provides: LCD rendering functions
```

### State Management Layer

```
ui_state.h
├─ Depends on: Arduino (IPAddress)
├─ Used by: main.cpp, TftDisplay, Buttons
└─ Provides: Global UI state access
```

### Network Layer

```
network.h
├─ Depends on: config.h, TftDisplay (for boot screen)
├─ Used by: main.cpp, WebServer (for OTA)
└─ Provides: WiFi, mDNS, OTA functions
```

### Web Layer

```
web_server.h
├─ Depends on: config.h, WebUI, Update
├─ Used by: main.cpp
└─ Provides: HTTP server management
```

```
web_ui.h
├─ Depends on: config.h, WebStyles
├─ Used by: WebServer
└─ Provides: HTML and JSON generation
```

```
web_styles.h
├─ Depends on: Arduino
├─ Used by: WebUI
└─ Provides: CSS and JavaScript strings
```

### Exports

```
app.h
├─ Exports: Global tft object
└─ Used by: All modules accessing TFT
```

## Dependency Matrix

```
Module           │ Psram │Button│Neo  │Tft  │State│Net  │Web  │UI  │Style
─────────────────┼───────┼──────┼─────┼─────┼─────┼─────┼─────┼────┼──────
main.cpp         │  X    │  X   │  X  │  X  │  X  │  X  │  X  │    │
Psram             │       │      │     │     │     │     │     │    │
Buttons          │       │      │     │     │  X  │     │     │    │
NeoPixel         │       │      │     │     │     │     │     │    │
TftDisplay       │       │      │     │     │  X  │     │     │    │
UiState          │       │      │     │     │     │     │     │    │
Network          │       │      │     │  X  │     │     │     │    │
WebServer        │       │      │     │     │     │  X  │  X  │ X  │
WebUI            │       │      │     │     │     │     │     │    │ X
WebStyles        │       │      │     │     │     │     │     │    │
```

Legend: X = depends on

## Initialization Order

Critical order (must be respected):

```
1. Psram::init()           # Memory first
2. Buttons::init()         # Input handling
3. TftDisplay::setupDisplay() # Display must be ready before other modules
4. NeoPixel::init()        # Optional LED
5. Network::connectWifiWithFeedback() # Uses TftDisplay for feedback
6. Network::initMdns()     # Networking services
7. Network::initOta()      # OTA after mDNS
8. WebServer::init()       # Web server
9. UiState updated         # State reflects initialization
```

Why this order:
- ✅ Memory initialized before anything that allocates
- ✅ Display ready before boot screens
- ✅ Network connectivity before web services
- ✅ State management available to all modules

## Loop Execution Order

```
loop() calls:
  1. WebServer::handleClient()    # Non-blocking, high priority
  2. ArduinoOTA.handle()          # OTA check
  3. Buttons::handle()            # User input
  4. TftDisplay::updateMainScreen() # Visual update
  5. NeoPixel::setColor()         # LED feedback
```

Why this order:
- ✅ Network first (time-sensitive)
- ✅ User input before display (responsive)
- ✅ Display update last (final state)

## Decoupling Examples

### Good: Decoupled Modules

```cpp
// Buttons doesn't know about Network
// Network doesn't know about Buttons
// Communication through UiState
```

### Good: Minimal Dependencies

```cpp
// TftDisplay only needs:
//   - config.h (settings)
//   - UiState (data to display)
// It doesn't include Network, WebServer, etc.
```

### Bad: Circular Dependencies (AVOID!)

```cpp
// ❌ Don't do this:
// network.h includes web_server.h
// web_server.h includes network.h
// This creates circular dependency!
```

## Adding a New Module

When adding `my_module.h`:

1. **Identify dependencies**: What other modules does it need?
2. **Minimize dependencies**: Only include what's necessary
3. **Update initialization order**: Where should it initialize?
4. **Update loop calls**: What periodic work is needed?
5. **Use UiState for communication**: Avoid direct module-to-module calls

Example:
```cpp
// my_module.h - Minimal dependencies
#pragma once
#include <Arduino.h>
// Only includes config and Arduino, NOT other modules

namespace MyModule {
  void init();
  void process();
}
```

## Performance Implications

- **Serial includes**: Reduce compilation time
- **Template instantiation**: Header-only code can slow compilation
- **Global state**: Prefer UiState for shared data
- **Memory usage**: Static data stays in RAM throughout execution

## Testing Single Modules

You can compile and test a single module:

```cpp
// test/test_my_module.cpp
#include "my_module.h"

void setup() {
  Serial.begin(115200);
  MyModule::init();
}

void loop() {
  MyModule::process();
}
```

This works because each module is independent with clear interfaces.

---

**Key Principle**: Modules should be independent and communicate through well-defined interfaces (functions and UiState).
