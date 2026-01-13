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

#define BUTTON_BOOT_PIN     0   // Bouton BOOT integre a la carte
                                // Alias module : "BOOT", "EN", "IO0"
                                // Fonction ESP : RTC_GPIO0 (garde etat en deep sleep)
                                // Usage : Appui long 3s = redemarrage systeme
                                // Note : Utilise aussi pour flasher le firmware

#define BUTTON_USER1_PIN    16  // Bouton utilisateur 1 (a cabler soi-meme)
                                // Alias module : "IO16", "RX2" (sur certaines cartes)
                                // Fonction ESP : U0RTS (UART0 Request To Send)
                                // Libre d'usage, configurer en INPUT_PULLUP

#define BUTTON_USER2_PIN    17  // Bouton utilisateur 2 (a cabler soi-meme)
                                // Alias module : "IO17", "TX2" (sur certaines cartes)
                                // Fonction ESP : U0CTS (UART0 Clear To Send)
                                // Libre d'usage, configurer en INPUT_PULLUP

// ────────────────────────────────────────────────────────────────────────────
// ECRAN LCD ST7789 (Interface SPI)
// ────────────────────────────────────────────────────────────────────────────
//
// QU'EST-CE QUE LE SPI ?
// ──────────────────────
// SPI = Serial Peripheral Interface (bus serie synchrone)
// Permet de communiquer tres rapidement avec des peripheriques (ecrans, SD, etc.)
//
// SIGNAUX SPI :
//   SCK  = Serial Clock      : Horloge generee par le maitre (ESP32)
//   MOSI = Master Out Slave In : Donnees ESP32 → ecran
//   MISO = Master In Slave Out : Donnees ecran → ESP32 (souvent non utilise)
//   CS   = Chip Select       : Selection du peripherique (actif = LOW)
//
// ALIAS FREQUENTS SUR LES MODULES ECRANS :
// ─────────────────────────────────────────
//   SCK  = CLK, SCLK, SCL (attention : pas le SCL de I2C !)
//   MOSI = SDA, DIN, SDI, DATA (attention : pas le SDA de I2C !)
//   MISO = SDO, DOUT (rarement present sur ecrans)
//   CS   = SS, CE, CSN
//   DC   = D/C, RS, A0, CMD
//   RST  = RES, RESET
//   BL   = LED, BLK, BACKLIGHT
//
// SCHEMA DE CABLAGE ECRAN ST7789 :
//
//     ESP32-S3              Module ST7789
//     ────────              ─────────────
//     3.3V ─────────────────► VCC (alimentation)
//     GND ──────────────────► GND (masse)
//     GPIO12 (SCK) ─────────► SCK/CLK/SCLK
//     GPIO11 (MOSI) ────────► SDA/DIN/MOSI
//     GPIO10 (CS) ──────────► CS/CE/SS
//     GPIO9  (DC) ──────────► DC/RS/A0
//     GPIO8  (RST) ─────────► RST/RES/RESET
//     GPIO7  (BL) ──────────► BL/LED (via transistor si >20mA)
//
// COMPOSANTS NECESSAIRES :
//   - Ecran ST7789 240x240 (ou 240x320)
//   - Condensateur 100nF ceramique entre VCC et GND (filtrage)
//   - Condensateur 10µF electrolytique entre VCC et GND (stabilisation)
//   - Pour BL avec forte consommation : transistor NPN + resistor 1kΩ
//
// TENSION : 3.3V (logique et alimentation)
// ────────────────────────────────────────────────────────────────────────────

#define DISPLAY_SCK_PIN     12  // Horloge SPI (clock)
                                // Alias module : "CLK", "SCLK", "SCK", "SCL" (pas I2C!)
                                // Fonction ESP : HSPI_CLK, SPI2_CLK
                                // Signal : Front montant = lecture bit

#define DISPLAY_MOSI_PIN    11  // Donnees vers l'ecran (Master Out Slave In)
                                // Alias module : "SDA", "DIN", "SDI", "MOSI", "DATA"
                                // Fonction ESP : HSPI_MOSI, SPI2_MOSI
                                // Note : "SDA" sur ecran ≠ SDA de I2C !

#define DISPLAY_MISO_PIN    13  // Donnees depuis l'ecran (Master In Slave Out)
                                // Alias module : "SDO", "DOUT", "MISO"
                                // Fonction ESP : HSPI_MISO, SPI2_MISO
                                // Note : Rarement utilise sur ecrans (ecriture seule)

#define DISPLAY_CS_PIN      10  // Selection de l'ecran (Chip Select)
                                // Alias module : "CS", "CE", "SS", "CSN"
                                // Fonction ESP : HSPI_CS0, SPI2_CS0
                                // Actif LOW : 0V = ecran ecoute, 3.3V = ecran ignore

#define DISPLAY_DC_PIN      9   // Selection Donnee/Commande (Data/Command)
                                // Alias module : "DC", "D/C", "RS", "A0", "CMD"
                                // Fonction ESP : MTCK, anciennement DAC2
                                // LOW = commande, HIGH = donnee pixel

