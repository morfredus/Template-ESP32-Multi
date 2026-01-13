# Architecture Modulaire - Dépendances des Modules

Ce document montre les dépendances entre les modules pour mieux comprendre l'architecture.

## Diagramme des Dépendances des Modules

```
┌─────────────────────────────────────────────────────────────┐
│                        main.cpp                             │
│                    (Orchestrateur)                          │
└──────────────────┬──────────────────────────────────────────┘
                   │
    ┌──────────────┼──────────────────────┬────────────────┐
    ▼              ▼                      ▼                ▼
┌─────────┐   ┌─────────┐         ┌──────────────┐   ┌─────────┐
│ Psram   │   │Buttons  │         │ NeoPixel     │   │TftDisplay│
│  (1)    │   │  (2)    │         │   (3)        │   │   (4)    │
└─────────┘   └─────────┘         └──────────────┘   └────┬─────┘
                                                          │
                   ┌───────────────────────────────────────┘
                   │
                   ▼
         ┌─────────────────────┐
         │   UiState (5)       │
         │ (État Partagé)      │
         └─────────────────────┘
                   │
    ┌──────────────┼──────────────┐
    ▼              ▼              ▼
┌─────────┐   ┌─────────┐   ┌──────────────┐
│ Network │   │WebServer│   │  WebUI (7)   │
│  (6)    │   │  (8)    │   └──────────────┘
└────┬────┘   └────┬────┘          │
     │             │               ▼
     │             │        ┌──────────────────┐
     │             │        │ WebStyles (9)    │
     │             │        │ (CSS + JS)       │
     │             │        └──────────────────┘
     │             │
     └─────┬───────┘
           │
           ▼
    ┌─────────────────┐
    │   app.h (10)    │
    │ (Exports globaux)│
    └─────────────────┘
```

## Détails des Modules par Couche

### Couche Matériel/Système (Aucune dépendance)

```
psram.h
├─ Dépend uniquement de: Arduino, esp32-hal-psram
├─ Utilisé par: main.cpp
└─ Fournit: Initialisation PSRAM
```

```
buttons.h
├─ Dépend de: board_config.h, config.h
├─ Utilisé par: main.cpp, gestionnaire Buttons
└─ Fournit: Gestion des entrées boutons
```

```
neopixel.h
├─ Dépend de: config.h, Adafruit_NeoPixel
├─ Utilisé par: main.cpp, Network (statut LED)
└─ Fournit: Fonctions de contrôle LED
```

### Couche Affichage

```
tft_display.h
├─ Dépend de: config.h, UiState::State
├─ Utilisé par: main.cpp, Network (feedback boot)
└─ Fournit: Fonctions de rendu LCD
```

### Couche Gestion d'État

```
ui_state.h
├─ Dépend de: Arduino (IPAddress)
├─ Utilisé par: main.cpp, TftDisplay, Buttons
└─ Fournit: Accès à l'état global de l'UI
```

### Couche Réseau

```
network.h
├─ Dépend de: config.h, TftDisplay (pour écran boot)
├─ Utilisé par: main.cpp, WebServer (pour OTA)
└─ Fournit: Fonctions WiFi, mDNS, OTA
```

### Couche Web

```
web_server.h
├─ Dépend de: config.h, WebUI, Update
├─ Utilisé par: main.cpp
└─ Fournit: Gestion serveur HTTP
```

```
web_ui.h
├─ Dépend de: config.h, WebStyles
├─ Utilisé par: WebServer
└─ Fournit: Génération HTML et JSON
```

```
web_styles.h
├─ Dépend de: Arduino
├─ Utilisé par: WebUI
└─ Fournit: Chaînes CSS et JavaScript
```

### Exports

```
app.h
├─ Exporte: Objet global tft
└─ Utilisé par: Tous les modules accédant au TFT
```

## Matrice de Dépendances

