#pragma once
#include <Arduino.h>

// ============================================================
// PIN MAPPING FOR BUTTONS
// ============================================================
#if defined(ENV_ESP32S3_N16R8)

#define BUTTON_BOOT_PIN     0   // BOOT button, long press to reboot (RTC_GPIO0)
#define BUTTON_USER1_PIN    16  // User button 1 (U0RTS)
#define BUTTON_USER2_PIN    17  // User button 2 (U0CTS)

// ============================================================
// PIN MAPPING FOR DISPLAY (ST7789, SPI - HSPI/SPI2)
// ============================================================
#define DISPLAY_SCK_PIN     12  // SPI clock (HSPI_CLK, SPI2_CLK, SCK)
#define DISPLAY_MOSI_PIN    11  // SPI MOSI (HSPI_MOSI, SPI2_MOSI, MOSI, SDA)
#define DISPLAY_MISO_PIN    13  // SPI MISO (HSPI_MISO, SPI2_MISO, MISO, SCL, optional)
#define DISPLAY_CS_PIN      10  // Chip select (HSPI_CS0, SPI2_CS0, CS)
#define DISPLAY_DC_PIN      9   // Data/Command (MTCK, DAC2)
#define DISPLAY_RST_PIN     8   // Reset (MTDI, DAC1)
#define DISPLAY_BL_PIN      7   // Backlight PWM
#define DISPLAY_WIDTH       240
#define DISPLAY_HEIGHT      240
#define DISPLAY_SPI_FREQ    40000000UL  // Hz

// I2C/SPI Dual-Use Correspondence (ESP32-S3):
// - GPIO41 (I2C_SDA) = data line; can be MOSI in SPI (data from master to slave)
// - GPIO42 (I2C_SCL) = clock line; can be SCK in SPI
// - GPIO11 (Display MOSI) can also be I2C_SDA if reassigning SPI pins
// - GPIO12 (Display SCK) can also be I2C_SCL if reassigning SPI pins
// Note: SCL ≈ SCK (both clocks), SDA ≈ MOSI (data from master to slave)
// For displays labeled SDA/SCL instead of MOSI/SCK: SDA=MOSI, SCL=SCK (MISO typically unused)

// ============================================================
// PIN MAPPING FOR NEOPIXEL (ESP32-S3 ONLY)
// ============================================================
#define NEOPIXEL_PIN        48  // NeoPixel data pin (RTC_GPIO18, MANDATORY on S3)
#define NEOPIXEL_COUNT      1   // Number of LEDs
#define NEOPIXEL_BRIGHTNESS 32  // Brightness 0-255

#elif defined(ENV_ESP32_DEVKIT)

#define BUTTON_BOOT_PIN     0   // BOOT button, long press to reboot (RTC_GPIO0, INPUT_PULLUP safe)
#define BUTTON_USER1_PIN    32  // User button 1 (ADC1_CH4, RTC_GPIO9)
#define BUTTON_USER2_PIN    33  // User button 2 (ADC1_CH5, RTC_GPIO8)

// ============================================================
// PIN MAPPING FOR DISPLAY (ST7789, SPI - VSPI/SPI3)
// ============================================================
#define DISPLAY_SCK_PIN     18  // SPI clock (VSPI_CLK, SPI3_CLK, SCK)
#define DISPLAY_MOSI_PIN    23  // SPI MOSI (VSPI_MOSI, SPI3_MOSI, MOSI, SDA)
#define DISPLAY_MISO_PIN    19  // SPI MISO (VSPI_MISO, SPI3_MISO, MISO, SCL)
#define DISPLAY_CS_PIN      15  // Chip select (MTDO, VSPI_CS0, SPI3_CS0, CS)
#define DISPLAY_DC_PIN      16  // Data/Command (U2RXD, RXD2)
#define DISPLAY_RST_PIN     14  // Reset (TMS, MTMS, CLK)
#define DISPLAY_BL_PIN      27  // Backlight PWM (RTC_GPIO17)
#define DISPLAY_WIDTH       240
#define DISPLAY_HEIGHT      240
#define DISPLAY_SPI_FREQ    27000000UL  // Hz

// I2C/SPI Dual-Use Correspondence (ESP32 DevKit):
// - GPIO21 (I2C_SDA) = data line; can be MOSI in SPI (data from master to slave)
// - GPIO22 (I2C_SCL) = clock line; can be SCK in SPI
// Note: SCL ≈ SCK (both clocks), SDA ≈ MOSI (data from master to slave)
// For displays labeled SDA/SCL instead of MOSI/SCK: SDA=MOSI, SCL=SCK (MISO typically unused)

// ============================================================
// NEOPIXEL NOT AVAILABLE ON ESP32 DEVKIT
// ============================================================
#define NEOPIXEL_AVAILABLE  0

#else
#error "Board configuration missing. Define ENV_ESP32S3_N16R8 or ENV_ESP32_DEVKIT in build flags."
#endif
