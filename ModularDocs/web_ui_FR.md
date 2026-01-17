# Module : web_ui (Interface Web)

## Rôle du module
Ce module gère le contenu HTML/CSS/JS servi par le serveur web de l’ESP32. Il fournit une interface conviviale pour interagir avec l’appareil depuis n’importe quel navigateur.

## Pourquoi séparer l’interface web ?
- Pour isoler la logique et les ressources UI du code backend.
- Pour permettre de modifier facilement l’interface sans toucher au cœur du code.
- Pour que des designers puissent travailler sur l’UI sans connaître le C++.

## Où se trouvent les fichiers ?
- `include/web_ui.h` : déclare les fonctions liées à l’UI.
- `src/web_ui.cpp` : contient le code pour servir HTML/CSS/JS.
- `include/web_styles.h` : contient les styles CSS sous forme de chaînes C++.

## Fonctions principales
- `void servePage()` : Envoie la page HTML principale au client.
- `void serveStyles()` : Envoie les styles CSS.
- `void serveScript()` : Envoie le JavaScript si besoin.

## Paramètres importants
- Les fonctionnalités UI sont activées/désactivées dans `config.h`.
- Le contenu des pages est défini dans des chaînes C++ pour l’embarqué.

## Interactions avec les autres modules
- Utilise les données de ui_state, network, etc. pour afficher l’état.
- Peut déclencher des actions (boutons, formulaires) qui appellent le backend.

## Bonnes pratiques
- Garder l’UI simple et responsive.
- Minimiser la taille des pages pour un chargement rapide.
- Utiliser des labels et info-bulles clairs pour les débutants.

## Pour les débutants
- Ouvrez l’IP de l’ESP32 dans votre navigateur pour accéder à l’UI.
- Si la page est blanche, vérifiez le réseau et le serveur.

---

> L’interface web rend votre ESP32 accessible à tous. Ce module la garde claire, rapide et facile à utiliser !