```
Module           │ Psram │Button│Neo  │Tft  │State│Net  │Web  │UI  │Style
─────────────────┼───────┼──────┼─────┼─────┼─────┼─────┼─────┼────┼──────
main.cpp         │  X    │  X   │  X  │  X  │  X  │  X  │  X  │    │
Psram             │       │      │     │     │     │     │     │    │
Buttons          │       │      │     │     │  X  │     │     │    │
NeoPixel         │       │      │     │     │     │     │     │    │
TftDisplay       │       │      │     │     │  X  │     │     │    │
UiState          │       │      │     │     │     │     │     │    │
Network          │       │      │     │  X  │     │     │     │    │
WebServer        │       │      │     │     │     │  X  │  X  │ X  │
WebUI            │       │      │     │     │     │     │     │    │ X
WebStyles        │       │      │     │     │     │     │     │    │
```

Légende: X = dépend de

## Ordre d'Initialisation

Ordre critique (doit être respecté):

```
1. Psram::init()           # Mémoire d'abord
2. Buttons::init()         # Gestion entrée
3. TftDisplay::setupDisplay() # L'affichage doit être prêt
4. NeoPixel::init()        # LED optionnelle
5. Network::connectWifiWithFeedback() # Utilise TftDisplay
6. Network::initMdns()     # Services réseau
7. Network::initOta()      # OTA après mDNS
8. WebServer::init()       # Serveur Web
9. UiState mise à jour     # L'état reflète l'initialisation
```

Pourquoi cet ordre:
- ✅ Mémoire initialisée avant tout allocation
- ✅ Affichage prêt avant écrans de boot
- ✅ Connectivité réseau avant services Web
- ✅ Gestion d'état disponible pour tous

## Ordre d'Exécution dans la Boucle

```
loop() appelle:
  1. WebServer::handleClient()    # Haute priorité, non-bloquant
  2. ArduinoOTA.handle()          # Vérification OTA
  3. Buttons::handle()            # Entrée utilisateur
  4. TftDisplay::updateMainScreen() # Mise à jour affichage
  5. NeoPixel::setColor()         # Feedback LED
```

Pourquoi cet ordre:
- ✅ Réseau d'abord (sensible au temps)
- ✅ Entrée utilisateur avant affichage (réactif)
- ✅ Mise à jour affichage en dernier (état final)

## Exemples de Découplage

### Bon: Modules Découplés

```cpp
// Buttons ne connaît pas Network
// Network ne connaît pas Buttons
// Communication via UiState
```

### Bon: Dépendances Minimales

```cpp
// TftDisplay ne dépend que de:
//   - config.h (paramètres)
//   - UiState (données à afficher)
// Il n'inclut pas Network, WebServer, etc.
```

### Mauvais: Dépendances Circulaires (À ÉVITER!)

```cpp
// ❌ Ne pas faire:
// network.h inclut web_server.h
// web_server.h inclut network.h
// Crée dépendance circulaire!
```

## Ajouter un Nouveau Module

Quand vous ajoutez `my_module.h`:

1. **Identifier les dépendances**: Quels modules avez-vous besoin?
2. **Minimiser les dépendances**: Inclure seulement le nécessaire
3. **Mettre à jour l'ordre d'init**: Où initialiser?
4. **Mettre à jour les appels loop**: Quel travail périodique?
5. **Utiliser UiState**: Éviter appels module-à-module directs

Exemple:
```cpp
// my_module.h - Dépendances minimales
#pragma once
#include <Arduino.h>
// Inclure SEULEMENT config et Arduino, PAS autres modules

namespace MyModule {
  void init();
  void process();
}
```

## Implications de Performance

- **Includes en série**: Réduire temps de compilation
- **Instanciation template**: Le code header uniquement peut ralentir
- **État global**: Préférer UiState pour données partagées
- **Utilisation mémoire**: Les données statiques restent en RAM

## Tester un Module Seul

Vous pouvez compiler et tester un module isolé:

```cpp
// test/test_my_module.cpp
#include "my_module.h"

void setup() {
  Serial.begin(115200);
  MyModule::init();
}

void loop() {
  MyModule::process();
}
```

Cela fonctionne car chaque module est indépendant avec interfaces claires.

---

**Principe Clé**: Les modules doivent être indépendants et communiquer via interfaces bien définies (fonctions et UiState).
