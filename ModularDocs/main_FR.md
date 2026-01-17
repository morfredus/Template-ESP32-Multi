# Module : main (Point d'entrée du programme)

## Rôle du module
Le fichier `main.cpp` est le cœur du projet. Il orchestre l'initialisation de tous les modules, gère la boucle principale et coordonne les interactions entre les différentes parties du système.

## Pourquoi un fichier principal ?
- Pour centraliser le démarrage du système.
- Pour garder une vue d'ensemble sur l'ordre d'initialisation.
- Pour séparer la logique d'orchestration du détail de chaque module.

## Où se trouve le fichier ?
- `src/main.cpp`

## Fonctions principales

### 1. `void setup()`
Fonction appelée une seule fois au démarrage. Elle :
- Initialise la communication série (pour le debug)
- Initialise chaque module (PSRAM, boutons, affichage, NeoPixel, réseau, etc.)
- Affiche les écrans de boot et de progression
- Lance la connexion WiFi et les services réseau
- Gère la montée progressive du rétroéclairage

### 2. `void loop()`
Fonction appelée en boucle. Elle :
- Gère les requêtes réseau et web
- Gère les entrées utilisateur (boutons)
- Rafraîchit l'affichage périodiquement
- Met à jour l'état de la LED NeoPixel selon la connexion

## Paramètres importants
- Utilise les paramètres globaux définis dans `config.h` et l'état global dans `ui_state.h`

## Interactions avec les autres modules
- Appelle les fonctions d'initialisation et d'affichage de chaque module
- Met à jour l'état global de l'interface utilisateur
- Coordonne l'affichage, le réseau, les boutons, etc.

## Bonnes pratiques
- Garder la fonction `setup()` claire et séquentielle
- Ne pas surcharger la boucle `loop()` : déléguer au maximum aux modules
- Utiliser les logs série pour le debug

## Pour les débutants
- C'est le point de départ du programme : tout commence ici !
- Lisez bien l'ordre d'initialisation pour comprendre le fonctionnement global
- N'hésitez pas à ajouter des logs pour suivre le déroulement du boot

---

> Le fichier principal doit rester simple : il orchestre, il ne fait pas tout !
