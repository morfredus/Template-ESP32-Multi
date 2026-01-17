# Module : buttons (Gestion des boutons)

> **Version minimale valide : 0.4.4**

---

> **Note :** Toutes les références aux boutons et GPIO dans le code sont désormais strictement conformes aux noms définis dans board_config.h (source officielle, immuable).

## Rôle du module
Ce module gère les boutons physiques de la carte. Il détecte les appuis, les appuis longs, et permet au système de réagir aux actions de l'utilisateur (reboot, navigation menu, etc.).

## Pourquoi séparer la gestion des boutons ?
- Pour garder la logique d'entrée propre et indépendante de l'affichage ou du réseau.
- Pour pouvoir ajouter ou modifier des fonctions de boutons sans casser le reste.
- Pour faciliter l'extension (plus de boutons, actions différentes).

## Où se trouvent les fichiers ?
- `include/buttons.h` : déclare les fonctions et constantes publiques.
- `src/buttons.cpp` : contient le code de gestion des boutons.

## Fonctions principales
- `void init()` : Initialise les broches et l'état des boutons.
- `void handle()` : Vérifie l'état, fait l'anti-rebond, déclenche les actions.
- `bool isPressed(uint8_t btn)` : Retourne vrai si un bouton est appuyé.
- `bool isLongPressed(uint8_t btn)` : Retourne vrai si un bouton est maintenu.

## Paramètres importants
- Les numéros de broches sont définis dans `board_config.h`.
- Les délais d'anti-rebond et d'appui long sont dans le code.

## Interactions avec les autres modules
- Peut déclencher des changements d'affichage (menu, reboot, etc.).
- Met à jour l'état global de l'interface utilisateur (`ui_state`).

## Bonnes pratiques
- Toujours faire l'anti-rebond pour éviter les faux appuis.
- Documenter clairement les actions de chaque bouton.
- Garder la logique simple et modulaire.

## Pour les débutants
- Si un bouton ne fonctionne pas, vérifiez le câblage et les broches dans `board_config.h`.
- Essayez de changer l'action d'un bouton pour voir la réaction du système.

---

> Les boutons sont le principal moyen d'interagir avec l'appareil. Ce module garantit fiabilité et évolutivité !
