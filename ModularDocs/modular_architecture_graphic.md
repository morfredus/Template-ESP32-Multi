# Modular Architecture – Visual Overview

Below is a visual summary of the modular architecture of the ESP32 project. Each block represents a module, and arrows show the main interactions. This diagram is for beginners and complements (without duplicating) the detailed explanations in `docs/MODULAR_ARCHITECTURE.md`.

```
+-------------------+
|    main.cpp       |
+-------------------+
        |  |  |  |  |  |  |
        v  v  v  v  v  v  v
+---------+   +-----------+   +-----------+
| buttons |   | tft_display|   |  neopixel |
+---------+   +-----------+   +-----------+
        |           |              |
        v           v              v
+---------------------------------------+
|             ui_state                  |
+---------------------------------------+
        |           |              |
        v           v              v
+---------+   +-----------+   +-----------+
| network |   | web_server|   |  psram    |
+---------+   +-----------+   +-----------+
        |           |
        v           v
+-------------------+
|   web_ui         |
+-------------------+
        |
        v
+-------------------+
| secrets_wrapper   |
+-------------------+
```

**Legend:**
- **main.cpp**: Orchestrates all modules
- **buttons, tft_display, neopixel**: Hardware interface modules
- **ui_state**: Central state shared by all modules
- **network, web_server, psram**: Service and resource modules
- **web_ui**: Web interface logic
- **secrets_wrapper**: Secure access to credentials

**How to read:**
- Arrows show the main direction of data or control flow
- All modules can access `ui_state` to share information
- `main.cpp` initializes and coordinates everything

---

> For a detailed description of each module, see the files in the ModularDocs/ folder and `docs/MODULAR_ARCHITECTURE.md`.
