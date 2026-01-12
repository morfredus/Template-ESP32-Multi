# Système d'écran LCD (FR)

## Vue d'ensemble
Le template utilise un écran **Adafruit ST7789** 240×240 SPI avec la librairie Adafruit GFX. Trois écrans :
1. **Écran de boot** — Nom, version, barre de progression
2. **Écran de statut** — Infos WiFi, IP, nom mDNS
3. **Invite de reboot** — Feedback appui long sur BOOT

## Connexion de l'écran
Voir [PIN_MAPPING_FR.md](PIN_MAPPING_FR.md) pour les GPIO par carte.

## Configuration
Éditer `include/config.h` :
```cpp
.lcdRotation = 2,           // 0-3 (0=portrait, 2=paysage)
.backlightLevel = 255,      // 0-255 (PWM)
.enableBootBar = true,      // Barre de progression au boot
```

## Déroulement du boot
1. **Démarrage** → ST7789 initialisé, affiche "Boot" 5%
2. **Connexion WiFi** → Affiche "Connecting WiFi" (15% → 70%)
3. **Résultat WiFi** → "WiFi OK" (NeoPixel vert) ou "WiFi fail" (rouge)
4. **Serveur web** → Affiche "Starting web" (70% → 85%)
5. **Prêt** → Affiche "Ready" (100%), puis écran principal

## Écran principal
```
Nom du projet
v0.1.0

SSID: MonRéseau
IP: 192.168.1.100
mDNS: esp32-template
```

## Écran de reboot
Appui long sur BOOT (GPIO 0) :
```
Reboot ?
[████████░░░░] 60%
```
- Tenir 3 secondes pour redémarrer
- Relâcher avant pour annuler

## Personnaliser l'écran

### Changer les messages de boot
Éditer `src/main.cpp` dans les appels `drawBootScreen()` :
```cpp
drawBootScreen("WiFi", 15);           // Changer le texte
drawBootScreen("WiFi OK", 70);        // Changer le texte de progression
```

### Modifier l'écran principal
Éditer la fonction `updateMainScreen()` dans `src/main.cpp` :
```cpp
tft.setCursor(10, 80);
tft.print("SSID: ");
tft.print(ssidNow);
// Ajouter plus de lignes ou modifier positions ici
```

### Couleurs disponibles
```cpp
ST77XX_BLACK      ST77XX_BLUE       ST77XX_RED
ST77XX_GREEN      ST77XX_CYAN       ST77XX_MAGENTA
ST77XX_YELLOW     ST77XX_WHITE      ST77XX_ORANGE
```

Exemple :
```cpp
tft.setTextColor(ST77XX_YELLOW);  // Changer couleur texte
tft.fillScreen(ST77XX_BLACK);     // Remplir en noir
```

### Tailles de texte
```cpp
tft.setTextSize(1);  // Petit
tft.setTextSize(2);  // Moyen (défaut)
tft.setTextSize(3);  // Grand
```

## Notes de performance
- Pas de boucles d'effacement plein écran en runtime
- Mises à jour throttlées à ~1 seconde (voir `loop()`)
- Le PWM du rétroéclairage peut scintiller si non initialisé

## Dépannage

### Écran vide
- Vérifier connexions SPI (SCK, MOSI, CS, DC, RST)
- Vérifier broche rétroéclairage (BL) et initialisation PWM
- Réinitialiser la carte

### Texte ou couleurs brouillées
- Vérifier fréquence SPI dans `board_config.h` (27 MHz DevKit, 40 MHz S3)
- Vérifier rotation écran cohérente avec orientation physique
- Essayer fréquence SPI plus basse

### Scintillement ou tearing
- Pas de double buffer ; éviter redraws plein écran rapides
- Utiliser mises à jour ciblées (effacer régions spécifiques)
- Augmenter délai entre redraws

## Personnalisation avancée

### Ajouter des graphiques
```cpp
// Tracer une ligne
tft.drawLine(x0, y0, x1, y1, color);

// Tracer un cercle
tft.drawCircle(x, y, radius, color);

// Remplir un rectangle
tft.fillRect(x, y, w, h, color);

// Afficher un float
tft.print(3.14159);
```

Voir [documentation Adafruit GFX](https://github.com/adafruit/Adafruit-GFX-Library).