#define DISPLAY_RST_PIN     8   // Reinitialisation ecran (Reset)
                                // Alias module : "RST", "RES", "RESET"
                                // Fonction ESP : MTDI, anciennement DAC1
                                // Pulse LOW pour reinitialiser l'ecran

#define DISPLAY_BL_PIN      7   // Retroeclairage (Backlight PWM)
                                // Alias module : "BL", "BLK", "LED", "BACKLIGHT"
                                // Controle PWM : 0 = eteint, 255 = max
                                // Si LED >20mA : utiliser transistor NPN
                                // Schema transistor :
                                //   GPIO7 ──[1kΩ]──► Base (B)
                                //   VCC ──────────► Collecteur (C) ──► LED+ module
                                //   GND ◄─────────── Emetteur (E)

#define DISPLAY_WIDTH       240   // Largeur ecran en pixels
#define DISPLAY_HEIGHT      320   // Hauteur ecran en pixels
#define DISPLAY_SPI_FREQ    40000000UL  // Frequence SPI : 40 MHz (rapide pour S3)

// ────────────────────────────────────────────────────────────────────────────
// CORRESPONDANCE I2C / SPI (pour comprendre les alias)
// ────────────────────────────────────────────────────────────────────────────
// ATTENTION AUX CONFUSIONS !
// Les fabricants utilisent parfois les memes noms pour I2C et SPI :
//
//   I2C (2 fils)          SPI (4+ fils)
//   ───────────           ─────────────
//   SDA = donnees         MOSI = donnees maitre→esclave
//   SCL = horloge         SCK  = horloge
//                         MISO = donnees esclave→maitre
//                         CS   = selection chip
//
// Sur un ecran marque "SDA/SCL" utilisant SPI :
//   SDA de l'ecran = MOSI (donnees vers ecran)
//   SCL de l'ecran = SCK  (horloge)
// ────────────────────────────────────────────────────────────────────────────

// ────────────────────────────────────────────────────────────────────────────
// LED RGB NEOPIXEL (WS2812B)
// ────────────────────────────────────────────────────────────────────────────
//
// QU'EST-CE QU'UN NEOPIXEL ?
// ──────────────────────────
// LED RGB adressable avec puce WS2812B integree
// Un seul fil de donnees controle des centaines de LEDs en chaine
//
// SCHEMA DE CABLAGE :
//
//     5V ────┬────────────────────► VCC NeoPixel (5V recommande)
//            │
//           [C] 100µF 6.3V+ (electrolytique, stabilisation)
//            │
//     GND ───┴────────────────────► GND NeoPixel
//
//     GPIO48 ──[R 330Ω]───────────► DIN NeoPixel (Data In)
//                                      │
//                                      └──► DOUT vers NeoPixel suivant
//
// COMPOSANTS NECESSAIRES :
//   - LED NeoPixel WS2812B (integree sur ESP32-S3 DevKitC-1)
//   - Resistor 300-500Ω 1/4W sur ligne DIN (protection signal)
//   - Condensateur 100µF 6.3V+ electrolytique (VCC-GND, anti-pics)
//   - Pour chaine longue : condensateur 100nF par LED
//
// TENSIONS :
//   - Alimentation LED : 5V DC (peut fonctionner a 3.3V mais moins lumineux)
//   - Signal DIN : 3.3V (compatible avec ESP32 sans level shifter)
//   - Consommation : ~60mA par LED a pleine luminosite blanche
//
// ATTENTION :
//   - Sur ESP32-S3, GPIO48 est souvent connectee au NeoPixel integre
//   - Ne pas depasser 5V sur VCC, risque de destruction
// ────────────────────────────────────────────────────────────────────────────

#define NEOPIXEL_PIN        48  // Broche donnees NeoPixel
                                // Alias module : "DIN", "DATA", "DI", "D"
                                // Fonction ESP : RTC_GPIO18 (garde etat deep sleep)
                                // OBLIGATOIRE sur S3 : LED RGB integree

#define NEOPIXEL_COUNT      1   // Nombre de LEDs dans la chaine
#define NEOPIXEL_BRIGHTNESS 32  // Luminosite par defaut (0-255)
                                // Conseil : 32 = economie energie, 128 = moyen

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

#define BUTTON_BOOT_PIN     0   // Bouton BOOT integre a la carte
                                // Alias module : "BOOT", "IO0", "FLASH"
                                // Fonction ESP : RTC_GPIO0
                                // Usage : Appui long 3s = redemarrage
                                // Securite : INPUT_PULLUP obligatoire

#define BUTTON_USER1_PIN    32  // Bouton utilisateur 1 (a cabler soi-meme)
                                // Alias module : "IO32", "32", "ADC1_4"
                                // Fonction ESP : ADC1_CH4, RTC_GPIO9
                                // Supporte : entree analogique 0-3.3V

