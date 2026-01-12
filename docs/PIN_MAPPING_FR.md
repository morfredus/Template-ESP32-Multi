# Mapping des broches (FR)

## ESP32 DevKit

| Fonction | GPIO | Alias fonctionnel | Type | Notes |
|----------|------|-------------------|------|-------|
| **Bouton BOOT** | 0 | RTC_GPIO0 | INPUT_PULLUP | Appui long 3s = reboot |
| **Bouton User 1** | 32 | ADC1_CH4, RTC_GPIO9 | INPUT_PULLUP | À utiliser librement |
| **Bouton User 2** | 33 | ADC1_CH5, RTC_GPIO8 | INPUT_PULLUP | À utiliser librement |
| **Écran SCK** | 18 | VSPI_CLK, SPI3_CLK, SCK | OUTPUT (SPI) | Horloge LCD (VSPI) |
| **Écran MOSI** | 23 | VSPI_MOSI, SPI3_MOSI, MOSI | OUTPUT (SPI) | Données LCD (VSPI) |
| **Écran MISO** | 19 | VSPI_MISO, SPI3_MISO, MISO | INPUT (SPI) | Données LCD (VSPI, optionnel) |
| **Écran CS** | 15 | MTDO, VSPI_CS0, SPI3_CS0, CS | OUTPUT | Sélection puce |
| **Écran DC** | 16 | U2RXD, RXD2 | OUTPUT | Données/Commande |
| **Écran RST** | 14 | TMS, MTMS, CLK | OUTPUT | Réinitialisation |
| **Écran BL** | 27 | RTC_GPIO17 | OUTPUT (PWM) | Rétroéclairage |

**Broches SPI utilisées :** 6 (VSPI/SPI3)  
**I2C défaut :** SDA=21, SCL=22  
**UART0 :** TX=1, RX=3  
**UART1 :** TX=10, RX=9  
**UART2 :** TX=17, RX=16  
**GPIO libres :** Nombreuses disponibles pour capteurs/modules  
**Sécurité :** Aucune broche de strapping en sortie (GPIO0 INPUT_PULLUP = sûr)

### Correspondance I2C/SPI (ESP32 DevKit)
- **GPIO21 (I2C_SDA)** peut partager avec données SPI si réaffectation
- **GPIO22 (I2C_SCL)** peut partager avec horloge SPI si réaffectation

## ESP32-S3 DevKitC-1 N16R8

| Fonction | GPIO | Alias fonctionnel | Type | Notes |
|----------|------|-------------------|------|-------|
| **Bouton BOOT** | 0 | RTC_GPIO0 | INPUT_PULLUP | Appui long 3s = reboot |
| **Bouton User 1** | 16 | U0RTS | INPUT_PULLUP | À utiliser librement |
| **Bouton User 2** | 17 | U0CTS | INPUT_PULLUP | À utiliser librement |
| **NeoPixel** | 48 | RTC_GPIO18 | OUTPUT | LED RGB (obligatoire) |
| **Écran SCK** | 12 | HSPI_CLK, SPI2_CLK, SCK | OUTPUT (SPI) | Horloge LCD |
| **Écran MOSI** | 11 | HSPI_MOSI, SPI2_MOSI, MOSI | OUTPUT (SPI) | Données LCD |
| **Écran MISO** | 13 | HSPI_MISO, SPI2_MISO, MISO | INPUT (SPI) | Données LCD (optionnel) |
| **Écran CS** | 10 | HSPI_CS0, SPI2_CS0, CS | OUTPUT | Sélection puce |
| **Écran DC** | 9 | MTCK, DAC2 | OUTPUT | Données/Commande |
| **Écran RST** | 8 | MTDI, DAC1 | OUTPUT | Réinitialisation |
| **Écran BL** | 7 | — | OUTPUT (PWM) | Rétroéclairage |

**Broches SPI utilisées :** 6 (HSPI/SPI2)  
**I2C défaut :** SDA=41, SCL=42  
**UART0 :** TX=43, RX=44  
**UART1 :** TX=2, RX=1  
**NeoPixel :** 1 GPIO (fixé sur S3)  
**GPIO libres :** Nombreuses disponibles pour capteurs/modules

