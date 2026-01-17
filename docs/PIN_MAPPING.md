# Pin Mapping Guide (EN)

> **Minimum version:** 0.4.0

## Beginner's Guide: Essential Electrical Concepts

### Voltage and Current Basics
| Term | Definition | ESP32 Value |
|------|------------|-------------|
| **VCC** | Supply voltage (positive) | 3.3V or 5V |
| **GND** | Ground (0V reference) | 0V |
| **Logic Level** | Voltage for HIGH/LOW signals | 3.3V |
| **Max GPIO Current** | Maximum current per pin | 12mA (40mA absolute max) |

### Safety Rules
- **NEVER** exceed 3.3V on any ESP32 GPIO pin
- **ALWAYS** connect GND between ESP32 and peripherals
- Use protection resistors when required
- Add decoupling capacitors near power pins

---

## Required Components by Peripheral

### Push Buttons

| Component | Value | Power Rating | Purpose |
|-----------|-------|--------------|---------|
| Push button | NO (Normally Open) | - | User input |
| Resistor (optional) | 10kΩ | 1/4W (0.25W) | Pull-up (internal available) |
| Capacitor (optional) | 100nF ceramic | 16V+ | Hardware debouncing |

**Wiring Diagram:**
```
    3.3V ──────┬────────────────── (internal pull-up enabled)
               │
              [R] 10kΩ (optional with INPUT_PULLUP)
               │
    GPIO ──────┴──────┤ ├────────── GND
                     BUTTON
```

**Operation:**
- Button NOT pressed: GPIO reads HIGH (3.3V via pull-up)
- Button pressed: GPIO reads LOW (0V connected to GND)

---

### ST7789 LCD Display (SPI)

| Component | Value | Power Rating | Purpose |
|-----------|-------|--------------|---------|
| ST7789 display | 240x240 or 240x320 | - | Visual output |
| Capacitor | 100nF ceramic | 16V+ | Power filtering |
| Capacitor | 10µF electrolytic | 6.3V+ | Power stabilization |
| Transistor (optional) | NPN (2N2222) | 500mW | Backlight >20mA |
| Resistor (optional) | 1kΩ | 1/4W | Transistor base |

**Voltage:** 3.3V (logic and power)

**Common Module Pin Labels (Aliases):**
| Signal | Common Aliases on Modules |
|--------|--------------------------|
| SCK | CLK, SCLK, SCL* |
| MOSI | SDA*, DIN, SDI, DATA |
| MISO | SDO, DOUT |
| CS | CE, SS, CSN |
| DC | D/C, RS, A0, CMD |
| RST | RES, RESET |
| BL | LED, BLK, BACKLIGHT |

> *Warning: SDA/SCL on SPI displays ≠ I2C SDA/SCL pins!

**Backlight with Transistor (if >20mA):**
```
    GPIO ──[1kΩ]──► Base (B)
    VCC ──────────► Collector (C) ──► LED+ on module
    GND ◄─────────── Emitter (E)
```

---

### NeoPixel RGB LED (WS2812B)

| Component | Value | Power Rating | Purpose |
|-----------|-------|--------------|---------|
| NeoPixel LED | WS2812B | - | RGB feedback |
| Resistor | 300-500Ω | 1/4W | Signal protection |
| Capacitor | 100µF electrolytic | 6.3V+ | Power stabilization |
| Capacitor (chain) | 100nF ceramic | 16V+ | Per LED (long chains) |

**Voltages:**
- Power (VCC): 5V recommended (3.3V works but dimmer)
- Data signal: 3.3V (ESP32 compatible without level shifter)
- Current: ~60mA per LED at full white brightness

**Wiring Diagram:**
```
    5V ────┬────────────────────► VCC NeoPixel
           │
          [C] 100µF 6.3V+ (electrolytic)
           │
    GND ───┴────────────────────► GND NeoPixel

    GPIO ──[R 330Ω]─────────────► DIN NeoPixel
                                     │
                                     └──► DOUT to next NeoPixel
```

**Module Pin Labels:**
| Signal | Common Aliases |
|--------|---------------|
| DIN | DATA, DI, D, IN |
| DOUT | DO, OUT |

---

## ESP32 DevKit