#define BUTTON_USER2_PIN    33  // Bouton utilisateur 2 (a cabler soi-meme)
                                // Alias module : "IO33", "33", "ADC1_5"
                                // Fonction ESP : ADC1_CH5, RTC_GPIO8
                                // Supporte : entree analogique 0-3.3V

// ────────────────────────────────────────────────────────────────────────────
// ECRAN LCD ST7789 (Interface SPI via VSPI/SPI3)
// ────────────────────────────────────────────────────────────────────────────
// (Voir explications detaillees dans section ESP32-S3 ci-dessus)
//
// SCHEMA DE CABLAGE ECRAN ST7789 :
//
//     ESP32 DevKit          Module ST7789
//     ────────────          ─────────────
//     3.3V ─────────────────► VCC
//     GND ──────────────────► GND
//     GPIO18 (SCK) ─────────► SCK/CLK/SCLK
//     GPIO23 (MOSI) ────────► SDA/DIN/MOSI
//     GPIO15 (CS) ──────────► CS/CE/SS
//     GPIO16 (DC) ──────────► DC/RS/A0
//     GPIO14 (RST) ─────────► RST/RES/RESET
//     GPIO27 (BL) ──────────► BL/LED
//
// COMPOSANTS : Voir section ESP32-S3 pour details
// TENSION : 3.3V (logique et alimentation)
// ────────────────────────────────────────────────────────────────────────────

#define DISPLAY_SCK_PIN     18  // Horloge SPI (clock)
                                // Alias module : "CLK", "SCLK", "SCK"
                                // Fonction ESP : VSPI_CLK, SPI3_CLK
                                // Bus VSPI natif de l'ESP32

#define DISPLAY_MOSI_PIN    23  // Donnees vers l'ecran (Master Out Slave In)
                                // Alias module : "SDA", "DIN", "SDI", "MOSI"
                                // Fonction ESP : VSPI_MOSI, SPI3_MOSI
                                // Rappel : "SDA" ici ≠ SDA du bus I2C

#define DISPLAY_MISO_PIN    19  // Donnees depuis l'ecran (Master In Slave Out)
                                // Alias module : "SDO", "DOUT", "MISO"
                                // Fonction ESP : VSPI_MISO, SPI3_MISO
                                // Note : Optionnel pour ecrans (non utilise)

#define DISPLAY_CS_PIN      15  // Selection de l'ecran (Chip Select)
                                // Alias module : "CS", "CE", "SS", "CSN"
                                // Fonction ESP : MTDO, VSPI_CS0, SPI3_CS0
                                // ATTENTION : Broche de strapping (boot)
                                // OK en OUTPUT apres demarrage

#define DISPLAY_DC_PIN      16  // Selection Donnee/Commande (Data/Command)
                                // Alias module : "DC", "D/C", "RS", "A0"
                                // Fonction ESP : U2RXD (UART2 RX)
                                // LOW = commande, HIGH = donnee

#define DISPLAY_RST_PIN     14  // Reinitialisation ecran (Reset)
                                // Alias module : "RST", "RES", "RESET"
                                // Fonction ESP : TMS, MTMS (JTAG)
                                // Pulse LOW au demarrage

#define DISPLAY_BL_PIN      27  // Retroeclairage (Backlight PWM)
                                // Alias module : "BL", "BLK", "LED"
                                // Fonction ESP : RTC_GPIO17
                                // PWM : 0=eteint, 255=max luminosite

#define DISPLAY_WIDTH       240   // Largeur ecran en pixels
#define DISPLAY_HEIGHT      240   // Hauteur ecran en pixels
#define DISPLAY_SPI_FREQ    27000000UL  // Frequence SPI : 27 MHz (limite ESP32)

// ────────────────────────────────────────────────────────────────────────────
// CORRESPONDANCE I2C / SPI (ESP32 DevKit)
// ────────────────────────────────────────────────────────────────────────────
// Broches I2C par defaut : SDA=GPIO21, SCL=GPIO22
// Ces broches peuvent etre reassignees en SPI si besoin :
//   GPIO21 (I2C_SDA) → MOSI possible
//   GPIO22 (I2C_SCL) → SCK possible
// ────────────────────────────────────────────────────────────────────────────

// ────────────────────────────────────────────────────────────────────────────
// NEOPIXEL NON DISPONIBLE SUR ESP32 DEVKIT
// ────────────────────────────────────────────────────────────────────────────
// L'ESP32 DevKit classique n'a pas de NeoPixel integre.
// Vous pouvez en ajouter un sur n'importe quelle GPIO (ex: GPIO5).
// Voir section ESP32-S3 pour le schema de cablage.
// ────────────────────────────────────────────────────────────────────────────

#define NEOPIXEL_AVAILABLE  0   // 0 = NeoPixel non disponible sur cette carte

// ============================================================================
// ERREUR : CARTE NON CONFIGUREE
// ============================================================================
#else
#error "Board configuration missing. Define ENV_ESP32S3_N16R8 or ENV_ESP32_DEVKIT in build flags."
#endif
