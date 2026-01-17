# Module: network (WiFi and networking)

## Module role
This module manages all network connections: WiFi, mDNS, OTA updates, and network status. It allows the ESP32 to connect to the internet, communicate with other devices, and provide web services.

## Why separate networking?
- To keep network logic isolated from display, buttons, etc.
- To make it easy to change network settings or add new features (like OTA) without breaking the rest.
- To allow reuse of the network code in other projects.

## Where are the files?
- `include/network.h`: declares public functions and constants.
- `src/network.cpp`: contains the code for WiFi, mDNS, OTA, etc.

## Main functions
- `bool connectWifiWithFeedback()`: Connects to WiFi and provides progress feedback for the display.
- `bool initMdns()`: Starts the mDNS service (so you can access the ESP32 by name).
- `void initOta()`: Enables OTA (Over-The-Air) firmware updates.
- `bool isMdnsRunning()`: Returns true if mDNS is active.
- `bool isConnected()`: Returns true if WiFi is connected.

## Important parameters
- WiFi credentials are stored in `secrets.h` (never hardcoded!).
- mDNS host name is set in `config.h`.
- Timeouts and retry delays are configurable in `config.h`.

## Interactions with other modules
- Provides network status to the display (for showing SSID, IP, etc.).
- Used by the web server to know if it can serve pages.
- Can trigger LED color changes (NeoPixel) based on connection state.

## Best practices
- Never hardcode WiFi credentials in the code.
- Always check connection status before starting network services.
- Use mDNS for easy access without knowing the IP.

## For beginners
- If WiFi doesn't connect, check your `secrets.h` file and your router.
- OTA lets you update the firmware without a cable—very useful!
- mDNS means you can type `http://esp32-template.local/` instead of an IP address.

---

> Networking is a key part of any connected device. This module keeps it clean and easy to manage!