| Function | GPIO | Function Aliases | Type | Notes |
|----------|------|------------------|------|-------|
| **Button BOOT** | 0 | RTC_GPIO0 | INPUT_PULLUP | Hold for 3s to reboot |
| **Button User 1** | 32 | ADC1_CH4, RTC_GPIO9 | INPUT_PULLUP | Free to use |
| **Button User 2** | 33 | ADC1_CH5, RTC_GPIO8 | INPUT_PULLUP | Free to use |
| **Display SCK** | 18 | VSPI_CLK, SPI3_CLK, SCK | OUTPUT (SPI) | LCD clock (VSPI) |
| **Display MOSI** | 23 | VSPI_MOSI, SPI3_MOSI, MOSI | OUTPUT (SPI) | LCD data (VSPI) |
| **Display MISO** | 19 | VSPI_MISO, SPI3_MISO, MISO | INPUT (SPI) | LCD data (VSPI, optional) |
| **Display CS** | 5 | VSPI_CS0, SPI3_CS0, CS | OUTPUT | Chip select |
| **Display DC** | 16 | U2RXD, RXD2 | OUTPUT | Data/Command |
| **Display RST** | 17 | U2TXD, TXD2 | OUTPUT | Reset |
| **Display BL** | 4 | RTC_GPIO10 | OUTPUT (PWM) | Backlight |

**Wiring Diagram:**
```
    ESP32 DevKit          ST7789 Module
    ────────────          ─────────────
    3.3V ─────────────────► VCC
    GND ──────────────────► GND
    GPIO18 (SCK) ─────────► SCK/CLK/SCLK
    GPIO23 (MOSI) ────────► SDA/DIN/MOSI
    GPIO5  (CS) ──────────► CS/CE/SS
    GPIO16 (DC) ──────────► DC/RS/A0
    GPIO17 (RST) ─────────► RST/RES/RESET
    GPIO4  (BL) ──────────► BL/LED
```

**Total SPI Pins Used:** 6 (VSPI/SPI3)
**I2C Default:** SDA=21, SCL=22
**UART0:** TX=1, RX=3
**UART1:** TX=10, RX=9
**UART2:** TX=17, RX=16
**Free GPIO:** Many available for your sensors/modules
**Safety:** All pins avoid strapping pins (0 is INPUT_PULLUP safe)

### I2C/SPI Dual-Use Correspondence (ESP32 DevKit)
- **GPIO21 (I2C_SDA)** = data line; can be reassigned as MOSI in SPI (data from master to slave)
- **GPIO22 (I2C_SCL)** = clock line; can be reassigned as SCK in SPI
- **Note:** SCL ≈ SCK (both clocks), SDA ≈ MOSI (data from master to slave)
- **For displays labeled SDA/SCL instead of MOSI/SCK:** SDA=MOSI, SCL=SCK (MISO typically unused on displays)

---

## ESP32-S3 DevKitC-1 N16R8

| Function | GPIO | Function Aliases | Type | Notes |
|----------|------|------------------|------|-------|
| **Button BOOT** | 0 | RTC_GPIO0 | INPUT_PULLUP | Hold for 3s to reboot |
| **Button User 1** | 16 | U0RTS | INPUT_PULLUP | Free to use |
| **Button User 2** | 17 | U0CTS | INPUT_PULLUP | Free to use |
| **NeoPixel** | 48 | RTC_GPIO18 | OUTPUT | RGB LED (mandatory) |
| **Display SCK** | 12 | HSPI_CLK, SPI2_CLK, SCK | OUTPUT (SPI) | LCD clock |
| **Display MOSI** | 11 | HSPI_MOSI, SPI2_MOSI, MOSI | OUTPUT (SPI) | LCD data |
| **Display MISO** | 15 | HSPI_MISO, SPI2_MISO, MISO | INPUT (SPI) | LCD data (optional) |
| **Display CS** | 10 | HSPI_CS0, SPI2_CS0, CS | OUTPUT | Chip select |
| **Display DC** | 17 | U0CTS | OUTPUT | Data/Command |
| **Display RST** | 18 | U0RTS | OUTPUT | Reset |
| **Display BL** | 16 | RTC_GPIO16 | OUTPUT (PWM) | Backlight |

**Wiring Diagram:**
```
    ESP32-S3              ST7789 Module
    ────────              ─────────────
    3.3V ─────────────────► VCC
    GND ──────────────────► GND
    GPIO12 (SCK) ─────────► SCK/CLK/SCLK
    GPIO11 (MOSI) ────────► SDA/DIN/MOSI
    GPIO10 (CS) ──────────► CS/CE/SS
    GPIO17 (DC) ──────────► DC/RS/A0
    GPIO18 (RST) ─────────► RST/RES/RESET
    GPIO16 (BL) ──────────► BL/LED
```

**Total SPI Pins Used:** 6 (HSPI/SPI2)
**I2C Default:** SDA=41, SCL=42
**UART0:** TX=43, RX=44
**UART1:** TX=2, RX=1
**NeoPixel:** 1 GPIO (fixed on S3)
**Free GPIO:** Many available for sensors/modules

