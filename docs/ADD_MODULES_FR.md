# Ajouter des modules (FR)

## Vue d'ensemble
Ce guide explique comment ajouter capteurs, modules et nouvelles fonctionnalités en gardant le code organisé.

## Structure des fichiers
```
src/
  main.cpp              ← Point d'entrée (setup, loop)
  [feature].cpp         ← Nouveau code de fonctionnalité
include/
  board_config.h        ← Définitions GPIO (NE PAS ÉDITER)
  config.h              ← Paramètres utilisateur
  [feature].h           ← Headers pour nouvelles fonctionnalités
lib/
  [library]/            ← Librairies locales personnalisées
```

## Ajouter un capteur simple (exemple DHT22)

### 1. Ajouter la librairie dans platformio.ini
```ini
[env]
lib_deps =
    adafruit/Adafruit DHT sensor library@^1.4.4
```

### 2. Définir la broche dans board_config.h
```cpp
// Ajouter après les définitions NeoPixel :

#if defined(ENV_ESP32S3_N16R8)
#define DHT_PIN  15  // GPIO de votre choix
#elif defined(ENV_ESP32_DEVKIT)
#define DHT_PIN  35  // GPIO de votre choix
#endif
```

### 3. Créer un fichier header include/dht_sensor.h
```cpp
#pragma once
#include <DHT.h>
#include "board_config.h"

extern DHT dhtSensor;

void initDHT();
float getTemperature();
float getHumidity();
```

### 4. Créer src/dht_sensor.cpp
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

### 5. Mettre à jour src/main.cpp
```cpp
#include "dht_sensor.h"

void setup() {
    // ... code existant ...
    initDHT();
    // ... reste du setup ...
}

void loop() {
    // ... code existant ...
    float temp = getTemperature();
    float humidity = getHumidity();
    // Utiliser temp et humidity
}
```

## Ajouter un appareil I2C (écran OLED)

### 1. Ajouter la librairie
```ini
[env]
lib_deps =
    adafruit/Adafruit SSD1306@^2.5.7
    adafruit/Adafruit Unified Sensor@^1.1.14
```

### 2. Ajouter les broches I2C (si nécessaire)
ESP32 et S3 ont des broches I2C par défaut :
```cpp
#include <Wire.h>
// Défaut : SDA=GPIO21, SCL=GPIO22 (ESP32)
//          SDA=GPIO41, SCL=GPIO42 (S3)
Wire.begin(SDA_PIN, SCL_PIN);
```

### 3. Initialiser et utiliser
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
    display.println("Bonjour!");
    display.display();
}
```

## Ajouter un gestionnaire de bouton personnalisé

### 1. Ajouter le bouton dans board_config.h (si absent)
```cpp
#define MON_BOUTON_PIN  25  // GPIO libr de votre choix
```

### 2. Gérer dans setup()
```cpp
pinMode(MON_BOUTON_PIN, INPUT_PULLUP);
```

### 3. Vérifier dans loop()
```cpp
void loop() {
    // ... code existant ...
    
    if (digitalRead(MON_BOUTON_PIN) == LOW) {
        // Bouton appuyé
        delay(20);  // Anti-rebond
        if (digitalRead(MON_BOUTON_PIN) == LOW) {
            // Appui confirmé
            monGestionnaireBouton();
        }
    }
}

void monGestionnaireBouton() {
    // Votre action ici
    Serial.println("Bouton appuyé!");
}
```

## Ajouter une tâche avec timing

Utiliser le pattern de timing existant dans `loop()` :
```cpp
static unsigned long lastCheck = 0;
const unsigned long INTERVAL_MS = 5000;  // Toutes les 5 secondes

void loop() {
    // ... code existant ...
    
    const unsigned long now = millis();
    if (now - lastCheck >= INTERVAL_MS) {
        lastCheck = now;
        maFonctionTache();
    }
}

void maFonctionTache() {
    // S'exécute toutes les 5 secondes
}
```

## Ajouter communication WiFi

Le template inclut déjà WiFiMulti. Ajouter requêtes HTTP :

```cpp
#include <HTTPClient.h>

void envoyer_vers_serveur(const String& data) {
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

## Conseils de performance
- Garder `loop()` rapide ; opérations longues à diviser
- Éviter les delay() bloquants ; utiliser patterns de timing
- Cacher valeurs fréquentes (température, ADC) avec intervalles
- Monitorer heap : `Serial.println(ESP.getFreeHeap())`

## Débogage
Ajouter affichage de débogage :
```cpp
#ifdef DEBUG
Serial.print("Debug: ");
Serial.println(value);
#endif
```

Compiler avec :
```ini
[env:esp32devkit]
build_flags = 
    ${env.build_flags}
    -D DEBUG
```

## Erreurs courantes
- Oublier #include de board_config.h
- Utiliser broches analogiques comme GPIO digital (restrictions ADC)
- Bloquer `loop()` avec delay() — utiliser timing patterns
- Ne pas vérifier WiFi avant opérations WiFi
- Fuites mémoire : `new` sans `delete`

Suite : Voir [PIN_MAPPING_FR.md](PIN_MAPPING_FR.md) pour GPIO disponibles.
