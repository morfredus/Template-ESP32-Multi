# Module: web_server (Web server)

## Module role
This module manages the HTTP web server running on the ESP32. It allows users to access a web interface, get status, and interact with the device from a browser.

## Why separate the web server?
- To keep web logic independent from hardware and core logic.
- To allow easy updates to web features without breaking the rest.
- To make the code reusable for other web-enabled projects.

## Where are the files?
- `include/web_server.h`: declares public functions and endpoints.
- `src/web_server.cpp`: contains the code for the HTTP server.

## Main functions
- `void init()`: Starts the web server and registers endpoints.
- `void handleClient()`: Handles incoming HTTP requests.

## Important parameters
- Port and endpoints are set in the code.
- Web UI features are enabled/disabled in `config.h`.

## Interactions with other modules
- Uses network status to serve pages only when connected.
- Can display data from other modules (status, logs, etc.).

## Best practices
- Keep endpoints simple and well-documented.
- Always check network connection before serving pages.
- Use JSON for status APIs when possible.

## For beginners
- Access the web UI at `http://<esp32-ip>/` or `http://<mdns>.local/`.
- If the page doesn't load, check WiFi and mDNS status.

---

> The web server is your window into the ESP32. This module keeps it robust and easy to extend!
