# Adding Modules (EN)

## Overview
This guide explains how to add sensors, modules, and new features to the template while maintaining code organization.

## File Structure
```
src/
  main.cpp              ← Entry point (setup, loop, initialization)
  [feature].cpp         ← New feature code
include/
  board_config.h        ← GPIO definitions (DO NOT EDIT)
  config.h              ← User settings
  [feature].h           ← Headers for new features
lib/
  [library]/            ← Local helper libraries
```

## Adding a Simple Sensor (DHT22 example)

### 1. Add library to platformio.ini
```ini
[env]
lib_deps =
    adafruit/Adafruit DHT sensor library@^1.4.4
```

### 2. Define the pin in board_config.h
```cpp
// Already done if ENV_ESP32S3_N16R8 or ENV_ESP32_DEVKIT:
// Add after NeoPixel definitions:

#if defined(ENV_ESP32S3_N16R8)
#define DHT_PIN  15  // Your chosen GPIO
#elif defined(ENV_ESP32_DEVKIT)
#define DHT_PIN  35  // Your chosen GPIO
#endif
```

### 3. Create a header file include/dht_sensor.h
```cpp
#pragma once
#include <DHT.h>
#include "board_config.h"

extern DHT dhtSensor;

void initDHT();
float getTemperature();
float getHumidity();
```

### 4. Create src/dht_sensor.cpp
```cpp
#include "dht_sensor.h"

DHT dhtSensor(DHT_PIN, DHT22);

void initDHT() {
    dhtSensor.begin();
}

float getTemperature() {
    return dhtSensor.readTemperature();
}

float getHumidity() {
    return dhtSensor.readHumidity();
}
```

### 5. Update src/main.cpp
```cpp
#include "dht_sensor.h"

void setup() {
    // ... existing code ...
    initDHT();
    // ... rest of setup ...
}

void loop() {
    // ... existing code ...
    float temp = getTemperature();
    float humidity = getHumidity();
    // Use temp and humidity
}
```

## Adding an I2C Device (OLED display)

### 1. Add library
```ini
[env]
lib_deps =
    adafruit/Adafruit SSD1306@^2.5.7
    adafruit/Adafruit Unified Sensor@^1.1.14
```

### 2. Add I2C pins (if needed)
ESP32 and ESP32-S3 have default I2C pins, but you can specify:
```cpp
#include <Wire.h>
// Default: SDA=GPIO21, SCL=GPIO22 (ESP32)
//          SDA=GPIO41, SCL=GPIO42 (S3)
Wire.begin(SDA_PIN, SCL_PIN);
```

### 3. Initialize and use
```cpp
#include <Adafruit_SSD1306.h>

#define OLED_WIDTH  128
#define OLED_HEIGHT 64
Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT, &Wire, -1);

void setup() {
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("OLED failed!");
        return;
    }
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Hello!");
    display.display();
}
```

## Adding a Custom Button Handler

### 1. Add button in board_config.h (if not already there)
```cpp
#define MY_BUTTON_PIN  25  // Choose a free GPIO
```

### 2. Handle in setup()
```cpp
pinMode(MY_BUTTON_PIN, INPUT_PULLUP);
```

### 3. Check in loop()
```cpp
void loop() {
    // ... existing code ...
    
    if (digitalRead(MY_BUTTON_PIN) == LOW) {
        // Button pressed
        delay(20);  // Debounce
        if (digitalRead(MY_BUTTON_PIN) == LOW) {
            // Confirmed press
            myButtonHandler();
        }
    }
}

void myButtonHandler() {
    // Your action here
    Serial.println("Button pressed!");
}
```

## Adding a Task with Timing

Use the timing pattern already in `loop()`:
```cpp
static unsigned long lastCheck = 0;
const unsigned long INTERVAL_MS = 5000;  // Every 5 seconds

void loop() {
    // ... existing code ...
    
    const unsigned long now = millis();
    if (now - lastCheck >= INTERVAL_MS) {
        lastCheck = now;
        myTaskFunction();
    }
}

void myTaskFunction() {
    // Runs every 5 seconds
}
```

## Adding WiFi Communication

The template already includes WiFiMulti. Add HTTP requests:

```cpp
#include <HTTPClient.h>

void sendToServer(const String& data) {
    if (!WiFi.isConnected()) {
        return;
    }
    HTTPClient http;
    http.begin("http://example.com/api/data");
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST(data);
    http.end();
}
```

## Performance Tips
- Keep `loop()` fast; long operations should be split across cycles
- Avoid blocking delays; use timing patterns instead
- Cache frequently-read values (temperature, ADC) with proper intervals
- Monitor heap with `Serial.println(ESP.getFreeHeap())`

## Debugging
Add debug output:
```cpp
#ifdef DEBUG
Serial.print("Debug: ");
Serial.println(value);
#endif
```

Compile with:
```ini
[env:esp32devkit]
build_flags = 
    ${env.build_flags}
    -D DEBUG
```

## Common Mistakes
- Forgetting to #include the board_config.h header
- Using analog pins on digital GPIO (ADC restrictions per board)
- Blocking `loop()` with delay() — use timing patterns instead
- Not checking WiFi status before WiFi operations
- Memory leaks from repeated `new` without `delete`

Next: See [PIN_MAPPING.md](PIN_MAPPING.md) for available GPIO on your board.
