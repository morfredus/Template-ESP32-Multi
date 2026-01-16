#pragma once
#include <Arduino.h>

// ╔══════════════════════════════════════════════════════════════════════════════╗
// ║                        GUIDE DEBUTANT - BOARD_CONFIG.H                       ║
// ╠══════════════════════════════════════════════════════════════════════════════╣
// ║  Ce fichier definit le "cablage" entre l'ESP32 et les peripheriques.         ║
// ║                                                                              ║
// ║  VOCABULAIRE ESSENTIEL :                                                     ║
// ║  ───────────────────────                                                     ║
// ║  GPIO  = General Purpose Input/Output (broche programmable)                  ║
// ║  PIN   = Broche physique sur la carte                                        ║
// ║  3.3V  = Tension logique de l'ESP32 (JAMAIS depasser 3.3V sur une GPIO !)    ║
// ║  GND   = Masse electrique (0V, reference commune)                            ║
// ║  VCC   = Alimentation positive (3.3V ou 5V selon le composant)               ║
// ║                                                                              ║
// ║  REGLES DE SECURITE :                                                        ║
// ║  ────────────────────                                                        ║
// ║  - Ne JAMAIS connecter plus de 3.3V sur une GPIO de l'ESP32                  ║
// ║  - Ne JAMAIS tirer plus de 12mA par GPIO (40mA max absolu)                   ║
// ║  - Toujours connecter GND entre l'ESP32 et les peripheriques                 ║
// ║  - Utiliser des resistances de protection si necessaire                      ║
// ╚══════════════════════════════════════════════════════════════════════════════╝

// ============================================================================
// CONFIGURATION ESP32-S3 DevKitC-1 N16R8
// ============================================================================
#if defined(ENV_ESP32S3_N16R8)

// ────────────────────────────────────────────────────────────────────────────
// BOUTONS POUSSOIRS
// ────────────────────────────────────────────────────────────────────────────
//
// SCHEMA DE CABLAGE D'UN BOUTON :
//
//     3.3V ──────┬────────────────── (resistor pull-up interne active)
//                │
//               [R] 10kΩ (optionnel si INPUT_PULLUP)
//                │
//     GPIO ──────┴──────┤ ├────────── GND
//                      BOUTON
//
// FONCTIONNEMENT :
//   - Bouton NON appuye : GPIO lit HIGH (3.3V via resistor pull-up)
//   - Bouton appuye     : GPIO lit LOW  (0V connecte a GND)
//
// COMPOSANTS NECESSAIRES :
//   - Bouton poussoir NO (Normally Open)
//   - Resistor 10kΩ 1/4W (optionnel car INPUT_PULLUP utilise ~45kΩ interne)
//   - Condensateur 100nF ceramique (optionnel, anti-rebond materiel)
//
// TENSION : 3.3V (logique ESP32)
// ────────────────────────────────────────────────────────────────────────────

// ────────────────────────────────────────────────────────────────────────────
// BOUTONS
// ────────────────────────────────────────────────────────────────────────────
#define BUTTON_BOOT_PIN      0   // BOOT/IO0 integre. Note: Utilisé aussi pour le flash.
                                 // Usage : Appui long 3s = redemarrage systeme.

#define BUTTON_USER1_PIN    16   // Bouton utilisateur 1 (Libre, INPUT_PULLUP).
#define BUTTON_USER2_PIN    17   // Bouton utilisateur 2 (Libre, INPUT_PULLUP).

// ────────────────────────────────────────────────────────────────────────────
// ECRAN LCD ST7789 (Interface SPI) - OPTIMISÉ POUR N16R8
// ────────────────────────────────────────────────────────────────────────────
// Note pour N16R8 : GPIO 10-14 evite les conflits avec la PSRAM Octal.
// ────────────────────────────────────────────────────────────────────────────
#define DISPLAY_CS_PIN      10   // Selection ecran (CS/CE). Actif LOW.
#define DISPLAY_MOSI_PIN    11   // Données ESP32 -> Ecran (SDA/DIN).
#define DISPLAY_SCK_PIN     12   // Horloge SPI (SCLK/CLK).
#define DISPLAY_DC_PIN      17   // Data/Command (RS/A0). LOW=Cmd, HIGH=Pixel.
#define DISPLAY_RST_PIN     18   // Reset materiel (RES). Pulse LOW pour reset.
#define DISPLAY_MISO_PIN    15   // Donnees Ecran -> ESP32 (SDO). Souvent inutilise.
#define DISPLAY_BL_PIN      16   // Retroeclairage (BLK/LED). PWM possible.
                                 // Situe cote droit pour equilibrer le cablage.

