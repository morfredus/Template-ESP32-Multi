# Module : ui_state (État de l’interface utilisateur)

## Rôle du module
Ce module gère l’état de l’interface utilisateur (UI) du projet ESP32. Il garde en mémoire ce qui est affiché, les interactions utilisateur et les transitions d’écran.

## Pourquoi séparer l’état UI ?
- Pour centraliser toutes les variables et la logique liées à l’UI.
- Pour faciliter la mise à jour de l’affichage ou de l’interface web selon l’état.
- Pour isoler la logique UI du matériel et du backend.

## Où se trouvent les fichiers ?
- `include/ui_state.h` : déclare les variables et fonctions d’état.
- `src/ui_state.cpp` : contient la logique de gestion et de mise à jour de l’état UI.

## Fonctions principales
- `void setState(UIState newState)` : Change l’état courant de l’UI.
- `UIState getState()` : Retourne l’état courant.
- `void update()` : Applique les changements d’état à l’affichage ou à l’UI web.

## Paramètres importants
- Les états UI sont définis comme des enums (ex : BOOT, HOME, ERROR, etc.).
- Les transitions d’état sont gérées dans le code pour plus de cohérence.

## Interactions avec les autres modules
- Utilisé par tft_display et web_ui pour savoir quoi afficher.
- Peut être modifié par les boutons, le réseau ou d’autres événements.

## Bonnes pratiques
- Garder les transitions d’état simples et prévisibles.
- Documenter tous les états possibles.
- Utiliser des enums pour la clarté et la sécurité de type.

## Pour les débutants
- L’état UI contrôle ce que l’utilisateur voit à l’écran ou sur la page web.
- Si l’affichage ou l’UI web semble bloqué, vérifiez l’état courant et les transitions.

---

> Le module ui_state est le cerveau de votre interface. Il garde tout synchronisé et facile à déboguer !
