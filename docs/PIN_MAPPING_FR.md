
# Guide de Mapping des Broches (FR)

> **Version minimale valide :** 0.4.4

---

> **Note :** Toutes les références aux boutons et GPIO dans le code sont désormais strictement conformes aux noms définis dans board_config.h (source officielle, immuable). Les constantes d'affichage (DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SPI_FREQ) sont définies pour ESP32 Classic et ESP32-S3.

## Guide du Debutant : Concepts Electriques Essentiels

### Bases de Tension et Courant
| Terme | Definition | Valeur ESP32 |
|-------|------------|--------------|
| **VCC** | Tension d'alimentation (positive) | 3.3V ou 5V |
| **GND** | Masse (reference 0V) | 0V |
| **Niveau logique** | Tension pour signaux HIGH/LOW | 3.3V |
| **Courant max GPIO** | Courant maximum par broche | 12mA (40mA max absolu) |

### Regles de Securite
- Ne **JAMAIS** depasser 3.3V sur une broche GPIO de l'ESP32
- **TOUJOURS** connecter GND entre l'ESP32 et les peripheriques
- Utiliser des resistances de protection si necessaire
- Ajouter des condensateurs de decouplage pres des broches d'alimentation

---

## Composants Requis par Peripherique

### Boutons Poussoirs

| Composant | Valeur | Puissance | Fonction |
|-----------|--------|-----------|----------|
| Bouton poussoir | NO (Normalement Ouvert) | - | Entree utilisateur |
| Resistance (optionnel) | 10kΩ | 1/4W (0.25W) | Pull-up (interne disponible) |
| Condensateur (optionnel) | 100nF ceramique | 16V+ | Anti-rebond materiel |

**Schema de Cablage :**
```
    3.3V ──────┬────────────────── (pull-up interne active)
               │
              [R] 10kΩ (optionnel avec INPUT_PULLUP)
               │
    GPIO ──────┴──────┤ ├────────── GND
                     BOUTON
```

**Fonctionnement :**
- Bouton NON appuye : GPIO lit HIGH (3.3V via pull-up)
- Bouton appuye : GPIO lit LOW (0V connecte a GND)

---

### Ecran LCD ST7789 (SPI)

| Composant | Valeur | Puissance | Fonction |
|-----------|--------|-----------|----------|
| Ecran ST7789 | 240x240 ou 240x320 | - | Affichage visuel |
| Condensateur | 100nF ceramique | 16V+ | Filtrage alimentation |
| Condensateur | 10µF electrolytique | 6.3V+ | Stabilisation alimentation |
| Transistor (optionnel) | NPN (2N2222) | 500mW | Retroeclairage >20mA |
| Resistance (optionnel) | 1kΩ | 1/4W | Base transistor |

**Tension :** 3.3V (logique et alimentation)

**Etiquettes Courantes sur les Modules (Alias) :**
| Signal | Alias Courants sur les Modules |
|--------|-------------------------------|
| SCK | CLK, SCLK, SCL* |
| MOSI | SDA*, DIN, SDI, DATA |
| MISO | SDO, DOUT |
| CS | CE, SS, CSN |
| DC | D/C, RS, A0, CMD |
| RST | RES, RESET |
| BL | LED, BLK, BACKLIGHT |

> *Attention : SDA/SCL sur les ecrans SPI ≠ broches SDA/SCL de I2C !

**Retroeclairage avec Transistor (si >20mA) :**
```
    GPIO ──[1kΩ]──► Base (B)
    VCC ──────────► Collecteur (C) ──► LED+ du module
    GND ◄─────────── Emetteur (E)
```

---

### LED RGB NeoPixel (WS2812B)

| Composant | Valeur | Puissance | Fonction |
|-----------|--------|-----------|----------|
| LED NeoPixel | WS2812B | - | Retour RGB |
| Resistance | 300-500Ω | 1/4W | Protection signal |
| Condensateur | 100µF electrolytique | 6.3V+ | Stabilisation alimentation |
| Condensateur (chaine) | 100nF ceramique | 16V+ | Par LED (longues chaines) |

**Tensions :**
- Alimentation (VCC) : 5V recommande (3.3V fonctionne mais moins lumineux)
- Signal de donnees : 3.3V (compatible ESP32 sans level shifter)
- Courant : ~60mA par LED a pleine luminosite blanche

**Schema de Cablage :**
```
    5V ────┬────────────────────► VCC NeoPixel
           │
          [C] 100µF 6.3V+ (electrolytique)
           │
    GND ───┴────────────────────► GND NeoPixel

    GPIO ──[R 330Ω]─────────────► DIN NeoPixel
                                     │
                                     └──► DOUT vers NeoPixel suivant
```

**Etiquettes des Broches Module :**
| Signal | Alias Courants |
|--------|----------------|
| DIN | DATA, DI, D, IN |
| DOUT | DO, OUT |

