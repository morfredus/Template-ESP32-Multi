# LCD System (EN)

> **Minimum version:** 0.4.2

## Overview
The template uses an **Adafruit ST7789** 240×240 SPI display with the Adafruit GFX library. The display shows three screens:
1. **Boot screen** — Project name, version, progress bar
2. **Status screen** — WiFi info, IP address, mDNS hostname
3. **Reboot prompt** — Long-press BOOT button feedback

## Display Connection
See [PIN_MAPPING.md](PIN_MAPPING.md) for GPIO assignments per board.

## Configuration
Edit `include/config.h` to adjust:
```cpp
.lcdRotation = 2,           // 0-3 (0=portrait, 2=landscape)
.backlightLevel = 255,      // 0-255 (PWM duty) - normal operation
.bootBacklightLevel = 77,   // 0-255 (PWM duty) - during boot (~30% to reduce current)
.enableBootBar = true,      // Show progress bar during boot
```

**Power Management:** The display backlight starts at `bootBacklightLevel` (~30% brightness) during initialization to minimize current draw and prevent bootloops on weak USB ports. After boot completes, it gradually fades to `backlightLevel` over 500ms. This progressive power management avoids inrush current spikes.

## Boot Flow
1. **Power-on** → ST7789 initialized, shows "Boot" with 5% bar
2. **WiFi connect** → Shows "Connecting WiFi" (15% → 70%)
3. **WiFi result** → Shows "WiFi OK" (green NeoPixel) or "WiFi fail" (red)
4. **Web server** → Shows "Starting web" (70% → 85%)
5. **Ready** → Shows "Ready" (100%), then main screen

## Main Screen Content
```
Project Name
v0.1.0

SSID: MyNetwork
IP: 192.168.1.100
mDNS: esp32-template
```

## Reboot Screen
Long-press BOOT button (GPIO 0):
```
Reboot ?
[████████░░░░] 60%
```
- Hold for 3 seconds to reboot
- Release early to cancel

## Customizing the Display

### Change boot messages
Edit `src/main.cpp` in the `drawBootScreen()` function calls:
```cpp
drawBootScreen("WiFi", 15);           // Change "WiFi" text
drawBootScreen("WiFi OK", 70);        // Change progress text
```

### Change main screen layout
Edit the `updateMainScreen()` function in `src/main.cpp`:
```cpp
tft.setCursor(10, 80);
tft.print("SSID: ");
tft.print(ssidNow);
// Add more lines or modify positions here
```

### Colors available
```cpp
ST77XX_BLACK      ST77XX_BLUE       ST77XX_RED
ST77XX_GREEN      ST77XX_CYAN       ST77XX_MAGENTA
ST77XX_YELLOW     ST77XX_WHITE      ST77XX_ORANGE
```

Example:
```cpp
tft.setTextColor(ST77XX_YELLOW);  // Change text color
tft.fillScreen(ST77XX_BLACK);     // Fill with black
```

### Text sizes
```cpp
tft.setTextSize(1);  // Small
tft.setTextSize(2);  // Medium (default)
tft.setTextSize(3);  // Large
```

## Performance Notes
- No full-screen clear loops at runtime (only on screen transitions)
- Screen updates are throttled to ~1 second intervals (see `loop()`)
- PWM backlight may flicker if not properly initialized; see `initBacklight()`

## Troubleshooting

### Display is blank
- Check SPI pin connections (SCK, MOSI, CS, DC, RST)
- Verify backlight pin (BL) is powered and PWM initialized
- Reset the board

### Garbled text or colors
- Check SPI frequency in `board_config.h` (27 MHz for DevKit, 40 MHz for S3)
- Verify display rotation matches your physical orientation
- Try a lower SPI frequency

### Screen flickers or tears
- Displays are not double-buffered; avoid rapid full-screen redraws
- Use targeted updates (clear specific regions) when possible
- Increase delay between redraws

## Advanced Customization

### Add custom graphics
```cpp
// Draw a line
tft.drawLine(x0, y0, x1, y1, color);

// Draw a circle
tft.drawCircle(x, y, radius, color);

// Draw a filled rect
tft.fillRect(x, y, w, h, color);

// Print a float
tft.print(3.14159);
```

See [Adafruit GFX documentation](https://github.com/adafruit/Adafruit-GFX-Library) for more.
