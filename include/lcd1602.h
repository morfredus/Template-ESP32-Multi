#pragma once
#include <LiquidCrystal_PCF8574.h>

namespace Lcd1602 {
	// Initialise l'écran LCD (adresse I2C 0x27 par défaut)
	void begin(uint8_t addr = 0x27, uint8_t cols = 16, uint8_t rows = 2);

	// Efface l'écran
	void clear();


	// Affiche un texte à la position (col, row)
	void print(const char* text, uint8_t col = 0, uint8_t row = 0);

	// Effet de défilement horizontal pour une ligne
	void scrollText(const char* text, uint8_t row = 0, uint16_t delayMs = 120);

	// Active/désactive le rétroéclairage
	void setBacklight(bool on);
}