#define DISPLAY_WIDTH      240   // Largeur ecran pixels.
#define DISPLAY_HEIGHT     240   // Hauteur ecran pixels.
#define DISPLAY_SPI_FREQ   40000000UL // 40 MHz : stable et rapide sur S3.

// ────────────────────────────────────────────────────────────────────────────
// LED RGB NEOPIXEL (WS2812B)
// ────────────────────────────────────────────────────────────────────────────
#define NEOPIXEL_PIN        48   // LED RGB integree nativement sur le S3 DevKitC-1.
#define NEOPIXEL_COUNT       1   // 1 seule LED sur la carte.
#define NEOPIXEL_BRIGHTNESS 32   // Intensite (0-255). 32 = economie d'energie.

// ────────────────────────────────────────────────────────────────────────────
// RAPPEL CABLAGE ALIMENTATION
// ────────────────────────────────────────────────────────────────────────────
// ESP32 3.3V  ------> VCC Ecran
// ESP32 GND   ------> GND Ecran
// Condensateur 100nF entre VCC et GND recommande pour stabiliser l'affichage.
// ────────────────────────────────────────────────────────────────────────────

// ============================================================================
// CONFIGURATION ESP32 DEVKIT V1 (WROOM-32)
// ============================================================================
#elif defined(ENV_ESP32_DEVKIT)

// ────────────────────────────────────────────────────────────────────────────
// BOUTONS POUSSOIRS
// ────────────────────────────────────────────────────────────────────────────
// (Voir explications detaillees dans section ESP32-S3 ci-dessus)
//
// TENSION : 3.3V (logique ESP32)
// COMPOSANTS : Bouton NO + optionnel 10kΩ pull-up + 100nF anti-rebond
// ────────────────────────────────────────────────────────────────────────────

// ────────────────────────────────────────────────────────────────────────────
// BOUTONS (ESP32 CLASSIC)
// ────────────────────────────────────────────────────────────────────────────
#define BUTTON_BOOT_PIN      0   // BOOT integre. Note: Doit etre HIGH au boot.
                                 // Usage : Appui long 3s = redemarrage.

#define BUTTON_USER1_PIN    32   // Bouton 1. Safe, accepte Analogique (ADC1_4).
#define BUTTON_USER2_PIN    33   // Bouton 2. Safe, accepte Analogique (ADC1_5).
                                 // Note : Les pins 34-39 sont Entree SEULEMENT.

// ────────────────────────────────────────────────────────────────────────────
// ECRAN LCD ST7789 (Interface SPI via bus VSPI)
// ────────────────────────────────────────────────────────────────────────────
// Le bus VSPI est le standard recommandé pour les écrans sur ESP32 classique.
// ────────────────────────────────────────────────────────────────────────────
#define DISPLAY_SCK_PIN     18   // Horloge SPI (SCLK). Bus VSPI natif.
#define DISPLAY_MOSI_PIN    23   // Donnees vers ecran (SDA). Bus VSPI natif.
#define DISPLAY_MISO_PIN    19   // Retour donnees (SDO). Optionnel.
#define DISPLAY_CS_PIN       5   // Selection ecran (CS). Pin natif VSPI.
#define DISPLAY_DC_PIN      16   // Data/Command. Safe (UART2 RX).
#define DISPLAY_RST_PIN     17   // Reset materiel. Safe (UART2 TX).
#define DISPLAY_BL_PIN       4   // Retroeclairage. Supporte PWM (LEDC).

#define DISPLAY_WIDTH      240   
#define DISPLAY_HEIGHT     240   
#define DISPLAY_SPI_FREQ   27000000UL // 27MHz est la limite stable sans DMA.

// ────────────────────────────────────────────────────────────────────────────
// LED RGB NEOPIXEL
// ────────────────────────────────────────────────────────────────────────────
#define NEOPIXEL_AVAILABLE   0   // Non integre sur DevKit classique.
#define NEOPIXEL_PIN        15   // Si vous en ajoutez une, GPIO 15 est libre.
#define NEOPIXEL_COUNT       1
// ============================================================================
// ERREUR : CARTE NON CONFIGUREE
// ============================================================================
#else
#error "Board configuration missing. Define ENV_ESP32S3_N16R8 or ENV_ESP32_DEVKIT in build flags."
#endif