---

## ESP32 DevKit

| Fonction | GPIO | Alias Fonctionnel | Type | Notes |
|----------|------|-------------------|------|-------|
| **Bouton BOOT** | 0 | RTC_GPIO0 | INPUT_PULLUP | Appui long 3s = reboot |
| **Bouton User 1** | 32 | ADC1_CH4, RTC_GPIO9 | INPUT_PULLUP | A utiliser librement |
| **Bouton User 2** | 33 | ADC1_CH5, RTC_GPIO8 | INPUT_PULLUP | A utiliser librement |
| **Ecran SCK** | 18 | VSPI_CLK, SPI3_CLK, SCK | OUTPUT (SPI) | Horloge LCD (VSPI) |
| **Ecran MOSI** | 23 | VSPI_MOSI, SPI3_MOSI, MOSI | OUTPUT (SPI) | Donnees LCD (VSPI) |
| **Ecran MISO** | 19 | VSPI_MISO, SPI3_MISO, MISO | INPUT (SPI) | Donnees LCD (VSPI, optionnel) |
| **Ecran CS** | 5 | VSPI_CS0, SPI3_CS0, CS | OUTPUT | Selection puce |
| **Ecran DC** | 16 | U2RXD, RXD2 | OUTPUT | Donnees/Commande |
| **Ecran RST** | 17 | U2TXD, TXD2 | OUTPUT | Reinitialisation |
| **Ecran BL** | 4 | RTC_GPIO10 | OUTPUT (PWM) | Retroeclairage |

**Schema de Cablage :**
```
    ESP32 DevKit          Module ST7789
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

**Broches SPI utilisees :** 6 (VSPI/SPI3)
**I2C defaut :** SDA=21, SCL=22
**UART0 :** TX=1, RX=3
**UART1 :** TX=10, RX=9
**UART2 :** TX=17, RX=16
**GPIO libres :** Nombreuses disponibles pour capteurs/modules
**Securite :** Aucune broche de strapping en sortie (GPIO0 INPUT_PULLUP = sur)

### Correspondance I2C/SPI (ESP32 DevKit)
- **GPIO21 (I2C_SDA)** = ligne donnees; peut etre reassignee en MOSI en SPI (donnees maitre vers esclave)
- **GPIO22 (I2C_SCL)** = ligne horloge; peut etre reassignee en SCK en SPI
- **Note :** SCL ≈ SCK (tous deux horloges), SDA ≈ MOSI (donnees maitre vers esclave)
- **Pour ecrans etiquetes SDA/SCL au lieu de MOSI/SCK :** SDA=MOSI, SCL=SCK (MISO generalement non utilise sur les ecrans)

---

## ESP32-S3 DevKitC-1 N16R8

| Fonction | GPIO | Alias Fonctionnel | Type | Notes |
|----------|------|-------------------|------|-------|
| **Bouton BOOT** | 0 | RTC_GPIO0 | INPUT_PULLUP | Appui long 3s = reboot |
| **Bouton User 1** | 16 | U0RTS | INPUT_PULLUP | A utiliser librement |
| **Bouton User 2** | 17 | U0CTS | INPUT_PULLUP | A utiliser librement |
| **NeoPixel** | 48 | RTC_GPIO18 | OUTPUT | LED RGB (obligatoire) |
| **Ecran SCK** | 12 | HSPI_CLK, SPI2_CLK, SCK | OUTPUT (SPI) | Horloge LCD |
| **Ecran MOSI** | 11 | HSPI_MOSI, SPI2_MOSI, MOSI | OUTPUT (SPI) | Donnees LCD |
| **Ecran MISO** | 15 | HSPI_MISO, SPI2_MISO, MISO | INPUT (SPI) | Donnees LCD (optionnel) |
| **Ecran CS** | 10 | HSPI_CS0, SPI2_CS0, CS | OUTPUT | Selection puce |
| **Ecran DC** | 17 | U0CTS | OUTPUT | Donnees/Commande |
| **Ecran RST** | 18 | U0RTS | OUTPUT | Reinitialisation |
| **Ecran BL** | 16 | RTC_GPIO16 | OUTPUT (PWM) | Retroeclairage |

**Schema de Cablage :**
```
    ESP32-S3              Module ST7789
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

**Broches SPI utilisees :** 6 (HSPI/SPI2)
**I2C defaut :** SDA=41, SCL=42
**UART0 :** TX=43, RX=44
**UART1 :** TX=2, RX=1
**NeoPixel :** 1 GPIO (fixe sur S3)
**GPIO libres :** Nombreuses disponibles pour capteurs/modules

