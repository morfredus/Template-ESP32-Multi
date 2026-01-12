# Web UI & Networking (EN)

## Web UI Overview
The template includes a built-in web server serving on port **80**. Two endpoints:
- `GET /` — HTML dashboard with system stats
- `GET /status` — JSON API for programmatic access

## Accessing the UI

### By IP address
Find the IP on the LCD screen or serial monitor:
```
http://192.168.x.x/
```

### By mDNS hostname
Default hostname is `esp32-template` (editable in `config.h`):
```
http://esp32-template.local/
```

**Note:** mDNS requires `.local` domain support on your network (macOS/Linux usually support this; Windows may need Bonjour).

## Dashboard

The HTML page displays:
- **Processor** — Chip model (e.g., "ESP32-S3")
- **CPU** — Frequency in MHz
- **Heap** — Free and total SRAM bytes
- **PSRAM** — Free and total external RAM (S3 only)
- **SSID** — Connected WiFi network name
- **IP** — Device IP address
- **OTA** — Button to open the OTA update page

All values update when the page loads. The dashboard is responsive and works on mobile browsers.

## OTA Update Page (`/update`)
- Reachable from the dashboard button or directly at `http://<host>/update`
- Upload a compiled `.bin` file; a progress bar shows upload status
- On success the device reboots automatically and returns to the dashboard
- Keep the device and your computer on the same WiFi; large uploads work best with strong signal
- Security: no authentication by default; do not expose this endpoint on untrusted networks

## JSON Status API

Endpoint: `http://esp32-template.local/status`

Response example:
```json
{
  "project": "Template ESP32 Multi",
  "version": "0.1.0",
  "chip": "ESP32-S3",
  "cpu_mhz": 240,
  "heap_free": 256000,
  "heap_total": 320000,
  "psram_free": 8000000,
  "psram_total": 8388608,
  "ssid": "MyNetwork",
  "ip": "192.168.1.100"
}
```

Use this to log data, monitor the device remotely, or trigger alerts based on heap usage.

## Customizing the Dashboard

### Change title and colors
Edit `buildRootPage()` in `src/main.cpp`:
```cpp
html += "<title>My IoT Device</title>";
html += "body{background:linear-gradient(135deg,#1a1a2e,#16213e);...}";
```

### Add new stat card
```cpp
html += "<div class=\"card\">";
html += "<strong>My Stat</strong>";
html += "<div>" + String(myValue) + "</div>";
html += "</div>";
```

### Change card styling
Edit the CSS in `buildRootPage()`:
```cpp
html += ".card{background:#0b1220;border:2px solid #green;...}";
```

## Monitoring & Data Collection

### Poll the JSON endpoint
```bash
# Bash example
while true; do
  curl -s http://esp32-template.local/status | jq .
  sleep 10
done
```

### Log to cloud
Add to `loop()`:
```cpp
static unsigned long lastLog = 0;
if (WiFi.isConnected() && millis() - lastLog > 60000) {
    lastLog = millis();
    String json = buildStatusJson();
    // Send json to your server
}
```

### Trigger on conditions
```cpp
if (ESP.getFreeHeap() < 50000) {
    Serial.println("Low memory warning!");
    // Send alert
}
```

## WiFi Configuration

### Add/remove networks
Edit `include/secrets.h`:
```cpp
static const char* WIFI_NETWORKS[][2] = {
    {"Home", "password1"},
    {"Office", "password2"},
    {"Mobile", "password3"}
};
```

### Adjust connection timeout
Edit `include/config.h`:
```cpp
.wifiConnectTimeoutMs = 12000,  // 12 seconds per attempt
.maxWifiAttempts = 10,           // Try up to 10 times
```

### Manual connection
The template uses `WiFiMulti` which automatically tries all configured networks. Check WiFi status in code:
```cpp
if (WiFi.isConnected()) {
    Serial.println("SSID: " + String(WiFi.SSID()));
    Serial.println("IP: " + WiFi.localIP().toString());
}
```

## mDNS Troubleshooting

### Name not resolving
- Ensure mDNS is enabled in config: `.mdnsHost = "esp32-template"`
- Check router doesn't block mDNS (port 5353 UDP)
- Try `ping esp32-template.local` (add `.local` if needed)

### Different hostname
Edit `config.h`:
```cpp
.mdnsHost = "my-device"  // Will be accessible at my-device.local
```

## Security Notes

**The template web server has NO authentication.** If exposed on an untrusted network:
- Disable web UI by setting `enableWebUi = false` in `config.h`
- OR restrict access at your router/firewall level
- OR add basic HTTP authentication (not implemented by default)

## Performance

- Web server runs on **core 0** alongside WiFi
- Main firmware loop also runs; responsiveness depends on task complexity
- JSON endpoint is lightweight; use for frequent polling

## Advanced: Add custom endpoints

```cpp
server.on("/api/temperature", HTTP_GET, []() {
    String json = "{\"temp\":" + String(getTemperature()) + "}";
    server.send(200, "application/json", json);
});
```

Then call during `setup()` after `setupWebServer()`:
```cpp
setupWebServer();
server.on("/api/temperature", ...);
```

See [Arduino WebServer documentation](https://github.com/espressif/arduino-esp32/tree/master/libraries/WebServer) for more.
