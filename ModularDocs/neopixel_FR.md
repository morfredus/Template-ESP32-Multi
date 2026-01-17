# Module : neopixel (LED RGB)

## Rôle du module
Ce module gère la ou les LED RGB NeoPixel de la carte. Il permet d'afficher des couleurs pour indiquer l'état, donner un retour utilisateur ou créer des effets.

## Pourquoi séparer la gestion NeoPixel ?
- Pour garder la logique LED indépendante du reste du code.
- Pour pouvoir changer facilement les effets ou ajouter des LEDs.
- Pour réutiliser le code dans d'autres projets avec NeoPixel.

## Où se trouvent les fichiers ?
- `include/neopixel.h` : déclare les fonctions et constantes publiques.
- `src/neopixel.cpp` : contient le code de contrôle de la LED.

## Fonctions principales
- `void init()` : Initialise la ou les NeoPixel.
- `void setColor(uint32_t color)` : Définit la couleur de toutes les LEDs.
- `uint32_t makeColor(uint8_t r, uint8_t g, uint8_t b)` : Crée une couleur.
- `void setBrightness(uint8_t brightness)` : Définit la luminosité.
- `void clear()` : Éteint toutes les LEDs.

## Paramètres importants
- Le numéro de broche et le nombre de LEDs sont dans `board_config.h`.
- La luminosité par défaut est dans le code.

## Interactions avec les autres modules
- Utilisé par main et network pour indiquer l'état (WiFi OK, erreur, etc.).
- Peut servir de retour utilisateur (reboot, notifications).

## Bonnes pratiques
- Toujours initialiser avant utilisation.
- Utiliser des codes couleur clairs pour chaque état.
- Éviter la forte luminosité pour économiser l'énergie.

## Pour les débutants
- Si la LED ne s'allume pas, vérifiez le câblage et la broche dans `board_config.h`.
- Essayez de changer la couleur dans le code pour voir l'effet.

---

> Les NeoPixels sont un moyen ludique d'ajouter du feedback et du style. Ce module garde la gestion simple et sûre !
