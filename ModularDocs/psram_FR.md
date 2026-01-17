# Module : psram (Mémoire PSRAM)

## Rôle du module
Ce module gère la mémoire PSRAM externe (Pseudo-Static RAM) disponible sur certaines cartes ESP32. Il permet d'utiliser plus de mémoire pour les buffers, images ou fonctions avancées.

## Pourquoi séparer la gestion PSRAM ?
- Pour isoler l'initialisation et les vérifications mémoire du reste du code.
- Pour s'adapter facilement si la carte change (avec ou sans PSRAM).
- Pour rendre le code robuste et portable.

## Où se trouvent les fichiers ?
- `include/psram.h` : déclare les fonctions publiques.
- `src/psram.cpp` : contient le code de détection et d'utilisation de la PSRAM.

## Fonctions principales
- `void init()` : Initialise la PSRAM si disponible.
- `bool isAvailable()` : Retourne vrai si la PSRAM est détectée et utilisable.
- `size_t getSize()` : Retourne la taille de PSRAM disponible.

## Paramètres importants
- Aucun paramètre utilisateur ; la détection est automatique.

## Interactions avec les autres modules
- Utilisé par l'affichage ou le web pour de gros buffers.
- Peut être vérifié dans main pour adapter les fonctions si la PSRAM manque.

## Bonnes pratiques
- Toujours vérifier la disponibilité avant d'utiliser de gros buffers.
- Réserver la PSRAM aux fonctions gourmandes en mémoire.

## Pour les débutants
- Toutes les cartes n'ont pas de PSRAM ! Vérifiez la fiche technique.
- Si vous avez des erreurs mémoire, vérifiez la détection de la PSRAM au boot.

---

> La PSRAM donne des super-pouvoirs à votre ESP32 pour les gros projets. Ce module la rend facile à utiliser en toute sécurité !
