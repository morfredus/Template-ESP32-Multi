# Module Paper154 écran e-paper (FR)

> **Version minimale valide : 0.1.1**

Ce module ajoute la gestion d'un écran e-paper 1.54" 200x200 (compatible GDEH0154D67) via la bibliothèque GxEPD2.

## Fonctionnalités
- Initialisation et effacement de l'écran e-paper
- Affichage de texte centré ou positionné
- Rafraîchissement partiel supporté

## Connexions (par défaut)
- CS : 15
- DC : 17
- RST : 16
- BUSY : 4

## Exemple d'utilisation
```
#include "paper154.h"

void setup() {
  Paper154::begin();
  Paper154::printCenter("Hello e-paper!");
}
```

## Documentation
- Toutes les fonctions sont documentées dans le header.
- Voir ModularDocs/paper154_FR.md pour l'usage détaillé et les fonctions avancées.

---

> **Ce document est valide à partir de la version 0.1.1.**
