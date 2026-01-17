# Module: web_ui (Web user interface)

## Module role
This module manages the HTML/CSS/JS content served by the ESP32 web server. It provides a user-friendly interface to interact with the device from any browser.

## Why separate the web UI?
- To keep UI logic and assets independent from backend code.
- To allow easy updates to the interface without touching core logic.
- To enable designers to work on the UI without C++ knowledge.

## Where are the files?
- `include/web_ui.h`: declares UI-related functions.
- `src/web_ui.cpp`: contains code to serve HTML/CSS/JS.
- `include/web_styles.h`: contains CSS styles as C++ strings.

## Main functions
- `void servePage()`: Sends the main HTML page to the client.
- `void serveStyles()`: Sends CSS styles.
- `void serveScript()`: Sends JavaScript if needed.

## Important parameters
- UI features are enabled/disabled in `config.h`.
- Page content is defined in C++ strings for embedded serving.

## Interactions with other modules
- Uses data from ui_state, network, and other modules to display status.
- Can trigger actions (buttons, forms) that call backend endpoints.

## Best practices
- Keep UI simple and responsive.
- Minimize page size for fast loading.
- Use clear labels and tooltips for beginners.

## For beginners
- Open the ESP32’s IP in your browser to access the UI.
- If you see a blank page, check network and server status.

---

> The web UI makes your ESP32 accessible to everyone. This module keeps it clean, fast, and easy to use!