### Correspondance I2C/SPI (ESP32-S3)
- **GPIO41 (I2C_SDA)** = ligne donnees; peut etre reassignee en MOSI en SPI (donnees maitre vers esclave)
- **GPIO42 (I2C_SCL)** = ligne horloge; peut etre reassignee en SCK en SPI
- **GPIO11 (Ecran MOSI)** peut aussi etre I2C_SDA en reassignant les broches SPI (ligne donnees ↔ ligne donnees)
- **GPIO12 (Ecran SCK)** peut aussi etre I2C_SCL en reassignant les broches SPI (ligne horloge ↔ ligne horloge)
- **Note :** SCL ≈ SCK (tous deux horloges), SDA ≈ MOSI (donnees maitre vers esclave)
- **Pour ecrans etiquetes SDA/SCL au lieu de MOSI/SCK :** SDA=MOSI, SCL=SCK (MISO generalement non utilise sur les ecrans)

---

## Broches de Strapping (A EVITER EN SORTIE)
Ces broches sont lues au demarrage pour le mode boot :
- **ESP32 :** 0, 2, 4, 5, 12, 15 (deja utilisees ou evitees dans le template)
- **ESP32-S3 :** 45, 46 (a eviter si possible)

---

## Ajouter vos Propres Broches
Pour un nouveau capteur, mettre a jour `board_config.h` :
```cpp
#if defined(ENV_ESP32_DEVKIT)
#define MON_CAPTEUR_PIN  34  // Entree ADC
#elif defined(ENV_ESP32S3_N16R8)
#define MON_CAPTEUR_PIN  3   // Entree ADC
#endif
```

Puis utiliser dans le code :
```cpp
#include "board_config.h"

pinMode(MON_CAPTEUR_PIN, INPUT);
int valeur = analogRead(MON_CAPTEUR_PIN);
```

---

## Notes SPI
Les deux cartes utilisent le **meme protocole SPI** pour l'ecran. La frequence est ajustee :
- **ESP32 :** 27 MHz
- **ESP32-S3 :** 40 MHz

Si vous ajoutez un autre peripherique SPI, partagez les lignes SCK/MOSI/MISO et utilisez un CS different.

---

## Broches ADC
**ESP32 DevKit** ADC1 : 32-39 (GPIO32-39)
**ESP32-S3** ADC1 : 1-10 (GPIO1-10), ADC2 : 11-20 (GPIO11-20)

---

## Reference des Alias Fonctionnels

### Peripheriques SPI
- **ESP32 DevKit** utilise VSPI (SPI3) : SCK=18, MOSI=23, MISO=19, CS flexible
- **ESP32-S3** utilise HSPI (SPI2) : SCK=12, MOSI=11, MISO=13, CS flexible

Alias SPI courants : **SCK**, **MOSI**, **MISO**, **CS**

### Peripheriques I2C
- **ESP32 DevKit** defaut : SDA=21 (U0CTS), SCL=22 (U0RTS)
- **ESP32-S3** defaut : SDA=41, SCL=42

N'importe quelle paire GPIO peut etre configuree en I2C logiciel.

### Peripheriques UART
- **ESP32 DevKit :**
  - UART0 : TX=1 (U0TXD), RX=3 (U0RXD) — utilise par moniteur serie
  - UART1 : TX=10, RX=9
  - UART2 : TX=17 (U2TXD), RX=16 (U2RXD)
- **ESP32-S3 :**
  - UART0 : TX=43, RX=44 — utilise par moniteur serie
  - UART1 : TX=2, RX=1
  - UART2 : A configurer librement

### Broches JTAG/Debug (A eviter si possible)
- **TCO** (Test Clock Out) alias : MTMS, TMS
- **TDI** (Test Data In) alias : MTDI
- **TDO** (Test Data Out) alias : MTDO
- **TCK** (Test Clock In) alias : MTCK

### Broches RTC (Ultra-basse consommation)
- Conservent l'etat pendant sommeil profond
- Format alias : `RTC_GPIO[N]` ou N differe du numero GPIO
- Exemple : GPIO0 = RTC_GPIO0, GPIO32 = RTC_GPIO9

### DAC (Numerique vers Analogique)
- **ESP32 DevKit :** GPIO25 (DAC1), GPIO26 (DAC2)
- **ESP32-S3 :** GPIO17 (DAC1, RST ecran), GPIO18 (DAC2, BUTTON template)

### GPIO Tactiles (capteur capacitif)
- **ESP32 DevKit :** GPIO4, GPIO0, GPIO2, GPIO15, GPIO13, GPIO12, GPIO14, GPIO27, GPIO33, GPIO32
- Utilisables pour pave tactile sans boutons physiques

---

## Alternative I2C
Pour les capteurs I2C, tout paire GPIO libre convient :
- **SDA/SCL recommandes :** GPIO21/22 (ESP32) ou GPIO41/42 (S3)

Voir [ADD_MODULES_FR.md](ADD_MODULES_FR.md) pour exemples d'integration.