### I2C/SPI Dual-Use Correspondence (ESP32-S3)
- **GPIO41 (I2C_SDA)** = data line; can be reassigned as MOSI in SPI (data from master to slave)
- **GPIO42 (I2C_SCL)** = clock line; can be reassigned as SCK in SPI
- **GPIO11 (Display MOSI)** can also be I2C_SDA if reassigning SPI pins (data line ↔ data line)
- **GPIO12 (Display SCK)** can also be I2C_SCL if reassigning SPI pins (clock line ↔ clock line)
- **Note:** SCL ≈ SCK (both clocks), SDA ≈ MOSI (data from master to slave)
- **For displays labeled SDA/SCL instead of MOSI/SCK:** SDA=MOSI, SCL=SCK (MISO typically unused on displays)

---

## Strapping Pins (DO NOT USE AS OUTPUTS)
These pins are read during boot to determine boot mode:
- **ESP32:** 0, 2, 4, 5, 12, 15 (already used or avoided in template)
- **ESP32-S3:** 45, 46 (avoid when possible)

---

## Adding Your Own Pins
To add a new sensor, update `board_config.h`:
```cpp
#if defined(ENV_ESP32_DEVKIT)
#define MY_SENSOR_PIN  34  // ADC input
#elif defined(ENV_ESP32S3_N16R8)
#define MY_SENSOR_PIN  3   // ADC input
#endif
```

Then use in your code:
```cpp
#include "board_config.h"

pinMode(MY_SENSOR_PIN, INPUT);
int sensorValue = analogRead(MY_SENSOR_PIN);
```

---

## SPI Notes
Both boards use the **same SPI protocol** for the LCD display. Clock speed is adjusted per board:
- **ESP32:** 27 MHz
- **ESP32-S3:** 40 MHz

If you add another SPI device, consider sharing the SCK/MOSI/MISO lines and using a different CS pin.

---

## ADC Pins
**ESP32 DevKit** ADC1: 32-39 (GPIO32-39)
**ESP32-S3** ADC1: 1-10 (GPIO1-10), ADC2: 11-20 (GPIO11-20)

---

## Function Aliases Reference

### SPI Peripherals
- **ESP32 DevKit** uses VSPI (SPI3): SCK=18, MOSI=23, MISO=19, CS pins flexible
- **ESP32-S3** uses HSPI (SPI2): SCK=12, MOSI=11, MISO=13, CS pins flexible

Common SPI function aliases: **SCK**, **MOSI**, **MISO**, **CS**

### I2C Peripherals
- **ESP32 DevKit** default: SDA=21 (U0CTS), SCL=22 (U0RTS)
- **ESP32-S3** default: SDA=41, SCL=42

Any GPIO pair can be configured as I2C with software-based bitbang protocol.

### UART Peripherals
- **ESP32 DevKit:**
  - UART0: TX=1 (U0TXD), RX=3 (U0RXD) — used by serial monitor
  - UART1: TX=10, RX=9
  - UART2: TX=17 (U2TXD), RX=16 (U2RXD)
- **ESP32-S3:**
  - UART0: TX=43, RX=44 — used by serial monitor
  - UART1: TX=2, RX=1
  - UART2: Free to configure

### JTAG/Debug Pins (Avoid when possible)
- **TCO** (Test Clock Out) aliases: MTMS, TMS
- **TDI** (Test Data In) aliases: MTDI
- **TDO** (Test Data Out) aliases: MTDO
- **TCK** (Test Clock In) aliases: MTCK

### RTC Pins (Ultra-low power)
- Can maintain state during deep sleep
- Alias format: `RTC_GPIO[N]` where N differs from GPIO number
- Example: GPIO0 = RTC_GPIO0, GPIO32 = RTC_GPIO9

### DAC (Digital-to-Analog Converter)
- **ESP32 DevKit:** GPIO25 (DAC1), GPIO26 (DAC2)
- **ESP32-S3:** GPIO17 (DAC1, display RST), GPIO18 (DAC2, BUTTON in template)

### Touch-Sensitive GPIO (capacitive touch)
- **ESP32 DevKit:** GPIO4, GPIO0, GPIO2, GPIO15, GPIO13, GPIO12, GPIO14, GPIO27, GPIO33, GPIO32
- Can be used for touchpad sensing without physical buttons

---

## I2C Alternative
If using I2C instead of SPI for additional sensors, any free GPIO pair works:
- **SDA/SCL** recommended: GPIO21/GPIO22 (ESP32) or GPIO41/GPIO42 (S3)

See [ADD_MODULES.md](ADD_MODULES.md) for sensor integration examples.
