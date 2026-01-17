# Module : tft_display (Affichage LCD)

## Rôle du module
Ce module gère tout ce qui concerne l'écran LCD (affichage, rétroéclairage, messages, etc.). Il permet d'afficher des informations à l'utilisateur, de montrer l'état du système, et d'améliorer l'expérience visuelle.

## Pourquoi séparer l'affichage ?
- Pour pouvoir changer d'écran facilement sans toucher au reste du code.
- Pour éviter de mélanger la logique métier (WiFi, boutons, etc.) avec l'affichage.
- Pour rendre le code plus lisible et réutilisable.

## Où se trouvent les fichiers ?
- `include/tft_display.h` : déclare les fonctions et constantes publiques.
- `src/tft_display.cpp` : contient le code qui pilote l'écran.

## Fonctions principales

### 1. `void setupDisplay()`
Initialise l'écran LCD :
- Configure la communication SPI
- Définit la rotation de l'écran
- Allume le rétroéclairage (en mode économie d'énergie au boot)
- Affiche un écran de démarrage

### 2. `void drawBootScreen(const char* message, uint8_t percent)`
Affiche un écran de démarrage avec un message et une barre de progression (ex : "WiFi", "Ready", etc.).

### 3. `void updateMainScreen(bool force = false)`
Affiche l'écran principal avec les infos WiFi, IP, mDNS, etc. Rafraîchit l'affichage si besoin.

### 4. `void fadeBacklightToNormal(uint16_t durationMs = 500)`
Augmente progressivement la luminosité du rétroéclairage après le boot pour éviter les pics de courant.

### 5. `void initBacklight()`
Initialise le rétroéclairage en mode basse consommation au démarrage.

### 6. Fonctions utilitaires
- `printFit`, `printWrap`, `fitText` : pour afficher du texte qui s'adapte à la largeur de l'écran.

## Paramètres importants
- `backlightLevel` : luminosité normale de l'écran (0-255)
- `bootBacklightLevel` : luminosité au démarrage (0-255)
- `lcdRotation` : orientation de l'écran
- `enableBootBar` : affiche ou non la barre de progression au boot

Tous ces paramètres sont définis dans `config.h`.

## Interactions avec les autres modules
- Utilise les infos du WiFi (module `network`) pour afficher l'état réseau.
- Utilise l'état global de l'interface utilisateur (module `ui_state`).
- Peut être appelé par le module principal (`main.cpp`) pour afficher des messages à chaque étape du boot.

## Bonnes pratiques
- Toujours passer par les fonctions du module pour afficher ou modifier l'écran.
- Ne jamais manipuler directement l'objet `tft` en dehors de ce module.
- Adapter la luminosité selon l'environnement pour économiser l'énergie.

## Pour les débutants
- L'affichage est souvent la première chose visible : un bug ici se voit tout de suite !
- N'hésitez pas à modifier les messages ou la couleur de fond pour expérimenter.
- Si l'écran reste noir, vérifiez le câblage et les paramètres SPI dans `board_config.h`.

---

> Ce module est un exemple de bonne séparation des responsabilités : il ne fait qu'une chose, mais il le fait bien !
