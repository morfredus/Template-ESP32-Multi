# Getting Started (EN)

> **Minimum valid version:** 0.4.4

> **Note:** All button and GPIO references in code now strictly match the names defined in board_config.h (immutable source of truth). Display constants (DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SPI_FREQ) are defined for both ESP32 Classic and ESP32-S3.
# Getting Started (EN)

## 1. Prerequisites
- Visual Studio Code with PlatformIO extension
- Python 3.6+
- USB cable compatible with your ESP32 board

## 2. Create your project
```bash
# Clone or download this template folder
cp -r 00-Template-ESP32-Multi my-esp32-project
cd my-esp32-project
```

## 3. Configure WiFi
- Copy `include/secrets_example.h` to `include/secrets.h`
- Edit `include/secrets.h` and add your WiFi networks:
```cpp
static const char* WIFI_NETWORKS[][2] = {
    {"MyHomeNetwork", "password123"},
    {"MyPhoneHotspot", "phone_password"}
};
```
- **IMPORTANT:** Never commit `secrets.h` to version control (it's in `.gitignore`)

## 4. Select your board
In `platformio.ini`, set `default_envs`:
```ini
default_envs = esp32devkit          # for classic ESP32
; default_envs = esp32s3_n16r8      # for ESP32-S3 (uncomment to use)
```

## 5. Connect your board
- Plug in your ESP32 via USB
- PlatformIO should auto-detect it

## 6. Flash the firmware
- Click the **Build & Upload** icon in VS Code (or press `Ctrl+Alt+U`)
- Wait for the upload to finish (status bar shows "SUCCESS")

## 7. Monitor the board
- Click the **Serial Monitor** icon (or `Ctrl+Alt+S`)
- Set baud rate to **115200**
- Watch boot messages and WiFi connection progress

## 8. Access the web interface
Once WiFi is connected, open:
- **By IP:** http://192.168.x.x (check your router)
- **By mDNS:** http://esp32-template.local (default hostname from config.h)

## What you should see
- **LCD screen** (if wired): project name, version, WiFi SSID, IP address
- **Web UI**: project stats, CPU info, heap usage, PSRAM, network details
- **LED feedback** (S3 only): blue=boot, green=WiFi OK, red=WiFi failed

## Troubleshooting

### No WiFi connection
- Check SSID and password in `secrets.h`
- Verify your WiFi is 2.4 GHz (ESP32 doesn't support 5 GHz on most models)
- Watch serial monitor for connection attempts

### Can't access web UI
- Check the IP address on the LCD or serial monitor
- Try mDNS: `ping esp32-template.local` (Windows: `ping esp32-template.local`)
- Firewall may block access; check your router settings

### Compile errors
- Make sure you chose the right environment (esp32devkit vs esp32s3_n16r8)
- Run "PlatformIO: Clean" then "PlatformIO: Build"
- Check that all #include paths are correct

## Next steps
- Read [PIN_MAPPING.md](PIN_MAPPING.md) to understand GPIO assignments
- Check [ADD_MODULES.md](ADD_MODULES.md) to extend functionality
- Review [LCD_SYSTEM.md](LCD_SYSTEM.md) for display customization