### Correspondance I2C/SPI (ESP32-S3)
- **GPIO41 (I2C_SDA)** = ligne données; peut être réaffectée en MOSI ou MISO en SPI
- **GPIO42 (I2C_SCL)** = ligne horloge; peut être réaffectée en SCK en SPI, ou MOSI/MISO si horloge ailleurs
- **GPIO11 (Écran MOSI)** peut aussi être I2C_SDA en réaffectant broches SPI (ligne données ↔ ligne données)
- **GPIO12 (Écran SCK)** peut aussi être I2C_SCL en réaffectant broches SPI (ligne horloge ↔ ligne horloge)
- **Remarque :** SCL ≈ SCK (tous deux horloges), SDA ≈ MOSI/MISO (tous deux données)

## Broches de strapping (À ÉVITER EN SORTIE)
Ces broches sont lues au démarrage pour le mode boot :
- **ESP32 :** 0, 2, 4, 5, 12, 15 (déjà utilisées ou évitées)
- **ESP32-S3 :** 45, 46 (à éviter si possible)

## Ajouter vos propres broches
Pour un nouveau capteur, mettre à jour `board_config.h` :
```cpp
#if defined(ENV_ESP32_DEVKIT)
#define MON_CAPTEUR_PIN  34  // Entrée ADC
#elif defined(ENV_ESP32S3_N16R8)
#define MON_CAPTEUR_PIN  3   // Entrée ADC
#endif
```

Puis utiliser dans le code :
```cpp
#include "board_config.h"

pinMode(MON_CAPTEUR_PIN, INPUT);
int valeur = analogRead(MON_CAPTEUR_PIN);
```

## Notes SPI
Les deux cartes utilisent le **même protocole SPI** pour l'écran. La fréquence est ajustée :
- **ESP32 :** 27 MHz
- **ESP32-S3 :** 40 MHz

Si vous ajoutez un autre appareil SPI, partager SCK/MOSI/MISO et utiliser un CS différent.

## Broches ADC
**ESP32 DevKit** ADC1 : 32-39  
**ESP32-S3** ADC1 : 1-10, ADC2 : 11-20

## Référence alias fonctionnels

### Périphériques SPI
- **ESP32 DevKit** utilise VSPI (SPI3) : SCK=18, MOSI=23, MISO=19, CS flexible
- **ESP32-S3** utilise HSPI (SPI2) : SCK=12, MOSI=11, MISO=13, CS flexible

Alias SPI courants : **SCK**, **MOSI**, **MISO**, **CS**

### Périphériques I2C
- **ESP32 DevKit** défaut : SDA=21 (U0CTS), SCL=22 (U0RTS)
- **ESP32-S3** défaut : SDA=41, SCL=42

N'importe quelle paire GPIO peut être configurée en I2C logiciel.

### Périphériques UART
- **ESP32 DevKit :**
  - UART0 : TX=1 (U0TXD), RX=3 (U0RXD) — utilisé par moniteur série
  - UART1 : TX=10, RX=9
  - UART2 : TX=17 (U2TXD), RX=16 (U2RXD)
- **ESP32-S3 :**
  - UART0 : TX=43, RX=44 — utilisé par moniteur série
  - UART1 : TX=2, RX=1
  - UART2 : À configurer librement

### Broches JTAG/Debug (À éviter si possible)
- **TCO** (Test Clock Out) alias : MTMS, TMS
- **TDI** (Test Data In) alias : MTDI
- **TDO** (Test Data Out) alias : MTDO
- **TCK** (Test Clock In) alias : MTCK

### Broches RTC (Ultra-basse consommation)
- Conservent l'état pendant sommeil profond
- Format alias : `RTC_GPIO[N]` où N diffère du numéro GPIO
- Exemple : GPIO0 = RTC_GPIO0, GPIO32 = RTC_GPIO9

### DAC (Numérique vers Analogique)
- **ESP32 DevKit :** GPIO25 (DAC1), GPIO26 (DAC2)
- **ESP32-S3 :** GPIO17 (DAC1, RST écran), GPIO18 (DAC2, BUTTON template)

### GPIO tactiles (capteur capacitif)
- **ESP32 DevKit :** GPIO4, GPIO0, GPIO2, GPIO15, GPIO13, GPIO12, GPIO14, GPIO27, GPIO33, GPIO32
- Utilisables pour pavé tactile sans boutons physiques

## Alternative I2C
Pour les capteurs I2C, tout pair GPIO libre convient :
- **SDA/SCL recommandés :** GPIO21/22 (ESP32) ou GPIO41/42 (S3)

Voir [ADD_MODULES_FR.md](ADD_MODULES_FR.md) pour exemples d'intégration.
